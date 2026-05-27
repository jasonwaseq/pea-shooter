/*
 * HSM.X
 *
 * ES Framework application entry point for the startup beacon alignment HSM.
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

    printf("\r\nStarting HSM beacon alignment project\r\n");

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
