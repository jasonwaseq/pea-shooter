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
#include "IO_Ports.h"
#include "peashooter.h"
#include "pwm.h"
#include "SensorsEventChecker.h"

#define HSM_LOCK_COLLECT_TIME_MS 2000
#define HSM_LOCK_STEP_DRIVE_TIME_MS 80
#define HSM_LOCK_STEP_PAUSE_TIME_MS 250
/* Time the base needs to physically coast to a stop after a brake command.
 * Beacon ADC samples taken before this elapses can be corrupted by the still-
 * spinning motors, so sampling stays disarmed until the settle time passes. */
#define HSM_BEACON_SETTLE_MS 80
#define HSM_ON_FIELD_BACKUP_TIME_MS 1500
#define HSM_LOCATE_TAPE_POLL_TIME_MS 50
#define HSM_HIT_PIVOT_TIME_MS 500
#define HSM_BUMP_REVERSE_TIME_MS 1000
#define HSM_BUMP_TURN_TIME_MS 1000
#define HSM_BUMP_FORWARD_TIME_MS 1500
#define HSM_TURN_TO_CORNER_180_TIME_MS 2001
#define HSM_ISZ_HOLD_TIME_MS 2000
#define HSM_INDEXER_FULL_TIME_MS 1000
#define HSM_BEACON_DETECT_MIN 900
#define HSM_ISZ_CONFIRM_COUNT 3
/* Number of consecutive motors-off ADC samples above threshold required before
 * completing a beacon lock. A single sample crossing the threshold can be a
 * transient spike or reflection at the wrong orientation; requiring a sustained
 * reading aligns the lock with the true beacon direction. */
#define HSM_BEACON_CONFIRM_COUNT 2

#define HSM_LOCK_TURN_POWER 700
#define HSM_TURN_TO_CORNER_POWER 700
#define HSM_DRIVE_POWER 700
#define HSM_HIT_FAST_POWER 900
#define HSM_HIT_SLOW_POWER 350
#define HSM_BUMP_POWER 700
#define HSM_TRAVEL_POWER 350
#define HSM_TRAVEL_CORRECT_POWER 400

#define HSM_INDEXER_PWM_PIN PWM_PORTZ06
#define HSM_UPPER_SHOOTER_RPWM_PIN PWM_PORTY04
#define HSM_LOWER_SHOOTER_LPWM_PIN PWM_PORTX11
#define HSM_SHOOT_PWM_PINS (HSM_INDEXER_PWM_PIN \
        | HSM_UPPER_SHOOTER_RPWM_PIN \
        | HSM_LOWER_SHOOTER_LPWM_PIN)

#define HSM_INDEXER_PWM_IO_PORT PORTZ
#define HSM_INDEXER_PWM_IO_BIT PIN6
#define HSM_UPPER_SHOOTER_RPWM_IO_PORT PORTY
#define HSM_UPPER_SHOOTER_RPWM_IO_BIT PIN4
#define HSM_LOWER_SHOOTER_LPWM_IO_PORT PORTX
#define HSM_LOWER_SHOOTER_LPWM_IO_BIT PIN11

#define HSM_UPPER_SHOOTER_ENABLE_PORT PORTX
#define HSM_UPPER_SHOOTER_REN_BIT PIN3
#define HSM_LOWER_SHOOTER_ENABLE_PORT PORTX
#define HSM_LOWER_SHOOTER_LEN_BIT PIN4

#define HSM_INDEXER_FULL_DUTY MAX_PWM
#define HSM_INDEXER_RUN_DUTY 400
#define HSM_UPPER_SHOOTER_DUTY 850
#define HSM_LOWER_SHOOTER_DUTY 350

#define TAPE_NONE 0
#define TAPE_ALL (LEFT_TAPE_MASK | MID_TAPE_MASK | RIGHT_TAPE_MASK)

typedef enum {
    InitPState,
    HomeState,
    TravelState,
} HSMState_t;

typedef enum {
    InitHomeSubState,
    LockOnState,
    OnFieldState,
    LocateTapeState,
    TurnToCornerState,
    DriveState,
    HitLeftState,
    HitRightState,
    ISZState,
    AimLockOnState,
    ShootState,
    BumpState,
    HomeDoneSubState,
} HomeSubState_t;

typedef enum {
    LockOnWaitingToTurn,
    LockOnInitialTurn,
    LockOnTurning,
} LockOnPhase_t;

typedef enum {
    InitLocateTapeSubState,
    LocateTapeForwardSubState,
} LocateTapeSubState_t;

typedef enum {
    InitDriveSubState,
    DriveForwardSubState,
} DriveSubState_t;

typedef enum {
    BumpReversePhase,
    BumpTurnRightPhase,
    BumpDriveForwardPhase,
    BumpTurnLeftPhase,
} BumpPhase_t;

static uint8_t MyPriority;
static HSMState_t CurrentState;
static HomeSubState_t CurrentHomeSubState;
static LocateTapeSubState_t CurrentLocateTapeSubState;
static DriveSubState_t CurrentDriveSubState;
static uint8_t TapeEventsEnabled;

static LockOnPhase_t LockOnPhase;
static uint8_t LockOnStepDriving;
static LockOnPhase_t AimLockOnPhase;
static uint8_t AimBeaconComplete;
static uint8_t AimLockOnStepDriving;
static uint32_t TurnStartTime;
static uint8_t DriveImmediateHit;
static unsigned char DriveImmediateTapeMask;
static unsigned char ISZTapeEventMask;
static uint8_t ISZAllTapeCount;
static uint8_t BeaconDetectCount;
static uint32_t BeaconSettleStartTime;
static HomeSubState_t DriveNextState;
static BumpPhase_t BumpPhase;
static uint8_t TravelTapeLossLatched;

static uint8_t InitHomeSubHSM(void);
static ES_Event RunHomeSubHSM(ES_Event thisEvent);
static uint8_t InitLocateTapeSubHSM(void);
static uint8_t RunLocateTapeSubHSM(ES_Event thisEvent);
static uint8_t InitDriveSubHSM(void);
static uint8_t RunDriveSubHSM(ES_Event thisEvent);

static ES_Event TransitionHomeSubState(HomeSubState_t nextState);
static ES_Event CompleteHome(void);
static uint8_t TransitionLocateTapeSubState(LocateTapeSubState_t nextState);
static uint8_t TransitionDriveSubState(DriveSubState_t nextState);

static void StartLockOn(void);
static void StartLockOnTurn(void);
static void StartLockOnStepDrive(void);
static void StartLockOnStepPause(void);
static uint8_t BeaconSampleIsDetected(uint16_t adcReading);
static ES_Event CompleteLockOn(uint16_t rawValue);
static void StartAimLockOn(void);
static void StartAimLockOnCollect(void);
static void StartAimLockOnTurn(void);
static void StartAimLockOnStepDrive(void);
static void StartAimLockOnStepPause(void);
static ES_Event CompleteAim(uint16_t rawValue);
static uint8_t ConfigureShootOutputs(void);
static uint8_t StartShootOutputs(void);
static uint8_t StopShootOutputs(void);
static uint8_t SetShootIndexerDuty(unsigned int duty);
static void StartTurnToCorner180(void);
static uint8_t TurnToCorner180Complete(void);

static uint8_t IsTapeEvent(ES_EventTyp_t eventType);
static uint8_t HomeSubStateIgnoresTapeEvents(HomeSubState_t state);
static uint8_t CountTapeSensors(unsigned char tapeMask);
static void ResetISZTapeEventMask(void);
static void LatchISZTapeDetectedEvent(ES_EventTyp_t eventType,
        unsigned char tapeMask);
static uint8_t AllISZTapeEventsDetected(void);
static uint8_t EvaluateLocateTape(void);
static uint8_t EvaluateDriveEvent(ES_EventTyp_t eventType);
static void EvaluateTravel(void);

static const char *HomeSubStateName(HomeSubState_t state);
static void DriveForward(void);
static void DriveBackward(void);
static void StartHitLeftPivot(void);
static void StartHitRightPivot(void);
static void StartBumpPhase(BumpPhase_t phase);
static uint8_t AdvanceBumpPhase(void);
static uint8_t AllTapeReleased(void);
static void TravelForward(void);
static void TravelLeftOnly(void);
static void TravelRightOnly(void);
static void ArmLocateTapePoll(void);

uint8_t InitHSMService(uint8_t priority) {
    MyPriority = priority;
    CurrentState = InitPState;
    CurrentHomeSubState = InitHomeSubState;
    CurrentLocateTapeSubState = InitLocateTapeSubState;
    CurrentDriveSubState = InitDriveSubState;
    TapeEventsEnabled = FALSE;
    LockOnPhase = LockOnWaitingToTurn;
    LockOnStepDriving = FALSE;
    AimLockOnPhase = LockOnWaitingToTurn;
    AimBeaconComplete = FALSE;
    AimLockOnStepDriving = FALSE;
    TurnStartTime = 0;
    DriveImmediateHit = FALSE;
    DriveImmediateTapeMask = TAPE_NONE;
    ISZTapeEventMask = TAPE_NONE;
    ISZAllTapeCount = 0;
    BeaconDetectCount = 0;
    BeaconSettleStartTime = 0;
    DriveNextState = DriveState;
    BumpPhase = BumpReversePhase;
    TravelTapeLossLatched = FALSE;

    PS_Init();
    PS_Stop();
    StopShootOutputs();

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
                    nextState = TravelState;
                    makeTransition = TRUE;
                    thisEvent.EventType = ES_NO_EVENT;
                }
            }
            break;

        case TravelState:
            if (thisEvent.EventType == ES_ENTRY) {
                TravelTapeLossLatched = FALSE;
                printf("RunHSMService: TRAVEL start\r\n");
                EvaluateTravel();
                ArmLocateTapePoll();
                thisEvent.EventType = ES_NO_EVENT;
            } else if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                    || ((thisEvent.EventType == ES_TIMEOUT)
                    && (thisEvent.EventParam == HSM_ROTATION_TIMER))) {
                EvaluateTravel();
                ArmLocateTapePoll();
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
    if ((CurrentState == HomeState)
            && (HomeSubStateIgnoresTapeEvents(CurrentHomeSubState) == TRUE)) {
        return FALSE;
    }

    return TapeEventsEnabled;
}

uint8_t HSM_BeaconAlignFastStopEnabled(void) {
    if (CurrentState != HomeState) {
        return FALSE;
    }

    if (CurrentHomeSubState == LockOnState) {
        return TRUE;
    }

    if ((CurrentHomeSubState == AimLockOnState)
            && (AimBeaconComplete == FALSE)) {
        return TRUE;
    }

    return FALSE;
}

/* Beacon ADC samples are only trustworthy while the base motors are off.
 * During the step-drive portion of the incremental tank turn the motors
 * inject noise that can spike the reading above the detection threshold and
 * cause a false lock. Only arm sampling during the motors-off hold/pause. */
static uint8_t BeaconSettleElapsed(void) {
    return ((ES_Timer_GetTime() - BeaconSettleStartTime)
            >= HSM_BEACON_SETTLE_MS) ? TRUE : FALSE;
}

uint8_t HSM_BeaconSampleArmed(void) {
    if (CurrentState != HomeState) {
        return FALSE;
    }

    if (CurrentHomeSubState == LockOnState) {
        return ((LockOnStepDriving == FALSE)
                && (BeaconSettleElapsed() == TRUE)) ? TRUE : FALSE;
    }

    if ((CurrentHomeSubState == AimLockOnState)
            && (AimBeaconComplete == FALSE)) {
        return ((AimLockOnStepDriving == FALSE)
                && (BeaconSettleElapsed() == TRUE)) ? TRUE : FALSE;
    }

    return FALSE;
}

uint8_t HSM_CheckBeaconLockImmediate(uint16_t adcReading) {
    uint8_t inLockOn = (uint8_t) ((CurrentState == HomeState)
            && (CurrentHomeSubState == LockOnState));
    uint8_t inAimLockOn = (uint8_t) ((CurrentState == HomeState)
            && (CurrentHomeSubState == AimLockOnState)
            && (AimBeaconComplete == FALSE));

    if ((inLockOn == FALSE) && (inAimLockOn == FALSE)) {
        BeaconDetectCount = 0;
        return FALSE;
    }

    /* A single sample crossing the threshold is not trusted: require several
     * consecutive motors-off detections so a transient spike at the wrong
     * orientation cannot complete the lock. The count resets on any below-
     * threshold sample and whenever the base starts moving again, so the
     * confirmation must accumulate within one stationary step. */
    if (BeaconSampleIsDetected(adcReading) != TRUE) {
        BeaconDetectCount = 0;
        return FALSE;
    }

    if (BeaconDetectCount < HSM_BEACON_CONFIRM_COUNT) {
        BeaconDetectCount++;
    }

    if (BeaconDetectCount < HSM_BEACON_CONFIRM_COUNT) {
        return FALSE;
    }

    BeaconDetectCount = 0;
    if (inLockOn == TRUE) {
        LockOnStepDriving = FALSE;
    } else {
        AimLockOnStepDriving = FALSE;
    }
    PS_Brake();
    return TRUE;
}

/* Confirmed "all three sensors on tape at once" detector. Runs every event
 * checker pass while the robot can still reach ISZ. It requires the raw mask to
 * read all three simultaneously for a few consecutive samples (rejecting motor
 * noise) and then posts ISZ_BORDER to force the transition the moment the
 * condition holds. Returning TRUE while the count is building suppresses the
 * single-side drive-immediate routing so a genuine three-sensor hit is not
 * diverted into a pivot before it can be confirmed. */
uint8_t HSM_CheckISZImmediate(unsigned char tapeMask) {
    ES_Event thisEvent;

    if ((CurrentState != HomeState)
            || ((CurrentHomeSubState != DriveState)
            && (CurrentHomeSubState != HitLeftState)
            && (CurrentHomeSubState != HitRightState))) {
        ISZAllTapeCount = 0;
        return FALSE;
    }

    if ((tapeMask & TAPE_ALL) != TAPE_ALL) {
        ISZAllTapeCount = 0;
        return FALSE;
    }

    if (ISZAllTapeCount < HSM_ISZ_CONFIRM_COUNT) {
        ISZAllTapeCount++;
    }

    if (ISZAllTapeCount >= HSM_ISZ_CONFIRM_COUNT) {
        ISZAllTapeCount = 0;
        thisEvent.EventType = ISZ_BORDER;
        thisEvent.EventParam = tapeMask;
        PostHSMService(thisEvent);
    }

    return TRUE;
}

uint8_t HSM_CheckDriveImmediate(unsigned char tapeMask) {
    ES_Event thisEvent;
    ES_EventTyp_t driveEvent;

    if ((CurrentState == HomeState)
            && (CurrentHomeSubState == DriveState)
            && (CurrentDriveSubState == DriveForwardSubState)) {
        if ((tapeMask & TAPE_ALL) == TAPE_ALL) {
            LatchISZTapeDetectedEvent(ES_NO_EVENT, tapeMask);
            DriveNextState = ISZState;
            driveEvent = MIDDLE_TAPE_DETECTED;
        } else if ((tapeMask & RIGHT_TAPE_MASK) != 0) {
            LatchISZTapeDetectedEvent(RIGHT_TAPE_DETECTED, tapeMask);
            DriveNextState = HitLeftState;
            driveEvent = RIGHT_TAPE_DETECTED;
        } else if ((tapeMask & LEFT_TAPE_MASK) != 0) {
            LatchISZTapeDetectedEvent(LEFT_TAPE_DETECTED, tapeMask);
            DriveNextState = HitRightState;
            driveEvent = LEFT_TAPE_DETECTED;
        } else {
            return FALSE;
        }

        DriveImmediateHit = TRUE;
        DriveImmediateTapeMask = tapeMask;
        ES_Timer_StopTimer(HSM_ROTATION_TIMER);

        thisEvent.EventType = driveEvent;
        thisEvent.EventParam = tapeMask;
        return PostHSMService(thisEvent);
    }

    return FALSE;
}

uint8_t HSM_CheckTravelTapeImmediate(unsigned char tapeMask) {
    ES_Event thisEvent;

    if (CurrentState != TravelState) {
        return FALSE;
    }

    if (CountTapeSensors(tapeMask) > 1) {
        TravelTapeLossLatched = FALSE;
        return FALSE;
    }

    PS_Stop();
    if (TravelTapeLossLatched == TRUE) {
        return FALSE;
    }

    TravelTapeLossLatched = TRUE;
    thisEvent.EventType = NO_TAPE_DETECTED;
    thisEvent.EventParam = tapeMask;
    return PostHSMService(thisEvent);
}

static uint8_t InitHomeSubHSM(void) {
    ES_Event returnEvent;

    CurrentHomeSubState = InitHomeSubState;
    returnEvent = RunHomeSubHSM(INIT_EVENT);
    return (returnEvent.EventType == ES_NO_EVENT) ? TRUE : FALSE;
}

static ES_Event RunHomeSubHSM(ES_Event thisEvent) {
    if ((IsTapeEvent(thisEvent.EventType) == TRUE)
            && (HomeSubStateIgnoresTapeEvents(CurrentHomeSubState) == TRUE)) {
        thisEvent.EventType = ES_NO_EVENT;
        return thisEvent;
    }

    if (thisEvent.EventType == ISZ_BORDER) {
        if ((CurrentHomeSubState == DriveState)
                || (CurrentHomeSubState == HitLeftState)
                || (CurrentHomeSubState == HitRightState)) {
            ES_Timer_StopTimer(HSM_ROTATION_TIMER);
            printf("HOME: all three tape detected -> ISZ\r\n");
            return TransitionHomeSubState(ISZState);
        }
        thisEvent.EventType = ES_NO_EVENT;
        return thisEvent;
    }

    switch (CurrentHomeSubState) {
        case InitHomeSubState:
            if (thisEvent.EventType == ES_INIT) {
                thisEvent = TransitionHomeSubState(LockOnState);
            }
            break;

        case LockOnState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    StartLockOn();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case BEACON_SAMPLE_READY:
                {
                    uint16_t rawValue;

                    rawValue =
                            BEACON_SAMPLE_EVENT_ADC(thisEvent.EventParam);
                    if (BeaconSampleIsDetected(rawValue) == TRUE) {
                        thisEvent = CompleteLockOn(rawValue);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                }

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        if (LockOnPhase == LockOnWaitingToTurn) {
                            printf("HOME: LOCK_ON hold complete -> tank turn\r\n");
                            StartLockOnTurn();
                        } else if (LockOnStepDriving == TRUE) {
                            StartLockOnStepPause();
                        } else {
                            StartLockOnStepDrive();
                        }
                    }
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_EXIT:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
            }
            break;

        case OnFieldState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    TapeEventsEnabled = FALSE;
                    DriveBackward();
                    ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                            HSM_ON_FIELD_BACKUP_TIME_MS);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        PS_Stop();
                        TapeEventsEnabled = TRUE;
                        printf("HOME: ON_FIELD backup timer complete -> LOCATE_TAPE\r\n");
                        thisEvent = TransitionHomeSubState(LocateTapeState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_EXIT:
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
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
                    StartTurnToCorner180();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam != HSM_ROTATION_TIMER) {
                        thisEvent.EventType = ES_NO_EVENT;
                    } else if (TurnToCorner180Complete() == TRUE) {
                        PS_Stop();
                        printf("HOME: TURN_TO_CORNER complete -> DRIVE\r\n");
                        ResetISZTapeEventMask();
                        thisEvent = TransitionHomeSubState(DriveState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
            }
            break;

        case DriveState:
            if (thisEvent.EventType == ES_ENTRY) {
                if (InitDriveSubHSM() == TRUE) {
                    thisEvent = TransitionHomeSubState(DriveNextState);
                } else {
                    thisEvent.EventType = ES_NO_EVENT;
                }
            } else if (RunDriveSubHSM(thisEvent) == TRUE) {
                ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                thisEvent = TransitionHomeSubState(DriveNextState);
            } else {
                thisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case HitLeftState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (AllISZTapeEventsDetected() == TRUE) {
                        printf("HOME: HIT_LEFT all tape events detected -> ISZ\r\n");
                        thisEvent = TransitionHomeSubState(ISZState);
                    } else {
                        StartHitLeftPivot();
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam != HSM_ROTATION_TIMER) {
                        thisEvent.EventType = ES_NO_EVENT;
                    } else if (AllISZTapeEventsDetected() == TRUE) {
                        ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                        printf("HOME: HIT_LEFT all tape events detected -> ISZ\r\n");
                        thisEvent = TransitionHomeSubState(ISZState);
                    } else if (AllTapeReleased() == TRUE) {
                        printf("HOME: HIT_LEFT all tape released -> DRIVE\r\n");
                        thisEvent = TransitionHomeSubState(DriveState);
                    } else {
                        StartHitLeftPivot();
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case BUMPER_TRIPPED:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    printf("HOME: HIT_LEFT bumper trip -> BUMP\r\n");
                    thisEvent = TransitionHomeSubState(BumpState);
                    break;

                default:
                    if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                        LatchISZTapeDetectedEvent(thisEvent.EventType,
                                TAPE_NONE);
                    }
                    if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                            && (AllISZTapeEventsDetected() == TRUE)) {
                        ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                        printf("HOME: HIT_LEFT all tape events detected -> ISZ\r\n");
                        thisEvent = TransitionHomeSubState(ISZState);
                    } else if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                            && (AllTapeReleased() == TRUE)) {
                        ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                        printf("HOME: HIT_LEFT all tape released -> DRIVE\r\n");
                        thisEvent = TransitionHomeSubState(DriveState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case HitRightState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    if (AllISZTapeEventsDetected() == TRUE) {
                        printf("HOME: HIT_RIGHT all tape events detected -> ISZ\r\n");
                        thisEvent = TransitionHomeSubState(ISZState);
                    } else {
                        StartHitRightPivot();
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam != HSM_ROTATION_TIMER) {
                        thisEvent.EventType = ES_NO_EVENT;
                    } else if (AllISZTapeEventsDetected() == TRUE) {
                        ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                        printf("HOME: HIT_RIGHT all tape events detected -> ISZ\r\n");
                        thisEvent = TransitionHomeSubState(ISZState);
                    } else if (AllTapeReleased() == TRUE) {
                        printf("HOME: HIT_RIGHT all tape released -> DRIVE\r\n");
                        thisEvent = TransitionHomeSubState(DriveState);
                    } else {
                        StartHitRightPivot();
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case BUMPER_TRIPPED:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    printf("HOME: HIT_RIGHT bumper trip -> BUMP\r\n");
                    thisEvent = TransitionHomeSubState(BumpState);
                    break;

                default:
                    if (IsTapeEvent(thisEvent.EventType) == TRUE) {
                        LatchISZTapeDetectedEvent(thisEvent.EventType,
                                TAPE_NONE);
                    }
                    if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                            && (AllISZTapeEventsDetected() == TRUE)) {
                        ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                        printf("HOME: HIT_RIGHT all tape events detected -> ISZ\r\n");
                        thisEvent = TransitionHomeSubState(ISZState);
                    } else if ((IsTapeEvent(thisEvent.EventType) == TRUE)
                            && (AllTapeReleased() == TRUE)) {
                        ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                        printf("HOME: HIT_RIGHT all tape released -> DRIVE\r\n");
                        thisEvent = TransitionHomeSubState(DriveState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
            }
            break;

        case ISZState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    TapeEventsEnabled = FALSE;
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    PS_Stop();
                    printf("HOME: ISZ reached, motors off\r\n");
                    ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                            HSM_ISZ_HOLD_TIME_MS);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        printf("HOME: ISZ hold complete -> AIM_LOCK_ON\r\n");
                        thisEvent = TransitionHomeSubState(AimLockOnState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_EXIT:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
            }
            break;

        case AimLockOnState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    StartAimLockOn();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case BEACON_SAMPLE_READY:
                {
                    uint16_t rawValue;

                    rawValue =
                            BEACON_SAMPLE_EVENT_ADC(thisEvent.EventParam);
                    if ((AimBeaconComplete == FALSE)
                            && (BeaconSampleIsDetected(rawValue) == TRUE)) {
                        thisEvent = CompleteAim(rawValue);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                }

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        if (AimLockOnPhase == LockOnInitialTurn) {
                            printf("HOME: AIM_LOCK_ON 180 complete -> hold\r\n");
                            StartAimLockOnCollect();
                        } else if (AimLockOnPhase == LockOnWaitingToTurn) {
                            printf("HOME: AIM_LOCK_ON hold complete -> tank turn\r\n");
                            StartAimLockOnTurn();
                        } else if (AimLockOnStepDriving == TRUE) {
                            StartAimLockOnStepPause();
                        } else {
                            StartAimLockOnStepDrive();
                        }
                    }
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_EXIT:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
            }
            break;

        case ShootState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    PS_Brake();
                    if (StartShootOutputs() != TRUE) {
                        StopShootOutputs();
                        thisEvent.EventType = ES_ERROR;
                    } else {
                        printf("HOME: SHOOT upper=85%% lower=35%% indexer=100%%\r\n");
                        ES_Timer_InitTimer(HSM_ROTATION_TIMER,
                                HSM_INDEXER_FULL_TIME_MS);
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam == HSM_ROTATION_TIMER) {
                        if (SetShootIndexerDuty(HSM_INDEXER_RUN_DUTY) != TRUE) {
                            StopShootOutputs();
                            thisEvent.EventType = ES_ERROR;
                        } else {
                            printf("HOME: SHOOT indexer dropped to 50%%\r\n");
                            thisEvent.EventType = ES_NO_EVENT;
                        }
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_EXIT:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    StopShootOutputs();
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
                    break;
            }
            break;

        case BumpState:
            switch (thisEvent.EventType) {
                case ES_ENTRY:
                    BumpPhase = BumpReversePhase;
                    StartBumpPhase(BumpPhase);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    if (thisEvent.EventParam != HSM_ROTATION_TIMER) {
                        thisEvent.EventType = ES_NO_EVENT;
                    } else if (AdvanceBumpPhase() == TRUE) {
                        printf("HOME: BUMP sequence complete -> DRIVE\r\n");
                        ResetISZTapeEventMask();
                        thisEvent = TransitionHomeSubState(DriveState);
                    } else {
                        thisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_EXIT:
                    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
                    thisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    thisEvent.EventType = ES_NO_EVENT;
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

        default:
            break;
    }

    return done;
}

static uint8_t InitDriveSubHSM(void) {
    CurrentDriveSubState = InitDriveSubState;
    DriveImmediateHit = FALSE;
    DriveImmediateTapeMask = TAPE_NONE;
    DriveNextState = DriveState;
    return RunDriveSubHSM(INIT_EVENT);
}

static uint8_t RunDriveSubHSM(ES_Event thisEvent) {
    uint8_t done = FALSE;

    switch (CurrentDriveSubState) {
        case InitDriveSubState:
            if (thisEvent.EventType == ES_INIT) {
                done = TransitionDriveSubState(DriveForwardSubState);
            }
            break;

        case DriveForwardSubState:
            if (thisEvent.EventType == ES_ENTRY) {
                DriveForward();
                ArmLocateTapePoll();
            } else if ((thisEvent.EventType == BUMPER_TRIPPED)
                    || (IsTapeEvent(thisEvent.EventType) == TRUE)
                    || ((thisEvent.EventType == ES_TIMEOUT)
                    && (thisEvent.EventParam == HSM_ROTATION_TIMER))) {
                if (DriveImmediateHit == TRUE) {
                    DriveImmediateHit = FALSE;
                    printf("HOME: DRIVE complete mask=0x%02x -> %s\r\n",
                            DriveImmediateTapeMask,
                            HomeSubStateName(DriveNextState));
                    done = TRUE;
                } else {
                    done = EvaluateDriveEvent(thisEvent.EventType);
                }
                if ((done == FALSE)
                        && (CurrentDriveSubState == DriveForwardSubState)) {
                    DriveForward();
                    ArmLocateTapePoll();
                }
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
    if (CurrentHomeSubState != ShootState) {
        StopShootOutputs();
    }
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

static uint8_t TransitionDriveSubState(DriveSubState_t nextState) {
    RunDriveSubHSM(EXIT_EVENT);
    CurrentDriveSubState = nextState;
    return RunDriveSubHSM(ENTRY_EVENT);
}

static void StartLockOn(void) {
    PS_Stop();
    TapeEventsEnabled = FALSE;
    LockOnPhase = LockOnWaitingToTurn;
    LockOnStepDriving = FALSE;
    BeaconSettleStartTime = ES_Timer_GetTime();

    printf("HOME: LOCK_ON hold %u ms, motors off\r\n",
            (unsigned) HSM_LOCK_COLLECT_TIME_MS);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_COLLECT_TIME_MS);
}

static void StartLockOnTurn(void) {
    LockOnPhase = LockOnTurning;
    printf("HOME: LOCK_ON tank turn waiting for sample > %u\r\n",
            (unsigned) HSM_BEACON_DETECT_MIN);
    StartLockOnStepDrive();
}

/* One increment of the stepped tank turn: drive briefly, then brake and pause.
 * Detected beacon samples complete immediately; non-detect samples are only
 * useful while the base is stationary. */
static void StartLockOnStepDrive(void) {
    LockOnStepDriving = TRUE;
    BeaconDetectCount = 0;
    PS_RightMtrSpeed(HSM_LOCK_TURN_POWER);
    PS_LeftMtrReverseSpeed(HSM_LOCK_TURN_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_STEP_DRIVE_TIME_MS);
}

static void StartLockOnStepPause(void) {
    LockOnStepDriving = FALSE;
    PS_Brake();
    BeaconSettleStartTime = ES_Timer_GetTime();
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_STEP_PAUSE_TIME_MS);
}

static uint8_t BeaconSampleIsDetected(uint16_t adcReading) {
    return (adcReading > HSM_BEACON_DETECT_MIN) ? TRUE : FALSE;
}

static ES_Event CompleteLockOn(uint16_t rawValue) {
    LockOnPhase = LockOnWaitingToTurn;
    LockOnStepDriving = FALSE;

    PS_Brake();
    TapeEventsEnabled = TRUE;
    printf("HOME: LOCK_ON complete raw=%u min=%u -> ON_FIELD\r\n",
            rawValue, (unsigned) HSM_BEACON_DETECT_MIN);

    return TransitionHomeSubState(OnFieldState);
}

static void StartAimLockOn(void) {
    TapeEventsEnabled = FALSE;
    AimLockOnPhase = LockOnInitialTurn;
    AimBeaconComplete = FALSE;
    AimLockOnStepDriving = TRUE;

    printf("HOME: AIM_LOCK_ON 180 degree tank turn\r\n");
    PS_TankTurnRightContinuous(HSM_TURN_TO_CORNER_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_TURN_TO_CORNER_180_TIME_MS);
}

static void StartAimLockOnCollect(void) {
    PS_Brake();
    AimLockOnPhase = LockOnWaitingToTurn;
    AimLockOnStepDriving = FALSE;
    BeaconDetectCount = 0;
    BeaconSettleStartTime = ES_Timer_GetTime();
    printf("HOME: AIM_LOCK_ON hold %u ms, motors off\r\n",
            (unsigned) HSM_LOCK_COLLECT_TIME_MS);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_COLLECT_TIME_MS);
}

static void StartAimLockOnTurn(void) {
    AimLockOnPhase = LockOnTurning;

    printf("HOME: AIM_LOCK_ON tank turn waiting for sample > %u\r\n",
            (unsigned) HSM_BEACON_DETECT_MIN);
    StartAimLockOnStepDrive();
}

static void StartAimLockOnStepDrive(void) {
    AimLockOnStepDriving = TRUE;
    BeaconDetectCount = 0;
    PS_RightMtrSpeed(HSM_LOCK_TURN_POWER);
    PS_LeftMtrReverseSpeed(HSM_LOCK_TURN_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_STEP_DRIVE_TIME_MS);
}

static void StartAimLockOnStepPause(void) {
    AimLockOnStepDriving = FALSE;
    PS_Brake();
    BeaconSettleStartTime = ES_Timer_GetTime();
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCK_STEP_PAUSE_TIME_MS);
}

static ES_Event CompleteAim(uint16_t rawValue) {
    AimBeaconComplete = TRUE;
    AimLockOnPhase = LockOnWaitingToTurn;
    AimLockOnStepDriving = FALSE;

    ES_Timer_StopTimer(HSM_ROTATION_TIMER);
    PS_Brake();
    printf("HOME: AIM beacon raw=%u min=%u -> SHOOT\r\n",
            rawValue, (unsigned) HSM_BEACON_DETECT_MIN);

    return TransitionHomeSubState(ShootState);
}

static uint8_t ConfigureShootOutputs(void) {
    unsigned int activePins;
    unsigned int missingPins;

    if (IO_PortsSetPortOutputs(HSM_INDEXER_PWM_IO_PORT,
            HSM_INDEXER_PWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(HSM_UPPER_SHOOTER_RPWM_IO_PORT,
            HSM_UPPER_SHOOTER_RPWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(HSM_LOWER_SHOOTER_LPWM_IO_PORT,
            HSM_LOWER_SHOOTER_LPWM_IO_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(HSM_UPPER_SHOOTER_ENABLE_PORT,
            HSM_UPPER_SHOOTER_REN_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortOutputs(HSM_LOWER_SHOOTER_ENABLE_PORT,
            HSM_LOWER_SHOOTER_LEN_BIT) != SUCCESS) {
        return FALSE;
    }

    activePins = PWM_ListPins();
    missingPins = HSM_SHOOT_PWM_PINS & (~activePins);
    if ((missingPins != 0)
            && (PWM_AddPins((unsigned short) missingPins) != SUCCESS)) {
        return FALSE;
    }

    return TRUE;
}

static uint8_t StartShootOutputs(void) {
    if (ConfigureShootOutputs() != TRUE) {
        return FALSE;
    }

    if (IO_PortsSetPortBits(HSM_UPPER_SHOOTER_ENABLE_PORT,
            HSM_UPPER_SHOOTER_REN_BIT) != SUCCESS) {
        return FALSE;
    }
    if (IO_PortsSetPortBits(HSM_LOWER_SHOOTER_ENABLE_PORT,
            HSM_LOWER_SHOOTER_LEN_BIT) != SUCCESS) {
        return FALSE;
    }

    if (PWM_SetDutyCycle(HSM_UPPER_SHOOTER_RPWM_PIN,
            HSM_UPPER_SHOOTER_DUTY) != SUCCESS) {
        return FALSE;
    }
    if (PWM_SetDutyCycle(HSM_LOWER_SHOOTER_LPWM_PIN,
            HSM_LOWER_SHOOTER_DUTY) != SUCCESS) {
        return FALSE;
    }

    return SetShootIndexerDuty(HSM_INDEXER_FULL_DUTY);
}

static uint8_t StopShootOutputs(void) {
    unsigned int activePins;
    uint8_t success = TRUE;

    activePins = PWM_ListPins();
    if (((activePins & HSM_INDEXER_PWM_PIN) != 0)
            && (PWM_SetDutyCycle(HSM_INDEXER_PWM_PIN, 0) != SUCCESS)) {
        success = FALSE;
    }
    if (((activePins & HSM_UPPER_SHOOTER_RPWM_PIN) != 0)
            && (PWM_SetDutyCycle(HSM_UPPER_SHOOTER_RPWM_PIN, 0) != SUCCESS)) {
        success = FALSE;
    }
    if (((activePins & HSM_LOWER_SHOOTER_LPWM_PIN) != 0)
            && (PWM_SetDutyCycle(HSM_LOWER_SHOOTER_LPWM_PIN, 0) != SUCCESS)) {
        success = FALSE;
    }

    IO_PortsClearPortBits(HSM_UPPER_SHOOTER_ENABLE_PORT,
            HSM_UPPER_SHOOTER_REN_BIT);
    IO_PortsClearPortBits(HSM_LOWER_SHOOTER_ENABLE_PORT,
            HSM_LOWER_SHOOTER_LEN_BIT);

    return success;
}

static uint8_t SetShootIndexerDuty(unsigned int duty) {
    if (duty > MAX_PWM) {
        return FALSE;
    }
    return PWM_SetDutyCycle(HSM_INDEXER_PWM_PIN, duty) == SUCCESS;
}

static void StartTurnToCorner180(void) {
    TurnStartTime = ES_Timer_GetTime();
    printf("HOME: TURN_TO_CORNER 180 degree tank turn\r\n");
    PS_TankTurnRightContinuous(HSM_TURN_TO_CORNER_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_TURN_TO_CORNER_180_TIME_MS);
}

static uint8_t TurnToCorner180Complete(void) {
    return ((ES_Timer_GetTime() - TurnStartTime)
            >= HSM_TURN_TO_CORNER_180_TIME_MS) ? TRUE : FALSE;
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

static uint8_t HomeSubStateIgnoresTapeEvents(HomeSubState_t state) {
    switch (state) {
        case InitHomeSubState:
        case LockOnState:
        case OnFieldState:
        case TurnToCornerState:
        case BumpState:
        case ISZState:
        case AimLockOnState:
        case ShootState:
            return TRUE;

        default:
            return FALSE;
    }
}

static uint8_t CountTapeSensors(unsigned char tapeMask) {
    uint8_t count = 0;

    if ((tapeMask & LEFT_TAPE_MASK) != 0) {
        count++;
    }
    if ((tapeMask & MID_TAPE_MASK) != 0) {
        count++;
    }
    if ((tapeMask & RIGHT_TAPE_MASK) != 0) {
        count++;
    }

    return count;
}

static uint8_t EvaluateLocateTape(void) {
    unsigned char tapeMask;

    tapeMask = PS_ReadTape();
    if (tapeMask != TAPE_NONE) {
        PS_Stop();
        printf("HOME: LOCATE_TAPE complete mask=0x%02x -> TURN_TO_CORNER\r\n",
                tapeMask);
        return TRUE;
    }

    return FALSE;
}

static uint8_t EvaluateDriveEvent(ES_EventTyp_t eventType) {
    if (IsTapeEvent(eventType) == TRUE) {
        LatchISZTapeDetectedEvent(eventType, TAPE_NONE);
    }

    if (AllISZTapeEventsDetected() == TRUE) {
        DriveNextState = ISZState;
    } else if (eventType == BUMPER_TRIPPED) {
        DriveNextState = BumpState;
    } else if (eventType == RIGHT_TAPE_DETECTED) {
        DriveNextState = HitLeftState;
    } else if (eventType == LEFT_TAPE_DETECTED) {
        DriveNextState = HitRightState;
    } else {
        return FALSE;
    }

    printf("HOME: DRIVE %s -> %s\r\n",
            EventNames[eventType],
            HomeSubStateName(DriveNextState));
    return TRUE;
}

static void EvaluateTravel(void) {
    unsigned char tapeMask;

    tapeMask = PS_ReadTape();
    if (CountTapeSensors(tapeMask) <= 1) {
        PS_Stop();
        TravelTapeLossLatched = TRUE;
        printf("TRAVEL: stopped, unsafe tape mask=0x%02x\r\n", tapeMask);
        return;
    }

    TravelTapeLossLatched = FALSE;
    if ((tapeMask & LEFT_TAPE_MASK) == 0) {
        printf("TRAVEL: left tape off mask=0x%02x, driving right motor\r\n",
                tapeMask);
        TravelRightOnly();
    } else if ((tapeMask & RIGHT_TAPE_MASK) == 0) {
        printf("TRAVEL: right tape off mask=0x%02x, driving left motor\r\n",
                tapeMask);
        TravelLeftOnly();
    } else {
        TravelForward();
    }
}

static const char *HomeSubStateName(HomeSubState_t state) {
    switch (state) {
        case DriveState:
            return "DRIVE";

        case HitLeftState:
            return "HIT_LEFT";

        case HitRightState:
            return "HIT_RIGHT";

        case ISZState:
            return "ISZ";

        case AimLockOnState:
            return "AIM_LOCK_ON";

        case ShootState:
            return "SHOOT";

        case BumpState:
            return "BUMP";

        default:
            return "UNKNOWN";
    }
}

static void DriveForward(void) {
    PS_Forward(HSM_DRIVE_POWER);
}

static void DriveBackward(void) {
    PS_Backward(HSM_DRIVE_POWER);
}

static void StartHitLeftPivot(void) {
    printf("HOME: HIT_LEFT correction, right motor faster\r\n");
    PS_LeftMtrSpeed(HSM_HIT_SLOW_POWER);
    PS_RightMtrSpeed(HSM_HIT_FAST_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_HIT_PIVOT_TIME_MS);
}

static void StartHitRightPivot(void) {
    printf("HOME: HIT_RIGHT correction, left motor faster\r\n");
    PS_LeftMtrSpeed(HSM_HIT_FAST_POWER);
    PS_RightMtrSpeed(HSM_HIT_SLOW_POWER);
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_HIT_PIVOT_TIME_MS);
}

static void StartBumpPhase(BumpPhase_t phase) {
    uint16_t phaseTime;

    switch (phase) {
        case BumpReversePhase:
            printf("HOME: BUMP reverse\r\n");
            PS_Backward(HSM_BUMP_POWER);
            phaseTime = HSM_BUMP_REVERSE_TIME_MS;
            break;

        case BumpTurnRightPhase:
            printf("HOME: BUMP tank turn right\r\n");
            PS_TankTurnRightContinuous(HSM_BUMP_POWER);
            phaseTime = HSM_BUMP_TURN_TIME_MS;
            break;

        case BumpDriveForwardPhase:
            printf("HOME: BUMP drive forward\r\n");
            PS_Forward(HSM_BUMP_POWER);
            phaseTime = HSM_BUMP_FORWARD_TIME_MS;
            break;

        case BumpTurnLeftPhase:
            printf("HOME: BUMP tank turn left\r\n");
            PS_TankTurnLeftContinuous(HSM_BUMP_POWER);
            phaseTime = HSM_BUMP_TURN_TIME_MS;
            break;

        default:
            PS_Stop();
            phaseTime = HSM_BUMP_REVERSE_TIME_MS;
            break;
    }

    ES_Timer_InitTimer(HSM_ROTATION_TIMER, phaseTime);
}

static uint8_t AdvanceBumpPhase(void) {
    if (BumpPhase == BumpTurnLeftPhase) {
        PS_Stop();
        return TRUE;
    }

    BumpPhase++;
    StartBumpPhase(BumpPhase);
    return FALSE;
}

static uint8_t AllTapeReleased(void) {
    return (PS_ReadTape() == TAPE_NONE) ? TRUE : FALSE;
}

static void ResetISZTapeEventMask(void) {
    ISZTapeEventMask = TAPE_NONE;
}

static void LatchISZTapeDetectedEvent(ES_EventTyp_t eventType,
        unsigned char tapeMask) {
    ISZTapeEventMask |= (tapeMask & TAPE_ALL);

    switch (eventType) {
        case LEFT_TAPE_DETECTED:
            ISZTapeEventMask |= LEFT_TAPE_MASK;
            break;

        case MIDDLE_TAPE_DETECTED:
            ISZTapeEventMask |= MID_TAPE_MASK;
            break;

        case RIGHT_TAPE_DETECTED:
            ISZTapeEventMask |= RIGHT_TAPE_MASK;
            break;

        default:
            break;
    }
}

static uint8_t AllISZTapeEventsDetected(void) {
    /* Only enter ISZ at the moment all three tape sensors are simultaneously on
     * tape. Use the debounced mask owned by the sensor event checker so motor
     * noise on a raw read cannot produce a false all-three detection. */
    return ((Sensors_GetDebouncedTapeMask() & TAPE_ALL) == TAPE_ALL)
            ? TRUE : FALSE;
}

static void TravelForward(void) {
    PS_Forward(HSM_TRAVEL_POWER);
}

static void TravelLeftOnly(void) {
    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(HSM_TRAVEL_CORRECT_POWER);
}

static void TravelRightOnly(void) {
    PS_LeftMtrSpeed(0);
    PS_RightMtrSpeed(HSM_TRAVEL_CORRECT_POWER);
}

static void ArmLocateTapePoll(void) {
    ES_Timer_InitTimer(HSM_ROTATION_TIMER, HSM_LOCATE_TAPE_POLL_TIME_MS);
}
