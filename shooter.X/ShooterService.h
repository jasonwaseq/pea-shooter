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
 * IBT-2/BTS7960 default wiring on the Uno32 I/O protection stack:
 *   PortY-04 -> RPWM
 *   PortX-11 -> LPWM
 *   PortX-03 -> R_EN
 *   PortX-04 -> L_EN
 *
 * Secondary shooter:
 *   PortZ-04 -> motor driver software PWM/control input
 *
 * PortW is usable as Uno stack digital I/O, but it is not supported by the
 * hardware PWM module in include/pwm.h. Keep RPWM/LPWM on PWM-capable pins.
 */
#ifndef SHOOTER_RPWM_PIN
#define SHOOTER_RPWM_PIN PWM_PORTY04
#endif

#ifndef SHOOTER_RPWM_PIN_NAME
#define SHOOTER_RPWM_PIN_NAME "PortY-04"
#endif

#ifndef SHOOTER_LPWM_PIN
#define SHOOTER_LPWM_PIN PWM_PORTX11
#endif

#ifndef SHOOTER_LPWM_PIN_NAME
#define SHOOTER_LPWM_PIN_NAME "PortX-11"
#endif

#ifndef SHOOTER_ENABLE_PORT
#define SHOOTER_ENABLE_PORT PORTX
#endif

#ifndef SHOOTER_REN_ENABLE_BIT
#define SHOOTER_REN_ENABLE_BIT PIN3
#endif

#ifndef SHOOTER_REN_ENABLE_NAME
#define SHOOTER_REN_ENABLE_NAME "PortX-03"
#endif

#ifndef SHOOTER_LEN_ENABLE_BIT
#define SHOOTER_LEN_ENABLE_BIT PIN4
#endif

#ifndef SHOOTER_LEN_ENABLE_NAME
#define SHOOTER_LEN_ENABLE_NAME "PortX-04"
#endif

#ifndef SHOOTER_PWM_FREQUENCY
#define SHOOTER_PWM_FREQUENCY PWM_1KHZ
#endif

#ifndef SHOOTER_FORWARD_DUTY
#define SHOOTER_FORWARD_DUTY MAX_PWM
#endif

#ifndef SECONDARY_SHOOTER_PWM_PORT
#define SECONDARY_SHOOTER_PWM_PORT PORTZ
#endif

#ifndef SECONDARY_SHOOTER_PWM_BIT
#define SECONDARY_SHOOTER_PWM_BIT PIN4
#endif

#ifndef SECONDARY_SHOOTER_PWM_PIN_NAME
#define SECONDARY_SHOOTER_PWM_PIN_NAME "PortZ-04"
#endif

#ifndef SECONDARY_SHOOTER_DUTY_CYCLE
#define SECONDARY_SHOOTER_DUTY_CYCLE (MAX_PWM / 2)
#endif

#ifndef SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS
#define SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS 20
#endif

uint8_t InitShooterService(uint8_t priority);
uint8_t PostShooterService(ES_Event thisEvent);
ES_Event RunShooterService(ES_Event thisEvent);

uint8_t StartShooterMotor(void);
uint8_t StopShooterMotor(void);
uint8_t StartSecondaryShooterMotor(void);
uint8_t StopSecondaryShooterMotor(void);

#endif /* SHOOTER_SERVICE_H */
