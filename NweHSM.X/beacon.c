#include "Beacon.h"

#include <stdio.h>

#include "ES_Timers.h"
#include "peashooter.h"
#include "ES_Configure.h"

#define HSM_FULL_ROTATION_TIME_MS 5000
#define HSM_LOCK_ON_START_DELAY_MS 250

#define HSM_BEACON_AVG_WINDOW 8
#define HSM_BEACON_LOCK_OFFSET 10

#define HSM_SWEEP_TURN_POWER 500
#define HSM_LOCK_TURN_POWER 500
#define HSM_DRIVE_POWER 700

static uint16_t BeaconSamples[HSM_BEACON_AVG_WINDOW];
static uint8_t BeaconSampleIndex;
static uint8_t BeaconSampleCount;
static uint32_t BeaconSampleSum;

static uint16_t MaxFilteredBeaconValue;
static uint16_t BeaconLockThreshold;
static uint8_t LockOnTurnStarted;
static uint16_t LockOnRotationMax;



void Beacon_StartSweepData(void)
{
    Beacon_ResetFilter();
    MaxFilteredBeaconValue = 0;

    printf("Beacon: SWEEP_DATA start\r\n");

    PS_RightMtrSpeed(HSM_SWEEP_TURN_POWER);
    PS_LeftMtrSpeed(HSM_SWEEP_TURN_POWER * -1);

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

void Beacon_StartLockOn(void)
{
    Beacon_ResetFilter();
    PS_Stop();

    LockOnTurnStarted = FALSE;

    if (MaxFilteredBeaconValue > HSM_BEACON_LOCK_OFFSET) {
        BeaconLockThreshold = MaxFilteredBeaconValue - HSM_BEACON_LOCK_OFFSET;
    } else {
        BeaconLockThreshold = 0;
    }

    printf("Beacon: LOCK_ON start max=%u threshold=%u\r\n",
            MaxFilteredBeaconValue, BeaconLockThreshold);

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_ON_START_DELAY_MS);
}

void Beacon_StartLockOnTurn(void)
{
    LockOnTurnStarted = TRUE;
    LockOnRotationMax = 0;

    Beacon_ResetFilter();

    printf("Beacon: LOCK_ON tank turn\r\n");

    PS_RightMtrSpeed(HSM_LOCK_TURN_POWER);
    PS_LeftMtrSpeed(HSM_LOCK_TURN_POWER * -1);

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

void Beacon_StartNextLockOnRotation(void)
{
    if (LockOnRotationMax > 0) {
        MaxFilteredBeaconValue = LockOnRotationMax;
    }

    if (MaxFilteredBeaconValue > HSM_BEACON_LOCK_OFFSET) {
        BeaconLockThreshold = MaxFilteredBeaconValue - HSM_BEACON_LOCK_OFFSET;
    } else {
        BeaconLockThreshold = 0;
    }

    printf("Beacon: LOCK_ON retry max=%u threshold=%u\r\n",
            MaxFilteredBeaconValue, BeaconLockThreshold);

    LockOnRotationMax = 0;

    Beacon_ResetFilter();

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

void Beacon_ResetFilter(void)
{
    uint8_t i;

    BeaconSampleIndex = 0;
    BeaconSampleCount = 0;
    BeaconSampleSum = 0;

    for (i = 0; i < HSM_BEACON_AVG_WINDOW; i++) {
        BeaconSamples[i] = 0;
    }
}

uint16_t Beacon_FilterSample(uint16_t sample)
{
    if (BeaconSampleCount < HSM_BEACON_AVG_WINDOW) {
        BeaconSampleCount++;
    } else {
        BeaconSampleSum -= BeaconSamples[BeaconSampleIndex];
    }

    BeaconSamples[BeaconSampleIndex] = sample;
    BeaconSampleSum += sample;

    BeaconSampleIndex++;

    if (BeaconSampleIndex >= HSM_BEACON_AVG_WINDOW) {
        BeaconSampleIndex = 0;
    }

    return (uint16_t)(BeaconSampleSum / BeaconSampleCount);
}

void Beacon_DriveBackward(void)
{
    PS_Backward(HSM_DRIVE_POWER);
}

uint16_t Beacon_GetMaxFilteredValue(void)
{
    return MaxFilteredBeaconValue;
}

void Beacon_SetMaxFilteredValue(uint16_t value)
{
    MaxFilteredBeaconValue = value;
}

uint16_t Beacon_GetLockThreshold(void)
{
    return BeaconLockThreshold;
}

uint8_t Beacon_GetLockOnTurnStarted(void)
{
    return LockOnTurnStarted;
}

uint16_t Beacon_GetLockOnRotationMax(void)
{
    return LockOnRotationMax;
}

void Beacon_SetLockOnRotationMax(uint16_t value)
{
    LockOnRotationMax = value;
}