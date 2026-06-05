/*
 * TapeSensorEventChecker.h
 *
 * Debounced event checker for the three digital tape sensor inputs.
 */

#ifndef TAPE_SENSOR_EVENT_CHECKER_H
#define TAPE_SENSOR_EVENT_CHECKER_H

#include <stdint.h>

#define TAPE_SENSOR_LEFT_MASK   0x01
#define TAPE_SENSOR_MIDDLE_MASK 0x02
#define TAPE_SENSOR_RIGHT_MASK  0x04

#ifndef TAPE_SENSOR_DEBOUNCE_COUNT
#define TAPE_SENSOR_DEBOUNCE_COUNT 20
#endif

uint8_t InitTapeSensorEventChecker(void);
uint8_t CheckTapeSensor(void);
uint8_t TapeSensorRawMask(void);
uint8_t TapeSensorStableMask(void);

#endif /* TAPE_SENSOR_EVENT_CHECKER_H */
