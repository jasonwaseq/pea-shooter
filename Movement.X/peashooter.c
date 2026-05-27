#include <peashooter.h>
#include <BOARD.h>
#include <xc.h>
#include <pwm.h>
#include <serial.h>
#include <AD.h>
#include <IO_Ports.h>

// PWM channels used to control speed for each motor.
#define LEFT_PWM PWM_PORTY10
#define RIGHT_PWM PWM_PORTY04
#define INDEXER_PWM PWM_PORTY12

// Left motor direction outputs in the Y09/Y10/Y11 motor cluster.
#define LEFT_IN1 LATEbits.LATE6      // PORTY09
#define LEFT_IN2 LATEbits.LATE5      // PORTY11

// Right motor direction outputs in the Y03/Y04/Y05 motor cluster.
#define RIGHT_IN1 LATDbits.LATD11    // PORTY03
#define RIGHT_IN2 LATDbits.LATD5     // PORTY05

// TRIS bits that set the motor direction pins as inputs or outputs.
#define LEFT_IN1_TRIS _TRISE6
#define LEFT_IN2_TRIS _TRISE5
#define RIGHT_IN1_TRIS _TRISD11
#define RIGHT_IN2_TRIS _TRISD5

// Analog input channel used by the peashooter switch sensor.
#define SWITCH_SENSOR AD_PORTW3

// Analog input channels used by the left, middle, and right tape sensors.
#define LEFT_TAPE   AD_PORTW4
#define MID_TAPE  AD_PORTW5
#define RIGHT_TAPE   AD_PORTW6

// Digital port reads for the same tape sensor pins.
#define LEFT_TAPE_DIGITAL PORTW04_BIT
#define MID_TAPE_DIGITAL PORTW05_BIT
#define RIGHT_TAPE_DIGITAL PORTW06_BIT

// TRIS controls used when reading the tape sensors as digital inputs.
#define LEFT_TAPE_DIGITAL_TRIS PORTW04_TRIS
#define MID_TAPE_DIGITAL_TRIS PORTW05_TRIS
#define RIGHT_TAPE_DIGITAL_TRIS PORTW06_TRIS

// AD1PCFG masks used to temporarily switch tape pins between analog and digital mode.
#define LEFT_TAPE_DIGITAL_PCFG (1 << _AD1PCFG_PCFG10_POSITION)
#define MID_TAPE_DIGITAL_PCFG (1 << _AD1PCFG_PCFG13_POSITION)
#define RIGHT_TAPE_DIGITAL_PCFG (1 << _AD1PCFG_PCFG12_POSITION)

// Battery readings used to scale motor PWM duty cycle as the battery voltage changes.
#define PS_NOMINAL_BATTERY_VOLTAGE 310
#define PS_MIN_BATTERY_COMP_VOLTAGE 263

// Adjusts a requested PWM duty cycle upward as battery voltage drops.
static unsigned int PS_CompensateDutyForBattery(unsigned int dutyCycle)
{
    // Read the current battery level from the AD subsystem.
    unsigned int batteryVoltage = PS_BatteryVoltage();
    unsigned long compensatedDuty;

    // Clamp very low readings so compensation math stays bounded.
    if (batteryVoltage < PS_MIN_BATTERY_COMP_VOLTAGE) {
        batteryVoltage = PS_MIN_BATTERY_COMP_VOLTAGE;
    }

    // Scale duty cycle relative to the nominal battery voltage.
    compensatedDuty = ((unsigned long)dutyCycle * PS_NOMINAL_BATTERY_VOLTAGE)
            / batteryVoltage;

    // Never request a duty cycle beyond what the PWM driver supports.
    if (compensatedDuty > MAX_PWM) {
        compensatedDuty = MAX_PWM;
    }

    // Return the compensated duty cycle as a normal PWM value.
    return (unsigned int)compensatedDuty;
}

// Initializes the left drive motor PWM pin and direction pins.
void PS_LeftMotorInit(void)
{
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
void PS_RightMotorInit(void)
{
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

// Initializes the indexer motor PWM pin and leaves it stopped.
void PS_IndexerMotorInit(void)
{
    // Enable PWM output on the indexer motor speed-control pin.
    PWM_AddPins(INDEXER_PWM);

    // Start the indexer stopped until a feed command sets its duty cycle.
    PWM_SetDutyCycle(INDEXER_PWM, 0);
}

// Initializes all peashooter hardware used by motors and sensors.
void PS_Init(void)
{
    // Initialize the PWM module before assigning motor pins or duty cycles.
    PWM_Init();

    // Use a shared 1 kHz PWM frequency for all peashooter motors.
    PWM_SetFrequency(1000);

    // Set up each motor's PWM output and default stopped state.
    PS_LeftMotorInit();
    PS_RightMotorInit();
    PS_IndexerMotorInit();

    // Configure tape sensor pins as inputs for digital reads.
    LEFT_TAPE_DIGITAL_TRIS = 1;
    MID_TAPE_DIGITAL_TRIS = 1;
    RIGHT_TAPE_DIGITAL_TRIS = 1;

    // Initialize ADC support for the switch and tape sensor analog channels.
    AD_Init();
    AD_AddPins(SWITCH_SENSOR | LEFT_TAPE | MID_TAPE | RIGHT_TAPE);
}

// Reads the current battery voltage from the always-enabled battery monitor.
unsigned int PS_BatteryVoltage(void)
{
    // Return the raw 10-bit ADC value for the battery voltage channel.
    return AD_ReadADPin(BAT_VOLTAGE);
}

// Sets the left motor speed and direction, with battery compensation.
char PS_LeftMtrSpeed(char power)
{
    unsigned int dutyCycle;

    // Reject commands outside the allowed signed speed range.
    if ((power < -PEASHOOTER_MAX_SPEED) || (power > PEASHOOTER_MAX_SPEED)) {
        return ERROR;
    }

    // Negative power reverses the left motor direction.
    if (power < 0) {
        LEFT_IN1 = 0;
        LEFT_IN2 = 1;

        // PWM duty cycle uses speed magnitude, not direction sign.
        power = -power;
    } else {
        // Zero or positive power sets the left motor forward.
        LEFT_IN1 = 1;
        LEFT_IN2 = 0;
    }

    // Scale the requested speed, then compensate duty cycle for battery level.
    dutyCycle = power * (MAX_PWM / PEASHOOTER_MAX_SPEED);
    dutyCycle = PS_CompensateDutyForBattery(dutyCycle);

    return PWM_SetDutyCycle(LEFT_PWM, dutyCycle);
}

// Sets the right motor speed and direction, with battery compensation.
char PS_RightMtrSpeed(char power)
{
    unsigned int dutyCycle;

    // Reject commands outside the allowed signed speed range.
    if ((power < -PEASHOOTER_MAX_SPEED) || (power > PEASHOOTER_MAX_SPEED)) {
        return ERROR;
    }

    // Negative power reverses the right motor direction.
    if (power < 0) {
        RIGHT_IN1 = 0;
        RIGHT_IN2 = 1;

        // PWM duty cycle uses speed magnitude, not direction sign.
        power = -power;
    } else {
        // Zero or positive power sets the right motor forward.
        RIGHT_IN1 = 1;
        RIGHT_IN2 = 0;
    }

    // Scale the requested speed, then compensate duty cycle for battery level.
    dutyCycle = power * (MAX_PWM / PEASHOOTER_MAX_SPEED);
    dutyCycle = PS_CompensateDutyForBattery(dutyCycle);

    return PWM_SetDutyCycle(RIGHT_PWM, dutyCycle);
}

// Reads the switch sensor and converts the analog value into a switch state.
unsigned char PS_ReadSwitch(void)
{
    // Sample the switch sensor ADC channel.
    unsigned int switchVal = AD_ReadADPin(SWITCH_SENSOR);

    // A value below the threshold means the switch is pressed/tripped.
    if (switchVal < SWITCH_THRESHOLD) {
        return SWITCH_TRIPPED;
    } else {
        // Otherwise report the switch as not tripped.
        return SWITCH_NOT_TRIPPED;
    }
}






// Reads the left tape sensor with hysteresis to avoid noisy state changes.
unsigned char PS_ReadLeftTape(void)
{
    // Keep the last stable tape state between calls.
    static unsigned char tapeState = TAPE_NOT_DETECTED;

    // Sample the left tape sensor ADC channel.
    unsigned int value = AD_ReadADPin(LEFT_TAPE);

    // High readings latch the state to tape detected.
    if (value > TAPE_HIGH) {
        tapeState = TAPE_DETECTED;
    }
    else if (value < TAPE_LOW) {
        // Low readings latch the state to tape not detected.
        tapeState = TAPE_NOT_DETECTED;
    }

    // Readings between the thresholds keep the previous state.
    return tapeState;
}

// Reads the middle tape sensor with hysteresis to avoid noisy state changes.
unsigned char PS_ReadMidTape(void)
{
    // Keep the last stable tape state between calls.
    static unsigned char tapeState = TAPE_NOT_DETECTED;

    // Sample the middle tape sensor ADC channel.
    unsigned int value = AD_ReadADPin(MID_TAPE);

    // High readings latch the state to tape detected.
    if (value > TAPE_HIGH) {
        tapeState = TAPE_DETECTED;
    }
    else if (value < TAPE_LOW) {
        // Low readings latch the state to tape not detected.
        tapeState = TAPE_NOT_DETECTED;
    }

    // Readings between the thresholds keep the previous state.
    return tapeState;
}

// Reads the right tape sensor with hysteresis to avoid noisy state changes.
unsigned char PS_ReadRightTape(void)
{
    // Keep the last stable tape state between calls.
    static unsigned char tapeState = TAPE_NOT_DETECTED;

    // Sample the right tape sensor ADC channel.
    unsigned int value = AD_ReadADPin(RIGHT_TAPE);

    // High readings latch the state to tape detected.
    if (value > TAPE_HIGH) {
        tapeState = TAPE_DETECTED;
    }
    else if (value < TAPE_LOW) {
        // Low readings latch the state to tape not detected.
        tapeState = TAPE_NOT_DETECTED;
    }

    // Readings between the thresholds keep the previous state.
    return tapeState;
}

// Combines the three analog tape sensor states into one bit mask.
unsigned char PS_ReadTape(void)
{
    // Pack left, middle, and right tape detections into bits 0, 1, and 2.
    return (PS_ReadLeftTape()
            + (PS_ReadMidTape() << 1)
            + (PS_ReadRightTape() << 2));
}

// Reads the left tape sensor as a digital input instead of an ADC channel.
unsigned char PS_ReadTapeDigital(void)
{
    // Save the current analog/digital configuration for the left tape pin.
    unsigned int oldConfig = AD1PCFG & LEFT_TAPE_DIGITAL_PCFG;
    unsigned char tapeDigital;

    // Temporarily force the pin into digital input mode.
    AD1PCFGSET = LEFT_TAPE_DIGITAL_PCFG;
    LEFT_TAPE_DIGITAL_TRIS = 1;

    // Convert the raw port bit into a 0 or 1 return value.
    tapeDigital = (LEFT_TAPE_DIGITAL ? 1 : 0);

    // Restore analog mode if the pin was analog before this read.
    if (!oldConfig) {
        AD1PCFGCLR = LEFT_TAPE_DIGITAL_PCFG;
    }

    // Return the sampled digital tape state.
    return tapeDigital;
}

// Reads all three tape sensors as digital inputs and packs them into a bit mask.
unsigned char PS_ReadTapeDigitalAll(void)
{
    // Save the current analog/digital configuration for all tape pins.
    unsigned int oldConfig = AD1PCFG & (LEFT_TAPE_DIGITAL_PCFG
            | MID_TAPE_DIGITAL_PCFG | RIGHT_TAPE_DIGITAL_PCFG);
    unsigned char tapeDigital;

    // Temporarily force all tape pins into digital input mode.
    AD1PCFGSET = LEFT_TAPE_DIGITAL_PCFG
            | MID_TAPE_DIGITAL_PCFG | RIGHT_TAPE_DIGITAL_PCFG;

    // Make sure each tape pin is configured as an input.
    LEFT_TAPE_DIGITAL_TRIS = 1;
    MID_TAPE_DIGITAL_TRIS = 1;
    RIGHT_TAPE_DIGITAL_TRIS = 1;

    // Pack left, middle, and right digital reads into bits 0, 1, and 2.
    tapeDigital = ((LEFT_TAPE_DIGITAL ? 1 : 0)
            + ((MID_TAPE_DIGITAL ? 1 : 0) << 1)
            + ((RIGHT_TAPE_DIGITAL ? 1 : 0) << 2));

    // Restore analog mode only on pins that were analog before this read.
    AD1PCFGCLR = (~oldConfig) & (LEFT_TAPE_DIGITAL_PCFG
            | MID_TAPE_DIGITAL_PCFG | RIGHT_TAPE_DIGITAL_PCFG);

    // Return the packed digital tape state.
    return tapeDigital;
}




// Drives both motors forward at the requested power.
char PS_Forward(char power)
{
    // Command both sides with the same positive speed.
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    // Report success after issuing the motor commands.
    return SUCCESS;
}

// Drives both motors backward at the requested power.
char PS_Backward(char power)
{
    // Convert the requested forward-style power into reverse power.
    power = -1 * power;

    // Command both sides with the same negative speed.
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(power);

    // Report success after issuing the motor commands.
    return SUCCESS;
}

// Stops both drive motors by commanding zero power.
char PS_Stop(void)
{
    // Zero duty cycle on both motors stops the drive base.
    PS_RightMtrSpeed(0);
    PS_LeftMtrSpeed(0);

    // Report success after issuing the stop commands.
    return SUCCESS;
}

// Drives forward for an approximate distance using a blocking delay.
char PS_ForwardDist(char power, char dist)
{
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
char PS_BackwardDist(char power, char dist)
{
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
char PS_TurnLeft(char power)
{
    unsigned int delay;

    // Right motor forward and left motor reverse creates a left turn.
    PS_RightMtrSpeed(power);
    PS_LeftMtrSpeed(-power);

    // Hold the turn for the calibrated left-turn delay.
    DELAY_COUNTS(TURN_LEFT_TIME);

    // Stop both drive motors after the turn delay.
    PS_Stop();

    // Report success after the turn completes.
    return SUCCESS;
}

// Spins the robot right by driving the motors in opposite directions.
char PS_TurnRight(char power)
{
    unsigned int delay;

    // Right motor reverse and left motor forward creates a right turn.
    PS_RightMtrSpeed(-power);
    PS_LeftMtrSpeed(power);

    // Hold the turn for the calibrated right-turn delay.
    DELAY_COUNTS(TURN_RIGHT_TIME);

    // Stop both drive motors after the turn delay.
    PS_Stop();

    // Report success after the turn completes.
    return SUCCESS;
}
