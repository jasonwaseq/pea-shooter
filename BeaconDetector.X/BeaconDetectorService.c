/*
 * BeaconDetectorService.c
 *
 * ES service that receives analog beacon strength and threshold events.
 */

#include "BeaconDetectorService.h"

#include <stdio.h>

#include "BOARD.h"
#include "BeaconEventChecker.h"
#include "ES_Framework.h"

static uint8_t MyPriority;
static uint8_t BeaconPresent;
static uint16_t BeaconStrength;

uint8_t InitBeaconDetectorService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
    BeaconPresent = FALSE;
    BeaconStrength = 0;

    if (InitBeaconEventChecker() != TRUE) {
        return FALSE;
    }

    initEvent.EventType = ES_INIT;
    initEvent.EventParam = 0;
    return ES_PostToService(MyPriority, initEvent);
}

uint8_t PostBeaconDetectorService(ES_Event thisEvent)
{
    return ES_PostToService(MyPriority, thisEvent);
}

ES_Event RunBeaconDetectorService(ES_Event thisEvent)
{
    ES_Event returnEvent;

    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    switch (thisEvent.EventType) {
    case ES_INIT:
        printf("BeaconDetectorService initialized; reading analog beacon strength\r\n");
        break;

    case BEACON_STRENGTH_CHANGED:
        BeaconStrength = thisEvent.EventParam;
        printf("Beacon strength ADC: %u\r\n", BeaconStrength);
        break;

    case BEACON_DETECTED:
        BeaconPresent = TRUE;
        BeaconStrength = thisEvent.EventParam;
        printf("Beacon detected, ADC: %u\r\n", BeaconStrength);
        break;

    case BEACON_LOST:
        BeaconPresent = FALSE;
        BeaconStrength = thisEvent.EventParam;
        printf("Beacon lost, ADC: %u\r\n", BeaconStrength);
        break;

    default:
        break;
    }

    return returnEvent;
}
