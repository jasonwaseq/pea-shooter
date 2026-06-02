/*
 * ShooterService.c
 *
 * ES service for commanding the primary IBT-2/BTS7960 shooter motor driver
 * and a fixed-duty secondary shooter PWM output.
 */

#include "ShooterService.h"

#include <stdio.h>
#include <xc.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "ES_Timers.h"
#include "ShooterEventChecker.h"

typedef enum {
    SHOOTER_DISABLED,
    PRIMARY_SHOOTER_RUNNING,
} ShooterState_t;

static uint8_t MyPriority;
static ShooterState_t CurrentState;

static uint8_t ConfigureSecondaryShooterPin(void)
{
    if (IO_PortsSetPortOutputs(SECONDARY_SHOOTER_PWM_IO_PORT,
            SECONDARY_SHOOTER_PWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(SECONDARY_SHOOTER_PWM_IO_PORT,
            SECONDARY_SHOOTER_PWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    SECONDARY_SHOOTER_PWM_IO_TRIS = 0;
    return TRUE;
}

static uint8_t ConfigurePrimaryShooterPins(void)
{
    const uint16_t enablePins = PRIMARY_SHOOTER_REN_ENABLE_BIT
            | PRIMARY_SHOOTER_LEN_ENABLE_BIT;

    if (IO_PortsSetPortOutputs(PRIMARY_SHOOTER_ENABLE_PORT,
            enablePins) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsClearPortBits(PRIMARY_SHOOTER_ENABLE_PORT,
            enablePins) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(PRIMARY_SHOOTER_RPWM_IO_PORT,
            PRIMARY_SHOOTER_RPWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(PRIMARY_SHOOTER_LPWM_IO_PORT,
            PRIMARY_SHOOTER_LPWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    return TRUE;
}

static uint8_t EnableDriver(void)
{
    const uint16_t enablePins = PRIMARY_SHOOTER_REN_ENABLE_BIT
            | PRIMARY_SHOOTER_LEN_ENABLE_BIT;

    return IO_PortsSetPortBits(PRIMARY_SHOOTER_ENABLE_PORT,
            enablePins) == SUCCESS;
}

static uint8_t DisableDriver(void)
{
    const uint16_t enablePins = PRIMARY_SHOOTER_REN_ENABLE_BIT
            | PRIMARY_SHOOTER_LEN_ENABLE_BIT;

    return IO_PortsClearPortBits(PRIMARY_SHOOTER_ENABLE_PORT,
            enablePins) == SUCCESS;
}

static uint8_t SetPrimaryShooterDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    if (PWM_SetDutyCycle(PRIMARY_SHOOTER_IDLE_PWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    return PWM_SetDutyCycle(PRIMARY_SHOOTER_DRIVE_PWM_PIN, duty) == SUCCESS;
}

static uint8_t StopMotorOutput(void)
{
    if (PWM_SetDutyCycle(PRIMARY_SHOOTER_RPWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    if (PWM_SetDutyCycle(PRIMARY_SHOOTER_LPWM_PIN, MIN_PWM) != SUCCESS) {
        return FALSE;
    }
    return DisableDriver();
}

static uint8_t SetSecondaryShooterDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(SECONDARY_SHOOTER_PWM_PIN, duty) == SUCCESS;
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

    if (PWM_Init() != SUCCESS) {
        return FALSE;
    }
    if (PWM_SetFrequency(SHOOTER_PWM_FREQUENCY) != SUCCESS) {
        return FALSE;
    }
    if (ConfigurePrimaryShooterPins() != TRUE) {
        return FALSE;
    }
    if (ConfigureSecondaryShooterPin() != TRUE) {
        return FALSE;
    }
    if (PWM_AddPins(PRIMARY_SHOOTER_RPWM_PIN
            | PRIMARY_SHOOTER_LPWM_PIN
            | SECONDARY_SHOOTER_PWM_PIN) != SUCCESS) {
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

uint8_t StartPrimaryShooterMotor(void)
{
    ES_Event startEvent;

    startEvent.EventType = PRIMARY_SHOOTER_START;
    startEvent.EventParam = PRIMARY_SHOOTER_DUTY;
    return PostShooterService(startEvent);
}

uint8_t StopPrimaryShooterMotor(void)
{
    ES_Event stopEvent;

    stopEvent.EventType = PRIMARY_SHOOTER_STOP;
    stopEvent.EventParam = 0;
    return PostShooterService(stopEvent);
}

uint8_t StartShooterMotor(void)
{
    return StartPrimaryShooterMotor();
}

uint8_t StopShooterMotor(void)
{
    return StopPrimaryShooterMotor();
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
        printf("Primary shooter ready: RPWM=%s, LPWM=%s, R_EN=%s, L_EN=%s\r\n",
                PRIMARY_SHOOTER_RPWM_PIN_NAME,
                PRIMARY_SHOOTER_LPWM_PIN_NAME,
                PRIMARY_SHOOTER_REN_ENABLE_NAME,
                PRIMARY_SHOOTER_LEN_ENABLE_NAME);
        printf("Primary shooter direction: drive %s, hold %s low\r\n",
                PRIMARY_SHOOTER_DRIVE_PWM_PIN_NAME,
                PRIMARY_SHOOTER_IDLE_PWM_PIN_NAME);
        printf("Secondary shooter ready: PWM=%s at %u.%u%% duty\r\n",
                SECONDARY_SHOOTER_PWM_PIN_NAME,
                SECONDARY_SHOOTER_DUTY_CYCLE / 10,
                SECONDARY_SHOOTER_DUTY_CYCLE % 10);
        printf("PWM frequency: %u Hz\r\n", PWM_GetFrequency());
        break;

    case PRIMARY_SHOOTER_START:
        if (EnableDriver() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        if (SetPrimaryShooterDuty(thisEvent.EventParam) != TRUE) {
            StopMotorOutput();
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = PRIMARY_SHOOTER_RUNNING;
        printf("Primary shooter running opposite secondary at %u.%u%% duty\r\n",
                thisEvent.EventParam / 10,
                thisEvent.EventParam % 10);
        break;

    case PRIMARY_SHOOTER_STOP:
        if (StopMotorOutput() != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = SHOOTER_DISABLED;
        printf("Primary shooter stopped\r\n");
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
