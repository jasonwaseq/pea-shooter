/*
 * ShooterEventChecker.c
 *
 * The first shooter project only starts the motor from main.c. This hook keeps
 * the ES Framework event-checker list valid for later sensors or buttons.
 */

#include "ShooterEventChecker.h"

#include <stdio.h>

#include "BOARD.h"

static uint16_t PrintCounter;

uint8_t InitShooterEventChecker(void)
{
    if ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
        if (AD_AddPins(BEACON_DETECTOR_AD_PIN) != SUCCESS) {
            return FALSE;
        }
        while ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
            ;
        }
    }

    PrintCounter = 0;
    printf("Beacon detector ADC input: %s\r\n", BEACON_DETECTOR_PIN_NAME);
    return TRUE;
}

uint8_t CheckShooterEvents(void)
{
    uint16_t reading;

    if (AD_IsNewDataReady() != TRUE) {
        return FALSE;
    }

    reading = AD_ReadADPin(BEACON_DETECTOR_AD_PIN);
    if (reading == (uint16_t) ERROR) {
        return FALSE;
    }

    PrintCounter++;
    if (PrintCounter >= BEACON_ADC_PRINT_DIVIDER) {
        PrintCounter = 0;
        printf("beacon_adc=%u\r\n", reading);
        return TRUE;
    }

    return FALSE;
}
