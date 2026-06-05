#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include <BOARD.h>
#include <stdbool.h>

// Switch sensor return states.
#define SWITCH_TRIPPED 1
#define SWITCH_NOT_TRIPPED 0

// Tape sensor return states.
#define TAPE_DETECTED 1
#define TAPE_NOT_DETECTED 0

// Tape bit masks: left = 100, middle = 010, right = 001.
#define LEFT_TAPE_BIT   0b100
#define MID_TAPE_BIT    0b010
#define RIGHT_TAPE_BIT  0b001

// Maximum motor command accepted by the peashooter speed functions.
#define PEASHOOTER_MAX_SPEED 1000

// Blocking delay calibration constants.
#define MOVE_TIME_PER_DIST 50000

#define TURN_LEFT_TIME  800000
#define TURN_RIGHT_TIME 800000
#define TURN_90_TIME    600000

// Busy-wait delay macro.
#define DELAY_COUNTS(x) for(delay = 0; delay < (x); delay++) {asm("nop");}

// Initialization.
void PS_Init(void);
void PS_ShooterInit(void);

// Battery.
unsigned int PS_BatteryVoltage(void);

// Sensors.
unsigned char PS_ReadSwitch(void);

unsigned char PS_ReadLeftTape(void);
unsigned char PS_ReadMidTape(void);
unsigned char PS_ReadRightTape(void);
unsigned char PS_ReadTape(void);

bool PS_IsTape(unsigned char tape);

// Motor speed functions.
// Positive power = forward.
// Negative power = reverse.
// Zero = stop.
char PS_LeftMtrSpeed(int power);
char PS_RightMtrSpeed(int power);

// Movement.
char PS_Forward(unsigned int power);
char PS_Backward(unsigned int power);
char PS_Stop(void);

char PS_ForwardDist(unsigned int power, char dist);
char PS_BackwardDist(unsigned int power, char dist);

char PS_TankTurnLeft(unsigned int power);
char PS_TankTurnRight(unsigned int power);

char PS_TankTurnLeftDist(unsigned int power, unsigned int dist);
char PS_TankTurnRightDist(unsigned int power, unsigned int dist);

char PS_TankTurnLeftContinuous(unsigned int power);
char PS_TankTurnRightContinuous(unsigned int power);

char PS_PivotTurnLeft(unsigned int power);
char PS_PivotTurnRight(unsigned int power);

char PS_TurnRight90(void);
char PS_TurnLeft90(void);

char PS_AngledRight(unsigned int power);
char PS_AngledLeft(unsigned int power);

bool PS_IsMoving(void);

#endif