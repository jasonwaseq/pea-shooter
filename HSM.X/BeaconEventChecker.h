/*
 * BeaconEventChecker.h
 *
 * Raw ADC event checker for the beacon detector output.
 */

#ifndef BEACON_EVENT_CHECKER_H
#define BEACON_EVENT_CHECKER_H

#include <stdint.h>

#include "AD.h"

/*
 * Wiring defaults:
 *   Detector analog output -> V3 analog input on the Uno32 I/O protection board.
 *
 * Change this define if your detector output is wired to a different analog
 * input.
 */
#ifndef BEACON_DETECTOR_AD_PIN
#define BEACON_DETECTOR_AD_PIN AD_PORTV3
#endif

#ifndef BEACON_DETECTOR_PIN_NAME
#define BEACON_DETECTOR_PIN_NAME "AD_PORTV3"
#endif

#ifndef BEACON_ADC_SAMPLE_DIVIDER
#define BEACON_ADC_SAMPLE_DIVIDER 1
#endif

uint8_t InitBeaconEventChecker(void);
uint8_t CheckBeaconDetector(void);

#endif /* BEACON_EVENT_CHECKER_H */
