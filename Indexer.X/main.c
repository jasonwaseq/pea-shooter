/*
 * Indexer PWM project
 *
 * ES Framework application entry point for running the indexer PWM output on
 * the Uno32/PIC32 I/O protection board.
 */

#include <BOARD.h>
#include <xc.h>

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "IndexerService.h"

int main(void)
{
    ES_Return_t errorType;

    BOARD_Init();

    errorType = ES_Initialize();
    if (errorType == Success) {
        StartIndexerSystem();
        errorType = ES_Run();
    }

    for (;;) {
        ;
    }
}
