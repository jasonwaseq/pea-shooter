/*
 * Shooter.X
 *
 * ES Framework application entry point for the shooter motor outputs.
 */

#include <BOARD.h>
#include <stdio.h>
#include <xc.h>

#include "pwm.h"
#include "serial.h"
#include "ShooterService.h"

static void InitShooterBoardNoAd(void)
{
    unsigned int status;
    unsigned int val;

    AD1PCFG = 0xffff;

    asm volatile("mfc0   %0,$13" : "=r"(val));
    val |= 0x00800000;
    asm volatile("mtc0   %0,$13" : "+r"(val));
    INTCONSET = _INTCON_MVEC_MASK;
    asm volatile("ei    %0" : "=r"(status));

    SERIAL_Init();

    IEC1bits.AD1IE = 0;
    IFS1bits.AD1IF = 0;
    AD1CON1CLR = _AD1CON1_ON_MASK;
}

int main(void)
{
    char pwmInitResult;
    char pwmFrequencyResult;
    char primaryRpwmOutputResult;
    char primaryLpwmOutputResult;
    char primaryEnableOutputResult;
    char primaryEnableSetResult;
    char secondaryOutputResult;
    char addPinsResult;
    char primaryIdleDutyResult;
    char primaryDriveDutyResult;
    char secondaryDutyResult;

    InitShooterBoardNoAd();

    printf("\r\nStarting primary and secondary shooter motors directly\r\n");

    pwmInitResult = PWM_Init();
    pwmFrequencyResult = PWM_SetFrequency(SHOOTER_PWM_FREQUENCY);

    primaryRpwmOutputResult = IO_PortsSetPortOutputs(PRIMARY_SHOOTER_RPWM_IO_PORT,
            PRIMARY_SHOOTER_RPWM_IO_BIT);
    primaryLpwmOutputResult = IO_PortsSetPortOutputs(PRIMARY_SHOOTER_LPWM_IO_PORT,
            PRIMARY_SHOOTER_LPWM_IO_BIT);

    primaryEnableOutputResult = IO_PortsSetPortOutputs(PRIMARY_SHOOTER_ENABLE_PORT,
            PRIMARY_SHOOTER_REN_ENABLE_BIT | PRIMARY_SHOOTER_LEN_ENABLE_BIT);
    primaryEnableSetResult = IO_PortsSetPortBits(PRIMARY_SHOOTER_ENABLE_PORT,
            PRIMARY_SHOOTER_REN_ENABLE_BIT | PRIMARY_SHOOTER_LEN_ENABLE_BIT);

    secondaryOutputResult = IO_PortsSetPortOutputs(SECONDARY_SHOOTER_PWM_IO_PORT,
            SECONDARY_SHOOTER_PWM_IO_BIT);
    SECONDARY_SHOOTER_PWM_IO_TRIS = 0;

    addPinsResult = PWM_AddPins(PRIMARY_SHOOTER_RPWM_PIN
            | PRIMARY_SHOOTER_LPWM_PIN
            | SECONDARY_SHOOTER_PWM_PIN);
    primaryIdleDutyResult = PWM_SetDutyCycle(PRIMARY_SHOOTER_IDLE_PWM_PIN, MIN_PWM);
    primaryDriveDutyResult = PWM_SetDutyCycle(PRIMARY_SHOOTER_DRIVE_PWM_PIN,
            PRIMARY_SHOOTER_DUTY);
    secondaryDutyResult = PWM_SetDutyCycle(SECONDARY_SHOOTER_PWM_PIN,
            SECONDARY_SHOOTER_DUTY_CYCLE);

    printf("Primary shooter: drive %s at %u.%u%% duty, hold %s low\r\n",
            PRIMARY_SHOOTER_DRIVE_PWM_PIN_NAME,
            PRIMARY_SHOOTER_DUTY / 10,
            PRIMARY_SHOOTER_DUTY % 10,
            PRIMARY_SHOOTER_IDLE_PWM_PIN_NAME);
    printf("Secondary shooter: hardware PWM %s at %u.%u%% duty\r\n",
            SECONDARY_SHOOTER_PWM_PIN_NAME,
            SECONDARY_SHOOTER_DUTY_CYCLE / 10,
            SECONDARY_SHOOTER_DUTY_CYCLE % 10);
    printf("Init results: PWM_Init=%d PWM_SetFrequency=%d AddPins=%d\r\n",
            pwmInitResult, pwmFrequencyResult, addPinsResult);
    printf("Output results: RPWM=%d LPWM=%d EnablesOut=%d EnablesSet=%d SecondaryOut=%d\r\n",
            primaryRpwmOutputResult, primaryLpwmOutputResult,
            primaryEnableOutputResult, primaryEnableSetResult,
            secondaryOutputResult);
    printf("Duty results: PrimaryIdle=%d PrimaryDrive=%d Secondary=%d\r\n",
            primaryIdleDutyResult, primaryDriveDutyResult,
            secondaryDutyResult);
    printf("PWM active pins=0x%03X primary duty readback=%u secondary duty readback=%u\r\n",
            PWM_ListPins(),
            PWM_GetDutyCycle(PRIMARY_SHOOTER_DRIVE_PWM_PIN),
            PWM_GetDutyCycle(SECONDARY_SHOOTER_PWM_PIN));
    printf("Z6/OC1 registers: TRISD0=%u LATD0=%u OC1CON=0x%08lX OC1R=%lu OC1RS=%lu PR2=%lu T2CON=0x%08lX\r\n",
            (unsigned int) PORTZ06_TRIS,
            (unsigned int) PORTZ06_LAT,
            (unsigned long) OC1CON,
            (unsigned long) OC1R,
            (unsigned long) OC1RS,
            (unsigned long) PR2,
            (unsigned long) T2CON);

    for (;;) {
        ;
    }
}
