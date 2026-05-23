/****************************************************************************
 Module
     ES_Configure.h
 Description
     Project-specific ES Framework configuration for shooter.
 *****************************************************************************/

#ifndef ES_CONFIGURE_H
#define ES_CONFIGURE_H

//#define USE_KEYBOARD_INPUT
#define USE_TATTLETALE
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
    SHOOTER_START,
    SHOOTER_STOP,
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
    "SHOOTER_START",
    "SHOOTER_STOP",
    "NUMBEROFEVENTS",
};

/****************************************************************************/
// User event checker list. This starter project is driven by posted events.

#define EVENT_CHECK_HEADER "ShooterEventChecker.h"
#define EVENT_CHECK_LIST CheckShooterEvents

/****************************************************************************/
// Timer routing. The PWM driver owns Timer2; ES timers use Timer1.

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

#define SHOOTER_TIMER 0

/****************************************************************************/
// Services.

#define MAX_NUM_SERVICES 8
#define NUM_SERVICES 1

#define SERV_0_HEADER "ShooterService.h"
#define SERV_0_INIT InitShooterService
#define SERV_0_RUN RunShooterService
#define SERV_0_QUEUE_SIZE 6

#define POST_KEY_FUNC ES_PostAll

/****************************************************************************/
// Distribution lists.

#define NUM_DIST_LISTS 0

#endif /* ES_CONFIGURE_H */
