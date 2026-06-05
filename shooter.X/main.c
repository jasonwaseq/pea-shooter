/*
 * Shooter.X
 *
 * Direct hardware smoke test for the upper and lower shooter outputs.
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
    char upperOutputResult;
    char lowerOutputResult;
    char upperEnableOutputResult;
    char lowerEnableOutputResult;
    char upperEnableSetResult;
    char lowerEnableSetResult;
    char addPinsResult;
    char upperDutyResult;
    char lowerDutyResult;

    InitShooterBoardNoAd();

    printf("\r\nStarting upper and lower shooter motors directly\r\n");

    pwmInitResult = PWM_Init();
    pwmFrequencyResult = PWM_SetFrequency(SHOOTER_PWM_FREQUENCY);

    upperOutputResult = IO_PortsSetPortOutputs(UPPER_SHOOTER_RPWM_IO_PORT,
            UPPER_SHOOTER_RPWM_IO_BIT);
    lowerOutputResult = IO_PortsSetPortOutputs(LOWER_SHOOTER_LPWM_IO_PORT,
            LOWER_SHOOTER_LPWM_IO_BIT);

    upperEnableOutputResult = IO_PortsSetPortOutputs(UPPER_SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_REN_ENABLE_BIT);
    lowerEnableOutputResult = IO_PortsSetPortOutputs(LOWER_SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_LEN_ENABLE_BIT);
    upperEnableSetResult = IO_PortsSetPortBits(UPPER_SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_REN_ENABLE_BIT);
    lowerEnableSetResult = IO_PortsSetPortBits(LOWER_SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_LEN_ENABLE_BIT);

    addPinsResult = PWM_AddPins(UPPER_SHOOTER_RPWM_PIN
            | LOWER_SHOOTER_LPWM_PIN);
    upperDutyResult = PWM_SetDutyCycle(UPPER_SHOOTER_RPWM_PIN,
            UPPER_SHOOTER_DUTY);
    lowerDutyResult = PWM_SetDutyCycle(LOWER_SHOOTER_LPWM_PIN,
            LOWER_SHOOTER_DUTY);

    printf("Upper shooter: RPWM %s at %u.%u%% duty, R_EN %s\r\n",
            UPPER_SHOOTER_RPWM_PIN_NAME,
            UPPER_SHOOTER_DUTY / 10,
            UPPER_SHOOTER_DUTY % 10,
            UPPER_SHOOTER_REN_ENABLE_NAME);
    printf("Lower shooter: LPWM %s at %u.%u%% duty, L_EN %s\r\n",
            LOWER_SHOOTER_LPWM_PIN_NAME,
            LOWER_SHOOTER_DUTY / 10,
            LOWER_SHOOTER_DUTY % 10,
            LOWER_SHOOTER_LEN_ENABLE_NAME);
    printf("Init results: PWM_Init=%d PWM_SetFrequency=%d AddPins=%d\r\n",
            pwmInitResult, pwmFrequencyResult, addPinsResult);
    printf("Output results: UpperPWM=%d LowerPWM=%d UpperEnOut=%d LowerEnOut=%d UpperEnSet=%d LowerEnSet=%d\r\n",
            upperOutputResult, lowerOutputResult,
            upperEnableOutputResult, lowerEnableOutputResult,
            upperEnableSetResult, lowerEnableSetResult);
    printf("Duty results: Upper=%d Lower=%d\r\n",
            upperDutyResult, lowerDutyResult);
    printf("PWM active pins=0x%03X upper duty readback=%u lower duty readback=%u\r\n",
            PWM_ListPins(),
            PWM_GetDutyCycle(UPPER_SHOOTER_RPWM_PIN),
            PWM_GetDutyCycle(LOWER_SHOOTER_LPWM_PIN));

    for (;;) {
        ;
    }
}
