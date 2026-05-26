/*
 * BeaconEventChecker.c
 *
 * Posts raw beacon detector ADC readings.
 */

#include "BeaconEventChecker.h"

#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "BeaconDetectorService.h"

static uint16_t PrintCounter;

static uint16_t ReadBeaconStrength(void)
{
    return AD_ReadADPin(BEACON_DETECTOR_AD_PIN);
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

    if (ReadBeaconStrength() == (uint16_t) ERROR) {
        return FALSE;
    }

    PrintCounter = 0;
    return TRUE;
}

uint8_t CheckBeaconDetector(void)
{
    ES_Event thisEvent;
    uint16_t currentReading;

    if (AD_IsNewDataReady() != TRUE) {
        return FALSE;
    }

    currentReading = ReadBeaconStrength();
    if (currentReading == (uint16_t) ERROR) {
        return FALSE;
    }

    PrintCounter++;
    if (PrintCounter < BEACON_ADC_PRINT_DIVIDER) {
        return FALSE;
    }
    PrintCounter = 0;

    thisEvent.EventType = BEACON_STRENGTH_CHANGED;
    thisEvent.EventParam = currentReading;
    PostBeaconDetectorService(thisEvent);

    return TRUE;
}
