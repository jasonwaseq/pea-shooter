/*
 * BeaconDetector.X
 *
 * ES Framework application entry point for reading an analog 2 kHz beacon
 * detector strength output on the Uno32/PIC32 I/O protection board.
 */

#include <BOARD.h>
#include <stdio.h>
#include <xc.h>

#include "ES_Configure.h"
#include "ES_Framework.h"

int main(void)
{
    ES_Return_t errorType;

    BOARD_Init();

    printf("\r\nStarting Beacon Detector ES Framework project\r\n");
    printf("Beacon input defaults to AD_PORTV3; edit BeaconEventChecker.h if rewired.\r\n");

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
