/*
 * IndexerEventChecker.h
 *
 * Beacon ADC event checker for the indexer project.
 */

#ifndef INDEXER_EVENT_CHECKER_H
#define INDEXER_EVENT_CHECKER_H

#include <stdint.h>

#include "AD.h"

/*
 * X3 is not an ADC-capable pin on this I/O board. Feed the analog beacon
 * detector voltage into a V or W analog input. Default: V3.
 */
#ifndef BEACON_DETECTOR_AD_PIN
#define BEACON_DETECTOR_AD_PIN AD_PORTV3
#endif

#ifndef BEACON_STRENGTH_DELTA
#define BEACON_STRENGTH_DELTA 4
#endif

#ifndef BEACON_FILTER_SHIFT
#define BEACON_FILTER_SHIFT 2
#endif

#ifndef BEACON_CALIBRATION_SAMPLES
#define BEACON_CALIBRATION_SAMPLES 128
#endif

#ifndef BEACON_DETECT_DELTA_THRESHOLD
#define BEACON_DETECT_DELTA_THRESHOLD 20
#endif

#ifndef BEACON_LOST_DELTA_THRESHOLD
#define BEACON_LOST_DELTA_THRESHOLD 6
#endif

#ifndef BEACON_SCORE_DETECT
#define BEACON_SCORE_DETECT 12
#endif

#ifndef BEACON_SCORE_LOST
#define BEACON_SCORE_LOST 0
#endif

#ifndef BEACON_SCORE_MAX
#define BEACON_SCORE_MAX 40
#endif

#ifndef BEACON_REPORT_INTERVAL_SAMPLES
#define BEACON_REPORT_INTERVAL_SAMPLES 1000
#endif

uint8_t InitIndexerEventChecker(void);
uint8_t CheckIndexerEvents(void);
uint16_t BeaconRawReading(void);
uint16_t BeaconFilteredReading(void);
uint16_t BeaconBaselineReading(void);
uint16_t BeaconExcessReading(void);
uint8_t BeaconConfidenceScore(void);

#endif /* INDEXER_EVENT_CHECKER_H */
