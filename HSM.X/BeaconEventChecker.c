/*
 * BeaconEventChecker.c
 *
 * Posts raw beacon detector ADC readings to the HSM service.
 */

#include "BeaconEventChecker.h"

#include <stdio.h>

#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "HSMService.h"

static uint16_t SampleDividerCount;

static uint16_t ReadBeaconStrength(void)
{
    return AD_ReadADPin(BEACON_DETECTOR_AD_PIN);
}

uint8_t InitBeaconEventChecker(void)
{
    if ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
        if (AD_AddPins(BEACON_DETECTOR_AD_PIN) != SUCCESS) {
            printf("HSM init failed: AD_AddPins beacon\r\n");
            return FALSE;
        }
        while ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
            ;
        }
    }

    if (ReadBeaconStrength() == (uint16_t) ERROR) {
        printf("HSM init failed: beacon ADC read\r\n");
        return FALSE;
    }

    SampleDividerCount = 0;
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

    SampleDividerCount++;
    if (SampleDividerCount < BEACON_ADC_SAMPLE_DIVIDER) {
        return FALSE;
    }
    SampleDividerCount = 0;

    thisEvent.EventType = BEACON_SAMPLE_READY;
    thisEvent.EventParam = currentReading;
    PostHSMService(thisEvent);

    return TRUE;
}
