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

static uint8_t SampleSequence;

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

    SampleSequence = 0;
    return TRUE;
}

uint8_t BeaconEventChecker_GetSampleSequence(void)
{
    return SampleSequence;
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
    SampleSequence = (SampleSequence + 1) & BEACON_SAMPLE_SEQ_MASK;

    /* Ignore readings taken while the base motors are running. Only samples
     * captured during the motors-off hold/pause may complete a beacon lock,
     * which prevents motor noise from producing a false detection during the
     * incremental tank turn. */
    if (HSM_BeaconSampleArmed() != TRUE) {
        return FALSE;
    }

    /* Detection is evaluated on every armed ADC sample, but only a genuine
     * detection is posted to the HSM. Posting every non-detected reading would
     * flood the service queue and could drop the rotation-timer timeout that
     * drives the incremental tank turn, stalling the turn before a beacon is
     * ever found. */
    if (HSM_CheckBeaconLockImmediate(currentReading) == TRUE) {
        thisEvent.EventType = BEACON_SAMPLE_READY;
        thisEvent.EventParam = BEACON_SAMPLE_EVENT_PARAM(currentReading,
                SampleSequence);
        PostHSMService(thisEvent);
        return TRUE;
    }

    return FALSE;
}
