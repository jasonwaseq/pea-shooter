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
 *   PortY-12 -> RPWM
 *   PortY-10 -> LPWM
 *   PortY-09 -> R_EN
 *   PortY-11 -> L_EN
 */
#ifndef SHOOTER_RPWM_PIN
#define SHOOTER_RPWM_PIN PWM_PORTY12
#endif

#ifndef SHOOTER_RPWM_PIN_NAME
#define SHOOTER_RPWM_PIN_NAME "PortY-12"
#endif

#ifndef SHOOTER_LPWM_PIN
#define SHOOTER_LPWM_PIN PWM_PORTY10
#endif

#ifndef SHOOTER_LPWM_PIN_NAME
#define SHOOTER_LPWM_PIN_NAME "PortY-10"
#endif

#ifndef SHOOTER_ENABLE_PORT
#define SHOOTER_ENABLE_PORT PORTY
#endif

#ifndef SHOOTER_REN_ENABLE_BIT
#define SHOOTER_REN_ENABLE_BIT PIN9
#endif

#ifndef SHOOTER_REN_ENABLE_NAME
#define SHOOTER_REN_ENABLE_NAME "PortY-09"
#endif

#ifndef SHOOTER_LEN_ENABLE_BIT
#define SHOOTER_LEN_ENABLE_BIT PIN11
#endif

#ifndef SHOOTER_LEN_ENABLE_NAME
#define SHOOTER_LEN_ENABLE_NAME "PortY-11"
#endif

#ifndef SHOOTER_PWM_FREQUENCY
#define SHOOTER_PWM_FREQUENCY PWM_1KHZ
#endif

#ifndef SHOOTER_FORWARD_DUTY
#define SHOOTER_FORWARD_DUTY MAX_PWM
#endif

uint8_t InitShooterService(uint8_t priority);
uint8_t PostShooterService(ES_Event thisEvent);
ES_Event RunShooterService(ES_Event thisEvent);

uint8_t StartShooterMotor(void);
uint8_t StopShooterMotor(void);

#endif /* SHOOTER_SERVICE_H */
