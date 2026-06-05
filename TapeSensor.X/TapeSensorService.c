/*
 * TapeSensorService.c
 *
 * ES service that prints tape sensor events and the current sensor mask.
 */

#include "TapeSensorService.h"

#include <stdio.h>

#include "BOARD.h"
#include "ES_Framework.h"
#include "TapeSensorEventChecker.h"

static uint8_t MyPriority;

static void PrintTapeState(const char *prefix, ES_Event thisEvent)
{
    uint8_t stableMask;

    stableMask = TapeSensorStableMask();
    printf("%s event=%s param=%u stable_mask=0x%X L=%u M=%u R=%u raw_mask=0x%X\r\n",
            prefix,
            EventNames[thisEvent.EventType],
            thisEvent.EventParam,
            stableMask,
            (stableMask & TAPE_SENSOR_LEFT_MASK) ? 1 : 0,
            (stableMask & TAPE_SENSOR_MIDDLE_MASK) ? 1 : 0,
            (stableMask & TAPE_SENSOR_RIGHT_MASK) ? 1 : 0,
            TapeSensorRawMask());
}

uint8_t InitTapeSensorService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
    if (InitTapeSensorEventChecker() != TRUE) {
        return FALSE;
    }

    initEvent.EventType = ES_INIT;
    initEvent.EventParam = 0;
    return ES_PostToService(MyPriority, initEvent);
}

uint8_t PostTapeSensorService(ES_Event thisEvent)
{
    return ES_PostToService(MyPriority, thisEvent);
}

ES_Event RunTapeSensorService(ES_Event thisEvent)
{
    ES_Event returnEvent;

    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    switch (thisEvent.EventType) {
    case ES_INIT:
        printf("Tape pins: left=W3 middle=W6 right=W5\r\n");
        PrintTapeState("initial", thisEvent);
        break;

    case LEFT_TAPE_DETECTED:
    case LEFT_TAPE_NOT_DETECTED:
    case MIDDLE_TAPE_DETECTED:
    case MIDDLE_TAPE_NOT_DETECTED:
    case RIGHT_TAPE_DETECTED:
    case RIGHT_TAPE_NOT_DETECTED:
    case NO_TAPE_DETECTED:
        PrintTapeState("transition", thisEvent);
        break;

    default:
        break;
    }

    return returnEvent;
}
