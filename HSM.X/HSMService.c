/*
 * HSMService.c
 *
 * ES Framework service containing a hierarchical state machine for startup
 * beacon alignment.
 */

#include "HSMService.h"

#include <stdio.h>

#include "peashooter.h"
#include "BeaconEventChecker.h"
#include "ES_Framework.h"
#include "ES_Timers.h"

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

static void StartSurveySpin(void);
static void RecordBeaconSample(uint16_t beaconValue);
static void StartAlignmentSpin(void);
static uint8_t InitBeaconAlignmentSubHSM(void);
static ES_Event RunBeaconAlignmentSubHSM(ES_Event thisEvent);

uint8_t InitHSMService(uint8_t priority)
{
    MyPriority = priority;
    CurrentState = InitPState;

    PS_Init();
    PS_Stop();

    printf("HSM beacon ADC pin=%s tank_power=%d full_rotation=%u ms\r\n",
            BEACON_DETECTOR_PIN_NAME, HSM_TANK_TURN_POWER,
            HSM_FULL_ROTATION_TIME_MS);

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
                PS_Stop();
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
                PS_Stop();
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
            PS_Stop();
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

static void StartSurveySpin(void)
{
    MaxBeaconValue = 0;
    MaxBeaconOffset = 0;
    AlignmentSpinTime = HSM_FULL_ROTATION_TIME_MS;
    SurveyStartTime = ES_Timer_GetTime();

    printf("Survey spin started\r\n");
    PS_TurnLeft(HSM_TANK_TURN_POWER);
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

    PS_TurnLeft(HSM_TANK_TURN_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, AlignmentSpinTime);
}
