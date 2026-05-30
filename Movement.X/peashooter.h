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
#define SWITCH_THRESHOLD 2

// Bit positions used when packing the three tape sensors into one byte.
#define LEFT_TAPE_MASK  (1)
#define MID_TAPE_MASK   (1 << 1)
#define RIGHT_TAPE_MASK (1 << 2)


// Maximum motor command accepted by the peashooter speed functions.
#define PEASHOOTER_MAX_SPEED 1000 

// Blocking delay calibration constants for distance and turn helper functions.
#define MOVE_TIME_PER_DIST 50000

#define TURN_LEFT_TIME  800000
#define TURN_RIGHT_TIME 800000
#define TURN_90_TIME 1200000

// Busy-wait delay macro used by the turn helper functions.
#define DELAY_COUNTS(x) for(delay = 0; delay < (x); delay++) {asm("nop");}

// Initializes peashooter motors, PWM, tape sensor inputs, and ADC channels.
void PS_Init(void);

// Returns the raw 10-bit ADC reading for the battery voltage monitor.
unsigned int PS_BatteryVoltage(void);

// Reads the switch sensor and returns SWITCH_TRIPPED or SWITCH_NOT_TRIPPED.
unsigned char PS_ReadSwitch(void);

// Reads the left analog tape sensor with threshold hysteresis.
unsigned char PS_ReadLeftTape(void);

// Reads the middle analog tape sensor with threshold hysteresis.
unsigned char PS_ReadMidTape(void);

// Reads the right analog tape sensor with threshold hysteresis.
unsigned char PS_ReadRightTape(void);

// Packs left, middle, and right analog tape states into bits 0, 1, and 2.
unsigned char PS_ReadTape(void);

// Reads the left tape sensor as a temporary digital input.
unsigned char PS_ReadTapeDigital(void);

// Reads all three tape sensors as digital inputs and packs them into one byte.
unsigned char PS_ReadTapeDigitalAll(void);

// Sets left motor forward speed from 0 to PEASHOOTER_MAX_SPEED.
char PS_LeftMtrSpeed(unsigned int power);

// Sets right motor forward speed from 0 to PEASHOOTER_MAX_SPEED.
char PS_RightMtrSpeed(unsigned int power);

// Sets both motors using a raw PWM duty cycle from 0 to MAX_PWM.
char PS_RawMotor(unsigned int power);

// Drives both motors forward at the requested power.
char PS_Forward(unsigned int power);

// Drives both motors backward at the requested power.
char PS_Backward(unsigned int power);

// Stops both drive motors.
char PS_Stop(void);

// Drives forward for an approximate distance using a blocking delay.
char PS_ForwardDist(unsigned int power, char dist);

// Drives backward for an approximate distance using a blocking delay.
char PS_BackwardDist(unsigned int power, char dist);

// Spins left by driving the motors in opposite directions for a fixed delay.
char PS_TankTurnLeft(unsigned int power);

// Spins right by driving the motors in opposite directions for a fixed delay.
char PS_TankTurnRight(unsigned int power);

// Spins left for an approximate distance using a blocking delay.
char PS_TankTurnLeftDist(unsigned int power, unsigned int dist);

// Spins right for an approximate distance using a blocking delay.
char PS_TankTurnRightDist(unsigned int power, unsigned int dist);

// Pivots left by stopping the left wheel and driving the right wheel.
char PS_PivotTurnLeft(unsigned int power);

// Pivots right by stopping the right wheel and driving the left wheel.
char PS_PivotTurnRight(unsigned int power);

char PS_TurnRight90(void);

char PS_AngledForward(unsigned int power);

#endif
