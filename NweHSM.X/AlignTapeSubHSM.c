/*
 * File: AlignTapeSubHSM.c
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "peashooter.h"
#include "ES_Timers.h"
#include "SensorsEventChecker.h"
#include <stdio.h>
#include "AlignTapeSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define ALIGNTAPE_FORWARD_POWER 700
#define ALIGNTAPE_PIVOTRIGHT_POWER 600
#define ALIGNTAPE_PIVOTLEFT_POWER 600

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    AlignTapeSubHSM_Init,
    AlignTape_Forward,
    AlignTape_PivotLeft,
    AlignTape_PivotRight,
} AlignTapeSubHSMState_t;

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static AlignTapeSubHSMState_t AlignTapeSubHSMCurrentState = AlignTapeSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitAlignTapeSubHSM(void) {
    ES_Event returnEvent;

    AlignTapeSubHSMCurrentState = AlignTapeSubHSM_Init;
    returnEvent = RunAlignTapeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunAlignTapeSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    AlignTapeSubHSMState_t nextState = AlignTapeSubHSMCurrentState;

    if (ThisEvent.EventType == ALL_TAPE_DETECTED) {
        return ThisEvent;
    }

    switch (AlignTapeSubHSMCurrentState) {
        case AlignTapeSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunAlignTapeSubHSM: AlignTapeSubHSM_Init: ES_INIT\r\n");
                nextState = AlignTape_Forward;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AlignTape_Forward:
            switch (ThisEvent.EventType) {

                case ES_ENTRY:
                    printf("RunAlignTapeSubHSM: AlignTape_Forward: ES_ENTRY\r\n");
                    PS_Forward(ALIGNTAPE_FORWARD_POWER);
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunAlignTapeSubHSM: AlignTape_Forward: LEFT_TAPE_DETECTED\r\n");
                    nextState = AlignTape_PivotRight;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunAlignTapeSubHSM: AlignTape_Forward: RIGHT_TAPE_DETECTED\r\n");
                    nextState = AlignTape_PivotLeft;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case AlignTape_PivotLeft:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAlignTapeSubHSM: AlignTape_PivotLeft: ES_ENTRY\r\n");
                    PS_PivotTurnLeft(ALIGNTAPE_PIVOTLEFT_POWER);
                    break;

                case RIGHT_TAPE_NOT_DETECTED:
                    printf("RunAlignTapeSubHSM: AlignTape_PivotLeft: RIGHT_TAPE_NOT_DETECTED\r\n");
                    nextState = AlignTape_PivotRight;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case AlignTape_PivotRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunAlignTapeSubHSM: AlignTape_PivotRight: ES_ENTRY\r\n");
                    PS_PivotTurnRight(ALIGNTAPE_PIVOTRIGHT_POWER);
                    break;

                case LEFT_TAPE_NOT_DETECTED:
                    printf("RunAlignTapeSubHSM: AlignTape_PivotRight: LEFT_TAPE_NOT_DETECTED\r\n");
                    nextState = AlignTape_PivotLeft;
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
        RunAlignTapeSubHSM(EXIT_EVENT);
        AlignTapeSubHSMCurrentState = nextState;
        RunAlignTapeSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}
