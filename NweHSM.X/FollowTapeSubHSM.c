/*
 * File: FollowTapeSubHSM.c
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

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define FOLLOWTAPE_ANGLEDLEFT_POWER 800
#define FOLLOWTAPE_ANGLEDRIGHT_POWER 800
#define FOLLOWTAPE_FORWARD_POWER 800
#define FOLLOWTAPE_CORRECT_TIME 500

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    FollowTapeSubHSM_Init,
    FollowTape_Forward,
    FollowTape_CorrectLeft,
    FollowTape_CorrectRight,
} FollowTapeSubHSMState_t;

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static FollowTapeSubHSMState_t FollowTapeSubHSMCurrentState = FollowTapeSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitFollowTapeSubHSM(void) {
    ES_Event returnEvent;

    FollowTapeSubHSMCurrentState = FollowTapeSubHSM_Init;
    returnEvent = RunFollowTapeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunFollowTapeSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    FollowTapeSubHSMState_t nextState = FollowTapeSubHSMCurrentState;

    switch (FollowTapeSubHSMCurrentState) {
        case FollowTapeSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunFollowTapeSubHSM: FollowTapeSubHSM_Init: ES_INIT\r\n");
                nextState = FollowTape_Forward;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case FollowTape_Forward:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFollowTapeSubHSM: FollowTape_Forward: ES_ENTRY\r\n");
                    PS_Forward(FOLLOWTAPE_FORWARD_POWER);
                    break;

                case NO_TAPES_DETECTED:
                    printf("RunFollowTapeSubHSM: FollowTape_Forward: NO_TAPES_DETECTED\r\n");
                    nextState = FollowTape_CorrectLeft;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunFollowTapeSubHSM: FollowTape_Forward: MIDDLE_TAPE_DETECTED\r\n");
                    nextState = FollowTape_CorrectRight;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case FollowTape_CorrectLeft:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectLeft: ES_ENTRY\r\n");
                    PS_PivotTurnLeft(FOLLOWTAPE_ANGLEDLEFT_POWER);
                    ES_Timer_InitTimer(CORRECT_TIMER, FOLLOWTAPE_CORRECT_TIME);
                    break;

                case ES_TIMEOUT:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectLeft: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == CORRECT_TIMER) {
                        PS_Stop();
                        nextState = FollowTape_Forward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    break;
            }
            break;

        case FollowTape_CorrectRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectRight: ES_ENTRY\r\n");
                    PS_PivotTurnRight(FOLLOWTAPE_ANGLEDRIGHT_POWER);
                    ES_Timer_InitTimer(CORRECT_TIMER, FOLLOWTAPE_CORRECT_TIME);
                    break;

                case ES_TIMEOUT:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectRight: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == CORRECT_TIMER) {
                        PS_Stop();
                        nextState = FollowTape_Forward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunFollowTapeSubHSM(EXIT_EVENT);
        FollowTapeSubHSMCurrentState = nextState;
        RunFollowTapeSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}
