/*
 * ShooterService.c
 *
 * ES service for commanding the upper and lower shooter motor outputs.
 */

#include "ShooterService.h"

#include <xc.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "ES_Timers.h"
#include "ShooterEventChecker.h"

typedef enum {
    SHOOTER_DISABLED,
    UPPER_SHOOTER_RUNNING,
} ShooterState_t;

static uint8_t MyPriority;
static ShooterState_t CurrentState;

static uint8_t ConfigureShooterPins(void)
{
    const uint16_t enablePins = UPPER_SHOOTER_ENABLE_BIT
            | LOWER_SHOOTER_ENABLE_BIT;

    if (IO_PortsSetPortOutputs(SHOOTER_ENABLE_PORT,
            enablePins) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(SHOOTER_ENABLE_PORT,
            enablePins) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(UPPER_SHOOTER_PWM_IO_PORT,
            UPPER_SHOOTER_PWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(LOWER_SHOOTER_PWM_IO_PORT,
            LOWER_SHOOTER_PWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(INDEXER_PWM_IO_PORT,
            INDEXER_PWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    return TRUE;
}

static uint8_t EnableUpperShooterDriver(void)
{
    return IO_PortsSetPortBits(SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_ENABLE_BIT) == SUCCESS;
}

static uint8_t DisableUpperShooterDriver(void)
{
    return IO_PortsClearPortBits(SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_ENABLE_BIT) == SUCCESS;
}

static uint8_t EnableLowerShooterDriver(void)
{
    return IO_PortsSetPortBits(SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_ENABLE_BIT) == SUCCESS;
}

static uint8_t DisableLowerShooterDriver(void)
{
    return IO_PortsClearPortBits(SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_ENABLE_BIT) == SUCCESS;
}

static uint8_t SetUpperShooterDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(UPPER_SHOOTER_PWM_PIN, duty) == SUCCESS;
}

static uint8_t StopUpperShooterOutput(void)
{
    if (PWM_SetDutyCycle(UPPER_SHOOTER_PWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    return DisableUpperShooterDriver();
}

static uint8_t SetLowerShooterDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(LOWER_SHOOTER_PWM_PIN, duty) == SUCCESS;
}

static uint8_t StopLowerShooterOutput(void)
{
    if (PWM_SetDutyCycle(LOWER_SHOOTER_PWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    return DisableLowerShooterDriver();
}

static uint8_t SetIndexerDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(INDEXER_PWM_PIN, duty) == SUCCESS;
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
    if (PWM_AddPins(UPPER_SHOOTER_PWM_PIN
            | LOWER_SHOOTER_PWM_PIN
            | INDEXER_PWM_PIN) != SUCCESS) {
        return FALSE;
    }
    if (StopUpperShooterOutput() != TRUE) {
        return FALSE;
    }
    if (StopLowerShooterOutput() != TRUE) {
        return FALSE;
    }
    if (EnableUpperShooterDriver() != TRUE) {
        return FALSE;
    }
    if (SetUpperShooterDuty(UPPER_SHOOTER_DUTY) != TRUE) {
        return FALSE;
    }
    if (StartLowerShooterMotor() != TRUE) {
        return FALSE;
    }
    if (SetIndexerDuty(INDEXER_STARTUP_DUTY) != TRUE) {
        return FALSE;
    }
    if (ES_Timer_InitTimer(INDEXER_STARTUP_TIMER,
            INDEXER_STARTUP_TIME_MS) != ES_Timer_OK) {
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

uint8_t StartShooterMotor(void)
{
    return StartUpperShooterMotor();
}

uint8_t StopShooterMotor(void)
{
    return StopUpperShooterMotor();
}

uint8_t StartLowerShooterMotor(void)
{
    if (EnableLowerShooterDriver() != TRUE) {
        return FALSE;
    }
    if (SetLowerShooterDuty(LOWER_SHOOTER_DUTY_CYCLE) != TRUE) {
        StopLowerShooterOutput();
        return FALSE;
    }
    return TRUE;
}

uint8_t StopLowerShooterMotor(void)
{
    return StopLowerShooterOutput();
}

ES_Event RunShooterService(ES_Event thisEvent)
{
    ES_Event returnEvent;

    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    switch (thisEvent.EventType) {
    case ES_INIT:
        break;

    case UPPER_SHOOTER_START:
        if (EnableUpperShooterDriver() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        if (SetUpperShooterDuty(thisEvent.EventParam) != TRUE) {
            StopUpperShooterOutput();
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = UPPER_SHOOTER_RUNNING;
        break;

    case UPPER_SHOOTER_STOP:
        if (StopUpperShooterOutput() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = SHOOTER_DISABLED;
        break;

    case ES_TIMEOUT:
        if (thisEvent.EventParam == INDEXER_STARTUP_TIMER) {
            if (SetIndexerDuty(INDEXER_RUN_DUTY) != TRUE) {
                returnEvent.EventType = ES_ERROR;
            }
        }
        break;

    case ES_EXIT:
        ES_Timer_StopTimer(INDEXER_STARTUP_TIMER);
        SetIndexerDuty(MIN_PWM);
        StopLowerShooterOutput();
        StopUpperShooterOutput();
        PWM_End();
        CurrentState = SHOOTER_DISABLED;
        break;

    default:
        break;
    }

    return returnEvent;
}
