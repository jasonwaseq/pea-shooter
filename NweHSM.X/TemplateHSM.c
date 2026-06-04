/*
 * File: TemplateHSM.c
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "TemplateHSM.h"
#include "TemplateSubHSM.h"
#include "peashooter.h"
#include "SensorsEventChecker.h"
#include <stdio.h>

typedef enum {
    Init,
    LocateCorner,
    FollowEdge,
    Launcher,
} TemplateHSMState_t;

static TemplateHSMState_t CurrentState = Init;
static uint8_t MyPriority;

uint8_t InitTemplateHSM(uint8_t Priority) {
    MyPriority = Priority;
    CurrentState = Init;

    PS_Init();

    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint8_t PostTemplateHSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

ES_Event RunTemplateHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    TemplateHSMState_t nextState = CurrentState;

    switch (CurrentState) {
        case Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunTemplateHSM: Init: ES_INIT\r\n");

                nextState = LocateCorner;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case LocateCorner:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunTemplateHSM: LocateCorner: ES_ENTRY\r\n");
                    InitLocateCornerSubHSM();
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_EXIT:
                    PS_Stop();
                    break;

                default:
                    ThisEvent = RunLocateCornerSubHSM(ThisEvent);

                    switch (ThisEvent.EventType) {
                        case ALL_TAPE_DETECTED:
                            nextState = FollowEdge;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                            break;

                        case TAPE_ARRAY_CHANGED:
                            if (ThisEvent.EventParam == ALL_TAPES) {
                                nextState = FollowEdge;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                            }
                            break;

                        default:
                            break;
                    }
                    break;
            }
            break;

        case FollowEdge:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunTemplateHSM: FollowEdge: ES_ENTRY\r\n");
                    InitFollowEdgeSubHSM();
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_EXIT:
                    PS_Stop();
                    break;

                default:
                    printf("RunTemplateHSM: FollowEdge\r\n");
                    ThisEvent = RunFollowEdgeSubHSM(ThisEvent);

                    switch (ThisEvent.EventType) {
                        case ALL_TAPE_DETECTED:
                            nextState = Launcher;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                            break;

                        case TAPE_ARRAY_CHANGED:
                            if (ThisEvent.EventParam == ALL_TAPES) {
                                nextState = Launcher;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                            }
                            break;

                        default:
                            break;
                    }
                    break;
            }
            break;

        case Launcher:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunTemplateHSM: Launcher: ES_ENTRY\r\n");
                    InitLauncherSubHSM();
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_EXIT:
                    PS_Stop();
                    break;

                default:
                    printf("RunTemplateHSM: Launcher\r\n");
                    ThisEvent = RunLauncherSubHSM(ThisEvent);
                    break;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunTemplateHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunTemplateHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}