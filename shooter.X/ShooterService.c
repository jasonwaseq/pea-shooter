/*
 * ShooterService.c
 *
 * ES service for commanding the upper and lower shooter motor outputs.
 */

#include "ShooterService.h"

#include <stdio.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "ShooterEventChecker.h"

typedef enum {
    SHOOTER_DISABLED,
    SHOOTERS_RUNNING,
} ShooterState_t;

static uint8_t MyPriority;
static ShooterState_t CurrentState;

static uint8_t ConfigureShooterPins(void)
{
    if (IO_PortsSetPortOutputs(UPPER_SHOOTER_RPWM_IO_PORT,
            UPPER_SHOOTER_RPWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(LOWER_SHOOTER_LPWM_IO_PORT,
            LOWER_SHOOTER_LPWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(UPPER_SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_REN_ENABLE_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(LOWER_SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_LEN_ENABLE_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(UPPER_SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_REN_ENABLE_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(LOWER_SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_LEN_ENABLE_BIT) != SUCCESS) {
        return FALSE;
    }
    return TRUE;
}

static uint8_t EnableShooters(void)
{
    if (IO_PortsSetPortBits(UPPER_SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_REN_ENABLE_BIT) != SUCCESS) {
        return FALSE;
    }
    return IO_PortsSetPortBits(LOWER_SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_LEN_ENABLE_BIT) == SUCCESS;
}

static uint8_t DisableShooters(void)
{
    if (IO_PortsClearPortBits(UPPER_SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_REN_ENABLE_BIT) != SUCCESS) {
        return FALSE;
    }
    return IO_PortsClearPortBits(LOWER_SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_LEN_ENABLE_BIT) == SUCCESS;
}

static uint8_t SetUpperShooterDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(UPPER_SHOOTER_RPWM_PIN, duty) == SUCCESS;
}

static uint8_t SetLowerShooterDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(LOWER_SHOOTER_LPWM_PIN, duty) == SUCCESS;
}

static uint8_t StopShooterOutputs(void)
{
    if (SetUpperShooterDuty(MIN_PWM) != TRUE) {
        return FALSE;
    }
    if (SetLowerShooterDuty(MIN_PWM) != TRUE) {
        return FALSE;
    }
    return DisableShooters();
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
    if (ConfigureShooterPins() != TRUE) {
        return FALSE;
    }
    if (PWM_AddPins(UPPER_SHOOTER_RPWM_PIN
            | LOWER_SHOOTER_LPWM_PIN) != SUCCESS) {
        return FALSE;
    }
    if (StopShooterOutputs() != TRUE) {
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

uint8_t StartUpperShooterMotor(void)
{
    ES_Event startEvent;

    startEvent.EventType = UPPER_SHOOTER_START;
    startEvent.EventParam = UPPER_SHOOTER_DUTY;
    return PostShooterService(startEvent);
}

uint8_t StopUpperShooterMotor(void)
{
    ES_Event stopEvent;

    stopEvent.EventType = UPPER_SHOOTER_STOP;
    stopEvent.EventParam = 0;
    return PostShooterService(stopEvent);
}

uint8_t StartLowerShooterMotor(void)
{
    if (EnableShooters() != TRUE) {
        return FALSE;
    }
    return SetLowerShooterDuty(LOWER_SHOOTER_DUTY);
}

uint8_t StopLowerShooterMotor(void)
{
    return SetLowerShooterDuty(MIN_PWM);
}

uint8_t StartShooterMotor(void)
{
    return StartUpperShooterMotor();
}

uint8_t StopShooterMotor(void)
{
    return StopUpperShooterMotor();
}

uint8_t StartPrimaryShooterMotor(void)
{
    return StartUpperShooterMotor();
}

uint8_t StopPrimaryShooterMotor(void)
{
    return StopUpperShooterMotor();
}

uint8_t StartSecondaryShooterMotor(void)
{
    return StartLowerShooterMotor();
}

uint8_t StopSecondaryShooterMotor(void)
{
    return StopLowerShooterMotor();
}

ES_Event RunShooterService(ES_Event thisEvent)
{
    ES_Event returnEvent;

    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    switch (thisEvent.EventType) {
    case ES_INIT:
        printf("Upper shooter ready: RPWM=%s, R_EN=%s, duty=%u.%u%%\r\n",
                UPPER_SHOOTER_RPWM_PIN_NAME,
                UPPER_SHOOTER_REN_ENABLE_NAME,
                UPPER_SHOOTER_DUTY / 10,
                UPPER_SHOOTER_DUTY % 10);
        printf("Lower shooter ready: LPWM=%s, L_EN=%s, duty=%u.%u%%\r\n",
                LOWER_SHOOTER_LPWM_PIN_NAME,
                LOWER_SHOOTER_LEN_ENABLE_NAME,
                LOWER_SHOOTER_DUTY / 10,
                LOWER_SHOOTER_DUTY % 10);
        printf("PWM frequency: %u Hz\r\n", PWM_GetFrequency());
        break;

    case UPPER_SHOOTER_START:
        if (EnableShooters() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        if ((SetUpperShooterDuty(thisEvent.EventParam) != TRUE)
                || (SetLowerShooterDuty(LOWER_SHOOTER_DUTY) != TRUE)) {
            StopShooterOutputs();
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = SHOOTERS_RUNNING;
        printf("Shooters running: upper=%u.%u%% lower=%u.%u%%\r\n",
                thisEvent.EventParam / 10,
                thisEvent.EventParam % 10,
                LOWER_SHOOTER_DUTY / 10,
                LOWER_SHOOTER_DUTY % 10);
        break;

    case UPPER_SHOOTER_STOP:
        if (StopShooterOutputs() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = SHOOTER_DISABLED;
        printf("Shooters stopped\r\n");
        break;

    case ES_EXIT:
        StopShooterOutputs();
        PWM_End();
        CurrentState = SHOOTER_DISABLED;
        printf("Shooter exited\r\n");
        break;

    default:
        break;
    }

    return returnEvent;
}
