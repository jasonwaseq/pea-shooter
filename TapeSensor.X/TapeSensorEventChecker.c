/*
 * TapeSensorEventChecker.c
 *
 * Posts one tape sensor transition at a time so the monitor can show which
 * LEFT/MIDDLE/RIGHT tape event is being generated.
 */

#include "TapeSensorEventChecker.h"

#include <stdio.h>
#include <xc.h>

#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "IO_Ports.h"
#include "TapeSensorService.h"

#define TAPE_DETECTED 1
#define TAPE_NOT_DETECTED 0
static uint8_t DebouncedLeft;
static uint8_t DebouncedMiddle;
static uint8_t DebouncedRight;

static uint8_t LastLeftSample;
static uint8_t LastMiddleSample;
static uint8_t LastRightSample;

static uint8_t LeftCount;
static uint8_t MiddleCount;
static uint8_t RightCount;

static uint8_t RawMask;
static uint8_t StableMask;

static uint8_t ReadLeftTape(void)
{
    return PORTW03_BIT ? TAPE_DETECTED : TAPE_NOT_DETECTED;
}

static uint8_t ReadMiddleTape(void)
{
    return PORTW06_BIT ? TAPE_DETECTED : TAPE_NOT_DETECTED;
}

static uint8_t ReadRightTape(void)
{
    return PORTW05_BIT ? TAPE_DETECTED : TAPE_NOT_DETECTED;
}

static uint8_t PackTapeMask(uint8_t leftTape, uint8_t middleTape,
        uint8_t rightTape)
{
    return (leftTape
            | (middleTape << 1)
            | (rightTape << 2));
}

static void UpdateRawMask(uint8_t leftTape, uint8_t middleTape,
        uint8_t rightTape)
{
    RawMask = PackTapeMask(leftTape, middleTape, rightTape);
}

static void UpdateStableMask(void)
{
    StableMask = PackTapeMask(DebouncedLeft, DebouncedMiddle, DebouncedRight);
}

static void UpdateDebounceCounter(uint8_t sample, uint8_t *lastSample,
        uint8_t *count)
{
    if (sample == *lastSample) {
        if (*count < TAPE_SENSOR_DEBOUNCE_COUNT) {
            (*count)++;
        }
    } else {
        *lastSample = sample;
        *count = 1;
    }
}

static uint8_t PostTapeEvent(ES_EventTyp_t eventType, uint8_t eventParam)
{
    ES_Event thisEvent;

    thisEvent.EventType = eventType;
    thisEvent.EventParam = eventParam;
    return PostTapeSensorService(thisEvent);
}

uint8_t InitTapeSensorEventChecker(void)
{
    uint8_t leftTape;
    uint8_t middleTape;
    uint8_t rightTape;

    PORTW03_TRIS = 1;
    PORTW05_TRIS = 1;
    PORTW06_TRIS = 1;

    leftTape = ReadLeftTape();
    middleTape = ReadMiddleTape();
    rightTape = ReadRightTape();

    DebouncedLeft = leftTape;
    DebouncedMiddle = middleTape;
    DebouncedRight = rightTape;

    LastLeftSample = leftTape;
    LastMiddleSample = middleTape;
    LastRightSample = rightTape;

    LeftCount = 0;
    MiddleCount = 0;
    RightCount = 0;

    UpdateRawMask(leftTape, middleTape, rightTape);
    UpdateStableMask();

    return TRUE;
}

uint8_t CheckTapeSensor(void)
{
    uint8_t leftTape;
    uint8_t middleTape;
    uint8_t rightTape;
    leftTape = ReadLeftTape();
    middleTape = ReadMiddleTape();
    rightTape = ReadRightTape();
    UpdateRawMask(leftTape, middleTape, rightTape);

    UpdateDebounceCounter(leftTape, &LastLeftSample, &LeftCount);
    UpdateDebounceCounter(middleTape, &LastMiddleSample, &MiddleCount);
    UpdateDebounceCounter(rightTape, &LastRightSample, &RightCount);

    if ((LeftCount >= TAPE_SENSOR_DEBOUNCE_COUNT)
            && (leftTape != DebouncedLeft)) {
        DebouncedLeft = leftTape;
        UpdateStableMask();
        return PostTapeEvent((DebouncedLeft == TAPE_DETECTED)
                ? LEFT_TAPE_DETECTED : LEFT_TAPE_NOT_DETECTED,
                DebouncedLeft);
    }

    if ((MiddleCount >= TAPE_SENSOR_DEBOUNCE_COUNT)
            && (middleTape != DebouncedMiddle)) {
        DebouncedMiddle = middleTape;
        UpdateStableMask();
        return PostTapeEvent((DebouncedMiddle == TAPE_DETECTED)
                ? MIDDLE_TAPE_DETECTED : MIDDLE_TAPE_NOT_DETECTED,
                DebouncedMiddle);
    }

    if ((RightCount >= TAPE_SENSOR_DEBOUNCE_COUNT)
            && (rightTape != DebouncedRight)) {
        DebouncedRight = rightTape;
        UpdateStableMask();
        return PostTapeEvent((DebouncedRight == TAPE_DETECTED)
                ? RIGHT_TAPE_DETECTED : RIGHT_TAPE_NOT_DETECTED,
                DebouncedRight);
    }

    return FALSE;
}

uint8_t TapeSensorRawMask(void)
{
    return RawMask;
}

uint8_t TapeSensorStableMask(void)
{
    return StableMask;
}
