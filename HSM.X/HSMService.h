/*
 * HSMService.h
 *
 * Public service interface for the HSM beacon alignment project.
 */

#ifndef HSM_SERVICE_H
#define HSM_SERVICE_H

#include <stdint.h>

#include "ES_Configure.h"
#include "ES_Events.h"

uint8_t InitHSMService(uint8_t priority);
uint8_t PostHSMService(ES_Event thisEvent);
ES_Event RunHSMService(ES_Event thisEvent);
uint8_t HSM_TapeEventsEnabled(void);
uint8_t HSM_BeaconAlignFastStopEnabled(void);
uint8_t HSM_BeaconSampleArmed(void);
uint8_t HSM_CheckBeaconLockImmediate(uint16_t adcReading);
uint8_t HSM_CheckISZImmediate(unsigned char tapeMask);
uint8_t HSM_CheckDriveImmediate(unsigned char tapeMask);
uint8_t HSM_CheckTravelTapeImmediate(unsigned char tapeMask);

#endif /* HSM_SERVICE_H */
