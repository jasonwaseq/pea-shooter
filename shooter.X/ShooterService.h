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
 * Upper shooter:
 *   PortY-04 -> RPWM
 *   PortX-03 -> R_EN
 *
 * Lower shooter:
 *   PortX-11 -> LPWM
 *   PortX-04 -> L_EN
 */
#ifndef UPPER_SHOOTER_RPWM_PIN
#define UPPER_SHOOTER_RPWM_PIN PWM_PORTY04
#endif

#ifndef UPPER_SHOOTER_RPWM_PIN_NAME
#define UPPER_SHOOTER_RPWM_PIN_NAME "PortY-04"
#endif

#ifndef UPPER_SHOOTER_RPWM_IO_PORT
#define UPPER_SHOOTER_RPWM_IO_PORT PORTY
#endif

#ifndef UPPER_SHOOTER_RPWM_IO_BIT
#define UPPER_SHOOTER_RPWM_IO_BIT PIN4
#endif

#ifndef UPPER_SHOOTER_ENABLE_PORT
#define UPPER_SHOOTER_ENABLE_PORT PORTX
#endif

#ifndef UPPER_SHOOTER_REN_ENABLE_BIT
#define UPPER_SHOOTER_REN_ENABLE_BIT PIN3
#endif

#ifndef UPPER_SHOOTER_REN_ENABLE_NAME
#define UPPER_SHOOTER_REN_ENABLE_NAME "PortX-03"
#endif

#ifndef LOWER_SHOOTER_LPWM_PIN
#define LOWER_SHOOTER_LPWM_PIN PWM_PORTX11
#endif

#ifndef LOWER_SHOOTER_LPWM_PIN_NAME
#define LOWER_SHOOTER_LPWM_PIN_NAME "PortX-11"
#endif

#ifndef LOWER_SHOOTER_LPWM_IO_PORT
#define LOWER_SHOOTER_LPWM_IO_PORT PORTX
#endif

#ifndef LOWER_SHOOTER_LPWM_IO_BIT
#define LOWER_SHOOTER_LPWM_IO_BIT PIN11
#endif

#ifndef LOWER_SHOOTER_ENABLE_PORT
#define LOWER_SHOOTER_ENABLE_PORT PORTX
#endif

#ifndef LOWER_SHOOTER_LEN_ENABLE_BIT
#define LOWER_SHOOTER_LEN_ENABLE_BIT PIN4
#endif

#ifndef LOWER_SHOOTER_LEN_ENABLE_NAME
#define LOWER_SHOOTER_LEN_ENABLE_NAME "PortX-04"
#endif

#ifndef SHOOTER_PWM_FREQUENCY
#define SHOOTER_PWM_FREQUENCY PWM_1KHZ
#endif

#ifndef UPPER_SHOOTER_DUTY
#define UPPER_SHOOTER_DUTY 850
#endif

#ifndef LOWER_SHOOTER_DUTY
#define LOWER_SHOOTER_DUTY 350
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

/* Compatibility wrappers for older call sites. */
uint8_t StartPrimaryShooterMotor(void);
uint8_t StopPrimaryShooterMotor(void);
uint8_t StartSecondaryShooterMotor(void);
uint8_t StopSecondaryShooterMotor(void);

#endif /* SHOOTER_SERVICE_H */
