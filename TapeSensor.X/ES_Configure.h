/****************************************************************************
 Module
     ES_Configure.h
 Description
     Project-specific ES Framework configuration for the tape sensor event
     monitor.
 *****************************************************************************/

#ifndef ES_CONFIGURE_H
#define ES_CONFIGURE_H

//#define USE_KEYBOARD_INPUT
//#define USE_TATTLETALE
//#define SUPPRESS_EXIT_ENTRY_IN_TATTLE

/****************************************************************************/
// Universal events occupy the lowest entries, followed by user-defined events.

typedef enum {
    ES_NO_EVENT,
    ES_ERROR,
    ES_INIT,
    ES_ENTRY,
    ES_EXIT,
    ES_KEYINPUT,
    ES_LISTEVENTS,
    ES_TIMEOUT,
    ES_TIMERACTIVE,
    ES_TIMERSTOPPED,
    RIGHT_TAPE_DETECTED,
    RIGHT_TAPE_NOT_DETECTED,
    MIDDLE_TAPE_DETECTED,
    MIDDLE_TAPE_NOT_DETECTED,
    LEFT_TAPE_DETECTED,
    LEFT_TAPE_NOT_DETECTED,
    NO_TAPE_DETECTED,
    NUMBEROFEVENTS,
} ES_EventTyp_t;

static const char *EventNames[] = {
    "ES_NO_EVENT",
    "ES_ERROR",
    "ES_INIT",
    "ES_ENTRY",
    "ES_EXIT",
    "ES_KEYINPUT",
    "ES_LISTEVENTS",
    "ES_TIMEOUT",
    "ES_TIMERACTIVE",
    "ES_TIMERSTOPPED",
    "RIGHT_TAPE_DETECTED",
    "RIGHT_TAPE_NOT_DETECTED",
    "MIDDLE_TAPE_DETECTED",
    "MIDDLE_TAPE_NOT_DETECTED",
    "LEFT_TAPE_DETECTED",
    "LEFT_TAPE_NOT_DETECTED",
    "NO_TAPE_DETECTED",
    "NUMBEROFEVENTS",
};

/****************************************************************************/
// User event checker list.

#define EVENT_CHECK_HEADER "TapeSensorEventChecker.h"
#define EVENT_CHECK_LIST CheckTapeSensor

/****************************************************************************/
// Timer routing.

#define TIMER_UNUSED ((pPostFunc)0)

#define TIMER0_RESP_FUNC TIMER_UNUSED
#define TIMER1_RESP_FUNC TIMER_UNUSED
#define TIMER2_RESP_FUNC TIMER_UNUSED
#define TIMER3_RESP_FUNC TIMER_UNUSED
#define TIMER4_RESP_FUNC TIMER_UNUSED
#define TIMER5_RESP_FUNC TIMER_UNUSED
#define TIMER6_RESP_FUNC TIMER_UNUSED
#define TIMER7_RESP_FUNC TIMER_UNUSED
#define TIMER8_RESP_FUNC TIMER_UNUSED
#define TIMER9_RESP_FUNC TIMER_UNUSED
#define TIMER10_RESP_FUNC TIMER_UNUSED
#define TIMER11_RESP_FUNC TIMER_UNUSED
#define TIMER12_RESP_FUNC TIMER_UNUSED
#define TIMER13_RESP_FUNC TIMER_UNUSED
#define TIMER14_RESP_FUNC TIMER_UNUSED
#define TIMER15_RESP_FUNC TIMER_UNUSED

/****************************************************************************/
// Services.

#define MAX_NUM_SERVICES 8
#define NUM_SERVICES 1

#define SERV_0_HEADER "TapeSensorService.h"
#define SERV_0_INIT InitTapeSensorService
#define SERV_0_RUN RunTapeSensorService
#define SERV_0_QUEUE_SIZE 8

#define POST_KEY_FUNC ES_PostAll

/****************************************************************************/
// Distribution lists.

#define NUM_DIST_LISTS 0

#endif /* ES_CONFIGURE_H */
