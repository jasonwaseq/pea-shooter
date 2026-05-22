#include <peashooter.h>
#include <BOARD.h>
#include <xc.h>
#include <pwm.h>
#include <serial.h>
#include <AD.h>
#include <IO_Ports.h>

#define LEFT_PWM PWM_PORTY10
#define RIGHT_PWM PWM_PORTY04
#define INDEXER_PWM PWM_PORTY12

// Left motor cluster: Y09, Y10, Y11
#define LEFT_IN1 LATEbits.LATE6      // PORTY09
#define LEFT_IN2 LATEbits.LATE5      // PORTY11

// Right motor cluster: Y03, Y04, Y05
#define RIGHT_IN1 LATDbits.LATD11    // PORTY03
#define RIGHT_IN2 LATDbits.LATD5     // PORTY05

#define LEFT_IN1_TRIS _TRISE6
#define LEFT_IN2_TRIS _TRISE5
#define RIGHT_IN1_TRIS _TRISD11
#define RIGHT_IN2_TRIS _TRISD5

#define SWITCH_SENSOR AD_PORTW3

#define LEFT_TAPE   AD_PORTW4
#define MID_TAPE  AD_PORTW5
#define RIGHT_TAPE   AD_PORTW6

#define LEFT_TAPE_DIGITAL PORTW04_BIT
#define MID_TAPE_DIGITAL PORTW05_BIT
#define RIGHT_TAPE_DIGITAL PORTW06_BIT

#define LEFT_TAPE_DIGITAL_TRIS PORTW04_TRIS
#define MID_TAPE_DIGITAL_TRIS PORTW05_TRIS
#define RIGHT_TAPE_DIGITAL_TRIS PORTW06_TRIS

#define LEFT_TAPE_DIGITAL_PCFG (1 << _AD1PCFG_PCFG10_POSITION)
#define MID_TAPE_DIGITAL_PCFG (1 << _AD1PCFG_PCFG13_POSITION)
#define RIGHT_TAPE_DIGITAL_PCFG (1 << _AD1PCFG_PCFG12_POSITION)

void PS_LeftMotorInit(void)
{
    PWM_AddPins(LEFT_PWM);

    LEFT_IN1_TRIS = 0;
    LEFT_IN2_TRIS = 0;

    LEFT_IN1 = 1;
    LEFT_IN2 = 0;

    PWM_SetDutyCycle(LEFT_PWM, 0);
}

void PS_RightMotorInit(void)
{
    PWM_AddPins(RIGHT_PWM);

    RIGHT_IN1_TRIS = 0;
    RIGHT_IN2_TRIS = 0;

    RIGHT_IN1 = 1;
    RIGHT_IN2 = 0;

    PWM_SetDutyCycle(RIGHT_PWM, 0);
}

void PS_IndexerMotorInit(void)
{
    PWM_AddPins(INDEXER_PWM);
    PWM_SetDutyCycle(INDEXER_PWM, 0);
}

void PS_Init(void)
{
    PWM_Init();
    PWM_SetFrequency(1000);
    PS_LeftMotorInit();
    PS_RightMotorInit();
    PS_IndexerMotorInit();

    LEFT_TAPE_DIGITAL_TRIS = 1;
    MID_TAPE_DIGITAL_TRIS = 1;
    RIGHT_TAPE_DIGITAL_TRIS = 1;

    AD_Init();
    AD_AddPins(SWITCH_SENSOR | LEFT_TAPE | MID_TAPE | RIGHT_TAPE);
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






unsigned char PS_ReadLeftTape(void)
{
    static unsigned char tapeState = TAPE_NOT_DETECTED;

    unsigned int value = AD_ReadADPin(LEFT_TAPE);

    if (value > TAPE_HIGH) {
        tapeState = TAPE_DETECTED;
    }
    else if (value < TAPE_LOW) {
        tapeState = TAPE_NOT_DETECTED;
    }

    return tapeState;
}

unsigned char PS_ReadMidTape(void)
{
    static unsigned char tapeState = TAPE_NOT_DETECTED;

    unsigned int value = AD_ReadADPin(MID_TAPE);

    if (value > TAPE_HIGH) {
        tapeState = TAPE_DETECTED;
    }
    else if (value < TAPE_LOW) {
        tapeState = TAPE_NOT_DETECTED;
    }

    return tapeState;
}

unsigned char PS_ReadRightTape(void)
{
    static unsigned char tapeState = TAPE_NOT_DETECTED;

    unsigned int value = AD_ReadADPin(RIGHT_TAPE);

    if (value > TAPE_HIGH) {
        tapeState = TAPE_DETECTED;
    }
    else if (value < TAPE_LOW) {
        tapeState = TAPE_NOT_DETECTED;
    }

    return tapeState;
}

unsigned char PS_ReadTape(void)
{
    //0, 1, 2
    return (PS_ReadLeftTape()
            + (PS_ReadMidTape() << 1)
            + (PS_ReadRightTape() << 2));
}

unsigned char PS_ReadTapeDigital(void)
{
    unsigned int oldConfig = AD1PCFG & LEFT_TAPE_DIGITAL_PCFG;
    unsigned char tapeDigital;

    AD1PCFGSET = LEFT_TAPE_DIGITAL_PCFG;
    LEFT_TAPE_DIGITAL_TRIS = 1;
    tapeDigital = (LEFT_TAPE_DIGITAL ? 1 : 0);

    if (!oldConfig) {
        AD1PCFGCLR = LEFT_TAPE_DIGITAL_PCFG;
    }

    return tapeDigital;
}

unsigned char PS_ReadTapeDigitalAll(void)
{
    unsigned int oldConfig = AD1PCFG & (LEFT_TAPE_DIGITAL_PCFG
            | MID_TAPE_DIGITAL_PCFG | RIGHT_TAPE_DIGITAL_PCFG);
    unsigned char tapeDigital;

    AD1PCFGSET = LEFT_TAPE_DIGITAL_PCFG
            | MID_TAPE_DIGITAL_PCFG | RIGHT_TAPE_DIGITAL_PCFG;

    LEFT_TAPE_DIGITAL_TRIS = 1;
    MID_TAPE_DIGITAL_TRIS = 1;
    RIGHT_TAPE_DIGITAL_TRIS = 1;

    tapeDigital = ((LEFT_TAPE_DIGITAL ? 1 : 0)
            + ((MID_TAPE_DIGITAL ? 1 : 0) << 1)
            + ((RIGHT_TAPE_DIGITAL ? 1 : 0) << 2));

    AD1PCFGCLR = (~oldConfig) & (LEFT_TAPE_DIGITAL_PCFG
            | MID_TAPE_DIGITAL_PCFG | RIGHT_TAPE_DIGITAL_PCFG);

    return tapeDigital;
}




char PS_Forward(char power)
{
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    return SUCCESS;
}

char PS_Backward(char power)
{
    power = -1 * power;
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    return SUCCESS;
}

char PS_ForwardDist(char power, char dist)
{
    unsigned int delay;

    PS_Forward(power);

    for (delay = 0; delay < dist * MOVE_TIME_PER_DIST; delay++) {
        asm("nop");
    }

    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);

    return SUCCESS;
}

char PS_BackwardDist(char power, char dist)
{
    unsigned int delay;

    PS_Backward(power);

    for (delay = 0; delay < dist * MOVE_TIME_PER_DIST; delay++) {
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
