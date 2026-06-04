#ifndef BEACON_DETECTOR_SERVICE_H
#define BEACON_DETECTOR_SERVICE_H

#include <stdint.h>
#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitBeaconDetectorService(uint8_t priority);
uint8_t PostBeaconDetectorService(ES_Event thisEvent);
ES_Event RunBeaconDetectorService(ES_Event thisEvent);

#endif