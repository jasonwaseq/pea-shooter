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
static uint16_t Beacon1Reading;
static uint16_t Beacon2Reading;

static uint16_t ReadBeacon1(void)
{
    return AD_ReadADPin(BEACON_1_DETECTOR_AD_PIN);
}

static uint16_t ReadBeacon2(void)
{
    return AD_ReadADPin(BEACON_2_DETECTOR_AD_PIN);
}

uint8_t InitBeaconEventChecker(void)
{
    unsigned int pinsToEnable;

    pinsToEnable = (BEACON_1_DETECTOR_AD_PIN | BEACON_2_DETECTOR_AD_PIN)
            & ~AD_ActivePins();
    if (pinsToEnable != 0) {
        if (AD_AddPins(pinsToEnable) != SUCCESS) {
            return FALSE;
        }
        while ((AD_ActivePins()
                & (BEACON_1_DETECTOR_AD_PIN | BEACON_2_DETECTOR_AD_PIN))
                != (BEACON_1_DETECTOR_AD_PIN | BEACON_2_DETECTOR_AD_PIN)) {
            ;
        }
    }

    Beacon1Reading = ReadBeacon1();
    Beacon2Reading = ReadBeacon2();
    if ((Beacon1Reading == (uint16_t) ERROR)
            || (Beacon2Reading == (uint16_t) ERROR)) {
        return FALSE;
    }

    PrintCounter = 0;
    return TRUE;
}

uint8_t CheckBeaconDetector(void)
{
    ES_Event thisEvent;

    if (AD_IsNewDataReady() != TRUE) {
        return FALSE;
    }

    Beacon1Reading = ReadBeacon1();
    Beacon2Reading = ReadBeacon2();
    if ((Beacon1Reading == (uint16_t) ERROR)
            || (Beacon2Reading == (uint16_t) ERROR)) {
        return FALSE;
    }

    PrintCounter++;
    if (PrintCounter < BEACON_ADC_PRINT_DIVIDER) {
        return FALSE;
    }
    PrintCounter = 0;

    thisEvent.EventType = BEACON_STRENGTH_CHANGED;
    thisEvent.EventParam = Beacon1Reading;
    PostBeaconDetectorService(thisEvent);

    return TRUE;
}

uint16_t Beacon1ADCReading(void)
{
    return Beacon1Reading;
}

uint16_t Beacon2ADCReading(void)
{
    return Beacon2Reading;
}
