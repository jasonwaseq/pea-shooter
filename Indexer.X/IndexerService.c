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

static uint8_t SetIndexerDuty(unsigned int duty)
{
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(INDEXER_PWM_PIN, duty) == SUCCESS;
}

uint8_t InitIndexerService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
    CurrentState = INDEXER_OFF;

    if (PWM_Init() != SUCCESS) {
        return FALSE;
    }
    if (PWM_SetFrequency(INDEXER_PWM_FREQUENCY) != SUCCESS) {
        return FALSE;
    }
    if (PWM_AddPins(INDEXER_PWM_PIN) != SUCCESS) {
        return FALSE;
    }
    if (SetIndexerDuty(MIN_PWM) != TRUE) {
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
        printf("Indexer ready on %s using hardware PWM\r\n",
                INDEXER_OUTPUT_PIN_NAME);
        printf("PWM frequency: %u Hz\r\n", PWM_GetFrequency());
        break;

    case INDEXER_START:
        if (SetIndexerDuty(INDEXER_STARTUP_DUTY) != TRUE) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        if (ES_Timer_InitTimer(INDEXER_STARTUP_TIMER,
                INDEXER_STARTUP_TIME_MS) != ES_Timer_OK) {
            returnEvent.EventType = ES_ERROR;
            break;
        }
        CurrentState = INDEXER_STARTUP;
        printf("Indexer startup: %u.%u%% duty for %u ms\r\n",
                INDEXER_STARTUP_DUTY / 10,
                INDEXER_STARTUP_DUTY % 10,
                INDEXER_STARTUP_TIME_MS);
        break;

    case ES_TIMEOUT:
        if ((thisEvent.EventParam == INDEXER_STARTUP_TIMER)
                && (CurrentState == INDEXER_STARTUP)) {
            if (SetIndexerDuty(INDEXER_RUN_DUTY) != TRUE) {
                returnEvent.EventType = ES_ERROR;
                break;
            }
            CurrentState = INDEXER_RUNNING;
            printf("Indexer running: %u.%u%% duty\r\n",
                    INDEXER_RUN_DUTY / 10,
                    INDEXER_RUN_DUTY % 10);
        }
        break;

    case INDEXER_STOP:
        if (CurrentState == INDEXER_STARTUP) {
            ES_Timer_StopTimer(INDEXER_STARTUP_TIMER);
        }
        SetIndexerDuty(MIN_PWM);
        CurrentState = INDEXER_OFF;
        printf("Indexer stopped\r\n");
        break;

    case ES_EXIT:
        if (CurrentState == INDEXER_STARTUP) {
            ES_Timer_StopTimer(INDEXER_STARTUP_TIMER);
        }
        SetIndexerDuty(MIN_PWM);
        PWM_End();
        CurrentState = INDEXER_OFF;
        printf("Indexer exited\r\n");
        break;

    default:
        break;
    }

    return returnEvent;
}
