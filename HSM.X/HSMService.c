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

//called in pivotturn
#define ALIGN_TO_FRONT_BORDER_POWER 900
#define ALIGN_TO_RIGHT_BORDER_POWER 900


#define HSM_FULL_ROTATION_TIME_MS 5000

//called in forward
#define SEARCH_FOR_FRONT_BORDER_POWER 900
#define SEARCH_FOR_RIGHT_BORDER_POWER 900

//called in forwarddist
#define OBSTACLE_TO_RIGHT_POWER 900
#define OBSTACLE_TO_RIGHT_DIST 12

#define OBSTACLE_TO_RIGHT_DIST 12


//called in backwarddist
#define ALIGN_TO_FRONT_BORDER_BACKWARD_POWER 900
#define ALIGN_TO_FRONT_BORDER_BACKWARD_DIST 2
#define FRONT_TO_LEFT_BORDER_BACKWARD_POWER 900
#define FRONT_TO_LEFT_BORDER_BACKWARD_DIST 2
#define LEFT_TO_OBSTACLE_BACKWARD_POWER 900
#define LEFT_TO_OBSTACLE_BACKWARD_DIST 2
#define OBSTACLE_TO_RIGHT_BACKWARD_POWER 900
#define OBSTACLE_TO_RIGHT_BACKWARD_DIST 2
#define RIGHT_TO_OBSTACLE_BACKWARD_POWER 900
#define RIGHT_TO_OBSTACLE_BACKWARD_DIST 2


//called in tankturn
#define ALIGN_TO_FRONT_BORDER_TANK_POWER 900
#define ALIGN_TO_FRONT_BORDER_TANK_DIST 4
#define FRONT_TO_LEFT_BORDER_TANK_POWER 900
#define FRONT_TO_LEFT_BORDER_TANK_DIST 4
#define LEFT_TO_OBSTACLE_TANK_POWER 700
#define LEFT_TO_OBSTACLE_TANK_DIST 7.7
#define OBSTACLE_TO_RIGHT_TANK_POWER 900
#define OBSTACLE_TO_RIGHT_TANK_DIST 4
#define RIGHT_TO_OBSTACLE_TANK_POWER 700
#define RIGHT_TO_OBSTACLE_TANK_DIST 7.7

//angledForward
#define ANGLED_FORWARD_POWER 800

#define HSM_CORRECT_TIMER 2
#define LEAVE_OBSTACLE_TIME 6.5

typedef enum {
    InitPState,
    BeaconAlignmentState,
    SearchForFrontBorderState,
    SearchForSideBorderState,
    RideLeftTapeState,
    RideRIghtTapeState,
    RideFrontTapeState,
    SearchForRightBorderState,
    SearchForLeftBorderState,
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
    FrontToLeftBorderSubState,
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
} RideLeftTapeSubState_t;

typedef enum {
    InitRideOtherTapeSubState,
    AngledOtherForward,
    CorrectRightSubState,
} RideRightTapeSubState_t;

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
} SearchForRightBorderSubState_t;

typedef enum {
    InitSecondObstacleClearedSub,
    SearchForBorderSubState1,
    LeftFrontTapeFoundSubState2,
    RightFrontTapeFoundSubState2,
    BorderFoundSubState1,
    MoveAlongBorderSubState2,
} SearchForLeftBorderSubState_t;



static uint8_t MyPriority;
static HSMState_t CurrentState;
static BeaconAlignmentSubState_t CurrentBeaconSubState;
static SearchForFrontBorderSubState_t CurrentFrontBorderSubState;
static SearchForSideBorderSubState_t CurrentSideBorderSubState;
static RideLeftTapeSubState_t CurrentRideTapeSubState;
static RideFrontTapeSubState_t CurrentRideFrontTapeSubState;
static SearchForRightBorderSubState_t CurrentFirstObstacleClearedSubState;
static RideRightTapeSubState_t CurrentRideOtherTapeSubState;
static SearchForLeftBorderSubState_t CurrentSecondObstacleClearedSubState;

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
static ES_Event RunRideLeftTapeSubHSM(ES_Event thisEvent);
static uint8_t InitRideFrontTapeSubHSM(void);
static ES_Event RunRideFrontTapeSubHSM(ES_Event thisEvent);
static uint8_t InitRideOtherTapeSubHSM(void);
static ES_Event RunRideRightTapeSubHSM(ES_Event thisEvent);
static uint8_t InitSearchForRightBorderSubHSM(void);
static ES_Event SearchForRightBorderSubHSM(ES_Event thisEvent);
static uint8_t InitSearchForLeftBorderSubHSM(void);
static ES_Event SearchForLeftBorderSubHSM(ES_Event thisEvent);

uint8_t InitHSMService(uint8_t priority) {
    MyPriority = priority;
    CurrentState = InitPState;

    PS_Init();
    PS_Stop();

    printf("InitHSMService: Started\r\n",
            BEACON_DETECTOR_PIN_NAME, ALIGN_TO_FRONT_BORDER_POWER,
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
                        // nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case RideLeftTapeState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (InitRideTapeSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunRideLeftTapeSubHSM(thisEvent);
                    if (thisEvent.EventType == ISZ_BORDER) {
                        nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    if (thisEvent.EventType == BUMPER_TRIPPED) {
                        nextState = SearchForRightBorderState;
                        // nextState = STOPPEDState;
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
                        nextState = RideLeftTapeState;
                        // nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case RideRIghtTapeState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (InitRideOtherTapeSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = RunRideRightTapeSubHSM(thisEvent);
                    if (thisEvent.EventType == ISZ_BORDER) {
                        nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    if (thisEvent.EventType == BUMPER_TRIPPED) {
                        // nextState = SearchForLeftBorderState;
                            nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;


        case SearchForRightBorderState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHSMService: SearchForRightBorderState\r\n");
                    if (InitSearchForRightBorderSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = SearchForRightBorderSubHSM(thisEvent);
                    if (thisEvent.EventType == FIRST_BORDER_DONE) {
                        nextState = RideRIghtTapeState;
                        // nextState = STOPPEDState;
                        makeTransition = TRUE;
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case SearchForLeftBorderState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHSMService: SearchForLeftBorderState\r\n");
                    if (InitSearchForLeftBorderSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = SearchForLeftBorderSubHSM(thisEvent);
                    if (thisEvent.EventType == SECOND_BORDER_DONE) {
                        nextState = RideLeftTapeState;
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
    returnEvent = RunRideLeftTapeSubHSM(INIT_EVENT);
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
    returnEvent = RunRideRightTapeSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSearchForFrontBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentFrontBorderSubState = InitFrontBorderSubState;
    returnEvent = RunSearchForFrontBorderSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSearchForRightBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentFirstObstacleClearedSubState = InitFirstObstacleClearedSub;
    returnEvent = SearchForRightBorderSubHSM(INIT_EVENT);

    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static uint8_t InitSearchForLeftBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentSecondObstacleClearedSubState = InitSecondObstacleClearedSub;
    returnEvent = SearchForLeftBorderSubHSM(INIT_EVENT);

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
                printf("RunBeaconAlignmentSubHSM: AlignedState: ES_ENTRY\r\n");
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
                    //go forward until any of the tape sensors are tripped
                    PS_Forward(SEARCH_FOR_FRONT_BORDER_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunSearchForFrontBorderSubHSM: SearchForFrontBorderSubState: LEFT_TAPE_DETECTED\r\n");
                    nextState = LeftFrontTapeFoundSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunSearchForFrontBorderSubHSM: SearchForFrontBorderSubState: RIGHT_TAPE_DETECTED\r\n");
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
                    PS_PivotTurnLeft(ALIGN_TO_FRONT_BORDER_POWER);
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
                    PS_PivotTurnRight(ALIGN_TO_FRONT_BORDER_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    nextState = FrontToLeftBorderSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case FrontToLeftBorderSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("RunSearchForFrontBorderSubHSM: FrontToLeftBorderSubState: ES_ENTRY\r\n");
                PS_BackwardDist(ALIGN_TO_FRONT_BORDER_BACKWARD_POWER, ALIGN_TO_FRONT_BORDER_BACKWARD_DIST);
                PS_TankTurnRightDist(ALIGN_TO_FRONT_BORDER_TANK_POWER, ALIGN_TO_FRONT_BORDER_TANK_DIST);
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

static ES_Event SearchForLeftBorderSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    SearchForLeftBorderSubState_t nextState = CurrentSecondObstacleClearedSubState;

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
                    printf("SearchForLeftBorderSubHSM: SearchForBorderSubState1: ES_ENTRY\r\n");
                    PS_Forward(SEARCH_FOR_FRONT_BORDER_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("SearchForLeftBorderSubHSM: SearchForBorderSubState1: LEFT_TAPE_DETECTED\r\n");
                    nextState = LeftFrontTapeFoundSubState2;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("SearchForLeftBorderSubHSM: SearchForBorderSubState1: RIGHT_TAPE_DETECTED\r\n");
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
                    PS_PivotTurnLeft(ALIGN_TO_FRONT_BORDER_POWER);
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
                    PS_PivotTurnRight(ALIGN_TO_FRONT_BORDER_POWER);
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
                printf("SearchForLeftBorderSubHSM: BorderFoundSubState1: ES_ENTRY\r\n");
                nextState = MoveAlongBorderSubState2;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveAlongBorderSubState2:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("SearchForLeftBorderSubHSM: MoveAlongBorderSubState2: ES_ENTRY\r\n");
                PS_BackwardDist(ALIGN_TO_FRONT_BORDER_BACKWARD_POWER, ALIGN_TO_FRONT_BORDER_BACKWARD_DIST);
                PS_TurnRight90();
                thisEvent.EventType = SECOND_BORDER_DONE;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        SearchForLeftBorderSubHSM(EXIT_EVENT);
        CurrentSecondObstacleClearedSubState = nextState;
        thisEvent = SearchForLeftBorderSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static ES_Event SearchForRightBorderSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    SearchForRightBorderSubState_t nextState = CurrentFirstObstacleClearedSubState;

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
                    printf("SearchForRightBorderSubHSM: SearchForBorderSubState: ES_ENTRY\r\n");
                    PS_Forward(SEARCH_FOR_RIGHT_BORDER_POWER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("SearchForRightBorderSubHSM: SearchForBorderSubState: LEFT_TAPE_DETECTED\r\n");
                    nextState = LeftFrontTapeFoundSubState1;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("SearchForRightBorderSubHSM: SearchForBorderSubState: RIGHT_TAPE_DETECTED\r\n");
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
                    PS_PivotTurnLeft(ALIGN_TO_RIGHT_BORDER_POWER);
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
                    PS_PivotTurnRight(ALIGN_TO_RIGHT_BORDER_POWER);
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
                printf("SearchForRightBorderSubHSM: BorderFoundSubState: ES_ENTRY\r\n");
                nextState = MoveAlongBorderSubState1;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case MoveAlongBorderSubState1:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("SearchForRightBorderSubHSM: MoveAlongBorderSubState1: ES_ENTRY\r\n");
                PS_BackwardDist(OBSTACLE_TO_RIGHT_BACKWARD_POWER, OBSTACLE_TO_RIGHT_BACKWARD_DIST);
                PS_TankTurnLeftDist(OBSTACLE_TO_RIGHT_TANK_POWER, OBSTACLE_TO_RIGHT_TANK_DIST);
                thisEvent.EventType = FIRST_BORDER_DONE;
            }
            break;

        default:
            break;
    }

    if (makeTransition == TRUE) {
        SearchForRightBorderSubHSM(EXIT_EVENT);
        CurrentFirstObstacleClearedSubState = nextState;
        thisEvent = SearchForRightBorderSubHSM(ENTRY_EVENT);
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
                printf("RunRideFrontTapeSubHSM: InitRideFrontTapeSubState: ES_INIT\r\n");
                nextState = AngledFrontForward;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AngledFrontForward:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideFrontTapeSubHSM: AngledFrontForward: ES_ENTRY\r\n");

                    PS_AngledForward(ANGLED_FORWARD_POWER);

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
                    PS_BackwardDist(FRONT_TO_LEFT_BORDER_BACKWARD_POWER, FRONT_TO_LEFT_BORDER_BACKWARD_DIST);
                    PS_TankTurnRightDist(FRONT_TO_LEFT_BORDER_TANK_POWER, FRONT_TO_LEFT_BORDER_TANK_DIST);
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

                    PS_TankTurnRightDist(500, 2);

                    nextState = AngledFrontForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideFrontTapeSubHSM: CorrectLeftSubState1: MIDDLE_TAPE_DETECTED\r\n");
                    PS_Stop();
                    PS_BackwardDist(FRONT_TO_LEFT_BORDER_BACKWARD_POWER, FRONT_TO_LEFT_BORDER_BACKWARD_DIST);
                    PS_TankTurnRightDist(FRONT_TO_LEFT_BORDER_TANK_POWER, FRONT_TO_LEFT_BORDER_TANK_DIST);
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

static ES_Event RunRideLeftTapeSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    RideLeftTapeSubState_t nextState = CurrentRideTapeSubState;

    switch (CurrentRideTapeSubState) {
        case InitRideTapeSubState:
            if (thisEvent.EventType == ES_INIT) {
                PS_Stop();
                printf("RunRideLeftTapeSubHSM: InitRideTapeSubState: ES_INIT\r\n");
                nextState = AngledForward;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AngledForward:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideLeftTapeSubHSM: AngledForward: ES_ENTRY\r\n");

                    // left motor faster, right motor slower = slight right curve
                    PS_AngledForward(ANGLED_FORWARD_POWER);

                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case RIGHT_TAPE_DETECTED:
                    printf("RunRideLeftTapeSubHSM: AngledForward: RIGHT_TAPE_DETECTED\r\n");
                    nextState = CorrectLeftSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideLeftTapeSubHSM: AngledForward: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(LEFT_TO_OBSTACLE_BACKWARD_POWER, LEFT_TO_OBSTACLE_BACKWARD_DIST);

                    PS_TankTurnRightDist(LEFT_TO_OBSTACLE_TANK_POWER, LEFT_TO_OBSTACLE_TANK_DIST);

                    PS_ForwardDist(OBSTACLE_TO_RIGHT_POWER, OBSTACLE_TO_RIGHT_DIST);
                    printf("RunRideLeftTapeSubHSM: AngledForward: Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideLeftTapeSubHSM: AngledForward: MIDDLE_TAPE_DETECTED\r\n");
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
                    printf("RunRideLeftTapeSubHSM: CorrectLeftSubState: ES_ENTRY\r\n");

                    PS_TankTurnRightDist(500, 2);

                    nextState = AngledForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideLeftTapeSubHSM: CorrectLeftSubState: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(ALIGN_TO_FRONT_BORDER_BACKWARD_POWER, 1);
                    printf("RunRideLeftTapeSubHSM: CorrectLeftSubState: Backup done\r\n");

                    PS_TurnRight90();
                    printf("RunRideLeftTapeSubHSM: CorrectLeftSubState: Turn done\r\n");

                    PS_ForwardDist(SEARCH_FOR_FRONT_BORDER_POWER, 12);
                    printf("RunRideLeftTapeSubHSM: CorrectLeftSubState: Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;
                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideLeftTapeSubHSM: CorrectLeftSubState: MIDDLE_TAPE_DETECTED\r\n");

                    PS_Stop();
                    thisEvent.EventType = ISZ_BORDER;
                    break;
                default:
                    break;
            }
            break;
    }

    if (makeTransition == TRUE) {
        RunRideLeftTapeSubHSM(EXIT_EVENT);
        CurrentRideTapeSubState = nextState;
        RunRideLeftTapeSubHSM(ENTRY_EVENT);
    }

    return thisEvent;
}

static ES_Event RunRideRightTapeSubHSM(ES_Event thisEvent) {
    uint8_t makeTransition = FALSE;
    RideRightTapeSubState_t nextState = CurrentRideOtherTapeSubState;

    switch (CurrentRideOtherTapeSubState) {
        case InitRideOtherTapeSubState:
            if (thisEvent.EventType == ES_INIT) {
                PS_Stop();
                printf("RunRideRightTapeSubHSM: InitRideOtherTapeSubState: ES_INIT\r\n");
                nextState = AngledOtherForward;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case AngledOtherForward:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: ES_ENTRY\r\n");

                    // left motor faster, right motor slower = slight right curve
                    PS_AngledForward2(ANGLED_FORWARD_POWER);

                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case LEFT_TAPE_DETECTED:
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: LEFT_TAPE_DETECTED\r\n");
                    nextState = CorrectRightSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(RIGHT_TO_OBSTACLE_BACKWARD_POWER, RIGHT_TO_OBSTACLE_BACKWARD_DIST);
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: Backup done\r\n");

                    PS_TankTurnLeftDist(RIGHT_TO_OBSTACLE_TANK_POWER, RIGHT_TO_OBSTACLE_TANK_DIST);
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: Turn done\r\n");

                    PS_ForwardDist(SEARCH_FOR_FRONT_BORDER_POWER, 10);
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;

                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideRightTapeSubHSM: AngledOtherForward: MIDDLE_TAPE_DETECTED\r\n");
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
                    printf("RunRideRightTapeSubHSM: CorrectRightSubState: ES_ENTRY\r\n");

                    PS_TankTurnLeftDist(500, 2);

                    nextState = AngledOtherForward;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
                case BUMPER_TRIPPED:
                    printf("RunRideRightTapeSubHSM: CorrectRightSubState: BUMPER_TRIPPED\r\n");
                    PS_BackwardDist(ALIGN_TO_FRONT_BORDER_BACKWARD_POWER, 1);
                    printf("RunRideRightTapeSubHSM: CorrectRightSubState: Backup done\r\n");

                    PS_TurnLeft90();
                    printf("RunRideRightTapeSubHSM: CorrectRightSubState: Turn done\r\n");

                    PS_ForwardDist(SEARCH_FOR_FRONT_BORDER_POWER, 12);
                    printf("RunRideRightTapeSubHSM: CorrectRightSubState: Forward done\r\n");
                    thisEvent.EventType = BUMPER_TRIPPED;
                    break;
                case MIDDLE_TAPE_DETECTED:
                    printf("RunRideRightTapeSubHSM: CorrectRightSubState: MIDDLE_TAPE_DETECTED\r\n");

                    PS_Stop();
                    thisEvent.EventType = ISZ_BORDER;
                    break;
                default:
                    break;
            }
            break;
    }

    if (makeTransition == TRUE) {
        RunRideRightTapeSubHSM(EXIT_EVENT);
        CurrentRideOtherTapeSubState = nextState;
        RunRideRightTapeSubHSM(ENTRY_EVENT);
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

    PS_TankTurnLeft(ALIGN_TO_FRONT_BORDER_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, AlignmentSpinTime);
}

