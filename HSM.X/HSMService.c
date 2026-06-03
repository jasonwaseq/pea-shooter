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
    LocateTapeState,
    TurnToCornerState,
    DriveToCornerState,
    TurnToEdgeState,
    HomeDoneSubState,
} HomeSubState_t;

typedef enum {
    InitLocateTapeSubState,
    LocateTapeForwardSubState,
    LocateTapeFixRightSubState,
    LocateTapeFixLeftSubState,
} LocateTapeSubState_t;

typedef enum {
    InitDriveToCornerSubState,
    DriveToCornerForwardSubState,
    DriveToCornerFixRightSubState,
    DriveToCornerFixLeftSubState,
    DriveToCornerHoldSubState,
} DriveToCornerSubState_t;

static uint8_t MyPriority;
static HSMState_t CurrentState;
static HomeSubState_t CurrentHomeSubState;
static LocateTapeSubState_t CurrentLocateTapeSubState;
static DriveToCornerSubState_t CurrentDriveToCornerSubState;
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
static uint8_t InitLocateTapeSubHSM(void);
static uint8_t RunLocateTapeSubHSM(ES_Event thisEvent);
static uint8_t InitDriveToCornerSubHSM(void);
static uint8_t RunDriveToCornerSubHSM(ES_Event thisEvent);

static ES_Event TransitionHomeSubState(HomeSubState_t nextState);
static ES_Event CompleteHome(void);
static uint8_t TransitionLocateTapeSubState(LocateTapeSubState_t nextState);
static uint8_t TransitionDriveToCornerSubState(DriveToCornerSubState_t nextState);

static void StartSweepData(void);
static void StartLockOn(void);
static void StartLockOnTurn(void);
static void StartNextLockOnRotation(void);
static void ResetBeaconFilter(void);
static uint16_t FilterBeaconSample(uint16_t sample);

static uint8_t IsTapeEvent(ES_EventTyp_t eventType);
static uint8_t EvaluateLocateTape(void);
static uint8_t EvaluateTurnToCorner(void);
static uint8_t EvaluateDriveToCorner(void);
static uint8_t EvaluateTurnToEdge(void);

static void DriveForward(void);
static void DriveBackward(void);
static void DriveLeftOnly(void);
static void DriveRightOnly(void);
static void TankTurnLeft(void);
static void ArmLocateTapePoll(void);

uint8_t InitHSMService(uint8_t priority) {
    MyPriority = priority;
    CurrentState = InitPState;
    CurrentHomeSubState = InitHomeSubState;
    CurrentLocateTapeSubState = InitLocateTapeSubState;
    CurrentDriveToCornerSubState = InitDriveToCornerSubState;
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
                        thisEvent = TransitionHomeSubState(LocateTapeState);
                    }
                    break;

                default:
                    break;
            }
            break;

        case LocateTapeState:
            if (thisEvent.EventType == ES_ENTRY) {
                if (InitLocateTapeSubHSM() == TRUE) {
                    thisEvent = TransitionHomeSubState(TurnToCornerState);
                } else {
                    thisEvent.EventType = ES_NO_EVENT;
                }
            } else if (RunLocateTapeSubHSM(thisEvent) == TRUE) {
                ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                thisEvent = TransitionHomeSubState(TurnToCornerState);
            } else {
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case TurnToCornerState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    TankTurnLeft();
                    if (EvaluateTurnToCorner() == TRUE) {
                        thisEvent = TransitionHomeSubState(DriveToCornerState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                        if (EvaluateTurnToCorner() == TRUE) {
                            thisEvent = TransitionHomeSubState(DriveToCornerState);
                        } else {
                            thisEvent.EventType = ES_NO_EVENT;
                        }
                    }
                    break;
            }
            break;

        case DriveToCornerState:
            if (thisEvent.EventType == ES_ENTRY) {
                if (InitDriveToCornerSubHSM() == TRUE) {
                    thisEvent = TransitionHomeSubState(TurnToEdgeState);
                } else {
                    thisEvent.EventType = ES_NO_EVENT;
                }
            } else if (RunDriveToCornerSubHSM(thisEvent) == TRUE) {
                thisEvent = TransitionHomeSubState(TurnToEdgeState);
            } else {
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case TurnToEdgeState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    TankTurnLeft();
                    if (EvaluateTurnToEdge() == TRUE) {
                        thisEvent = CompleteHome();
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                        if (EvaluateTurnToEdge() == TRUE) {
                            thisEvent = CompleteHome();
                        } else {
                            thisEvent.EventType = ES_NO_EVENT;
                        }
                    }
                    break;
            }
            break;

        case HomeDoneSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        default:
            break;
    }

    return thisEvent;
}

static uint8_t InitLocateTapeSubHSM(void) {
    CurrentLocateTapeSubState = InitLocateTapeSubState;
    return RunLocateTapeSubHSM(INIT_EVENT);
}

static uint8_t RunLocateTapeSubHSM(ES_Event thisEvent) {
    uint8_t done = FALSE;

    switch (CurrentLocateTapeSubState) {
        case InitLocateTapeSubState:
            if (thisEvent.EventType == ES_INIT) {
                done = TransitionLocateTapeSubState(LocateTapeForwardSubState);
            }
            break;

        case LocateTapeForwardSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                DriveForward();
                done = EvaluateLocateTape();
                if (done == FALSE) {
                    ArmLocateTapePoll();
                }
            } else if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                    || ((thisEvent.EventType == ES_TIMEOUT)
                    && (thisEvent.EventParam == HSM_ROTATION_TIMER))) {
                done = EvaluateLocateTape();
                if ((done == FALSE)
                        && (CurrentLocateTapeSubState == LocateTapeForwardSubState)) {
                    DriveForward();
                    ArmLocateTapePoll();
                }
            }
            break;

        case LocateTapeFixRightSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                done = EvaluateLocateTape();
                if ((done == FALSE)
                        && (CurrentLocateTapeSubState == LocateTapeFixRightSubState)) {
                    DriveRightOnly();
                    ArmLocateTapePoll();
                }
            } else if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                    || ((thisEvent.EventType == ES_TIMEOUT)
                    && (thisEvent.EventParam == HSM_ROTATION_TIMER))) {
                done = EvaluateLocateTape();
                if ((done == FALSE)
                        && (CurrentLocateTapeSubState == LocateTapeFixRightSubState)) {
                    DriveRightOnly();
                    ArmLocateTapePoll();
                }
            }
            break;

        case LocateTapeFixLeftSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                done = EvaluateLocateTape();
                if ((done == FALSE)
                        && (CurrentLocateTapeSubState == LocateTapeFixLeftSubState)) {
                    DriveLeftOnly();
                    ArmLocateTapePoll();
                }
            } else if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                    || ((thisEvent.EventType == ES_TIMEOUT)
                    && (thisEvent.EventParam == HSM_ROTATION_TIMER))) {
                done = EvaluateLocateTape();
                if ((done == FALSE)
                        && (CurrentLocateTapeSubState == LocateTapeFixLeftSubState)) {
                    DriveLeftOnly();
                    ArmLocateTapePoll();
                }
            }
            break;

        default:
            break;
    }

    return done;
}

static uint8_t InitDriveToCornerSubHSM(void) {
    CurrentDriveToCornerSubState = InitDriveToCornerSubState;
    return RunDriveToCornerSubHSM(INIT_EVENT);
}

static uint8_t RunDriveToCornerSubHSM(ES_Event thisEvent) {
    uint8_t done = FALSE;

    switch (CurrentDriveToCornerSubState) {
        case InitDriveToCornerSubState:
            if (thisEvent.EventType == ES_INIT) {
                done = TransitionDriveToCornerSubState(DriveToCornerForwardSubState);
            }
            break;

        case DriveToCornerForwardSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                DriveForward();
                done = EvaluateDriveToCorner();
            } else if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                done = EvaluateDriveToCorner();
            }
            break;

        case DriveToCornerFixRightSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                DriveRightOnly();
                done = EvaluateDriveToCorner();
            } else if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                done = EvaluateDriveToCorner();
            }
            break;

        case DriveToCornerFixLeftSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                DriveLeftOnly();
                done = EvaluateDriveToCorner();
            } else if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                done = EvaluateDriveToCorner();
            }
            break;

        case DriveToCornerHoldSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                PS_Stop();
                done = EvaluateDriveToCorner();
            } else if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                done = EvaluateDriveToCorner();
            }
            break;

        default:
            break;
    }

    return done;
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

    RunHomeSubHSM(EXIT_EVENT);
    CurrentHomeSubState = HomeDoneSubState;
    RunHomeSubHSM(ENTRY_EVENT);

    completeEvent.EventType = HOME_COMPLETE;
    completeEvent.EventParam = 0;
    return completeEvent;
}

static uint8_t TransitionLocateTapeSubState(LocateTapeSubState_t nextState) {
    RunLocateTapeSubHSM(EXIT_EVENT);
    CurrentLocateTapeSubState = nextState;
    return RunLocateTapeSubHSM(ENTRY_EVENT);
}

static uint8_t TransitionDriveToCornerSubState(DriveToCornerSubState_t nextState) {
    RunDriveToCornerSubHSM(EXIT_EVENT);
    CurrentDriveToCornerSubState = nextState;
    return RunDriveToCornerSubHSM(ENTRY_EVENT);
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

static uint8_t EvaluateLocateTape(void) {
    unsigned char tapeMask;

    tapeMask = PS_ReadTape();
    if (tapeMask == TAPE_ALL) {
        PS_Stop();
        printf("HOME: LOCATE_TAPE complete\r\n");
        return TRUE;
    }

    if (CurrentLocateTapeSubState == LocateTapeForwardSubState) {
        if ((tapeMask == LEFT_TAPE_MASK) || (tapeMask == TAPE_LEFT_MID)) {
            return TransitionLocateTapeSubState(LocateTapeFixRightSubState);
        }

        if (tapeMask == RIGHT_TAPE_MASK) {
            return TransitionLocateTapeSubState(LocateTapeFixLeftSubState);
        }
    }

    return FALSE;
}

static uint8_t EvaluateTurnToCorner(void) {
    if (PS_ReadTape() == TAPE_LEFT_MID) {
        PS_Stop();
        printf("HOME: TURN_TO_CORNER complete\r\n");
        return TRUE;
    }

    return FALSE;
}

static uint8_t EvaluateDriveToCorner(void) {
    unsigned char tapeMask;

    tapeMask = PS_ReadTape();
    if (tapeMask == TAPE_ALL) {
        PS_Stop();
        printf("HOME: DRIVE_TO_CORNER complete\r\n");
        return TRUE;
    }

    if (tapeMask == TAPE_LEFT_MID) {
        if (CurrentDriveToCornerSubState != DriveToCornerForwardSubState) {
            return TransitionDriveToCornerSubState(DriveToCornerForwardSubState);
        }
    } else if (tapeMask == LEFT_TAPE_MASK) {
        if (CurrentDriveToCornerSubState != DriveToCornerFixRightSubState) {
            return TransitionDriveToCornerSubState(DriveToCornerFixRightSubState);
        }
    } else if (tapeMask == MID_TAPE_MASK) {
        if (CurrentDriveToCornerSubState != DriveToCornerFixLeftSubState) {
            return TransitionDriveToCornerSubState(DriveToCornerFixLeftSubState);
        }
    } else if (CurrentDriveToCornerSubState != DriveToCornerHoldSubState) {
        return TransitionDriveToCornerSubState(DriveToCornerHoldSubState);
    }

    return FALSE;
}

static uint8_t EvaluateTurnToEdge(void) {
    if (PS_ReadTape() == TAPE_LEFT_MID) {
        PS_Stop();
        printf("HOME: TURN_TO_EDGE complete\r\n");
        return TRUE;
    }

    return FALSE;
}

static void DriveForward(void) {
    PS_Forward(HSM_DRIVE_POWER);
}

static void DriveBackward(void) {
    PS_Backward(HSM_DRIVE_POWER);
}

static void DriveLeftOnly(void) {
    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(HSM_CORRECT_POWER);
}

static void DriveRightOnly(void) {
    PS_LeftMtrSpeed(0);
    PS_RightMtrSpeed(HSM_CORRECT_POWER);
}

static void TankTurnLeft(void) {
    PS_TankTurnLeftContinuous(HSM_TAPE_TURN_POWER);
}

static void ArmLocateTapePoll(void) {
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCATE_TAPE_POLL_TIME_MS);
}
