/*
 * File: AvoidObstacleSubHSM.c
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "peashooter.h"
#include "ES_Timers.h"
#include "SensorsEventChecker.h"
#include <stdio.h>
#include "TemplateHSM.h"
#include "AvoidObstacleSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define AVOIDOBSTACLE_FORWARD_POWER 700
#define AVOIDOBSTACLE_TURNRIGHT_POWER 700
#define AVOIDOBSTACLE_TURNLEFT_POWER 700
#define AVOIDOBSTACLE_FORWARDDIST_POWER 700
#define AVOIDOBSTACLE_BACKWARDDIST_POWER 700
#define AVOIDOBSTACLE_PIVOT_POWER 700
#define AVOIDOBSTACLE_BACKWARD_TIME 250
#define AVOIDOBSTACLE_TURNRIGHT_TIME 1150
#define PING_CLEARANCE_TIME 300
#define OBSTACLE_SIDE_CLEARANCE_TIME 1000

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    AvoidObstacleSubHSM_Init,
    AvoidObstacle_Backward,
    AvoidObstacle_Forward,
    AvoidObstacle_TurnRight,
    AvoidObstacle_TurnRight2,
    AvoidObstacle_TurnLeft,
    AvoidObstacle_PassFront,
    AvoidObstacle_PassSide,
    AvoidObstacle_SearchForTape
} AvoidObstacleSubHSMState_t;

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static AvoidObstacleSubHSMState_t AvoidObstacleSubHSMCurrentState = AvoidObstacleSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitAvoidObstacleSubHSM(void) {
    ES_Event returnEvent;

    AvoidObstacleSubHSMCurrentState = AvoidObstacleSubHSM_Init;
    returnEvent = RunAvoidObstacleSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunAvoidObstacleSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    AvoidObstacleSubHSMState_t nextState = AvoidObstacleSubHSMCurrentState;

    if (!(AvoidObstacleSubHSMCurrentState == AvoidObstacle_TurnLeft)) {
        switch (ThisEvent.EventType) {
            case LEFT_TAPE_DETECTED:
            case LEFT_TAPE_NOT_DETECTED:
            case MIDDLE_TAPE_DETECTED:
            case MIDDLE_TAPE_NOT_DETECTED:
            case RIGHT_TAPE_DETECTED:
            case RIGHT_TAPE_NOT_DETECTED:
            case NO_TAPES_DETECTED:
                ThisEvent.EventType = ES_NO_EVENT;
                break;
            default:
                break;
        }
    }

    switch (AvoidObstacleSubHSMCurrentState) {
        case AvoidObstacleSubHSM_Init: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                printf("RunAvoidObstacleSubHSM: AvoidObstacleSubHSM_Init: ES_INIT\r\n");
                nextState = AvoidObstacle_Backward;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;
        case AvoidObstacle_Backward:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_Backward: ES_ENTRY\r\n");
                    PS_Backward(AVOIDOBSTACLE_BACKWARDDIST_POWER);
                    ES_Timer_InitTimer(AVOIDOBSTACLE_BACKWARD_TIMER, AVOIDOBSTACLE_BACKWARD_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_Backward: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == AVOIDOBSTACLE_BACKWARD_TIMER) {
                        PS_Stop();

                        nextState = AvoidObstacle_TurnRight;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case AvoidObstacle_TurnRight: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnRight: ES_ENTRY\r\n");
                    PS_TankTurnRight(AVOIDOBSTACLE_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(AVOIDOBSTACLE_TURNRIGHT_TIMER, AVOIDOBSTACLE_TURNRIGHT_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnRight: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == AVOIDOBSTACLE_TURNRIGHT_TIMER) {
                        PS_Stop();
                        TemplateResetPingReference();
                        //zero here

                        nextState = AvoidObstacle_PassFront;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case AvoidObstacle_PassFront: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassFront: ES_ENTRY\r\n");
                    PS_Forward(AVOIDOBSTACLE_FORWARD_POWER);
                    break;
                case PING_FAR:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassFront: PING_FAR\r\n");
                    PS_Stop();

                    PS_Forward(AVOIDOBSTACLE_FORWARD_POWER);
                    ES_Timer_InitTimer(PING_CLEARANCE, PING_CLEARANCE_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassFront: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == PING_CLEARANCE) {
                        PS_Stop();
                        //zero

                        nextState = AvoidObstacle_TurnRight2;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case AvoidObstacle_TurnRight2: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnRight2: ES_ENTRY\r\n");
                    PS_TankTurnLeft(AVOIDOBSTACLE_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(AVOIDOBSTACLE_TURNRIGHT_TIMER, AVOIDOBSTACLE_TURNRIGHT_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnRight2: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == AVOIDOBSTACLE_TURNRIGHT_TIMER) {
                        PS_Stop();
                        while (1) {
                        };
                        //zero

                        nextState = AvoidObstacle_PassSide;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case AvoidObstacle_PassSide: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassSide: ES_ENTRY\r\n");
                    PS_Forward(AVOIDOBSTACLE_FORWARD_POWER);
                    ES_Timer_InitTimer(OBSTACLE_SIDE_CLEARANCE, OBSTACLE_SIDE_CLEARANCE_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassSide: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == OBSTACLE_SIDE_CLEARANCE) {
                        PS_Stop();
                        //zero
                        PS_Stop();
                        while (1) {
                        };
                        nextState = AvoidObstacle_TurnLeft;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case AvoidObstacle_TurnLeft: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnLeft: ES_ENTRY\r\n");
                    PS_TankTurnRight(AVOIDOBSTACLE_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(AVOIDOBSTACLE_TURNRIGHT_TIMER, AVOIDOBSTACLE_TURNRIGHT_TIME);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnLeft: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == AVOIDOBSTACLE_TURNRIGHT_TIMER) {
                        PS_Forward(AVOIDOBSTACLE_FORWARD_POWER);
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunAvoidObstacleSubHSM(EXIT_EVENT); // <- rename to your own Run function
        AvoidObstacleSubHSMCurrentState = nextState;
        RunAvoidObstacleSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    return ThisEvent;
}
