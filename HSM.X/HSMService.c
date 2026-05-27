/*
 * HSMService.c
 *
 * ES Framework service containing a hierarchical state machine for startup
 * beacon alignment.
 */

#include "HSMService.h"

#include <stdio.h>
#include <xc.h>

#include "AD.h"
#include "BOARD.h"
#include "BeaconEventChecker.h"
#include "ES_Framework.h"
#include "ES_Timers.h"
#include "pwm.h"

/*
 * Movement hardware mapping copied from Movement.X/peashooter.c so the HSM can
 * issue nonblocking tank turns while ADC event checks continue to run.
 */
#define LEFT_PWM PWM_PORTY10
#define RIGHT_PWM PWM_PORTY04

#define LEFT_IN1 LATEbits.LATE6
#define LEFT_IN2 LATEbits.LATE5
#define RIGHT_IN1 LATDbits.LATD11
#define RIGHT_IN2 LATDbits.LATD5

#define LEFT_IN1_TRIS _TRISE6
#define LEFT_IN2_TRIS _TRISE5
#define RIGHT_IN1_TRIS _TRISD11
#define RIGHT_IN2_TRIS _TRISD5

#define PS_NOMINAL_BATTERY_VOLTAGE 310
#define PS_MIN_BATTERY_COMP_VOLTAGE 263
#define PEASHOOTER_MAX_SPEED 100

#define HSM_TANK_TURN_POWER 65
#define HSM_FULL_ROTATION_TIME_MS 3200

typedef enum {
    InitPState,
    BeaconAlignmentState,
} HSMState_t;

typedef enum {
    InitPSubState,
    SurveySpinState,
    AlignSpinState,
    AlignedState,
} BeaconAlignmentSubState_t;

static uint8_t MyPriority;
static HSMState_t CurrentState;
static BeaconAlignmentSubState_t CurrentSubState;

static uint16_t MaxBeaconValue;
static uint32_t SurveyStartTime;
static uint32_t MaxBeaconOffset;
static uint32_t AlignmentSpinTime;

static uint8_t InitMovementHardware(void);
static uint8_t SetLeftMotorSpeed(char power);
static uint8_t SetRightMotorSpeed(char power);
static void StartTankTurnLeft(char power);
static void StopDrive(void);
static void StartSurveySpin(void);
static void RecordBeaconSample(uint16_t beaconValue);
static void StartAlignmentSpin(void);
static uint8_t InitBeaconAlignmentSubHSM(void);
static ES_Event RunBeaconAlignmentSubHSM(ES_Event thisEvent);

uint8_t InitHSMService(uint8_t priority)
{
    MyPriority = priority;
    CurrentState = InitPState;

    if (InitMovementHardware() != TRUE) {
        return FALSE;
    }

    if (InitBeaconEventChecker() != TRUE) {
        return FALSE;
    }

    return ES_PostToService(MyPriority, INIT_EVENT);
}

uint8_t PostHSMService(ES_Event thisEvent)
{
    return ES_PostToService(MyPriority, thisEvent);
}

ES_Event RunHSMService(ES_Event thisEvent)
{
    uint8_t makeTransition = FALSE;
    HSMState_t nextState = CurrentState;

    switch (CurrentState) {
    case InitPState:
        if (thisEvent.EventType == ES_INIT) {
            nextState = BeaconAlignmentState;
            makeTransition = TRUE;
            thisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case BeaconAlignmentState:
        if (thisEvent.EventType == ES_ENTRY) {
            if (InitBeaconAlignmentSubHSM() != TRUE) {
                thisEvent.EventType = ES_ERROR;
            } else {
                thisEvent.EventType = ES_NO_EVENT;
            }
        } else {
            thisEvent = RunBeaconAlignmentSubHSM(thisEvent);
        }
        break;

    default:
        break;
    }

    if (makeTransition == TRUE) {
        RunHSMService(EXIT_EVENT);
        CurrentState = nextState;
        RunHSMService(ENTRY_EVENT);
    }

    return thisEvent;
}

static uint8_t InitBeaconAlignmentSubHSM(void)
{
    ES_Event returnEvent;

    CurrentSubState = InitPSubState;
    returnEvent = RunBeaconAlignmentSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static ES_Event RunBeaconAlignmentSubHSM(ES_Event thisEvent)
{
    uint8_t makeTransition = FALSE;
    BeaconAlignmentSubState_t nextState = CurrentSubState;

    switch (CurrentSubState) {
    case InitPSubState:
        if (thisEvent.EventType == ES_INIT) {
            nextState = SurveySpinState;
            makeTransition = TRUE;
            thisEvent.EventType = ES_NO_EVENT;
        }
        break;

    case SurveySpinState:
        switch (thisEvent.EventType) {
        case ES_ENTRY:
            StartSurveySpin();
            thisEvent.EventType = ES_NO_EVENT;
            break;

        case BEACON_SAMPLE_READY:
            RecordBeaconSample(thisEvent.EventParam);
            thisEvent.EventType = ES_NO_EVENT;
            break;

        case ES_TIMEOUT:
            if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                StopDrive();
                nextState = AlignSpinState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        default:
            break;
        }
        break;

    case AlignSpinState:
        switch (thisEvent.EventType) {
        case ES_ENTRY:
            StartAlignmentSpin();
            thisEvent.EventType = ES_NO_EVENT;
            break;

        case ES_TIMEOUT:
            if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                StopDrive();
                nextState = AlignedState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        default:
            break;
        }
        break;

    case AlignedState:
        if (thisEvent.EventType == ES_ENTRY) {
            StopDrive();
            printf("Aligned to max beacon=%u at survey_offset=%lu ms\r\n",
                    MaxBeaconValue, (unsigned long) MaxBeaconOffset);
            thisEvent.EventType = ES_NO_EVENT;
        } else if (thisEvent.EventType == BEACON_SAMPLE_READY) {
            thisEvent.EventType = ES_NO_EVENT;
        }
        break;

    default:
        break;
    }

    if (makeTransition == TRUE) {
        RunBeaconAlignmentSubHSM(EXIT_EVENT);
        CurrentSubState = nextState;
        RunBeaconAlignmentSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static uint8_t InitMovementHardware(void)
{
    if ((AD_ActivePins() == 0) && (AD_Init() != SUCCESS)) {
        printf("HSM init failed: AD_Init\r\n");
        return FALSE;
    }

    if (PWM_Init() != SUCCESS) {
        printf("HSM init failed: PWM_Init\r\n");
        return FALSE;
    }

    if (PWM_SetFrequency(1000) != SUCCESS) {
        printf("HSM init failed: PWM_SetFrequency\r\n");
        return FALSE;
    }

    if (PWM_AddPins(LEFT_PWM | RIGHT_PWM) != SUCCESS) {
        printf("HSM init failed: PWM_AddPins\r\n");
        return FALSE;
    }

    LEFT_IN1_TRIS = 0;
    LEFT_IN2_TRIS = 0;
    RIGHT_IN1_TRIS = 0;
    RIGHT_IN2_TRIS = 0;
    StopDrive();

    printf("HSM beacon ADC pin=%s tank_power=%d full_rotation=%u ms\r\n",
            BEACON_DETECTOR_PIN_NAME, HSM_TANK_TURN_POWER,
            HSM_FULL_ROTATION_TIME_MS);

    return TRUE;
}

static uint16_t CompensateDutyForBattery(uint16_t dutyCycle)
{
    uint16_t batteryVoltage = AD_ReadADPin(BAT_VOLTAGE);
    uint32_t compensatedDuty;

    if (batteryVoltage < PS_MIN_BATTERY_COMP_VOLTAGE) {
        batteryVoltage = PS_MIN_BATTERY_COMP_VOLTAGE;
    }

    compensatedDuty = ((uint32_t) dutyCycle * PS_NOMINAL_BATTERY_VOLTAGE)
            / batteryVoltage;
    if (compensatedDuty > MAX_PWM) {
        compensatedDuty = MAX_PWM;
    }

    return (uint16_t) compensatedDuty;
}

static uint8_t SetLeftMotorSpeed(char power)
{
    uint16_t dutyCycle;

    if ((power < -PEASHOOTER_MAX_SPEED) || (power > PEASHOOTER_MAX_SPEED)) {
        return ERROR;
    }

    if (power < 0) {
        LEFT_IN1 = 0;
        LEFT_IN2 = 1;
        power = -power;
    } else {
        LEFT_IN1 = 1;
        LEFT_IN2 = 0;
    }

    dutyCycle = power * (MAX_PWM / PEASHOOTER_MAX_SPEED);
    dutyCycle = CompensateDutyForBattery(dutyCycle);
    return PWM_SetDutyCycle(LEFT_PWM, dutyCycle);
}

static uint8_t SetRightMotorSpeed(char power)
{
    uint16_t dutyCycle;

    if ((power < -PEASHOOTER_MAX_SPEED) || (power > PEASHOOTER_MAX_SPEED)) {
        return ERROR;
    }

    if (power < 0) {
        RIGHT_IN1 = 0;
        RIGHT_IN2 = 1;
        power = -power;
    } else {
        RIGHT_IN1 = 1;
        RIGHT_IN2 = 0;
    }

    dutyCycle = power * (MAX_PWM / PEASHOOTER_MAX_SPEED);
    dutyCycle = CompensateDutyForBattery(dutyCycle);
    return PWM_SetDutyCycle(RIGHT_PWM, dutyCycle);
}

static void StartTankTurnLeft(char power)
{
    SetRightMotorSpeed(power);
    SetLeftMotorSpeed(-power);
}

static void StopDrive(void)
{
    SetRightMotorSpeed(0);
    SetLeftMotorSpeed(0);
}

static void StartSurveySpin(void)
{
    MaxBeaconValue = 0;
    MaxBeaconOffset = 0;
    AlignmentSpinTime = HSM_FULL_ROTATION_TIME_MS;
    SurveyStartTime = ES_Timer_GetTime();

    printf("Survey spin started\r\n");
    StartTankTurnLeft(HSM_TANK_TURN_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

static void RecordBeaconSample(uint16_t beaconValue)
{
    uint32_t elapsedTime;

    if (beaconValue <= MaxBeaconValue) {
        return;
    }

    elapsedTime = ES_Timer_GetTime() - SurveyStartTime;
    if (elapsedTime > HSM_FULL_ROTATION_TIME_MS) {
        elapsedTime = HSM_FULL_ROTATION_TIME_MS;
    }

    MaxBeaconValue = beaconValue;
    MaxBeaconOffset = elapsedTime;
}

static void StartAlignmentSpin(void)
{
    AlignmentSpinTime = MaxBeaconOffset;
    if (AlignmentSpinTime == 0) {
        AlignmentSpinTime = HSM_FULL_ROTATION_TIME_MS;
    }

    printf("Alignment spin started max=%u offset=%lu ms\r\n",
            MaxBeaconValue, (unsigned long) MaxBeaconOffset);

    StartTankTurnLeft(HSM_TANK_TURN_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, AlignmentSpinTime);
}
