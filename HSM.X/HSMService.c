/*
 * HSMService.c
 *
 * ES Framework service containing a hierarchical state machine for startup
 * beacon alignment.
 */

#include "HSMService.h"

#include <stdio.h>

#include "SensorsEventChecker.h"
#include "peashooter.h"
#include "BeaconEventChecker.h"
#include "ES_Framework.h"
#include "ES_Timers.h"

#define HSM_TANK_TURN_POWER 800
#define HSM_FULL_ROTATION_TIME_MS 5000
#define HSM_BORDER_SEARCH_POWER 800
#define HSM_BORDER_BACKUP_POWER 800
#define HSM_BORDER_BACKUP_DIST 2
#define HSM_CORRECT_TIMER 2

typedef enum {
    InitPState,
    BeaconAlignmentState,
    SearchForFrontBorderState,
    SearchForSideBorderState,
    RideTapeState,
    AvoidObstableState,
    STOPPEDState,
} HSMState_t;

typedef enum {
    InitPSubState,
    SurveySpinState,
    AlignSpinState,
    AlignedState,
} BeaconAlignmentSubState_t;

typedef enum {
    InitFrontBorderSubState,
    SearchForFrontBorderSubState,
    LeftFrontTapeFoundSubState,
    RightFrontTapeFoundSubState,
    FrontBorderFoundSubState,
    MoveAlongBorderSubState,
} SearchForFrontBorderSubState_t;

typedef enum {
    InitSideBorderSubState,
    SearchForSideBorderSubState,
    LeftSideTapeFoundSubState,
    SideBorderFoundSubState,
    MoveAlongSideBorderSubState,
} SearchForSideBorderSubState_t;



typedef enum { //turn down motor on same side of sensor, then turn it back on when it doesnt detect
    InitRideTapeSubState,
    AngledForward,
    CorrectLeftSubState,
} RideTapeSubState_t;


typedef enum {
    InitAvoidObstacleSubState,
    
} AvoidObstacleSubState_t;



static uint8_t MyPriority;
static HSMState_t CurrentState;
static BeaconAlignmentSubState_t CurrentBeaconSubState;
static SearchForFrontBorderSubState_t CurrentFrontBorderSubState;
static SearchForSideBorderSubState_t CurrentSideBorderSubState;
static RideTapeSubState_t CurrentRideTapeSubState;

static uint16_t MaxBeaconValue;
static uint32_t SurveyStartTime;
static uint32_t MaxBeaconOffset;
static uint32_t AlignmentSpinTime;

static void StartSurveySpin(void);
static void RecordBeaconSample(uint16_t beaconValue);
static void StartAlignmentSpin(void);
static uint8_t InitBeaconAlignmentSubHSM(void);
static ES_Event RunBeaconAlignmentSubHSM(ES_Event thisEvent);
static uint8_t InitSearchForFrontBorderSubHSM(void);
static ES_Event RunSearchForFrontBorderSubHSM(ES_Event thisEvent);
static uint8_t InitSearchForSideBorderSubHSM(void);
static ES_Event RunSearchForSideBorderSubHSM(ES_Event thisEvent);
static uint8_t InitRideTapeSubHSM(void);
static ES_Event RunRideTapeSubHSM(ES_Event thisEvent);

uint8_t InitHSMService(uint8_t priority) {
    MyPriority = priority;
    CurrentState = InitPState;

    PS_Init();
    PS_Stop();

    printf("InitHSMService: Started\r\n",
            BEACON_DETECTOR_PIN_NAME, HSM_TANK_TURN_POWER,
            HSM_FULL_ROTATION_TIME_MS);

    if (InitBeaconEventChecker() != TRUE) {
        return FALSE;
    }

    return ES_PostToService(MyPriority, INIT_EVENT);
}

uint8_t PostHSMService(ES_Event thisEvent) {
    return ES_PostToService(MyPriority, thisEvent);
}

ES_Event RunHSMService(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    HSMState_t nextState = CurrentState;

    switch (CurrentState) {
        case InitPState:
            if (thisEvent.EventType == ES_INIT) {
                // nextState = BeaconAlignmentState;
                nextState = SearchForFrontBorderState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case BeaconAlignmentState:
            if (thisEvent.EventType == ES_ENTRY) {
                if (InitBeaconAlignmentSubHSM() != TRUE) {
                    thisEvent.EventType = ES_ERROR;
                } else {
                    thisEvent.EventType = ES_NO_EVENT;
                }
            } else {
                thisEvent = RunBeaconAlignmentSubHSM(thisEvent);

                if (CurrentBeaconSubState == AlignedState) {
                    nextState = SearchForFrontBorderState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                }
            }
            break;
        case SearchForFrontBorderState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (InitSearchForFrontBorderSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunSearchForFrontBorderSubHSM(thisEvent);

                    if (thisEvent.EventType == FRONT_BORDER_DONE) {
                        nextState = SearchForSideBorderState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case SearchForSideBorderState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHSMService: SearchForSideBorderState\r\n");
                    if (InitSearchForSideBorderSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunSearchForSideBorderSubHSM(thisEvent);

                    if (thisEvent.EventType == SIDE_BORDER_DONE) {
                        nextState = RideTapeState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case RideTapeState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (InitRideTapeSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunRideTapeSubHSM(thisEvent);
                    break;
            }
            break;

        case STOPPEDState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                printf("RunHSMService: STOPPEDState\r\n");
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunHSMService(EXIT_EVENT);
        CurrentState = nextState;
        RunHSMService(ENTRY_EVENT);
    }

    return thisEvent;
}

static uint8_t InitBeaconAlignmentSubHSM(void) {
    ES_Event returnEvent;

    CurrentBeaconSubState = InitPSubState;
    returnEvent = RunBeaconAlignmentSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitRideTapeSubHSM(void) {
    ES_Event returnEvent;

    CurrentRideTapeSubState = InitRideTapeSubState;
    returnEvent = RunRideTapeSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSearchForFrontBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentFrontBorderSubState = InitFrontBorderSubState;
    returnEvent = RunSearchForFrontBorderSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSearchForSideBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentSideBorderSubState = InitSideBorderSubState;
    returnEvent = RunSearchForSideBorderSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static ES_Event RunBeaconAlignmentSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    BeaconAlignmentSubState_t nextState = CurrentBeaconSubState;

    switch (CurrentBeaconSubState) {
        case InitPSubState:
            if (thisEvent.EventType == ES_INIT) {
                nextState = SurveySpinState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SurveySpinState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    StartSurveySpin();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case BEACON_SAMPLE_READY:
                    RecordBeaconSample(thisEvent.EventParam);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        PS_Stop();
                        nextState = AlignSpinState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    break;
            }
            break;

        case AlignSpinState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    StartAlignmentSpin();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        PS_Stop();
                        nextState = AlignedState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    break;
            }
            break;

        case AlignedState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                printf("RunHSMService: AlignedState\r\n");
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunBeaconAlignmentSubHSM(EXIT_EVENT);
        CurrentBeaconSubState = nextState;
        RunBeaconAlignmentSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static ES_Event RunSearchForFrontBorderSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    SearchForFrontBorderSubState_t nextState = CurrentFrontBorderSubState;

    switch (CurrentFrontBorderSubState) {
        case InitFrontBorderSubState:
            if (thisEvent.EventType == ES_INIT) {
                nextState = SearchForFrontBorderSubState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SearchForFrontBorderSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunSearchForFrontBorderSubHSM: SearchForFrontBorderSubState: ES_ENTRY\r\n");
                    PS_Forward(HSM_BORDER_SEARCH_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunSearchForFrontBorderSubHSM: LeftFrontTapeFoundSubState\r\n");
                    nextState = LeftFrontTapeFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunSearchForFrontBorderSubHSM: RightFrontTapeFoundSubState\r\n");
                    nextState = RightFrontTapeFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case LeftFrontTapeFoundSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_PivotTurnLeft(HSM_TANK_TURN_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    nextState = FrontBorderFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case RightFrontTapeFoundSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_PivotTurnRight(HSM_TANK_TURN_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    nextState = FrontBorderFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case FrontBorderFoundSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                printf("RunSearchForFrontBorderSubHSM: FrontBorderFoundSubState\r\n");
                nextState = MoveAlongBorderSubState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveAlongBorderSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("RunSearchForFrontBorderSubHSM: MoveAlongBorderSubState\r\n");
                PS_BackwardDist(HSM_BORDER_BACKUP_POWER, HSM_BORDER_BACKUP_DIST);
                PS_TurnRight90();
                thisEvent.EventType = FRONT_BORDER_DONE;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunSearchForFrontBorderSubHSM(EXIT_EVENT);
        CurrentFrontBorderSubState = nextState;
        thisEvent = RunSearchForFrontBorderSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static ES_Event RunSearchForSideBorderSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    SearchForSideBorderSubState_t nextState = CurrentSideBorderSubState;

    switch (CurrentSideBorderSubState) {
        case InitSideBorderSubState:
            if (thisEvent.EventType == ES_INIT) {
                nextState = SearchForSideBorderSubState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SearchForSideBorderSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunSearchForSideBorderSubHSM: SearchForSideBorderSubState: ES_ENTRY\r\n");
                    PS_Forward(HSM_BORDER_SEARCH_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunSearchForSideBorderSubHSM: SearchForSideBorderSubState: LEFT_TAPE_DETECTED\r\n");
                    nextState = LeftSideTapeFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case LeftSideTapeFoundSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 1);
                    PS_TurnRight90();
                    thisEvent.EventType = SIDE_BORDER_DONE;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunSearchForSideBorderSubHSM(EXIT_EVENT);
        CurrentSideBorderSubState = nextState;
        thisEvent = RunSearchForSideBorderSubHSM(ENTRY_EVENT);
    }


    return thisEvent;
}

static ES_Event RunRideTapeSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    RideTapeSubState_t nextState = CurrentRideTapeSubState;

    switch (CurrentRideTapeSubState) {
        case InitRideTapeSubState:
            if (thisEvent.EventType == ES_INIT) {
                PS_Stop();
                printf("RunRideTapeSubHSM: InitRideTapeSubState\r\n");
                nextState = AngledForward;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AngledForward:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideTapeSubHSM: AngledForward: ES_ENTRY\r\n");

                    // left motor faster, right motor slower = slight right curve
                    PS_AngledForward(500);

                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideTapeSubHSM: AngledForward: MIDDLE_TAPE_DETECTED\r\n");
                    nextState = CorrectLeftSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case CorrectLeftSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideTapeSubHSM: CorrectLeftSubState: ES_ENTRY\r\n");

                    PS_TankTurnRightDist(500, 5);
                    
                    nextState = AngledForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                default:
                    break;
            }
    break;
    }

    if (makeTransition == TRUE) {
        RunRideTapeSubHSM(EXIT_EVENT);
        CurrentRideTapeSubState = nextState;
        RunRideTapeSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static void StartSurveySpin(void) {
    MaxBeaconValue = 0;
    MaxBeaconOffset = 0;
    AlignmentSpinTime = HSM_FULL_ROTATION_TIME_MS;
    SurveyStartTime = ES_Timer_GetTime();

    printf("RunSurveySpin: Survey spin started\r\n");
    PS_TankTurnLeft(50);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_FULL_ROTATION_TIME_MS);
}

static void RecordBeaconSample(uint16_t beaconValue) {
    uint32_t elapsedTime;

    if (beaconValue <= MaxBeaconValue) {
        return;
    }

    elapsedTime = ES_Timer_GetTime() - SurveyStartTime;
    if (elapsedTime > HSM_FULL_ROTATION_TIME_MS) {
        elapsedTime = HSM_FULL_ROTATION_TIME_MS;
    }

    MaxBeaconValue = beaconValue;
    MaxBeaconOffset = elapsedTime;
}

static void StartAlignmentSpin(void) {
    AlignmentSpinTime = MaxBeaconOffset;
    if (AlignmentSpinTime == 0) {
        AlignmentSpinTime = HSM_FULL_ROTATION_TIME_MS;
    }

    printf("Alignment spin started max=%u offset=%lu ms\r\n",
            MaxBeaconValue, (unsigned long) MaxBeaconOffset);

    PS_TankTurnLeft(HSM_TANK_TURN_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, AlignmentSpinTime);
}

