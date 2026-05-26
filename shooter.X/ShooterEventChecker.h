/*
 * ShooterEventChecker.h
 *
 * Empty event checker for the starter shooter motor project.
 */

#ifndef SHOOTER_EVENT_CHECKER_H
#define SHOOTER_EVENT_CHECKER_H

#include <stdint.h>

#include "AD.h"

/*
 * Beacon detector analog output defaults to V3 on the Uno32 I/O protection
 * stack. Change this if the detector is wired to a different analog input.
 */
#ifndef BEACON_DETECTOR_AD_PIN
#define BEACON_DETECTOR_AD_PIN AD_PORTV3
#endif

#ifndef BEACON_DETECTOR_PIN_NAME
#define BEACON_DETECTOR_PIN_NAME "AD_PORTV3"
#endif

#ifndef BEACON_ADC_PRINT_DIVIDER
#define BEACON_ADC_PRINT_DIVIDER 100
#endif

uint8_t InitShooterEventChecker(void);
uint8_t CheckShooterEvents(void);

#endif /* SHOOTER_EVENT_CHECKER_H */
