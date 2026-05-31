/*
 * Shooter.X
 *
 * ES Framework application entry point for the shooter motor outputs.
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

    printf("\r\nStarting Shooter ES Framework project\r\n");
    printf("IBT-2 RPWM: %s, LPWM: %s\r\n",
            SHOOTER_RPWM_PIN_NAME,
            SHOOTER_LPWM_PIN_NAME);
    printf("IBT-2 R_EN: %s, L_EN: %s\r\n",
            SHOOTER_REN_ENABLE_NAME,
            SHOOTER_LEN_ENABLE_NAME);
    printf("Secondary shooter PWM: %s\r\n", SECONDARY_SHOOTER_PWM_PIN_NAME);
    printf("Primary motor auto-start disabled; secondary PWM auto-starts.\r\n");

    errorType = ES_Initialize();
    if (errorType == Success) {
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
