/*
 * File: FindBeaconSubHSM.c
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "peashooter.h"
#include "ES_Timers.h"
#include "SensorsEventChecker.h"
#include <stdio.h>
#include "beacon.h"
#include "FindBeaconSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define HSM_FULL_ROTATION_TIME_MS 5000
#define HSM_LOCK_ON_START_DELAY_MS 250
#define HSM_BEACON_AVG_WINDOW 8
#define HSM_BEACON_LOCK_OFFSET 10
#define HSM_SWEEP_TURN_POWER 800
#define HSM_LOCK_TURN_POWER 800
#define HSM_DRIVE_POWER 800

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    FindBeaconSubHSM_Init,
    SweepDataState,
    LockOnState,
} FindBeaconSubHSMState_t;

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
static ES_Event TransitionFindBeaconSubState(FindBeaconSubHSMState_t nextState);
static void StartSweepData(void);
static void StartLockOn(void);
static void StartLockOnTurn(void);
static void StartNextLockOnRotation(void);
static void ResetBeaconFilter(void);
static uint16_t FilterBeaconSample(uint16_t sample);

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static uint16_t BeaconSamples[HSM_BEACON_AVG_WINDOW];
static uint8_t BeaconSampleIndex;
static uint8_t BeaconSampleCount;
static uint32_t BeaconSampleSum;

static uint16_t MaxFilteredBeaconValue;
static uint16_t BeaconLockThreshold;
static uint8_t LockOnTurnStarted;
static uint16_t LockOnRotationMax;
static uint8_t TapeEventsEnabled;

static FindBeaconSubHSMState_t FindBeaconSubHSMCurrentState = FindBeaconSubHSM_Init;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
uint8_t InitFindBeaconSubHSM(void) {
    ES_Event returnEvent;

    FindBeaconSubHSMCurrentState = FindBeaconSubHSM_Init;
    returnEvent = RunFindBeaconSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
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
                            printf("RunFindBeaconSubHSM: LockOnState: locked adc=%u threshold=%u\r\n",
                                    filteredValue,
                                    BeaconLockThreshold);
                            TapeEventsEnabled = TRUE;
                            ThisEvent.EventType = BEACON_LOCKED;
                            ThisEvent.EventParam = filteredValue;
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
