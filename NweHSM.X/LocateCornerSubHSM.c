/*
 * File: LocateCornerSubHSM.c
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "peashooter.h"
#include "ES_Timers.h"
#include "SensorsEventChecker.h"
#include <stdio.h>
#include "FindBeaconSubHSM.h"
#include "AlignTapeSubHSM.h"
#include "FollowTapeSubHSM.h"
#include "LocateCornerSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LOCATECORNER_TURNRIGHT_POWER 600
#define LOCATECORNER_TURNRIGHT_TIME 1500

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    LocateCornerSubHSM_Init,
    LocateCorner_FindBeacon,
    LocateCorner_AlignTape,
    LocateCorner_TurnRight,
    LocateCorner_FollowTape,
} LocateCornerSubHSMState_t;

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static LocateCornerSubHSMState_t LocateCornerSubHSMCurrentState = LocateCornerSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitLocateCornerSubHSM(void) {
    ES_Event returnEvent;

    LocateCornerSubHSMCurrentState = LocateCornerSubHSM_Init;
    returnEvent = RunLocateCornerSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunLocateCornerSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    LocateCornerSubHSMState_t nextState = LocateCornerSubHSMCurrentState;
    switch (LocateCornerSubHSMCurrentState) {
        case LocateCornerSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunLocateCornerSubHSM: LocateCornerSubHSM_Init: ES_INIT\r\n");
                nextState = LocateCorner_FindBeacon;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;
        case LocateCorner_FindBeacon:
            if (ThisEvent.EventType == ES_ENTRY) {
                printf("RunLocateCornerSubHSM: LocateCorner_FindBeacon: ES_ENTRY\r\n");
                InitFindBeaconSubHSM();
                ThisEvent.EventType = ES_NO_EVENT;
            } else {
                ThisEvent = RunFindBeaconSubHSM(ThisEvent);
            }

            switch (ThisEvent.EventType) {
                case BEACON_LOCKED:
                    printf("RunLocateCornerSubHSM: LocateCorner_FindBeacon: BEACON_LOCKED\r\n");
                    nextState = LocateCorner_AlignTape;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;
        case LocateCorner_AlignTape:
            if (ThisEvent.EventType == ES_ENTRY) {
                printf("RunLocateCornerSubHSM: LocateCorner_AlignTape: ES_ENTRY\r\n");
                InitAlignTapeSubHSM();
                ThisEvent.EventType = ES_NO_EVENT;
            } else {
                ThisEvent = RunAlignTapeSubHSM(ThisEvent);
            }
            switch (ThisEvent.EventType) {
                case ALL_TAPE_DETECTED:
                    printf("RunLocateCornerSubHSM: LocateCorner_AlignTape: ALL_TAPE_DETECTED\r\n");
                    nextState = LocateCorner_TurnRight;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                default:
                    break;
            }
            break;
        case LocateCorner_TurnRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunLocateCornerSubHSM: LocateCorner_TurnRight: ES_ENTRY\r\n");
                    PS_TankTurnRight(LOCATECORNER_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(LOCATECORNER_TURNRIGHT_TIMER, LOCATECORNER_TURNRIGHT_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunLocateCornerSubHSM: LocateCorner_TurnRight: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == LOCATECORNER_TURNRIGHT_TIMER) {
                        PS_Stop();
                        nextState = LocateCorner_FollowTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default:
                    break;
            }
            break;
        case LocateCorner_FollowTape:
            if (ThisEvent.EventType == ES_ENTRY) {
                printf("RunLocateCornerSubHSM: LocateCorner_FollowTape: ES_ENTRY\r\n");
                InitFollowTapeSubHSM();
                ThisEvent.EventType = ES_NO_EVENT;
            } else {
                ThisEvent = RunFollowTapeSubHSM(ThisEvent);
            }
            switch (ThisEvent.EventType) {
                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {

        RunLocateCornerSubHSM(EXIT_EVENT);
        LocateCornerSubHSMCurrentState = nextState;
        RunLocateCornerSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}
