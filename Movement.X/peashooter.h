#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include <BOARD.h>

#define SWITCH_TRIPPED 1
#define SWITCH_NOT_TRIPPED 0

#define TAPE_DETECTED 1
#define TAPE_NOT_DETECTED 0

#define TAPE_HIGH 900
#define TAPE_LOW  200
#define SWITCH_THRESHOLD 50

#define LEFT_TAPE_MASK  (1)
#define MID_TAPE_MASK   (1 << 1)
#define RIGHT_TAPE_MASK (1 << 2)


#define PEASHOOTER_MAX_SPEED 100 

#define MOVE_TIME_PER_DIST 50000

#define TURN_LEFT_TIME  800000
#define TURN_RIGHT_TIME 800000

#define DELAY_COUNTS(x) for(delay = 0; delay < (x); delay++) {asm("nop");}

/**
 * @Function Roach_Init(void)
 * @param None.
 * @return None.
 * @brief  Performs all the initialization necessary for the roach. this includes initializing
 * the PWM module, the A/D converter, the data directions on some pins, and
 * setting the initial motor directions.
 * @note  None.
 * @author Max Dunne, 2012.01.06 */
void PS_Init(void);

/**
 * @Function Roach_BatteryVoltage(void)
 * @param None.
 * @return a 10-bit value corresponding to the current voltage of the roach
 * @brief  returns a 10:1 scaled value of the roach battery level
 * @author Max Dunne, 2013.07.12 */
unsigned int PS_BatteryVoltage(void);

/**
 * @Function Roach_ReadFrontLeftBumper(void)
 * @param None.
 * @return BUMPER_TRIPPED or BUMPER_NOT_TRIPPED
 * @brief  Returns the state of the front left bumper
 * @author Max Dunne, 2012.01.06 */
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
