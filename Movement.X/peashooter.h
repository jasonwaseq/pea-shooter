#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include <BOARD.h>

// Switch sensor return states.
#define SWITCH_TRIPPED 1
#define SWITCH_NOT_TRIPPED 0

// Tape sensor return states.
#define TAPE_DETECTED 1
#define TAPE_NOT_DETECTED 0

// Analog thresholds used to classify switch and tape sensor readings.
#define TAPE_HIGH 900
#define TAPE_LOW  200
#define SWITCH_THRESHOLD 50

// Bit positions used when packing the three tape sensors into one byte.
#define LEFT_TAPE_MASK  (1)
#define MID_TAPE_MASK   (1 << 1)
#define RIGHT_TAPE_MASK (1 << 2)


// Maximum signed motor command accepted by the peashooter speed functions.
#define PEASHOOTER_MAX_SPEED 100 

// Blocking delay calibration constants for distance and turn helper functions.
#define MOVE_TIME_PER_DIST 50000

#define TURN_LEFT_TIME  800000
#define TURN_RIGHT_TIME 800000

// Busy-wait delay macro used by the turn helper functions.
#define DELAY_COUNTS(x) for(delay = 0; delay < (x); delay++) {asm("nop");}

void PS_Init(void);
unsigned int PS_BatteryVoltage(void);

unsigned char PS_ReadSwitch(void);

unsigned char PS_ReadLeftTape(void);
unsigned char PS_ReadMidTape(void);
unsigned char PS_ReadRightTape(void);
unsigned char PS_ReadTape(void);
unsigned char PS_ReadTapeDigital(void);
unsigned char PS_ReadTapeDigitalAll(void);

char PS_Forward(char power);
char PS_Backward(char power);
char PS_ForwardDist(char power, char dist);
char PS_BackwardDist(char power, char dist);
char PS_TurnLeft(char power);
char PS_TurnRight(char power);


#endif
