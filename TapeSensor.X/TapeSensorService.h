/*
 * TapeSensorService.h
 */

#ifndef TAPE_SENSOR_SERVICE_H
#define TAPE_SENSOR_SERVICE_H

#include <stdint.h>

#include "ES_Configure.h"
#include "ES_Events.h"

uint8_t InitTapeSensorService(uint8_t priority);
uint8_t PostTapeSensorService(ES_Event thisEvent);
ES_Event RunTapeSensorService(ES_Event thisEvent);

#endif /* TAPE_SENSOR_SERVICE_H */
