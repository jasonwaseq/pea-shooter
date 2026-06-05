#ifndef BEACON_H
#define BEACON_H

#include <stdint.h>

void Beacon_StartSweepData(void);
void Beacon_StartLockOn(void);
void Beacon_StartLockOnTurn(void);
void Beacon_StartNextLockOnRotation(void);

void Beacon_ResetFilter(void);
uint16_t Beacon_FilterSample(uint16_t sample);

void Beacon_DriveBackward(void);

uint16_t Beacon_GetMaxFilteredValue(void);
void Beacon_SetMaxFilteredValue(uint16_t value);

uint16_t Beacon_GetLockThreshold(void);
uint8_t Beacon_GetLockOnTurnStarted(void);
uint16_t Beacon_GetLockOnRotationMax(void);
void Beacon_SetLockOnRotationMax(uint16_t value);

#endif