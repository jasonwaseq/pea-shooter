/*
 * File: TemplateSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is for a substate machine. Make sure it has a unique name
 *
 * This is provided as an example and a good place to start.
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "TemplateHSM.h"
#include "TemplateSubHSM.h"
#include "peashooter.h"
#include "beacon.h"
#include "ES_Timers.h"
#include "SensorsEventChecker.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


#define HSM_FULL_ROTATION_TIME_MS 5000
#define HSM_LOCK_ON_START_DELAY_MS 250
#define HSM_ON_FIELD_BACKUP_TIME_MS 500
#define HSM_ON_FIELD_STOP_DELAY_MS 250
#define HSM_BEACON_AVG_WINDOW 8
#define HSM_BEACON_LOCK_OFFSET 10

#define HSM_SWEEP_TURN_POWER 800
#define HSM_LOCK_TURN_POWER 800
#define HSM_DRIVE_POWER 800


#define FOLLOWTAPE_ANGLEDLEFT_POWER 800
#define FOLLOWTAPE_ANGLEDRIGHT_POWER 800
#define FOLLOWTAPE_FORWARD_POWER 800

//big ones

typedef enum {
    LocateCornerSubHSM_Init,
    LocateCorner_FindBeacon,
    LocateCorner_AlignTape,
    LocateCorner_TurnRight,
    LocateCorner_FollowTape,
} LocateCornerSubHSMState_t;

typedef enum {
    FollowEdgeSubHSM_Init,
    FollowEdge_TurnRight,
    FollowEdge_FollowTape,
    FollowEdge_AvoidObstacle,

} FollowEdgeSubHSMState_t;

typedef enum {
    LauncherSubHSM_Init,
    Launcher_PreRev,
    Launcher_FindBeacon,
    Launcher_Launch,
} LauncherSubHSMState_t;

typedef enum {
    InitPSubState,
    SubFirstState,
} TemplateSubHSMState_t;

typedef enum {
    FollowTapeSubHSM_Init,
    FollowTape_Forward,
    FollowTape_CorrectLeft,
    FollowTape_CorrectRight,
} FollowTapeSubHSMState_t;

typedef enum {
    AlignTapeSubHSM_Init,
    AlignTape_Forward,
    AlignTape_PivotLeft,
    AlignTape_PivotRight,
} AlignTapeSubHSMState_t;

typedef enum {
    FindBeaconSubHSM_Init,
    SweepDataState,
    LockOnState,
    OnFieldState,
} FindBeaconSubHSMState_t;

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
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */
static ES_Event TransitionFindBeaconSubState(FindBeaconSubHSMState_t nextState);

static void StartSweepData(void);
static void StartLockOn(void);
static void StartLockOnTurn(void);
static void StartNextLockOnRotation(void);
static void ResetBeaconFilter(void);
static uint16_t FilterBeaconSample(uint16_t sample);
static void DriveBackward(void);

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/

static uint16_t BeaconSamples[HSM_BEACON_AVG_WINDOW];
static uint8_t BeaconSampleIndex;
static uint8_t BeaconSampleCount;
static uint32_t BeaconSampleSum;

static uint16_t MaxFilteredBeaconValue;
static uint16_t BeaconLockThreshold;
static uint8_t LockOnTurnStarted;
static uint16_t LockOnRotationMax;
static uint8_t OnFieldStopped;
static uint8_t TapeEventsEnabled;

/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static TemplateSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static FollowTapeSubHSMState_t FollowTapeSubHSMCurrentState = FollowTapeSubHSM_Init;
static AvoidObstacleSubHSMState_t AvoidObstacleSubHSMCurrentState = AvoidObstacleSubHSM_Init;
static FindBeaconSubHSMState_t FindBeaconSubHSMCurrentState = FindBeaconSubHSM_Init;
static AlignTapeSubHSMState_t AlignTapeSubHSMCurrentState = AlignTapeSubHSM_Init;

//big ones
static LocateCornerSubHSMState_t LocateCornerSubHSMCurrentState = LocateCornerSubHSM_Init;
static FollowEdgeSubHSMState_t FollowEdgeSubHSMCurrentState = FollowEdgeSubHSM_Init;
static LauncherSubHSMState_t LauncherSubHSMCurrentState = LauncherSubHSM_Init;

static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateSubHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitTemplateSubHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunTemplateSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

//big ones

uint8_t InitLocateCornerSubHSM(void) {
    ES_Event returnEvent;

    LocateCornerSubHSMCurrentState = LocateCornerSubHSM_Init;
    returnEvent = RunLocateCornerSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t InitFollowEdgeSubHSM(void) {
    ES_Event returnEvent;

    FollowEdgeSubHSMCurrentState = FollowEdgeSubHSM_Init;
    returnEvent = RunFollowEdgeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t InitLauncherSubHSM(void) {
    ES_Event returnEvent;

    LauncherSubHSMCurrentState = LauncherSubHSM_Init;
    returnEvent = RunLauncherSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t InitAlignTapeSubHSM(void) {
    ES_Event returnEvent;

    AlignTapeSubHSMCurrentState = AlignTapeSubHSM_Init;
    returnEvent = RunAlignTapeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t InitFollowTapeSubHSM(void) {
    ES_Event returnEvent;

    FollowTapeSubHSMCurrentState = FollowTapeSubHSM_Init;
    returnEvent = RunFollowTapeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t InitAvoidObstacleSubHSM(void) {
    ES_Event returnEvent;

    AvoidObstacleSubHSMCurrentState = AvoidObstacleSubHSM_Init;
    returnEvent = RunAvoidObstacleSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

uint8_t InitFindBeaconSubHSM(void) {
    ES_Event returnEvent;

    FindBeaconSubHSMCurrentState = FindBeaconSubHSM_Init;
    returnEvent = RunFindBeaconSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}


//big ones
#define LOCATECORNER_TURNRIGHT_POWER 600

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
            ThisEvent = RunAlignTapeSubHSM(ThisEvent);
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
                    ES_Timer_InitTimer(LOCATECORNER_TURNRIGHT_TIMER, 500);
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
            ThisEvent = RunFollowTapeSubHSM(ThisEvent);
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

#define FOLLOWEDGE_TURNRIGHT_POWER 700

ES_Event RunFollowEdgeSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    FollowEdgeSubHSMState_t nextState = FollowEdgeSubHSMCurrentState;

    switch (FollowEdgeSubHSMCurrentState) {
        case FollowEdgeSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunFollowEdgeSubHSM: FollowEdgeSubHSM_Init: ES_INIT\r\n");
                nextState = FollowEdge_TurnRight;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;
        case FollowEdge_TurnRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFollowEdgeSubHSM: FollowEdge_TurnRight: ES_ENTRY\r\n");
                    PS_TankTurnRight(FOLLOWEDGE_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(FOLLOWEDGE_TURNRIGHT_TIMER, 1500);
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

ES_Event RunLauncherSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    LauncherSubHSMState_t nextState = LauncherSubHSMCurrentState;

    switch (LauncherSubHSMCurrentState) {
        case LauncherSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunLauncherSubHSM: LauncherSubHSM_Init: ES_INIT\r\n");
                nextState = Launcher_PreRev;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;
        case Launcher_PreRev:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunLauncherSubHSM: Launcher_PreRev: ES_ENTRY\r\n");

                    break;
                default:
                    break;
            }
            break;
        case Launcher_FindBeacon:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunLauncherSubHSM: Launcher_FindBeacon: ES_ENTRY\r\n");

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




















#define ALIGNTAPE_FORWARD_POWER 600
#define ALIGNTAPE_PIVOTRIGHT_POWER 600
#define ALIGNTAPE_PIVOTLEFT_POWER 600

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

/**
 * @Function RunTemplateSubHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */


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

                case MIDDLE_TAPE_DETECTED:
                    printf("RunFollowTapeSubHSM: FollowTape_Forward: MIDDLE_TAPE_DETECTED\r\n");
                    nextState = FollowTape_CorrectLeft;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case NO_TAPES_DETECTED:
                    printf("RunFollowTapeSubHSM: FollowTape_Forward: NO_TAPES_DETECTED\r\n");
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
                    PS_AngledLeft(FOLLOWTAPE_ANGLEDLEFT_POWER);
                    break;
                case NO_TAPES_DETECTED:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectLeft: tape lost, returning forward\r\n");
                    nextState = FollowTape_CorrectRight;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case FollowTape_CorrectRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectRight: ES_ENTRY\r\n");
                    PS_AngledRight(FOLLOWTAPE_ANGLEDRIGHT_POWER);
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunFollowTapeSubHSM: FollowTape_CorrectRight: MIDDLE_TAPE_DETECTED\r\n");
                    nextState = FollowTape_CorrectLeft;
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
        RunFollowTapeSubHSM(EXIT_EVENT);
        FollowTapeSubHSMCurrentState = nextState;
        RunFollowTapeSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}

#define AVOIDOBSTACLE_FORWARD_POWER 700
#define AVOIDOBSTACLE_TURNRIGHT_POWER 700
#define AVOIDOBSTACLE_TURNLEFT_POWER 700
#define AVOIDOBSTACLE_FORWARDDIST_POWER 700
#define AVOIDOBSTACLE_BACKWARDDIST_POWER 700
#define AVOIDOBSTACLE_PIVOT_POWER 700

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
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
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
                    ES_Timer_InitTimer(AVOIDOBSTACLE_BACKWARD_TIMER, 1500);
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
                    ES_Timer_InitTimer(AVOIDOBSTACLE_TURNRIGHT_TIMER, 500);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnRight: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == AVOIDOBSTACLE_TURNRIGHT_TIMER) {
                        PS_Stop();
                        //zero
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
                    ES_Timer_InitTimer(PING_CLEARANCE, 2000);
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
                    PS_TankTurnRight(AVOIDOBSTACLE_TURNRIGHT_POWER);
                    ES_Timer_InitTimer(AVOIDOBSTACLE_TURNRIGHT_TIMER, 500);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_TurnRight2: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == AVOIDOBSTACLE_TURNRIGHT_TIMER) {
                        PS_Stop();
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
                    break;
                case PING_FAR:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassSide: PING_FAR\r\n");
                    PS_Stop();
                    PS_Forward(AVOIDOBSTACLE_FORWARD_POWER);
                    ES_Timer_InitTimer(OBSTACLE_SIDE_CLEARANCE, 2000);
                    break;
                case ES_TIMEOUT:
                    printf("RunAvoidObstacleSubHSM: AvoidObstacle_PassSide: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == OBSTACLE_SIDE_CLEARANCE) {
                        PS_Stop();
                        //zero
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
                    ES_Timer_InitTimer(AVOIDOBSTACLE_TURNRIGHT_TIMER, 500);
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

ES_Event RunFindBeaconSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    FindBeaconSubHSMState_t nextState = FindBeaconSubHSMCurrentState;

    switch (FindBeaconSubHSMCurrentState) {
        case FindBeaconSubHSM_Init:
            if (ThisEvent.EventType == ES_INIT) {
                printf("RunFindBeaconSubHSM: FindBeaconSubHSM_Init: ES_INIT\r\n");
                nextState = SweepDataState;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SweepDataState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFindBeaconSubHSM: SweepDataState: ES_ENTRY\r\n");
                    StartSweepData();
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case BEACON_SAMPLE_READY:
                {
                    uint16_t filteredValue;

                    printf("RunFindBeaconSubHSM: SweepDataState: BEACON_SAMPLE_READY\r\n");
                    filteredValue = FilterBeaconSample(ThisEvent.EventParam);
                    if (filteredValue > MaxFilteredBeaconValue) {
                        MaxFilteredBeaconValue = filteredValue;
                    }

                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                }

                case ES_TIMEOUT:
                    printf("RunFindBeaconSubHSM: SweepDataState: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == HSM_ROTATION_TIMER) {
                        PS_Stop();
                        ThisEvent = TransitionFindBeaconSubState(LockOnState);
                    }
                    break;

                default:
                    break;
            }
            break;

        case LockOnState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFindBeaconSubHSM: LockOnState: ES_ENTRY\r\n");
                    StartLockOn();
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    printf("RunFindBeaconSubHSM: LockOnState: ES_TIMEOUT\r\n");
                    if ((ThisEvent.EventParam == HSM_ROTATION_TIMER)
                            && (LockOnTurnStarted == FALSE)) {
                        StartLockOnTurn();
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if ((ThisEvent.EventParam == HSM_ROTATION_TIMER)
                            && (LockOnTurnStarted == TRUE)) {
                        StartNextLockOnRotation();
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case BEACON_SAMPLE_READY:
                    printf("RunFindBeaconSubHSM: LockOnState: BEACON_SAMPLE_READY\r\n");
                    if (LockOnTurnStarted == FALSE) {
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else {
                        uint16_t filteredValue;

                        filteredValue = FilterBeaconSample(ThisEvent.EventParam);
                        if (filteredValue > LockOnRotationMax) {
                            LockOnRotationMax = filteredValue;
                        }

                        if (filteredValue >= BeaconLockThreshold) {
                            PS_Stop();
                            TapeEventsEnabled = TRUE;
                            ThisEvent = TransitionFindBeaconSubState(OnFieldState);
                        } else {
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                    }
                    break;

                default:
                    break;
            }
            break;

        case OnFieldState:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFindBeaconSubHSM: OnFieldState: ES_ENTRY\r\n");
                    OnFieldStopped = FALSE;
                    DriveBackward();
                    ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                            HSM_ON_FIELD_BACKUP_TIME_MS);
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    printf("RunFindBeaconSubHSM: OnFieldState: ES_TIMEOUT\r\n");
                    if (ThisEvent.EventParam == HSM_ROTATION_TIMER) {
                        if (OnFieldStopped == FALSE) {
                            PS_Stop();
                            OnFieldStopped = TRUE;
                            ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                                    HSM_ON_FIELD_STOP_DELAY_MS);
                            ThisEvent.EventType = ES_NO_EVENT;
                        } else {
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
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
        RunFindBeaconSubHSM(EXIT_EVENT);
        FindBeaconSubHSMCurrentState = nextState;
        RunFindBeaconSubHSM(ENTRY_EVENT);
    }

    return ThisEvent;
}

ES_Event RunTemplateSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    TemplateSubHSMState_t nextState; // <- change type to correct enum

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                printf("RunTemplateSubHSM: InitPSubState: ES_INIT\r\n");
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = SubFirstState;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SubFirstState: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    printf("RunTemplateSubHSM: SubFirstState: ES_NO_EVENT\r\n");
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunTemplateSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunTemplateSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    return ThisEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

static ES_Event TransitionFindBeaconSubState(FindBeaconSubHSMState_t nextState) {
    ES_Event returnEvent;

    RunFindBeaconSubHSM(EXIT_EVENT);
    FindBeaconSubHSMCurrentState = nextState;
    returnEvent = RunFindBeaconSubHSM(ENTRY_EVENT);

    return returnEvent;
}

static void StartSweepData(void) {
    ResetBeaconFilter();
    MaxFilteredBeaconValue = 0;

    PS_RightMtrSpeed(HSM_SWEEP_TURN_POWER);
    PS_LeftMtrSpeed(HSM_SWEEP_TURN_POWER * -1);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

static void StartLockOn(void) {
    ResetBeaconFilter();
    PS_Stop();
    LockOnTurnStarted = FALSE;

    if (MaxFilteredBeaconValue > HSM_BEACON_LOCK_OFFSET) {
        BeaconLockThreshold = MaxFilteredBeaconValue - HSM_BEACON_LOCK_OFFSET;
    } else {
        BeaconLockThreshold = 0;
    }

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_ON_START_DELAY_MS);
}

static void StartLockOnTurn(void) {
    LockOnTurnStarted = TRUE;
    LockOnRotationMax = 0;
    ResetBeaconFilter();

    PS_RightMtrSpeed(HSM_LOCK_TURN_POWER);
    PS_LeftMtrSpeed(HSM_LOCK_TURN_POWER * -1);

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

static void StartNextLockOnRotation(void) {
    if (LockOnRotationMax > 0) {
        MaxFilteredBeaconValue = LockOnRotationMax;
    }

    if (MaxFilteredBeaconValue > HSM_BEACON_LOCK_OFFSET) {
        BeaconLockThreshold = MaxFilteredBeaconValue - HSM_BEACON_LOCK_OFFSET;
    } else {
        BeaconLockThreshold = 0;
    }

    LockOnRotationMax = 0;
    ResetBeaconFilter();

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

static void ResetBeaconFilter(void) {
    uint8_t i;

    BeaconSampleIndex = 0;
    BeaconSampleCount = 0;
    BeaconSampleSum = 0;

    for (i = 0; i < HSM_BEACON_AVG_WINDOW; i++) {
        BeaconSamples[i] = 0;
    }
}

static uint16_t FilterBeaconSample(uint16_t sample) {
    if (BeaconSampleCount < HSM_BEACON_AVG_WINDOW) {
        BeaconSampleCount++;
    } else {
        BeaconSampleSum -= BeaconSamples[BeaconSampleIndex];
    }

    BeaconSamples[BeaconSampleIndex] = sample;
    BeaconSampleSum += sample;

    BeaconSampleIndex++;
    if (BeaconSampleIndex >= HSM_BEACON_AVG_WINDOW) {
        BeaconSampleIndex = 0;
    }

    return (uint16_t) (BeaconSampleSum / BeaconSampleCount);
}

static void DriveBackward(void) {
    PS_Backward(HSM_DRIVE_POWER);
}
