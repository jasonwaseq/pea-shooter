/*
 * ShooterService.c
 *
 * ES service for commanding an IBT-2/BTS7960 motor driver.
 */

#include "ShooterService.h"

#include <stdio.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "ShooterEventChecker.h"

typedef enum {
    SHOOTER_DISABLED,
    SHOOTER_FORWARD,
} ShooterState_t;

static uint8_t MyPriority;
static ShooterState_t CurrentState;

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

uint8_t InitShooterService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
    CurrentState = SHOOTER_DISABLED;

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
    if (StopMotorOutput() != TRUE) {
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

    case ES_EXIT:
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
