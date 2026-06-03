/*
 * HSMService.c
 *
 * HOME hierarchical state machine for beacon lock and tape-based homing.
 */

#include "HSMService.h"

#include <stdio.h>

#include "BeaconEventChecker.h"
#include "ES_Framework.h"
#include "ES_Timers.h"
#include "peashooter.h"

#define HSM_FULL_ROTATION_TIME_MS 5000
#define HSM_LOCK_ON_START_DELAY_MS 250
#define HSM_ON_FIELD_BACKUP_TIME_MS 500
#define HSM_ON_FIELD_STOP_DELAY_MS 250
#define HSM_LOCATE_TAPE_POLL_TIME_MS 50
#define HSM_BEACON_AVG_WINDOW 8
#define HSM_BEACON_LOCK_OFFSET 10

#define HSM_SWEEP_TURN_POWER 500
#define HSM_LOCK_TURN_POWER 500
#define HSM_TAPE_TURN_POWER 500
#define HSM_DRIVE_POWER 700
#define HSM_CORRECT_POWER 650

#define ALIGN_TO_FRONT_BORDER_POWER 900
#define ALIGN_TO_RIGHT_BORDER_POWER 900
#define SEARCH_FOR_FRONT_BORDER_POWER 900
#define SEARCH_FOR_RIGHT_BORDER_POWER 900
#define OBSTACLE_TO_RIGHT_POWER 900
#define OBSTACLE_TO_RIGHT_DIST 12
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
#define ALIGN_TO_FRONT_BORDER_TANK_POWER 900
#define ALIGN_TO_FRONT_BORDERRIGHT_TANK_DIST 4
#define ALIGN_TO_FRONT_BORDERLEFT_TANK_DIST 5


#define FRONT_TO_LEFT_BORDER_TANK_POWER 900
#define FRONT_TO_LEFT_BORDER_TANK_DIST 4
#define LEFT_TO_OBSTACLE_TANK_POWER 700
#define LEFT_TO_OBSTACLE_TANK_DIST 7.7
#define OBSTACLE_TO_RIGHT_TANK_POWER 900
#define OBSTACLE_TO_RIGHT_TANK_DIST 4
#define RIGHT_TO_OBSTACLE_TANK_POWER 700
#define RIGHT_TO_OBSTACLE_TANK_DIST 7.7
#define ANGLED_FORWARD_POWER 800

#define TAPE_NONE 0
#define TAPE_ALL (LEFT_TAPE_MASK | MID_TAPE_MASK | RIGHT_TAPE_MASK)
#define TAPE_LEFT_MID (LEFT_TAPE_MASK | MID_TAPE_MASK)

typedef enum {
    InitPState,
    HomeState,
    HomeCompleteState,
} HSMState_t;

typedef enum {
    InitHomeSubState,
    SweepDataState,
    LockOnState,
    OnFieldState,
    SearchForFrontBorderState,
    SearchForSideBorderState,
    RideLeftTapeState,
    RideRIghtTapeState,
    RideFrontTapeState,
    SearchForRightBorderState,
    SearchForLeftBorderState,
    STOPPEDState,
} HomeSubState_t;

typedef enum {
    InitFrontBorderSubState,
    SearchForFrontBorderSubState,
    LeftFrontTapeFoundSubState,
    RightFrontTapeFoundSubState,
    FrontBorderFoundSubState,
    FrontToLeftBorderFromLeftSubState,
    FrontToLeftBorderFromRightSubState,
} SearchForFrontBorderSubState_t;

typedef enum {
    InitSideBorderSubState,
    SearchForSideBorderSubState,
    LeftSideTapeFoundSubState,
    SideBorderFoundSubState,
    MoveAlongSideBorderSubState,
} SearchForSideBorderSubState_t;

typedef enum {
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
static HomeSubState_t CurrentHomeSubState;
static SearchForFrontBorderSubState_t CurrentFrontBorderSubState;
static SearchForSideBorderSubState_t CurrentSideBorderSubState;
static RideLeftTapeSubState_t CurrentRideTapeSubState;
static RideFrontTapeSubState_t CurrentRideFrontTapeSubState;
static SearchForRightBorderSubState_t CurrentFirstObstacleClearedSubState;
static RideRightTapeSubState_t CurrentRideOtherTapeSubState;
static SearchForLeftBorderSubState_t CurrentSecondObstacleClearedSubState;
static uint8_t TapeEventsEnabled;

static uint16_t BeaconSamples[HSM_BEACON_AVG_WINDOW];
static uint8_t BeaconSampleIndex;
static uint8_t BeaconSampleCount;
static uint32_t BeaconSampleSum;
static uint16_t MaxFilteredBeaconValue;
static uint16_t BeaconLockThreshold;
static uint8_t LockOnTurnStarted;
static uint16_t LockOnRotationMax;
static uint8_t OnFieldStopped;

static uint8_t InitHomeSubHSM(void);
static ES_Event RunHomeSubHSM(ES_Event thisEvent);
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

static ES_Event TransitionHomeSubState(HomeSubState_t nextState);
static ES_Event CompleteHome(void);

static void StartSweepData(void);
static void StartLockOn(void);
static void StartLockOnTurn(void);
static void StartNextLockOnRotation(void);
static void ResetBeaconFilter(void);
static uint16_t FilterBeaconSample(uint16_t sample);

static uint8_t IsTapeEvent(ES_EventTyp_t eventType);

static void DriveBackward(void);

uint8_t InitHSMService(uint8_t priority) {
    MyPriority = priority;
    CurrentState = InitPState;
    CurrentHomeSubState = InitHomeSubState;
    TapeEventsEnabled = FALSE;
    LockOnTurnStarted = FALSE;
    LockOnRotationMax = 0;
    OnFieldStopped = FALSE;

    PS_Init();
    PS_Stop();

    printf("InitHSMService: HOME HSM starting, beacon=%s\r\n",
            BEACON_DETECTOR_PIN_NAME);

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
                nextState = HomeState;
                makeTransition = TRUE;
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case HomeState:
            if (thisEvent.EventType == ES_ENTRY) {
                if (InitHomeSubHSM() != TRUE) {
                    thisEvent.EventType = ES_ERROR;
                } else {
                    thisEvent.EventType = ES_NO_EVENT;
                }
            } else {
                thisEvent = RunHomeSubHSM(thisEvent);
                if (thisEvent.EventType == HOME_COMPLETE) {
                    nextState = HomeCompleteState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                }
            }
            break;

        case HomeCompleteState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                printf("RunHSMService: HOME_COMPLETE\r\n");
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

uint8_t HSM_TapeEventsEnabled(void) {
    return TapeEventsEnabled;
}

static uint8_t InitHomeSubHSM(void) {
    ES_Event returnEvent;

    CurrentHomeSubState = InitHomeSubState;
    returnEvent = RunHomeSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static ES_Event RunHomeSubHSM(ES_Event thisEvent) {
    switch (CurrentHomeSubState) {
        case InitHomeSubState:
            if (thisEvent.EventType == ES_INIT) {
                thisEvent = TransitionHomeSubState(SweepDataState);
            }
            break;

        case SweepDataState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    StartSweepData();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case BEACON_SAMPLE_READY:
                    {
                        uint16_t filteredValue;

                        filteredValue = FilterBeaconSample(thisEvent.EventParam);
                        if (filteredValue > MaxFilteredBeaconValue) {
                            MaxFilteredBeaconValue = filteredValue;
                        }
                    }
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        PS_Stop();
                        printf("HOME: SWEEP_DATA complete max=%u\r\n",
                                MaxFilteredBeaconValue);
                        thisEvent = TransitionHomeSubState(LockOnState);
                    }
                    break;

                default:
                    break;
            }
            break;

        case LockOnState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    StartLockOn();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if ((thisEvent.EventParam == HSM_ROTATION_TIMER)
                            && (LockOnTurnStarted == FALSE)) {
                        StartLockOnTurn();
                        thisEvent.EventType = ES_NO_EVENT;
                    } else if ((thisEvent.EventParam == HSM_ROTATION_TIMER)
                            && (LockOnTurnStarted == TRUE)) {
                        StartNextLockOnRotation();
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case BEACON_SAMPLE_READY:
                    if (LockOnTurnStarted == FALSE) {
                        thisEvent.EventType = ES_NO_EVENT;
                    } else {
                        uint16_t filteredValue;

                        filteredValue = FilterBeaconSample(thisEvent.EventParam);
                        if (filteredValue > LockOnRotationMax) {
                            LockOnRotationMax = filteredValue;
                        }

                        if (filteredValue >= BeaconLockThreshold) {
                            PS_Stop();
                            printf("HOME: LOCK_ON complete adc=%u threshold=%u\r\n",
                                    filteredValue, BeaconLockThreshold);
                            TapeEventsEnabled = TRUE;
                            thisEvent = TransitionHomeSubState(OnFieldState);
                        } else {
                            thisEvent.EventType = ES_NO_EVENT;
                        }
                    }
                    break;

                default:
                    break;
            }
            break;

        case OnFieldState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    OnFieldStopped = FALSE;
                    DriveBackward();
                    ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                            HSM_ON_FIELD_BACKUP_TIME_MS);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam != HSM_ROTATION_TIMER) {
                        break;
                    }

                    if (OnFieldStopped == FALSE) {
                        PS_Stop();
                        OnFieldStopped = TRUE;
                        ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                                HSM_ON_FIELD_STOP_DELAY_MS);
                        thisEvent.EventType = ES_NO_EVENT;
                    } else {
                        thisEvent = TransitionHomeSubState(SearchForFrontBorderState);
                    }
                    break;

                default:
                    break;
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
                        thisEvent = TransitionHomeSubState(RideFrontTapeState);
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
                        thisEvent = TransitionHomeSubState(STOPPEDState);
                    } else if (thisEvent.EventType == BUMPER_TRIPPED) {
                        thisEvent = TransitionHomeSubState(SearchForRightBorderState);
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
                        thisEvent = TransitionHomeSubState(RideLeftTapeState);
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
                        thisEvent = TransitionHomeSubState(STOPPEDState);
                    } else if (thisEvent.EventType == BUMPER_TRIPPED) {
                        thisEvent = TransitionHomeSubState(STOPPEDState);
                    }
                    break;
            }
            break;

        case SearchForRightBorderState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHomeSubHSM: SearchForRightBorderState\r\n");
                    if (InitSearchForRightBorderSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = SearchForRightBorderSubHSM(thisEvent);
                    if (thisEvent.EventType == FIRST_BORDER_DONE) {
                        thisEvent = TransitionHomeSubState(RideRIghtTapeState);
                    }
                    break;
            }
            break;

        case SearchForLeftBorderState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    printf("RunHomeSubHSM: SearchForLeftBorderState\r\n");
                    if (InitSearchForLeftBorderSubHSM() != TRUE) {
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent = SearchForLeftBorderSubHSM(thisEvent);
                    if (thisEvent.EventType == SECOND_BORDER_DONE) {
                        thisEvent = TransitionHomeSubState(RideLeftTapeState);
                    }
                    break;
            }
            break;

        case STOPPEDState:
            if (thisEvent.EventType == ES_ENTRY) {
                thisEvent = CompleteHome();
            }
            break;

        default:
            break;
    }

    return thisEvent;
}

static uint8_t InitSearchForFrontBorderSubHSM(void) {
    ES_Event returnEvent;

    CurrentFrontBorderSubState = InitFrontBorderSubState;
    returnEvent = RunSearchForFrontBorderSubHSM(INIT_EVENT);
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

static ES_Event TransitionHomeSubState(HomeSubState_t nextState) {
    ES_Event returnEvent;

    RunHomeSubHSM(EXIT_EVENT);
    CurrentHomeSubState = nextState;
    returnEvent = RunHomeSubHSM(ENTRY_EVENT);

    return returnEvent;
}

static ES_Event CompleteHome(void) {
    ES_Event completeEvent;

    PS_Stop();

    completeEvent.EventType = HOME_COMPLETE;
    completeEvent.EventParam = 0;
    return completeEvent;
}

static void StartSweepData(void) {
    ResetBeaconFilter();
    MaxFilteredBeaconValue = 0;

    printf("HOME: SWEEP_DATA start, right forward / left reverse\r\n");
    PS_RightMtrSpeed(HSM_SWEEP_TURN_POWER);
    PS_LeftMtrReverseSpeed(HSM_SWEEP_TURN_POWER);
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

    printf("HOME: LOCK_ON start max=%u threshold=%u\r\n",
            MaxFilteredBeaconValue, BeaconLockThreshold);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_ON_START_DELAY_MS);
}

static void StartLockOnTurn(void) {
    LockOnTurnStarted = TRUE;
    LockOnRotationMax = 0;
    ResetBeaconFilter();
    printf("HOME: LOCK_ON tank turn, right forward / left reverse\r\n");
    PS_RightMtrSpeed(HSM_LOCK_TURN_POWER);
    PS_LeftMtrReverseSpeed(HSM_LOCK_TURN_POWER);
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

    printf("HOME: LOCK_ON retry max=%u threshold=%u\r\n",
            MaxFilteredBeaconValue, BeaconLockThreshold);

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

static uint8_t IsTapeEvent(ES_EventTyp_t eventType) {
    switch (eventType) {
        case LEFT_TAPE_DETECTED:
        case LEFT_TAPE_NOT_DETECTED:
        case MIDDLE_TAPE_DETECTED:
        case MIDDLE_TAPE_NOT_DETECTED:
        case RIGHT_TAPE_DETECTED:
        case RIGHT_TAPE_NOT_DETECTED:
        case NO_TAPE_DETECTED:
            return TRUE;

        default:
            return FALSE;
    }
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
                    nextState = FrontToLeftBorderFromLeftSubState;
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
                    nextState = FrontToLeftBorderFromRightSubState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case FrontToLeftBorderFromRightSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("RunSearchForFrontBorderSubHSM: FrontToLeftBorderFromRightSubState: ES_ENTRY\r\n");
                PS_BackwardDist(ALIGN_TO_FRONT_BORDER_BACKWARD_POWER, ALIGN_TO_FRONT_BORDER_BACKWARD_DIST);
                PS_TankTurnRightDist(ALIGN_TO_FRONT_BORDER_TANK_POWER, ALIGN_TO_FRONT_BORDERRIGHT_TANK_DIST);
                thisEvent.EventType = FRONT_BORDER_ALIGNED;
            }
            break;

        case FrontToLeftBorderFromLeftSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                printf("RunSearchForFrontBorderSubHSM: FrontToLeftBorderFromLeftSubState: ES_ENTRY\r\n");
                PS_BackwardDist(ALIGN_TO_FRONT_BORDER_BACKWARD_POWER, ALIGN_TO_FRONT_BORDER_BACKWARD_DIST);
                PS_TankTurnRightDist(ALIGN_TO_FRONT_BORDER_TANK_POWER, ALIGN_TO_FRONT_BORDERLEFT_TANK_DIST);
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

static void DriveBackward(void) {
    PS_Backward(HSM_DRIVE_POWER);
}
