/*
 * shooter
 *
 * ES Framework application entry point for driving an IBT-2/BTS7960 motor
 * driver from the Uno32 I/O protection stack.
 */

#include <BOARD.h>
#include <stdio.h>
#include <xc.h>

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ShooterService.h"

int main(void)
{
    ES_Return_t errorType;

    BOARD_Init();

    printf("\r\nStarting shooter ES Framework project\r\n");
    printf("IBT-2 RPWM: %s, LPWM: %s\r\n",
            SHOOTER_RPWM_PIN_NAME,
            SHOOTER_LPWM_PIN_NAME);
    printf("Initial motor command is forward at %u.%u%% duty\r\n",
            SHOOTER_FORWARD_DUTY / 10,
            SHOOTER_FORWARD_DUTY % 10);

    errorType = ES_Initialize();
    if (errorType == Success) {
        StartShooterMotor();
        errorType = ES_Run();
    }

    switch (errorType) {
    case FailedPointer:
        printf("Failed on NULL pointer\r\n");
        break;
    case FailedInit:
        printf("Failed initialization\r\n");
        break;
    default:
        printf("Other failure: %d\r\n", errorType);
        break;
    }

    for (;;) {
        ;
    }
}
