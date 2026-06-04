/*
 * Shooter.X
 *
 * ES Framework application entry point for the shooter motor outputs.
 */

#include <BOARD.h>
#include <xc.h>

#include "pwm.h"
#include "ShooterService.h"

#define CORE_TIMER_500MS_TICKS 20000000UL

static unsigned int ReadCoreTimer(void)
{
    unsigned int count;

    asm volatile("mfc0 %0,$9" : "=r"(count));
    return count;
}

static void WaitCoreTimerTicks(unsigned int ticks)
{
    unsigned int start = ReadCoreTimer();

    while ((unsigned int) (ReadCoreTimer() - start) < ticks) {
        ;
    }
}

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

    IEC1bits.AD1IE = 0;
    IFS1bits.AD1IF = 0;
    AD1CON1CLR = _AD1CON1_ON_MASK;
}

int main(void)
{
    InitShooterBoardNoAd();

    PWM_Init();
    PWM_SetFrequency(SHOOTER_PWM_FREQUENCY);

    IO_PortsSetPortOutputs(UPPER_SHOOTER_PWM_IO_PORT,
            UPPER_SHOOTER_PWM_IO_BIT);
    IO_PortsSetPortOutputs(LOWER_SHOOTER_PWM_IO_PORT,
            LOWER_SHOOTER_PWM_IO_BIT);
    IO_PortsSetPortOutputs(INDEXER_PWM_IO_PORT,
            INDEXER_PWM_IO_BIT);

    IO_PortsSetPortOutputs(SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_ENABLE_BIT | LOWER_SHOOTER_ENABLE_BIT);
    IO_PortsSetPortBits(SHOOTER_ENABLE_PORT,
            UPPER_SHOOTER_ENABLE_BIT);
    IO_PortsSetPortBits(SHOOTER_ENABLE_PORT,
            LOWER_SHOOTER_ENABLE_BIT);

    PWM_AddPins(UPPER_SHOOTER_PWM_PIN
            | LOWER_SHOOTER_PWM_PIN
            | INDEXER_PWM_PIN);
    PWM_SetDutyCycle(UPPER_SHOOTER_PWM_PIN,
            UPPER_SHOOTER_DUTY);
    PWM_SetDutyCycle(LOWER_SHOOTER_PWM_PIN,
            LOWER_SHOOTER_DUTY_CYCLE);
    PWM_SetDutyCycle(INDEXER_PWM_PIN,
            INDEXER_STARTUP_DUTY);

    WaitCoreTimerTicks(CORE_TIMER_500MS_TICKS);

    PWM_SetDutyCycle(INDEXER_PWM_PIN,
            INDEXER_RUN_DUTY);

    for (;;) {
        ;
    }
}
