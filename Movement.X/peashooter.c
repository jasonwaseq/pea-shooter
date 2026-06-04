#include <peashooter.h>
#include <BOARD.h>
#include <xc.h>
#include <pwm.h>
#include <serial.h>
#include <AD.h>
#include <IO_Ports.h>
#include <stdbool.h>


#define TOP_SHOOTER_PWM PWM_PORTY04
#define BOT_SHOOTER_PWM PWM_PORTX11

// PWM channels used to control speed for each motor.
#define LEFT_PWM PWM_PORTY10
#define RIGHT_PWM PWM_PORTY12

// Motor direction outputs use four consecutive Port Y pins: Y05-Y08.
#define LEFT_IN1 PORTY06_LAT
#define LEFT_IN2 PORTY05_LAT

#define RIGHT_IN1 PORTY08_LAT
#define RIGHT_IN2 PORTY07_LAT

// TRIS bits that set the motor direction pins as inputs or outputs.
#define LEFT_IN1_TRIS PORTY06_TRIS
#define LEFT_IN2_TRIS PORTY05_TRIS
#define RIGHT_IN1_TRIS PORTY08_TRIS
#define RIGHT_IN2_TRIS PORTY07_TRIS

// Digital input pin used by the peashooter switch sensor.
#define SWITCH_SENSOR PORTW04_BIT

// Digital input pins used by the left, middle, and right tape sensors.
#define LEFT_TAPE  PORTW03_BIT
#define MID_TAPE   PORTW06_BIT
#define RIGHT_TAPE PORTW05_BIT


// Battery readings used to scale motor PWM duty cycle as the battery voltage changes.
#define PS_NOMINAL_BATTERY_VOLTAGE 310
#define PS_MIN_BATTERY_COMP_VOLTAGE 263
#define PS_SHOOTER_START_DUTY 800
#define PS_SHOOTER_IDLE_DUTY 100
#define PS_SHOOTER_STARTUP_DELAY_COUNTS 1000000

// Wheel offsets
#define LEFT_MTR_OFFSET 1
#define RIGHT_MTR_OFFSET 1.11 //105% aka 5% higher


#define LEFT_TAPE_BIT   0b100
#define MID_TAPE_BIT    0b010
#define RIGHT_TAPE_BIT  0b001



// Adjusts a requested PWM duty cycle upward as battery voltage drops.

static unsigned int PS_CompensateDutyForBattery(unsigned int dutyCycle) {
    // Read the current battery level from the AD subsystem.
    unsigned int batteryVoltage = PS_BatteryVoltage();
    unsigned long compensatedDuty;

    // Clamp very low readings so compensation math stays bounded.
    if (batteryVoltage < PS_MIN_BATTERY_COMP_VOLTAGE) {
        batteryVoltage = PS_MIN_BATTERY_COMP_VOLTAGE;
    }

    // Scale duty cycle relative to the nominal battery voltage.
    compensatedDuty = ((unsigned long) dutyCycle * PS_NOMINAL_BATTERY_VOLTAGE)
            / batteryVoltage;

    // Never request a duty cycle beyond what the PWM driver supports.
    if (compensatedDuty > MAX_PWM) {
        compensatedDuty = MAX_PWM;
    }

    // Return the compensated duty cycle as a normal PWM value.
    return (unsigned int) compensatedDuty;
}

char PS_LeftMtrSpeed(int power) {
    unsigned int dutyCycle;
    power *= LEFT_MTR_OFFSET;
    if (power > MAX_PWM || power < -MAX_PWM) {
        return ERROR;
    }

    if (power == 0) {
        LEFT_IN1 = 0;
        LEFT_IN2 = 0;
        return PWM_SetDutyCycle(LEFT_PWM, 0);
    }

    if (power > 0) {
        LEFT_IN1 = 0;
        LEFT_IN2 = 1;
    } else {
        LEFT_IN1 = 1;
        LEFT_IN2 = 0;
        power = -power;
    }

    dutyCycle = power * (MAX_PWM / PEASHOOTER_MAX_SPEED);
    dutyCycle = PS_CompensateDutyForBattery(dutyCycle);
    return PWM_SetDutyCycle(LEFT_PWM, dutyCycle);
}

char PS_RightMtrSpeed(int power) {
    unsigned int dutyCycle;
    if (power > MAX_PWM || power < -MAX_PWM) {
        return ERROR;
    }

    if (power == 0) {
        RIGHT_IN1 = 0;
        RIGHT_IN2 = 0;
        return PWM_SetDutyCycle(RIGHT_PWM, 0);
    }

    if (power > 0) {
        RIGHT_IN1 = 0;
        RIGHT_IN2 = 1;
    } else {
        RIGHT_IN1 = 1;
        RIGHT_IN2 = 0;
        power = -power;
    }

    dutyCycle = power * (MAX_PWM / PEASHOOTER_MAX_SPEED);
    dutyCycle = PS_CompensateDutyForBattery(dutyCycle);
    return PWM_SetDutyCycle(RIGHT_PWM, dutyCycle);
}

// Initializes the left drive motor PWM pin and direction pins.

void PS_LeftMotorInit(void) {
    // Enable PWM output on the left motor speed-control pin.
    PWM_AddPins(LEFT_PWM);

    // Configure the two left motor direction pins as digital outputs.
    LEFT_IN1_TRIS = 0;
    LEFT_IN2_TRIS = 0;

    // Default the left motor driver to the forward direction.
    LEFT_IN1 = 1;
    LEFT_IN2 = 0;

    // Start stopped; speed is commanded later by changing the duty cycle.
    PWM_SetDutyCycle(LEFT_PWM, 0);
}

// Initializes the right drive motor PWM pin and direction pins.

void PS_RightMotorInit(void) {
    // Enable PWM output on the right motor speed-control pin.
    PWM_AddPins(RIGHT_PWM);

    // Configure the two right motor direction pins as digital outputs.
    RIGHT_IN1_TRIS = 0;
    RIGHT_IN2_TRIS = 0;

    // Default the right motor driver to the forward direction.
    RIGHT_IN1 = 1;
    RIGHT_IN2 = 0;

    // Start stopped; speed is commanded later by changing the duty cycle.
    PWM_SetDutyCycle(RIGHT_PWM, 0);
}

// Initializes the indexer motor output.

void PS_IndexerMotorInit(void) {
    // The standalone Indexer.X project drives the indexer on Z6 with hardware PWM.
}

// Spins up both shooter motors, then drops them to idle duty.

void PS_ShooterInit(void) {
    unsigned int delay;

    PWM_AddPins(TOP_SHOOTER_PWM | BOT_SHOOTER_PWM);
    PWM_SetDutyCycle(TOP_SHOOTER_PWM, PS_SHOOTER_START_DUTY);
    PWM_SetDutyCycle(BOT_SHOOTER_PWM, PS_SHOOTER_START_DUTY);

    DELAY_COUNTS(PS_SHOOTER_STARTUP_DELAY_COUNTS);

    PWM_SetDutyCycle(TOP_SHOOTER_PWM, PS_SHOOTER_IDLE_DUTY);
    PWM_SetDutyCycle(BOT_SHOOTER_PWM, PS_SHOOTER_IDLE_DUTY);
}

// Initializes all peashooter hardware used by motors and sensors.

void PS_Init(void) {
    // Initialize the PWM module before assigning motor pins or duty cycles.
    PWM_Init();

    // Use a shared 1 kHz PWM frequency for all peashooter motors.
    PWM_SetFrequency(1000);

    // Set up each motor's PWM output and default stopped state.
    PS_LeftMotorInit();
    PS_RightMotorInit();
    PS_IndexerMotorInit();
    PS_ShooterInit();

    // Configure switch and tape sensor pins as digital inputs.
    PORTW03_TRIS = 1;
    PORTW04_TRIS = 1;
    PORTW05_TRIS = 1;
    PORTW06_TRIS = 1;
}

// Reads the current battery voltage from the always-enabled battery monitor.

unsigned int PS_BatteryVoltage(void) {
    // Return the raw 10-bit ADC value for the battery voltage channel.
    return AD_ReadADPin(BAT_VOLTAGE);
}

unsigned char PS_ReadSwitch(void) {
    return SWITCH_SENSOR ? SWITCH_NOT_TRIPPED : SWITCH_TRIPPED;
}


bool PS_IsTape(unsigned char tape)
{
    return (PS_ReadTape() & tape) != 0;
}


unsigned char PS_ReadLeftTape(void) {
    return LEFT_TAPE ? TAPE_DETECTED : TAPE_NOT_DETECTED;
}

// Reads the middle tape sensor with hysteresis to avoid noisy state changes.

unsigned char PS_ReadMidTape(void) {
    return MID_TAPE ? TAPE_DETECTED : TAPE_NOT_DETECTED;
}

// Reads the right tape sensor with hysteresis to avoid noisy state changes.

unsigned char PS_ReadRightTape(void) {
    return RIGHT_TAPE ? TAPE_DETECTED : TAPE_NOT_DETECTED;
}

// Combines the three analog tape sensor states into one bit mask.

unsigned char PS_ReadTape(void)
{
    return (PS_ReadLeftTape()  << 2) |
           (PS_ReadMidTape()   << 1) |
            PS_ReadRightTape();
}





// Drives both motors forward at the requested power.

char PS_Forward(unsigned int power) {
    // Command both sides with the same positive speed.
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    // Report success after issuing the motor commands.
    return SUCCESS;
}

// Drives both motors backward at the requested power.

char PS_Backward(unsigned int power) {
    // Command both sides with the same reverse speed.
    PS_RightMtrSpeed(power * -1);
    PS_LeftMtrSpeed(power * -1);

    // Report success after issuing the motor commands.
    return SUCCESS;
}

// Stops both drive motors by commanding zero power.

char PS_Stop(void) {
    // Zero duty cycle on both motors stops the drive base.
    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);
    // Report success after issuing the stop commands.
    return SUCCESS;
}

// Drives forward for an approximate distance using a blocking delay.

char PS_ForwardDist(unsigned int power, char dist) {
    unsigned int delay;

    // Start moving forward at the requested power.
    PS_Forward(power);

    // Wait for a distance-scaled amount of time.
    for (delay = 0; delay < dist * MOVE_TIME_PER_DIST; delay++) {
        asm("nop");
    }

    // Stop both drive motors after the delay finishes.
    PS_Stop();

    // Report success after the move completes.
    return SUCCESS;
}

// Drives backward for an approximate distance using a blocking delay.

char PS_BackwardDist(unsigned int power, char dist) {
    unsigned int delay;

    // Start moving backward at the requested power.
    PS_Backward(power);

    // Wait for a distance-scaled amount of time.
    for (delay = 0; delay < dist * MOVE_TIME_PER_DIST; delay++) {
        asm("nop");
    }

    // Stop both drive motors after the delay finishes.
    PS_Stop();

    // Report success after the move completes.
    return SUCCESS;
}

// Spins the robot left by driving the motors in opposite directions.

char PS_TankTurnLeft(unsigned int power) {
    unsigned int delay;

    // Right motor forward and left motor reverse creates a left turn.
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power * -1);

    // Report success after the turn completes.
    return SUCCESS;
}

// Spins the robot right by driving the motors in opposite directions.

char PS_TankTurnRight(unsigned int power) {
    unsigned int delay;

    // Right motor forward and left motor reverse creates a left turn.
    PS_RightMtrSpeed(power * -1);
    PS_LeftMtrSpeed(power);

    // Report success after the turn completes.
    return SUCCESS;
}

// Spins the robot left for an approximate distance using a blocking delay.

char PS_TankTurnLeftDist(unsigned int power, unsigned int dist) {
    unsigned int delay;

    // Right motor forward and left motor reverse creates a left turn.
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power * -1);

    // Wait for a distance-scaled amount of time.
    for (delay = 0; delay < dist * MOVE_TIME_PER_DIST; delay++) {
        asm("nop");
    }

    // Stop both drive motors after the delay finishes.
    PS_Stop();

    // Report success after the turn completes.
    return SUCCESS;
}

// Spins the robot right for an approximate distance using a blocking delay.

char PS_TankTurnRightDist(unsigned int power, unsigned int dist) {
    unsigned int delay;

    // Right motor reverse and left motor forward creates a right turn.
    PS_RightMtrSpeed(power * -1);
    PS_LeftMtrSpeed(power);

    // Wait for a distance-scaled amount of time.
    for (delay = 0; delay < dist * MOVE_TIME_PER_DIST; delay++) {
        asm("nop");
    }

    // Stop both drive motors after the delay finishes.
    PS_Stop();

    // Report success after the turn completes.
    return SUCCESS;
}

//zero is forward

char PS_TankTurnLeftContinuous(unsigned int power) {
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power * -1);

    return SUCCESS;
}

char PS_TankTurnRightContinuous(unsigned int power) {
    PS_RightMtrSpeed(power * -1);
    PS_LeftMtrSpeed(power);

    return SUCCESS;
}

// Pivots left by stopping the left wheel and driving the right wheel forward.

char PS_PivotTurnLeft(unsigned int power) {
    PS_LeftMtrSpeed(power / 4);
    PS_RightMtrSpeed(power);

    return SUCCESS;
}

// Pivots right by stopping the right wheel and driving the left wheel forward.

char PS_PivotTurnRight(unsigned int power) {
    PS_RightMtrSpeed(power / 4);
    PS_LeftMtrSpeed(power);

    return SUCCESS;
}

char PS_TurnRight90() {
    unsigned int delay;

    // Right motor reverse and left motor forward creates a right turn.
    PS_RightMtrSpeed(-700);
    PS_LeftMtrSpeed(700);

    // Hold the turn for the calibrated right-turn delay.
    DELAY_COUNTS(TURN_90_TIME);

    // Stop both drive motors after the turn delay.
    PS_Stop();

    // Report success after the turn completes.
    return SUCCESS;
}

char PS_TurnLeft90() {
    unsigned int delay;

    // Right motor reverse and left motor forward creates a right turn.
    PS_RightMtrSpeed(700);
    PS_LeftMtrSpeed(-700);

    // Hold the turn for the calibrated right-turn delay.
    DELAY_COUNTS(TURN_90_TIME);

    // Stop both drive motors after the turn delay.
    PS_Stop();

    // Report success after the turn completes.
    return SUCCESS;
}

char PS_AngledRight(unsigned int power) {
    unsigned int delay;
    // Left motor faster, right motor slower = slight right curve
    PS_LeftMtrSpeed(power);
    PS_RightMtrSpeed(power - 100);


    return SUCCESS;
}

bool PS_IsMoving(void){
    return PWM_GetDutyCycle(LEFT_PWM) != 0 || PWM_GetDutyCycle(RIGHT_PWM) != 0;

}

char PS_AngledLeft(unsigned int power) {
    unsigned int delay;
    // Left motor faster, right motor slower = slight right curve
    PS_LeftMtrSpeed(power - 100);
    PS_RightMtrSpeed(power);


    return SUCCESS;
}

