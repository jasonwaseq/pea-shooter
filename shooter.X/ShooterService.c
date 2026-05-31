/*
 * ShooterService.c
 *
 * ES service for commanding the primary IBT-2/BTS7960 shooter motor driver
 * and a fixed-duty secondary shooter PWM output.
 */

#include "ShooterService.h"

#include <stdio.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "ES_Timers.h"
#include "ShooterEventChecker.h"

typedef enum {
    SHOOTER_DISABLED,
    SHOOTER_FORWARD,
} ShooterState_t;

static uint8_t MyPriority;
static ShooterState_t CurrentState;
static unsigned int SecondaryShooterDuty;
static uint8_t SecondaryShooterPwmActive;
static uint8_t SecondaryShooterOutputHigh;

static uint8_t ConfigureSecondaryShooterPin(void)
{
    if (IO_PortsSetPortOutputs(SECONDARY_SHOOTER_PWM_PORT,
            SECONDARY_SHOOTER_PWM_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(SECONDARY_SHOOTER_PWM_PORT,
            SECONDARY_SHOOTER_PWM_BIT) != SUCCESS) {
        return FALSE;
    }
    return TRUE;
}

static uint8_t ConfigureEnablePins(void)
{
    const uint16_t enablePins = SHOOTER_REN_ENABLE_BIT | SHOOTER_LEN_ENABLE_BIT;

    if (IO_PortsSetPortOutputs(SHOOTER_ENABLE_PORT, enablePins) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(SHOOTER_ENABLE_PORT, enablePins) != SUCCESS) {
        return FALSE;
    }
    return TRUE;
}

static uint8_t EnableDriver(void)
{
    const uint16_t enablePins = SHOOTER_REN_ENABLE_BIT | SHOOTER_LEN_ENABLE_BIT;

    return IO_PortsSetPortBits(SHOOTER_ENABLE_PORT, enablePins) == SUCCESS;
}

static uint8_t DisableDriver(void)
{
    const uint16_t enablePins = SHOOTER_REN_ENABLE_BIT | SHOOTER_LEN_ENABLE_BIT;

    return IO_PortsClearPortBits(SHOOTER_ENABLE_PORT, enablePins) == SUCCESS;
}

static uint8_t SetForwardDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    if (PWM_SetDutyCycle(SHOOTER_LPWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    return PWM_SetDutyCycle(SHOOTER_RPWM_PIN, duty) == SUCCESS;
}

static uint8_t StopMotorOutput(void)
{
    if (PWM_SetDutyCycle(SHOOTER_RPWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    if (PWM_SetDutyCycle(SHOOTER_LPWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    return DisableDriver();
}

static uint8_t SetSecondaryShooterDuty(unsigned int duty)
{
    uint32_t highTime;
    uint32_t lowTime;

    if (duty > MAX_PWM) {
        return FALSE;
    }

    SecondaryShooterDuty = duty;

    if (duty == MIN_PWM) {
        SecondaryShooterPwmActive = FALSE;
        ES_Timer_StopTimer(SHOOTER_TIMER);
        return IO_PortsClearPortBits(SECONDARY_SHOOTER_PWM_PORT,
                SECONDARY_SHOOTER_PWM_BIT) == SUCCESS;
    }

    if (duty == MAX_PWM) {
        SecondaryShooterPwmActive = FALSE;
        ES_Timer_StopTimer(SHOOTER_TIMER);
        return IO_PortsSetPortBits(SECONDARY_SHOOTER_PWM_PORT,
                SECONDARY_SHOOTER_PWM_BIT) == SUCCESS;
    }

    highTime = ((uint32_t) SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS * duty)
            / MAX_PWM;
    if (highTime == 0) {
        highTime = 1;
    }
    lowTime = SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS - highTime;
    if (lowTime == 0) {
        lowTime = 1;
    }

    SecondaryShooterPwmActive = TRUE;
    SecondaryShooterOutputHigh = TRUE;

    if (IO_PortsSetPortBits(SECONDARY_SHOOTER_PWM_PORT,
            SECONDARY_SHOOTER_PWM_BIT) != SUCCESS) {
        SecondaryShooterPwmActive = FALSE;
        return FALSE;
    }
    return ES_Timer_InitTimer(SHOOTER_TIMER, highTime) == ES_Timer_OK;
}

static uint8_t StopSecondaryShooterOutput(void)
{
    return SetSecondaryShooterDuty(MIN_PWM);
}

uint8_t InitShooterService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
    CurrentState = SHOOTER_DISABLED;
    SecondaryShooterDuty = MIN_PWM;
    SecondaryShooterPwmActive = FALSE;
    SecondaryShooterOutputHigh = FALSE;

    if (PWM_Init() != SUCCESS) {
        return FALSE;
    }
    if (PWM_SetFrequency(SHOOTER_PWM_FREQUENCY) != SUCCESS) {
        return FALSE;
    }
    if (PWM_AddPins(SHOOTER_RPWM_PIN | SHOOTER_LPWM_PIN) != SUCCESS) {
        return FALSE;
    }
    if (ConfigureEnablePins() != TRUE) {
        return FALSE;
    }
    if (ConfigureSecondaryShooterPin() != TRUE) {
        return FALSE;
    }
    if (StopMotorOutput() != TRUE) {
        return FALSE;
    }
    if (StartSecondaryShooterMotor() != TRUE) {
        return FALSE;
    }
    if (InitShooterEventChecker() != TRUE) {
        return FALSE;
    }

    initEvent.EventType = ES_INIT;
    initEvent.EventParam = 0;
    return ES_PostToService(MyPriority, initEvent);
}

uint8_t PostShooterService(ES_Event thisEvent)
{
    return ES_PostToService(MyPriority, thisEvent);
}

uint8_t StartShooterMotor(void)
{
    ES_Event startEvent;

    startEvent.EventType = SHOOTER_START;
    startEvent.EventParam = SHOOTER_FORWARD_DUTY;
    return PostShooterService(startEvent);
}

uint8_t StopShooterMotor(void)
{
    ES_Event stopEvent;

    stopEvent.EventType = SHOOTER_STOP;
    stopEvent.EventParam = 0;
    return PostShooterService(stopEvent);
}

uint8_t StartSecondaryShooterMotor(void)
{
    return SetSecondaryShooterDuty(SECONDARY_SHOOTER_DUTY_CYCLE);
}

uint8_t StopSecondaryShooterMotor(void)
{
    return StopSecondaryShooterOutput();
}

ES_Event RunShooterService(ES_Event thisEvent)
{
    ES_Event returnEvent;

    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    switch (thisEvent.EventType) {
    case ES_INIT:
        printf("Shooter ready: RPWM=%s, LPWM=%s, R_EN=%s, L_EN=%s\r\n",
                SHOOTER_RPWM_PIN_NAME,
                SHOOTER_LPWM_PIN_NAME,
                SHOOTER_REN_ENABLE_NAME,
                SHOOTER_LEN_ENABLE_NAME);
        printf("Secondary shooter PWM: %s at %u.%u%% duty\r\n",
                SECONDARY_SHOOTER_PWM_PIN_NAME,
                SECONDARY_SHOOTER_DUTY_CYCLE / 10,
                SECONDARY_SHOOTER_DUTY_CYCLE % 10);
        printf("PWM frequency: %u Hz\r\n", PWM_GetFrequency());
        break;

    case SHOOTER_START:
        if (EnableDriver() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        if (SetForwardDuty(thisEvent.EventParam) != TRUE) {
            StopMotorOutput();
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = SHOOTER_FORWARD;
        printf("Shooter motor forward at %u.%u%% duty\r\n",
                thisEvent.EventParam / 10,
                thisEvent.EventParam % 10);
        break;

    case SHOOTER_STOP:
        if (StopMotorOutput() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = SHOOTER_DISABLED;
        printf("Shooter motor stopped\r\n");
        break;

    case ES_TIMEOUT:
        if ((thisEvent.EventParam == SHOOTER_TIMER)
                && (SecondaryShooterPwmActive == TRUE)) {
            uint32_t highTime = ((uint32_t) SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS
                    * SecondaryShooterDuty) / MAX_PWM;
            uint32_t lowTime = SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS - highTime;

            if (highTime == 0) {
                highTime = 1;
            }
            if (lowTime == 0) {
                lowTime = 1;
            }

            if (SecondaryShooterOutputHigh == TRUE) {
                IO_PortsClearPortBits(SECONDARY_SHOOTER_PWM_PORT,
                        SECONDARY_SHOOTER_PWM_BIT);
                SecondaryShooterOutputHigh = FALSE;
                ES_Timer_InitTimer(SHOOTER_TIMER, lowTime);
            } else {
                IO_PortsSetPortBits(SECONDARY_SHOOTER_PWM_PORT,
                        SECONDARY_SHOOTER_PWM_BIT);
                SecondaryShooterOutputHigh = TRUE;
                ES_Timer_InitTimer(SHOOTER_TIMER, highTime);
            }
        }
        break;

    case ES_EXIT:
        StopSecondaryShooterOutput();
        StopMotorOutput();
        PWM_End();
        CurrentState = SHOOTER_DISABLED;
        printf("Shooter exited\r\n");
        break;

    default:
        break;
    }

    return returnEvent;
}
