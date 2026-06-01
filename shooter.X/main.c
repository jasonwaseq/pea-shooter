/*
 * Shooter.X
 *
 * ES Framework application entry point for the shooter motor outputs.
 */

#include <BOARD.h>
#include <stdio.h>
#include <xc.h>

#include "IO_Ports.h"
#include "pwm.h"

int main(void)
{
    BOARD_Init();

    printf("\r\nStarting both shooter motors directly\r\n");

    PWM_Init();
    PWM_SetFrequency(PWM_1KHZ);
    PWM_AddPins(PWM_PORTY04 | PWM_PORTX11);

    IO_PortsSetPortOutputs(PORTX, PIN3 | PIN4);
    IO_PortsSetPortBits(PORTX, PIN3 | PIN4);

    IO_PortsSetPortOutputs(PORTZ, PIN4);
    IO_PortsSetPortBits(PORTZ, PIN4);

    PWM_SetDutyCycle(PWM_PORTX11, 1000);
    PWM_SetDutyCycle(PWM_PORTY04, 1000);

    for (;;) {
        ;
    }
}
