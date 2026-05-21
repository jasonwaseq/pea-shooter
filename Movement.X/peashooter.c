#include <peashooter.h>
#include <BOARD.h>
#include <xc.h>
#include <pwm.h>
#include <serial.h>
#include <AD.h>

#define LEFT_PWM PWM_PORTY10
#define RIGHT_PWM PWM_PORTY12

#define LEFT_IN1 LATBbits.LATB3
#define LEFT_IN2 LATBbits.LATB2
#define RIGHT_IN1 LATEbits.LATE5
#define RIGHT_IN2 LATEbits.LATE6

#define LEFT_IN1_TRIS _TRISB3
#define LEFT_IN2_TRIS _TRISB2
#define RIGHT_IN1_TRIS _TRISE5
#define RIGHT_IN2_TRIS _TRISE6

#define SWITCH_SENSOR AD_PORTW3
#define TAPE_SENSOR   AD_PORTW4

void PS_Init(void)
{
    PWM_Init();
    PWM_SetFrequency(1000);
    PWM_AddPins(LEFT_PWM | RIGHT_PWM);

    LEFT_IN1_TRIS = 0;
    LEFT_IN2_TRIS = 0;
    RIGHT_IN1_TRIS = 0;
    RIGHT_IN2_TRIS = 0;

    LEFT_IN1 = 1;
    LEFT_IN2 = 0;
    RIGHT_IN1 = 1;
    RIGHT_IN2 = 0;

    AD_Init();
    AD_AddPins(SWITCH_SENSOR | TAPE_SENSOR);
}

char PS_LeftMtrSpeed(char power)
{
    if ((power < -PEASHOOTER_MAX_SPEED) || (power > PEASHOOTER_MAX_SPEED)) {
        return ERROR;
    }

    if (power < 0) {
        LEFT_IN1 = 0;
        LEFT_IN2 = 1;
        power = -power;
    } else {
        LEFT_IN1 = 1;
        LEFT_IN2 = 0;
    }

    return PWM_SetDutyCycle(LEFT_PWM,
            power * (MAX_PWM / PEASHOOTER_MAX_SPEED));
}

char PS_RightMtrSpeed(char power)
{
    if ((power < -PEASHOOTER_MAX_SPEED) || (power > PEASHOOTER_MAX_SPEED)) {
        return ERROR;
    }

    if (power < 0) {
        RIGHT_IN1 = 0;
        RIGHT_IN2 = 1;
        power = -power;
    } else {
        RIGHT_IN1 = 1;
        RIGHT_IN2 = 0;
    }

    return PWM_SetDutyCycle(RIGHT_PWM,
            power * (MAX_PWM / PEASHOOTER_MAX_SPEED));
}

unsigned char PS_ReadSwitch(void)
{
    unsigned int switchVal = AD_ReadADPin(SWITCH_SENSOR);

    if (switchVal < SWITCH_THRESHOLD) {
        return SWITCH_TRIPPED;
    } else {
        return SWITCH_NOT_TRIPPED;
    }
}

unsigned int PS_ReadTapeSensor(void)
{
    return AD_ReadADPin(TAPE_SENSOR);
}

unsigned char PS_ReadTapeDigital(void)
{
    unsigned int tapeVal = AD_ReadADPin(TAPE_SENSOR);

    if (tapeVal > TAPE_THRESHOLD) {
        return TAPE_DETECTED;
    } else {
        return TAPE_NOT_DETECTED;
    }
}

char PS_Forward(char power, char dist)
{
    unsigned int delay;

    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    // crude distance timing estimate
    for(delay = 0; delay < dist * 50000; delay++) {
        asm("nop");
    }

    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);

    return SUCCESS;
}

char PS_Backward(char power, char dist)
{
    unsigned int delay;

    power = -1 * power;
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    // crude distance timing estimate
    for(delay = 0; delay < dist * 50000; delay++) {
        asm("nop");
    }

    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);

    return SUCCESS;
}

char PS_TurnLeft(char power)
{
    unsigned int delay;

    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(-power);

    DELAY_COUNTS(TURN_LEFT_TIME);

    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);

    return SUCCESS;
}

char PS_TurnRight(char power)
{
    unsigned int delay;

    PS_RightMtrSpeed(-power);
    PS_LeftMtrSpeed(power);

    DELAY_COUNTS(TURN_RIGHT_TIME);

    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);

    return SUCCESS;
}
