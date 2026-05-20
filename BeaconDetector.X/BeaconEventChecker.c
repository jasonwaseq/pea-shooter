/*
 * BeaconEventChecker.c
 *
 * Converts analog beacon detector voltage readings into ES events.
 */

#include "BeaconEventChecker.h"

#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "BeaconDetectorService.h"

static uint16_t LastBeaconReading;
static uint8_t LastBeaconPresent;

static uint16_t ReadBeaconStrength(void)
{
    return AD_ReadADPin(BEACON_DETECTOR_AD_PIN);
}

static uint8_t IsBeaconPresent(uint16_t reading)
{
    if (LastBeaconPresent == TRUE) {
        return (reading > (BEACON_PRESENT_THRESHOLD - BEACON_LOST_HYSTERESIS)) ? TRUE : FALSE;
    }
    return (reading >= BEACON_PRESENT_THRESHOLD) ? TRUE : FALSE;
}

uint8_t InitBeaconEventChecker(void)
{
    if ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
        if (AD_AddPins(BEACON_DETECTOR_AD_PIN) != SUCCESS) {
            return FALSE;
        }
        while ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
            ;
        }
    }

    LastBeaconReading = ReadBeaconStrength();
    if (LastBeaconReading == (uint16_t) ERROR) {
        return FALSE;
    }

    LastBeaconPresent = IsBeaconPresent(LastBeaconReading);
    return TRUE;
}

uint8_t CheckBeaconDetector(void)
{
    ES_Event thisEvent;
    uint16_t currentReading;
    uint8_t currentBeaconPresent;
    uint16_t readingDelta;

    if (AD_IsNewDataReady() != TRUE) {
        return FALSE;
    }

    currentReading = ReadBeaconStrength();
    if (currentReading == (uint16_t) ERROR) {
        return FALSE;
    }

    currentBeaconPresent = IsBeaconPresent(currentReading);
    if (currentBeaconPresent != LastBeaconPresent) {
        LastBeaconPresent = currentBeaconPresent;
        LastBeaconReading = currentReading;
        thisEvent.EventType = currentBeaconPresent ? BEACON_DETECTED : BEACON_LOST;
        thisEvent.EventParam = currentReading;
        PostBeaconDetectorService(thisEvent);
        return TRUE;
    }

    readingDelta = (currentReading > LastBeaconReading)
            ? (currentReading - LastBeaconReading)
            : (LastBeaconReading - currentReading);
    if (readingDelta < BEACON_STRENGTH_DELTA) {
        return FALSE;
    }

    LastBeaconReading = currentReading;
    thisEvent.EventType = BEACON_STRENGTH_CHANGED;
    thisEvent.EventParam = currentReading;
    PostBeaconDetectorService(thisEvent);

    return TRUE;
}
