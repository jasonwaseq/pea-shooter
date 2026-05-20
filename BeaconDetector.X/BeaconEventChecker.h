/*
 * BeaconEventChecker.h
 *
 * Analog input event checker for a 2 kHz beacon detector strength output.
 */

#ifndef BEACON_EVENT_CHECKER_H
#define BEACON_EVENT_CHECKER_H

#include <stdint.h>

#include "AD.h"

/*
 * Wiring defaults:
 *   Detector analog output -> V3 analog input on the Uno32 I/O protection board.
 *
 * Change these defines if your detector output is wired to a different analog
 * input or needs a different detection threshold.
 */
#ifndef BEACON_DETECTOR_AD_PIN
#define BEACON_DETECTOR_AD_PIN AD_PORTV3
#endif

/*
 * ADC readings are 10-bit values from 0..1023. Tune these values using serial
 * prints from BeaconDetectorService while testing your actual detector.
 */
#ifndef BEACON_PRESENT_THRESHOLD
#define BEACON_PRESENT_THRESHOLD 512
#endif

#ifndef BEACON_LOST_HYSTERESIS
#define BEACON_LOST_HYSTERESIS 32
#endif

#ifndef BEACON_STRENGTH_DELTA
#define BEACON_STRENGTH_DELTA 8
#endif

uint8_t InitBeaconEventChecker(void);
uint8_t CheckBeaconDetector(void);

#endif /* BEACON_EVENT_CHECKER_H */
