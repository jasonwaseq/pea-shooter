/*
 * Indexer PWM project
 *
 * ES Framework application entry point for running the indexer PWM output on
 * the Uno32/PIC32 I/O protection board.
 */

#include <BOARD.h>
#include <stdio.h>
#include <xc.h>

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "IndexerService.h"

int main(void)
{
    ES_Return_t errorType;

    BOARD_Init();

    printf("\r\nStarting Indexer ES Framework project\r\n");
    printf("Indexer PWM output defaults to %s; edit IndexerService.h if rewired.\r\n",
            INDEXER_PWM_PIN_NAME);
    printf("Beacon detector logic lives in BeaconDetector.X.\r\n");

    errorType = ES_Initialize();
    if (errorType == Success) {
        StartIndexerSystem();
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
