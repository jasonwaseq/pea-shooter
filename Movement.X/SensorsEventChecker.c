/*
 * File:   SensorsEventChecker.c
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This file includes a test harness that will run the event detectors listed in the
 * ES_Configure file in the project, and will conditionally compile main if the macro
 * EVENTCHECKER_TEST is defined (either in the project or in the file). This will allow
 * you to check you event detectors in their own project, and then leave them untouched
 * for your project unless you need to alter their post functions.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "SensorsEventChecker.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include "peashooter.h"
#include "TemplateHSM.h"
#include "pwm.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define BATTERY_DISCONNECT_THRESHOLD 175
#define SENSOR_STATE_UNKNOWN 0xFF

/* peashooter.h uses SWITCH_TRIPPED / SWITCH_NOT_TRIPPED as raw switch states.
 * ES_Configure.h also uses those names as event types, so keep the raw values
 * under local names and then restore access to the event enum identifiers. */

/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

//#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular event checker */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function TemplateCheckBattery(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function is a prototype event checker that checks the battery voltage
 *        against a fixed threshold (#defined in the .c file). Note that you need to
 *        keep track of previous history, and that the actual battery voltage is checked
 *        only once at the beginning of the function. The function will post an event
 *        of either BATTERY_CONNECTED or BATTERY_DISCONNECTED if the power switch is turned
 *        on or off with the USB cord plugged into the Uno32. Returns TRUE if there was an 
 *        event, FALSE otherwise.
 * @note Use this code as a template for your other event checkers, and modify as necessary.
 * @author Gabriel H Elkaim, 2013.09.27 09:18
 * @modified Gabriel H Elkaim/Max Dunne, 2016.09.12 20:08 */
uint8_t TemplateCheckBattery(void) {
    static ES_EventTyp_t lastEvent = BATTERY_DISCONNECTED;
    static uint8_t initialized = FALSE;
    ES_EventTyp_t curEvent;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint16_t batVoltage = AD_ReadADPin(BAT_VOLTAGE); // read the battery voltage

    if (batVoltage > BATTERY_DISCONNECT_THRESHOLD) { // is battery connected?
        curEvent = BATTERY_CONNECTED;
    } else {
        curEvent = BATTERY_DISCONNECTED;
    }

    if (initialized == FALSE) {
        lastEvent = curEvent;
        initialized = TRUE;
        return returnVal;
    }

    if (curEvent != lastEvent) { // check for change from last time
        thisEvent.EventType = curEvent;
        thisEvent.EventParam = batVoltage;
        returnVal = TRUE;
        lastEvent = curEvent; // update history
#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
        PostHSMService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif  
    }
    return (returnVal);
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
        lastEvent = (switchState == SWITCH_TRIPPED)
                ? BUMPER_TRIPPED : BUMPER_NOT_TRIPPED;
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

        if (curEvent != lastEvent) { // check for change from last time
            thisEvent.EventType = curEvent;
            thisEvent.EventParam = debouncedState;
            returnVal = TRUE;
            lastEvent = curEvent; // update history
#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
            PostHSMService(thisEvent);
#else
            SaveEvent(thisEvent);
#endif
        }
    }
    return (returnVal);
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

#define TAPE_DEBOUNCE_COUNT 20

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
        thisEvent.EventType = TAPE_ARRAY_CHANGED;
        thisEvent.EventParam = tapeState;
        returnVal = TRUE;
        lastTapeState = tapeState;
    }

    if (returnVal == TRUE) {
#if !defined(EVENTCHECKER_TEST) && !defined(MOVEMENT_TEST)
        PostHSMService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif
    }

    return returnVal;
}

/* 
 * The Test Harness for the event checkers is conditionally compiled using
 * the EVENTCHECKER_TEST macro (defined either in the file or at the project level).
 * No other main() can exist within the project.
 * 
 * It requires a valid ES_Configure.h file in the project with the correct events in 
 * the enum, and the correct list of event checkers in the EVENT_CHECK_LIST.
 * 
 * The test harness will run each of your event detectors identically to the way the
 * ES_Framework will call them, and if an event is detected it will print out the function
 * name, event, and event parameter. Use this to test your event checking code and
 * ensure that it is fully functional.
 * 
 * If you are locking up the output, most likely you are generating too many events.
 * Remember that events are detectable changes, not a state in itself.
 * 
 * Once you have fully tested your event checking code, you can leave it in its own
 * project and point to it from your other projects. If the EVENTCHECKER_TEST marco is
 * defined in the project, no changes are necessary for your event checkers to work
 * with your other projects.
 */
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
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X\r\n", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
    while (!IsTransmitEmpty()) {
        ;
    }
}
#endif

#ifdef MOVING_TEST

#include <stdio.h>
#define MOVING_TEST_ONE_SECOND 1000000

void main(void) {
    unsigned int delay;

    BOARD_Init();
    SERIAL_Init();
    PS_Init();

    printf("Testing Moving Functions\n");

    while (1) {
        PS_TurnRight90();
        DELAY_COUNTS(MOVING_TEST_ONE_SECOND);

        PS_TurnLeft90();
        DELAY_COUNTS(MOVING_TEST_ONE_SECOND);
    }
}

#endif
