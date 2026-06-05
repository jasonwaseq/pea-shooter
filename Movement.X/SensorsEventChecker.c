/*
 * File:   SensorsEventChecker.c
 * Author: Gabriel Hugh Elkaim
 */

#include "ES_Configure.h"
#include "SensorsEventChecker.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include "BeaconEventChecker.h"
#include "peashooter.h"
#include "MainHSM.h"
#include "ping.c"
#include "pwm.h"
#include <stdio.h>

#define BATTERY_DISCONNECT_THRESHOLD 175
#define SENSOR_STATE_UNKNOWN 0xFF
#define TAPE_DEBOUNCE_COUNT 20
#define PING_SAMPLE_INTERVAL_US 50000u

//#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

uint8_t TemplateCheckBattery(void) {
    static ES_EventTyp_t lastEvent = BATTERY_DISCONNECTED;
    static uint8_t initialized = FALSE;
    ES_EventTyp_t curEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint16_t batVoltage = AD_ReadADPin(BAT_VOLTAGE);

    if (batVoltage > BATTERY_DISCONNECT_THRESHOLD) {
        curEvent = BATTERY_CONNECTED;
    } else {
        curEvent = BATTERY_DISCONNECTED;
    }

    if (initialized == FALSE) {
        lastEvent = curEvent;
        initialized = TRUE;
        return returnVal;
    }

    if (curEvent != lastEvent) {
        thisEvent.EventType = curEvent;
        thisEvent.EventParam = batVoltage;
        returnVal = TRUE;
        lastEvent = curEvent;

#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
        PostMainHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif
    }

    return returnVal;
}

uint8_t TemplateCheckSwitch(void) {
    static ES_EventTyp_t lastEvent = ES_NO_EVENT;
    static unsigned char debouncedState = SENSOR_STATE_UNKNOWN;
    static unsigned char lastSample = SWITCH_NOT_TRIPPED;
    static unsigned char sameSampleCount = 0;
    static uint8_t initialized = FALSE;

    ES_EventTyp_t curEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    unsigned char switchState = PS_ReadSwitch();

    enum {
        SWITCH_DEBOUNCE_COUNT = 150
    };

    if (initialized == FALSE) {
        debouncedState = switchState;
        lastSample = switchState;
        sameSampleCount = 0;

        if (switchState == SWITCH_TRIPPED) {
            lastEvent = BUMPER_TRIPPED;
        } else {
            lastEvent = BUMPER_NOT_TRIPPED;
        }

        initialized = TRUE;
        return returnVal;
    }

    if (switchState == lastSample) {
        if (sameSampleCount < SWITCH_DEBOUNCE_COUNT) {
            sameSampleCount++;
        }
    } else {
        lastSample = switchState;
        sameSampleCount = 1;
        return returnVal;
    }

    if ((sameSampleCount >= SWITCH_DEBOUNCE_COUNT)
            && (switchState != debouncedState)) {
        debouncedState = switchState;

        if (debouncedState == SWITCH_TRIPPED) {
            curEvent = BUMPER_TRIPPED;
        } else {
            curEvent = BUMPER_NOT_TRIPPED;
        }

        if (curEvent != lastEvent) {
            thisEvent.EventType = curEvent;
            thisEvent.EventParam = debouncedState;
            returnVal = TRUE;
            lastEvent = curEvent;

#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
            PostMainHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif
        }
    }

    return returnVal;
}

uint8_t TemplateCheckTape(void) {
    static unsigned char debouncedLeft = SENSOR_STATE_UNKNOWN;
    static unsigned char debouncedMiddle = SENSOR_STATE_UNKNOWN;
    static unsigned char debouncedRight = SENSOR_STATE_UNKNOWN;

    static unsigned char lastLeftSample = SENSOR_STATE_UNKNOWN;
    static unsigned char lastMiddleSample = SENSOR_STATE_UNKNOWN;
    static unsigned char lastRightSample = SENSOR_STATE_UNKNOWN;

    static unsigned char leftCount = 0;
    static unsigned char middleCount = 0;
    static unsigned char rightCount = 0;

    static uint8_t initialized = FALSE;
    static uint8_t lastTapeState = 0;

    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint8_t tapeState;

    unsigned char leftTape = PS_ReadLeftTape();
    unsigned char middleTape = PS_ReadMidTape();
    unsigned char rightTape = PS_ReadRightTape();

    if (initialized == FALSE) {
        debouncedLeft = leftTape;
        debouncedMiddle = middleTape;
        debouncedRight = rightTape;

        lastLeftSample = leftTape;
        lastMiddleSample = middleTape;
        lastRightSample = rightTape;

        leftCount = 0;
        middleCount = 0;
        rightCount = 0;

        lastTapeState =
                ((debouncedLeft == TAPE_DETECTED) << 0) |
                ((debouncedMiddle == TAPE_DETECTED) << 1) |
                ((debouncedRight == TAPE_DETECTED) << 2);

        initialized = TRUE;
        return FALSE;
    }

    if (leftTape == lastLeftSample) {
        if (leftCount < TAPE_DEBOUNCE_COUNT) {
            leftCount++;
        }
    } else {
        lastLeftSample = leftTape;
        leftCount = 1;
    }

    if (middleTape == lastMiddleSample) {
        if (middleCount < TAPE_DEBOUNCE_COUNT) {
            middleCount++;
        }
    } else {
        lastMiddleSample = middleTape;
        middleCount = 1;
    }

    if (rightTape == lastRightSample) {
        if (rightCount < TAPE_DEBOUNCE_COUNT) {
            rightCount++;
        }
    } else {
        lastRightSample = rightTape;
        rightCount = 1;
    }

    if ((leftCount >= TAPE_DEBOUNCE_COUNT) && (leftTape != debouncedLeft)) {
        debouncedLeft = leftTape;
    }

    if ((middleCount >= TAPE_DEBOUNCE_COUNT) && (middleTape != debouncedMiddle)) {
        debouncedMiddle = middleTape;
    }

    if ((rightCount >= TAPE_DEBOUNCE_COUNT) && (rightTape != debouncedRight)) {
        debouncedRight = rightTape;
    }

    tapeState =
            ((debouncedLeft == TAPE_DETECTED) << 0) |
            ((debouncedMiddle == TAPE_DETECTED) << 1) |
            ((debouncedRight == TAPE_DETECTED) << 2);

    if (tapeState != lastTapeState) {

        if (tapeState == NO_TAPE) {
            thisEvent.EventType = NO_TAPES_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (tapeState == ALL_TAPES) {
            thisEvent.EventType = ALL_TAPE_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (((lastTapeState & LEFT_TAPE_MASK) == 0) &&
                ((tapeState & LEFT_TAPE_MASK) != 0)) {
            thisEvent.EventType = LEFT_TAPE_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (((lastTapeState & LEFT_TAPE_MASK) != 0) &&
                ((tapeState & LEFT_TAPE_MASK) == 0)) {
            thisEvent.EventType = LEFT_TAPE_NOT_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (((lastTapeState & MID_TAPE_MASK) == 0) &&
                ((tapeState & MID_TAPE_MASK) != 0)) {
            thisEvent.EventType = MIDDLE_TAPE_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (((lastTapeState & MID_TAPE_MASK) != 0) &&
                ((tapeState & MID_TAPE_MASK) == 0)) {
            thisEvent.EventType = MIDDLE_TAPE_NOT_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (((lastTapeState & RIGHT_TAPE_MASK) == 0) &&
                ((tapeState & RIGHT_TAPE_MASK) != 0)) {
            thisEvent.EventType = RIGHT_TAPE_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        } else if (((lastTapeState & RIGHT_TAPE_MASK) != 0) &&
                ((tapeState & RIGHT_TAPE_MASK) == 0)) {
            thisEvent.EventType = RIGHT_TAPE_NOT_DETECTED;
            thisEvent.EventParam = tapeState;
            returnVal = TRUE;
        }

        lastTapeState = tapeState;

        if (returnVal == TRUE) {
#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
            PostMainHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif
        }
    }

    return returnVal;
}

uint8_t TemplateCheckBeacon(void) {
    static uint8_t initialized = FALSE;
    ES_Event thisEvent;
    uint16_t beaconReading;

    if (initialized == FALSE) {
        if (InitBeaconEventChecker() != TRUE) {
            return FALSE;
        }
        initialized = TRUE;
    }

    if (AD_IsNewDataReady() != TRUE) {
        return FALSE;
    }

    beaconReading = AD_ReadADPin(BEACON_1_DETECTOR_AD_PIN);
    if (beaconReading == (uint16_t) ERROR) {
        return FALSE;
    }

    thisEvent.EventType = BEACON_SAMPLE_READY;
    thisEvent.EventParam = beaconReading;

#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
    PostMainHSM(thisEvent);
#else
    SaveEvent(thisEvent);
#endif

    return TRUE;
}

uint8_t TemplateCheckPing(void) {
    static uint8_t ping_close = FALSE;
    static uint8_t ping_far = FALSE;
    static uint8_t pingInitialized = FALSE;
    static uint8_t sampleTimerInitialized = FALSE;
    static uint32_t lastSampleTicks;

    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint8_t current_ping_close;
    uint8_t current_ping_far;
    uint32_t currentTicks;
    uint32_t sampleIntervalTicks;

    if (pingInitialized == FALSE) {
        HCSR04_Init();
        TryZeroReference();
        pingInitialized = TRUE;
    }

    currentTicks = CoreTimerNow();
    sampleIntervalTicks = PING_SAMPLE_INTERVAL_US * CoreTimerTicksPerUs();

    if ((sampleTimerInitialized == TRUE)
            && ((uint32_t) (currentTicks - lastSampleTicks) < sampleIntervalTicks)) {
        return FALSE;
    }
    lastSampleTicks = currentTicks;
    sampleTimerInitialized = TRUE;

    if (HCSR04_TakeReading() == FALSE) {
        return FALSE;
    }

    current_ping_close = HCSR04_LastReadingIsReallyClose();
    current_ping_far = HCSR04_LastReadingIsReallyFar();

    if ((current_ping_close == ping_close) && (current_ping_far == ping_far)) {
        return FALSE;
    }

    ping_close = current_ping_close;
    ping_far = current_ping_far;

    if (ping_close == TRUE) {
        thisEvent.EventType = PING_CLOSE;
        thisEvent.EventParam = LastReadingMm;
        returnVal = TRUE;
    } else if (ping_far == TRUE) {
        thisEvent.EventType = PING_FAR;
        thisEvent.EventParam = LastReadingMm;
        returnVal = TRUE;
    }

    if (returnVal == TRUE) {
        printf("TemplateCheckPing: posting %s reading_mm=%lu close=%u far=%u\r\n",
                EventNames[thisEvent.EventType],
                (unsigned long) LastReadingMm,
                ping_close,
                ping_far);
#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
        PostMainHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif
    }

    return returnVal;
}

void TemplateResetPingReference(void) {
    ReferenceValid = FALSE;
    LastReadingValid = FALSE;
    DistanceFilter_Reset(&DistanceAverage);
    TryZeroReference();
}















#ifdef EVENTCHECKER_TEST
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    SERIAL_Init();
    PS_Init();

    int i;

    printf("\r\nPeashooter event checking test harness for %s", __FILE__);
    printf("\r\nTesting switch and tape events...\r\n");

    while (1) {
        if (IsTransmitEmpty()) {
            for (i = 0; i < sizeof (EventList) / sizeof (EventList[0]); i++) {
                if (EventList[i]() == TRUE) {
                    PrintEvent();
                    break;
                }
            }
        }
    }
}

void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X\r\n",
            eventName,
            EventNames[storedEvent.EventType],
            storedEvent.EventParam);

    while (!IsTransmitEmpty()) {
        ;
    }
}
#endif

#ifdef MOVING_TEST

#include <stdio.h>
#define MOVING_TEST_ONE_SECOND 1000000u

void main(void) {
    unsigned int delay;

    BOARD_Init();
    SERIAL_Init();
    PS_Init();

    printf("Testing Moving Functions\n");
    int power = 900;
    while (1) {

        PS_Forward(power);
    }
}

#endif
