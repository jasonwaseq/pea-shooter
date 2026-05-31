/*
 * BeaconDetectorService.c
 *
 * ES service that prints raw beacon ADC readings.
 */

#include "BeaconDetectorService.h"

#include <stdio.h>

#include "BOARD.h"
#include "BeaconEventChecker.h"
#include "ES_Framework.h"

static uint8_t MyPriority;

uint8_t InitBeaconDetectorService(uint8_t priority)
{
    ES_Event initEvent;

    MyPriority = priority;
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
        printf("adc1_pin=%s adc2_pin=%s\r\n",
                BEACON_1_DETECTOR_PIN_NAME,
                BEACON_2_DETECTOR_PIN_NAME);
        break;

    case BEACON_STRENGTH_CHANGED:
        printf("%u,%u\r\n", Beacon1ADCReading(), Beacon2ADCReading());
        break;

    default:
        break;
    }

    return returnEvent;
}
