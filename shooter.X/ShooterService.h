/*
 * ShooterService.h
 */

#ifndef SHOOTER_SERVICE_H
#define SHOOTER_SERVICE_H

#include <stdint.h>

#include "ES_Configure.h"
#include "ES_Events.h"
#include "IO_Ports.h"
#include "pwm.h"

/*
 * Shooter IBT-2/BTS7960 default wiring on the Uno32 I/O protection stack:
 *   Upper shooter: PortY-04 -> RPWM, PortX-03 -> R_EN
 *   Lower shooter: PortX-11 -> LPWM, PortX-04 -> L_EN
 *
 * PortW is usable as Uno stack digital I/O, but it is not supported by the
 * hardware PWM module in include/pwm.h. Keep RPWM/LPWM on PWM-capable pins.
 */
#ifndef UPPER_SHOOTER_PWM_PIN
#define UPPER_SHOOTER_PWM_PIN PWM_PORTY04
#endif

#ifndef UPPER_SHOOTER_PWM_PIN_NAME
#define UPPER_SHOOTER_PWM_PIN_NAME "PortY-04"
#endif

#ifndef UPPER_SHOOTER_PWM_IO_PORT
#define UPPER_SHOOTER_PWM_IO_PORT PORTY
#endif

#ifndef UPPER_SHOOTER_PWM_IO_BIT
#define UPPER_SHOOTER_PWM_IO_BIT PIN4
#endif

#ifndef LOWER_SHOOTER_PWM_PIN
#define LOWER_SHOOTER_PWM_PIN PWM_PORTX11
#endif

#ifndef LOWER_SHOOTER_PWM_PIN_NAME
#define LOWER_SHOOTER_PWM_PIN_NAME "PortX-11"
#endif

#ifndef LOWER_SHOOTER_PWM_IO_PORT
#define LOWER_SHOOTER_PWM_IO_PORT PORTX
#endif

#ifndef LOWER_SHOOTER_PWM_IO_BIT
#define LOWER_SHOOTER_PWM_IO_BIT PIN11
#endif

#ifndef SHOOTER_ENABLE_PORT
#define SHOOTER_ENABLE_PORT PORTX
#endif

#ifndef UPPER_SHOOTER_ENABLE_BIT
#define UPPER_SHOOTER_ENABLE_BIT PIN3
#endif

#ifndef UPPER_SHOOTER_ENABLE_NAME
#define UPPER_SHOOTER_ENABLE_NAME "PortX-03"
#endif

#ifndef LOWER_SHOOTER_ENABLE_BIT
#define LOWER_SHOOTER_ENABLE_BIT PIN4
#endif

#ifndef LOWER_SHOOTER_ENABLE_NAME
#define LOWER_SHOOTER_ENABLE_NAME "PortX-04"
#endif

#ifndef SHOOTER_PWM_FREQUENCY
#define SHOOTER_PWM_FREQUENCY PWM_1KHZ
#endif

#ifndef UPPER_SHOOTER_DUTY
#define UPPER_SHOOTER_DUTY 200
#endif

#ifndef LOWER_SHOOTER_DUTY_CYCLE
#define LOWER_SHOOTER_DUTY_CYCLE 200
#endif

uint8_t InitShooterService(uint8_t priority);
uint8_t PostShooterService(ES_Event thisEvent);
ES_Event RunShooterService(ES_Event thisEvent);

uint8_t StartUpperShooterMotor(void);
uint8_t StopUpperShooterMotor(void);
uint8_t StartLowerShooterMotor(void);
uint8_t StopLowerShooterMotor(void);
uint8_t StartShooterMotor(void);
uint8_t StopShooterMotor(void);

#endif /* SHOOTER_SERVICE_H */
