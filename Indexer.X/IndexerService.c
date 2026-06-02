/*
 * IndexerService.c
 *
 * ES service that controls the indexer PWM output.
 */

#include "IndexerService.h"

#include <stdio.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "ES_Timers.h"
#include "IndexerEventChecker.h"

typedef enum {
    INDEXER_OFF,
    INDEXER_STARTUP,
    INDEXER_RUNNING,
} IndexerState_t;

static uint8_t MyPriority;
static IndexerState_t CurrentState;
static uint8_t IndexerOutputActive;

static uint8_t SetIndexerPulse(unsigned int pulseTime)
{
    if (pulseTime == 0) {
        if (IndexerOutputActive == TRUE) {
            IndexerOutputActive = FALSE;
            return RC_RemovePins(INDEXER_RC_PIN) == SUCCESS;
        }
        return TRUE;
    }

    if (IndexerOutputActive == FALSE) {
        if (RC_AddPins(INDEXER_RC_PIN) != SUCCESS) {
            return FALSE;
        }
        IndexerOutputActive = TRUE;
    }
    return RC_SetPulseTime(INDEXER_RC_PIN, pulseTime) == SUCCESS;
}

uint8_t InitIndexerService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
    CurrentState = INDEXER_OFF;
    IndexerOutputActive = FALSE;

    if (RC_Init() != SUCCESS) {
        return FALSE;
    }
    if (InitIndexerEventChecker() != TRUE) {
        return FALSE;
    }

    initEvent.EventType = ES_INIT;
    initEvent.EventParam = 0;
    return ES_PostToService(MyPriority, initEvent);
}

uint8_t PostIndexerService(ES_Event thisEvent)
{
    return ES_PostToService(MyPriority, thisEvent);
}

uint8_t StartIndexerSystem(void)
{
    ES_Event startEvent;

    startEvent.EventType = INDEXER_START;
    startEvent.EventParam = 0;
    return PostIndexerService(startEvent);
}

uint8_t StopIndexerSystem(void)
{
    ES_Event stopEvent;

    stopEvent.EventType = INDEXER_STOP;
    stopEvent.EventParam = 0;
    return PostIndexerService(stopEvent);
}

ES_Event RunIndexerService(ES_Event thisEvent)
{
    ES_Event returnEvent;

    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    switch (thisEvent.EventType) {
    case ES_INIT:
        printf("Indexer ready on %s using RC_Servo output\r\n",
                INDEXER_OUTPUT_PIN_NAME);
        break;

    case INDEXER_START:
        if (SetIndexerPulse(INDEXER_STARTUP_PULSE_US) != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        if (ES_Timer_InitTimer(INDEXER_STARTUP_TIMER,
                INDEXER_STARTUP_TIME_MS) != ES_Timer_OK) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = INDEXER_STARTUP;
        printf("Indexer startup: %u us pulse for %u ms\r\n",
                INDEXER_STARTUP_PULSE_US,
                INDEXER_STARTUP_TIME_MS);
        break;

    case ES_TIMEOUT:
        if ((thisEvent.EventParam == INDEXER_STARTUP_TIMER)
                && (CurrentState == INDEXER_STARTUP)) {
            if (SetIndexerPulse(INDEXER_RUN_PULSE_US) != TRUE) {
                returnEvent.EventType = ES_ERROR;
                break;
            }
            CurrentState = INDEXER_RUNNING;
            printf("Indexer running: %u us pulse\r\n",
                    INDEXER_RUN_PULSE_US);
        }
        break;

    case INDEXER_STOP:
        if (CurrentState == INDEXER_STARTUP) {
            ES_Timer_StopTimer(INDEXER_STARTUP_TIMER);
        }
        SetIndexerPulse(0);
        CurrentState = INDEXER_OFF;
        printf("Indexer stopped\r\n");
        break;

    case ES_EXIT:
        if (CurrentState == INDEXER_STARTUP) {
            ES_Timer_StopTimer(INDEXER_STARTUP_TIMER);
        }
        SetIndexerPulse(0);
        RC_End();
        CurrentState = INDEXER_OFF;
        printf("Indexer exited\r\n");
        break;

    default:
        break;
    }

    return returnEvent;
}
