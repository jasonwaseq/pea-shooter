/*
 * File: LauncherSubHSM.c
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
#include "LauncherSubHSM.h"

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    LauncherSubHSM_Init,
    Launcher_PreRev,
    Launcher_FindBeacon,
    Launcher_Launch,
} LauncherSubHSMState_t;

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static LauncherSubHSMState_t LauncherSubHSMCurrentState = LauncherSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitLauncherSubHSM(void) {
    ES_Event returnEvent;

    LauncherSubHSMCurrentState = LauncherSubHSM_Init;
    returnEvent = RunLauncherSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

ES_Event RunLauncherSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    LauncherSubHSMState_t nextState = LauncherSubHSMCurrentState;

    switch (LauncherSubHSMCurrentState) {
        case LauncherSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunLauncherSubHSM: LauncherSubHSM_Init: ES_INIT\r\n");
                //prerev
                nextState = Launcher_FindBeacon;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;
        case Launcher_FindBeacon:
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
                    nextState = Launcher_Launch;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;
        case Launcher_Launch:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunLauncherSubHSM: Launcher_Launch: ES_ENTRY\r\n");

                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {

        RunLauncherSubHSM(EXIT_EVENT);
        LauncherSubHSMCurrentState = nextState;
        RunLauncherSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}
