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

#define HSM_TANK_TURN_POWER 900
#define HSM_FULL_ROTATION_TIME_MS 5000
#define HSM_BORDER_SEARCH_POWER 900
#define HSM_BORDER_BACKUP_POWER 900
#define HSM_BORDER_BACKUP_DIST 3
#define HSM_CORRECT_TIMER 2

typedef enum {
    InitPState,
    BeaconAlignmentState,
    SearchForFrontBorderState,
    SearchForSideBorderState,
    RideTapeState,
    RideOtherTapeState,
    RideFrontTapeState,
    AvoidObstableState,
    FirstObstacleClearedState,
    SecondObstacleClearedState,
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
    InitRideOtherTapeSubState,
    AngledOtherForward,
    CorrectRightSubState,
} RideOtherTapeSubState_t;

typedef enum {
    InitRideFrontTapeSubState,
    AngledFrontForward,
    CorrectLeftSubState1,
} RideFrontTapeSubState_t;

typedef enum {
    InitFirstObstacleClearedSub,
    SearchForBorderSubState,
    LeftFrontTapeFoundSubState1,
    RightFrontTapeFoundSubState1,
    BorderFoundSubState,
    MoveAlongBorderSubState1,
} FirstObstacleClearedSubState_t;

typedef enum {
    InitSecondObstacleClearedSub,
    SearchForBorderSubState1,
    LeftFrontTapeFoundSubState2,
    RightFrontTapeFoundSubState2,
    BorderFoundSubState1,
    MoveAlongBorderSubState2,
} SecondObstacleClearedSubState_t;



static uint8_t MyPriority;
static HSMState_t CurrentState;
static BeaconAlignmentSubState_t CurrentBeaconSubState;
static SearchForFrontBorderSubState_t CurrentFrontBorderSubState;
static SearchForSideBorderSubState_t CurrentSideBorderSubState;
static RideTapeSubState_t CurrentRideTapeSubState;
static RideFrontTapeSubState_t CurrentRideFrontTapeSubState;
static FirstObstacleClearedSubState_t CurrentFirstObstacleClearedSubState;
static RideOtherTapeSubState_t CurrentRideOtherTapeSubState;
static SecondObstacleClearedSubState_t CurrentSecondObstacleClearedSubState;

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
static uint8_t InitRideTapeSubHSM(void);
static ES_Event RunRideTapeSubHSM(ES_Event thisEvent);
static uint8_t InitRideFrontTapeSubHSM(void);
static ES_Event RunRideFrontTapeSubHSM(ES_Event thisEvent);
static uint8_t InitRideOtherTapeSubHSM(void);
static ES_Event RunRideOtherTapeSubHSM(ES_Event thisEvent);
static uint8_t InitFirstObstacleClearedSubHSM(void);
static ES_Event RunFirstObstacleClearedSubHSM(ES_Event thisEvent);
static uint8_t InitSecondObstacleClearedSubHSM(void);
static ES_Event RunSecondObstacleClearedSubHSM(ES_Event thisEvent);

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
                printf("RunHSMService: InitPState\r\n");
                nextState = BeaconAlignmentState;
                // nextState = SearchForFrontBorderState;
                // nextState = RideTapeState;
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

                    if (thisEvent.EventType == FRONT_BORDER_ALIGNED) {
                        nextState = RideFrontTapeState;
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
                    if (thisEvent.EventType == ISZ_BORDER) {
                        nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    if (thisEvent.EventType == BUMPER_TRIPPED) {
                        nextState = FirstObstacleClearedState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case RideFrontTapeState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (InitRideFrontTapeSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunRideFrontTapeSubHSM(thisEvent);
                    if (thisEvent.EventType == FRONT_BORDER_DONE) {
                        nextState = RideTapeState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case RideOtherTapeState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (InitRideOtherTapeSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunRideOtherTapeSubHSM(thisEvent);
                    if (thisEvent.EventType == ISZ_BORDER) {
                        nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    if (thisEvent.EventType == BUMPER_TRIPPED) {
                        nextState = SecondObstacleClearedState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;


        case FirstObstacleClearedState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHSMService: FirstObstacleClearedState\r\n");
                    if (InitFirstObstacleClearedSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunFirstObstacleClearedSubHSM(thisEvent);
                    if (thisEvent.EventType == FIRST_BORDER_DONE) {
                        nextState = RideOtherTapeState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case SecondObstacleClearedState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHSMService: SecondObstacleClearedState\r\n");
                    if (InitSecondObstacleClearedSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunSecondObstacleClearedSubHSM(thisEvent);
                    if (thisEvent.EventType == SECOND_BORDER_DONE) {
                        nextState = RideTapeState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
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

static uint8_t InitRideFrontTapeSubHSM(void) {
    ES_Event returnEvent;

    CurrentRideFrontTapeSubState = InitRideFrontTapeSubState;
    returnEvent = RunRideFrontTapeSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitRideOtherTapeSubHSM(void) {
    ES_Event returnEvent;

    CurrentRideOtherTapeSubState = InitRideOtherTapeSubState;
    returnEvent = RunRideOtherTapeSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSearchForFrontBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentFrontBorderSubState = InitFrontBorderSubState;
    returnEvent = RunSearchForFrontBorderSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitFirstObstacleClearedSubHSM(void) {
    ES_Event returnEvent;

    CurrentFirstObstacleClearedSubState = InitFirstObstacleClearedSub;
    returnEvent = RunFirstObstacleClearedSubHSM(INIT_EVENT);

    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSecondObstacleClearedSubHSM(void) {
    ES_Event returnEvent;

    CurrentSecondObstacleClearedSubState = InitSecondObstacleClearedSub;
    returnEvent = RunSecondObstacleClearedSubHSM(INIT_EVENT);

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
                thisEvent.EventType = FRONT_BORDER_ALIGNED;
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

static ES_Event RunSecondObstacleClearedSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    SecondObstacleClearedSubState_t nextState = CurrentSecondObstacleClearedSubState;

    switch (CurrentSecondObstacleClearedSubState) {
        case InitSecondObstacleClearedSub:
            if (thisEvent.EventType == ES_INIT) {
                nextState = SearchForBorderSubState1;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SearchForBorderSubState1:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunSearchForSecondObstacleClearedSubHSM: SearchForSecondObstacleClearedSubState: ES_ENTRY\r\n");
                    PS_Forward(HSM_BORDER_SEARCH_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunSearchForFrontBorderSubHSM: LeftFrontTapeFoundSubState\r\n");
                    nextState = LeftFrontTapeFoundSubState2;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunSearchForFrontBorderSubHSM: RightFrontTapeFoundSubState\r\n");
                    nextState = RightFrontTapeFoundSubState2;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case LeftFrontTapeFoundSubState2:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_PivotTurnLeft(HSM_TANK_TURN_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    nextState = BorderFoundSubState1;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case RightFrontTapeFoundSubState2:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_PivotTurnRight(HSM_TANK_TURN_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    nextState = BorderFoundSubState1;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case BorderFoundSubState1:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                printf("RunSearchForFrontBorderSubHSM: FrontBorderFoundSubState\r\n");
                nextState = MoveAlongBorderSubState2;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveAlongBorderSubState2:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("RunSearchForFrontBorderSubHSM: MoveAlongBorderSubState\r\n");
                PS_BackwardDist(HSM_BORDER_BACKUP_POWER, HSM_BORDER_BACKUP_DIST);
                PS_TurnRight90();
                thisEvent.EventType = SECOND_BORDER_DONE;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunSecondObstacleClearedSubHSM(EXIT_EVENT);
        CurrentSecondObstacleClearedSubState = nextState;
        thisEvent = RunSecondObstacleClearedSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static ES_Event RunFirstObstacleClearedSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    FirstObstacleClearedSubState_t nextState = CurrentFirstObstacleClearedSubState;

    switch (CurrentFirstObstacleClearedSubState) {
        case InitFirstObstacleClearedSub:
            if (thisEvent.EventType == ES_INIT) {
                nextState = SearchForBorderSubState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SearchForBorderSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunFirstObstacleClearedSubHSM: SearchForBorderSubState: ES_ENTRY\r\n");
                    PS_Forward(HSM_BORDER_SEARCH_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunFirstObstacleClearedSubHSM: LeftFrontTapeFoundSubState\r\n");
                    nextState = LeftFrontTapeFoundSubState1;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunFirstObstacleClearedSubHSM: RightFrontTapeFoundSubState\r\n");
                    nextState = RightFrontTapeFoundSubState1;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case LeftFrontTapeFoundSubState1:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_PivotTurnLeft(HSM_TANK_TURN_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    nextState = BorderFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case RightFrontTapeFoundSubState1:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_PivotTurnRight(HSM_TANK_TURN_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    nextState = BorderFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case BorderFoundSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                printf("RunFirstObstacleClearedSubHSM: BorderFoundSubState\r\n");
                nextState = MoveAlongBorderSubState1;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveAlongBorderSubState1:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("RunFirstObstacleClearedSubHSM: MoveAlongBorderSubState\r\n");
                PS_BackwardDist(HSM_BORDER_BACKUP_POWER, HSM_BORDER_BACKUP_DIST);
                PS_TurnLeft90();
                thisEvent.EventType = FIRST_BORDER_DONE;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        RunFirstObstacleClearedSubHSM(EXIT_EVENT);
        CurrentFirstObstacleClearedSubState = nextState;
        thisEvent = RunFirstObstacleClearedSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static ES_Event RunRideFrontTapeSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    RideFrontTapeSubState_t nextState = CurrentRideFrontTapeSubState;

    switch (CurrentRideFrontTapeSubState) {
        case InitRideFrontTapeSubState:
            if (thisEvent.EventType == ES_INIT) {
                PS_Stop();
                printf("RunRideFrontTapeSubHSM: InitRideFrontTapeSubState\r\n");

                nextState = AngledFrontForward;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AngledFrontForward:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideFrontTapeSubHSM: AngledFrontForward: ES_ENTRY\r\n");

                    PS_AngledForward(800);

                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunRideFrontTapeSubHSM: AngledFrontForward: RIGHT_TAPE_DETECTED\r\n");

                    nextState = CorrectLeftSubState1;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideFrontTapeSubHSM: AngledFrontForward: MIDDLE_TAPE_DETECTED\r\n");
                    PS_Stop();
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 2);
                    PS_TurnRight90();
                    thisEvent.EventType = FRONT_BORDER_DONE;
                    break;

                default:
                    break;
            }
            break;

        case CorrectLeftSubState1:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideFrontTapeSubHSM: CorrectLeftSubState1: ES_ENTRY\r\n");

                    PS_TankTurnRightDist(700, 2);

                    nextState = AngledFrontForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideFrontTapeSubHSM: CorrectLeftSubState1: MIDDLE_TAPE_DETECTED\r\n");
                    PS_Stop();
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 2);
                    PS_TurnRight90();
                    thisEvent.EventType = FRONT_BORDER_DONE;
                    break;

                default:
                    break;
            }
            break;
    }

    if (makeTransition == TRUE) {
        RunRideFrontTapeSubHSM(EXIT_EVENT);
        CurrentRideFrontTapeSubState = nextState;
        RunRideFrontTapeSubHSM(ENTRY_EVENT);
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
                    PS_AngledForward(800);

                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunRideTapeSubHSM: AngledForward: RIGHT_TAPE_DETECTED\r\n");
                    nextState = CorrectLeftSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideTapeSubHSM: AngledForward: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 1);
                    printf("Backup done\r\n");

                    PS_TurnRight90();
                    printf("Turn done\r\n");

                    PS_ForwardDist(HSM_BORDER_SEARCH_POWER, 12);
                    printf("Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideTapeSubHSM: AngledForward: MIDDLE_TAPE_DETECTED\r\n");
                    PS_Stop();
                    thisEvent.EventType = ISZ_BORDER;
                    break;

                default:
                    break;
            }
            break;

        case CorrectLeftSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideTapeSubHSM: CorrectLeftSubState: ES_ENTRY\r\n");

                    PS_TankTurnRightDist(700, 2);

                    nextState = AngledForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideTapeSubHSM: CorrectLeftSubState: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 1);
                    printf("Backup done\r\n");

                    PS_TurnRight90();
                    printf("Turn done\r\n");

                    PS_ForwardDist(HSM_BORDER_SEARCH_POWER, 12);
                    printf("Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;
                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideTapeSubHSM: AngledForward: MIDDLE_TAPE_DETECTED\r\n");

                    PS_Stop();
                    thisEvent.EventType = ISZ_BORDER;
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

static ES_Event RunRideOtherTapeSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    RideOtherTapeSubState_t nextState = CurrentRideOtherTapeSubState;

    switch (CurrentRideOtherTapeSubState) {
        case InitRideOtherTapeSubState:
            if (thisEvent.EventType == ES_INIT) {
                PS_Stop();
                printf("RunRideOtherTapeSubHSM: InitRideOtherTapeSubState\r\n");
                nextState = AngledOtherForward;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AngledOtherForward:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideOtherTapeSubHSM: AngledOtherForward: ES_ENTRY\r\n");

                    // left motor faster, right motor slower = slight right curve
                    PS_AngledForward2(800);

                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunRideOtherTapeSubHSM: AngledOtherForward: LEFT_TAPE_DETECTED\r\n");
                    nextState = CorrectRightSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideOtherTapeSubHSM: AngledOtherForward: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 1);
                    printf("Backup done\r\n");

                    PS_TurnLeft90();
                    printf("Turn done\r\n");

                    PS_ForwardDist(HSM_BORDER_SEARCH_POWER, 12);
                    printf("Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideOtherTapeSubHSM: AngledOtherForward: MIDDLE_TAPE_DETECTED\r\n");
                    PS_Stop();
                    thisEvent.EventType = ISZ_BORDER;
                    break;

                default:
                    break;
            }
            break;

        case CorrectRightSubState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideTapeSubHSM: CorrectLeftSubState: ES_ENTRY\r\n");

                    PS_TankTurnLeftDist(700, 2);

                    nextState = AngledOtherForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideTapeSubHSM: CorrectRightSubState: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(HSM_BORDER_BACKUP_POWER, 1);
                    printf("Backup done\r\n");

                    PS_TurnLeft90();
                    printf("Turn done\r\n");

                    PS_ForwardDist(HSM_BORDER_SEARCH_POWER, 12);
                    printf("Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;
                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideTapeSubHSM: CorrectRightSubState: MIDDLE_TAPE_DETECTED\r\n");

                    PS_Stop();
                    thisEvent.EventType = ISZ_BORDER;
                    break;
                default:
                    break;
            }
            break;
    }

    if (makeTransition == TRUE) {
        RunRideOtherTapeSubHSM(EXIT_EVENT);
        CurrentRideOtherTapeSubState = nextState;
        RunRideOtherTapeSubHSM(ENTRY_EVENT);
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

