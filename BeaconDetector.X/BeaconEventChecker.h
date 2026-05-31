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
 *   Detector 1 analog output -> V3 analog input on the Uno32 I/O protection board.
 *   Detector 2 analog output -> V5 analog input on the Uno32 I/O protection board.
 *
 * Change these defines if either detector output is wired to a different analog
 * input. PortV-05 / AD_PORTV5 is chosen for detector 2 because it is not listed
 * as claimed in pin_conflicts_report.md.
 */
#ifndef BEACON_1_DETECTOR_AD_PIN
#define BEACON_1_DETECTOR_AD_PIN AD_PORTV3
#endif

#ifndef BEACON_1_DETECTOR_PIN_NAME
#define BEACON_1_DETECTOR_PIN_NAME "AD_PORTV3"
#endif

#ifndef BEACON_2_DETECTOR_AD_PIN
#define BEACON_2_DETECTOR_AD_PIN AD_PORTV5
#endif

#ifndef BEACON_2_DETECTOR_PIN_NAME
#define BEACON_2_DETECTOR_PIN_NAME "AD_PORTV5"
#endif

#ifndef BEACON_ADC_PRINT_DIVIDER
#define BEACON_ADC_PRINT_DIVIDER 100
#endif

uint8_t InitBeaconEventChecker(void);
uint8_t CheckBeaconDetector(void);
uint16_t Beacon1ADCReading(void);
uint16_t Beacon2ADCReading(void);

#endif /* BEACON_EVENT_CHECKER_H */
