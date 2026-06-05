/*
 * File: FollowEdgeSubHSM.c
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "peashooter.h"
#include "ES_Timers.h"
#include "SensorsEventChecker.h"
#include <stdio.h>
#include "FollowTapeSubHSM.h"
#include "AvoidObstacleSubHSM.h"
#include "FollowEdgeSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define FOLLOWEDGE_TURNRIGHT_POWER 700
#define FOLLOWEDGE_TURNRIGHT_TIME 1500

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    FollowEdgeSubHSM_Init,
    FollowEdge_TurnRight,
    FollowEdge_FollowTape,
    FollowEdge_AvoidObstacle,
} FollowEdgeSubHSMState_t;

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static FollowEdgeSubHSMState_t FollowEdgeSubHSMCurrentState = FollowEdgeSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitFollowEdgeSubHSM(void) {
    ES_Event returnEvent;

    FollowEdgeSubHSMCurrentState = FollowEdgeSubHSM_Init;
    returnEvent = RunFollowEdgeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunFollowEdgeSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    FollowEdgeSubHSMState_t nextState = FollowEdgeSubHSMCurrentState;

    switch (FollowEdgeSubHSMCurrentState) {
        case FollowEdgeSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunFollowEdgeSubHSM: FollowEdgeSubHSM_Init: ES_INIT\r\n");
                //    nextState = FollowEdge_TurnRight;
                nextState = FollowEdge_FollowTape;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;
        case FollowEdge_TurnRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFollowEdgeSubHSM: FollowEdge_TurnRight: ES_ENTRY\r\n");
                    PS_PivotTurnRight(FOLLOWEDGE_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(FOLLOWEDGE_TURNRIGHT_TIMER, FOLLOWEDGE_TURNRIGHT_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunFollowEdgeSubHSM: FollowEdge_TurnRight: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == FOLLOWEDGE_TURNRIGHT_TIMER) {
                        PS_Stop();
                        nextState = FollowEdge_FollowTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default:
                    break;
            }
            break;
        case FollowEdge_FollowTape:
            if (ThisEvent.EventType == ES_ENTRY) {
                printf("RunFollowEdgeSubHSM: FollowEdge_FollowTape: ES_ENTRY\r\n");
                InitFollowTapeSubHSM();
                ThisEvent.EventType = ES_NO_EVENT;
            } else {
                ThisEvent = RunFollowTapeSubHSM(ThisEvent);
            }

            switch (ThisEvent.EventType) {
                case BUMPER_TRIPPED:
                    printf("RunFollowEdgeSubHSM: FollowEdge_FollowTape: BUMPER_TRIPPED\r\n");
                    PS_Stop();
                    while(1){}
                    nextState = FollowEdge_AvoidObstacle;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;
        case FollowEdge_AvoidObstacle:
            if (ThisEvent.EventType == ES_ENTRY) {
                printf("RunFollowEdgeSubHSM: FollowEdge_AvoidObstacle: ES_ENTRY\r\n");
                RunAvoidObstacleSubHSM(INIT_EVENT);
            }
            ThisEvent = RunAvoidObstacleSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ALL_TAPE_DETECTED:
                    printf("RunFollowEdgeSubHSM: FollowEdge_AvoidObstacle: ALL_TAPE_DETECTED\r\n");
                    nextState = FollowEdge_TurnRight;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if (makeTransition == TRUE) {

        RunFollowEdgeSubHSM(EXIT_EVENT);
        FollowEdgeSubHSMCurrentState = nextState;
        RunFollowEdgeSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}
