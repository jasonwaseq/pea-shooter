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
 * Primary shooter, IBT-2/BTS7960 default wiring on the Uno32 I/O protection stack:
 *   PortY-04 -> RPWM
 *   PortX-11 -> LPWM
 *   PortX-03 -> R_EN
 *   PortX-04 -> L_EN
 *
 * Secondary shooter:
 *   PortZ-06 -> motor driver hardware PWM/control input
 *
 * PortW is usable as Uno stack digital I/O, but it is not supported by the
 * hardware PWM module in include/pwm.h. Keep RPWM/LPWM on PWM-capable pins.
 *
 * Primary shooter direction command:
 * drive RPWM and hold LPWM low on the IBT-2.
 */
#ifndef PRIMARY_SHOOTER_RPWM_PIN
#define PRIMARY_SHOOTER_RPWM_PIN PWM_PORTY04
#endif

#ifndef PRIMARY_SHOOTER_RPWM_PIN_NAME
#define PRIMARY_SHOOTER_RPWM_PIN_NAME "PortY-04"
#endif

#ifndef PRIMARY_SHOOTER_RPWM_IO_PORT
#define PRIMARY_SHOOTER_RPWM_IO_PORT PORTY
#endif

#ifndef PRIMARY_SHOOTER_RPWM_IO_BIT
#define PRIMARY_SHOOTER_RPWM_IO_BIT PIN4
#endif

#ifndef PRIMARY_SHOOTER_LPWM_PIN
#define PRIMARY_SHOOTER_LPWM_PIN PWM_PORTX11
#endif

#ifndef PRIMARY_SHOOTER_LPWM_PIN_NAME
#define PRIMARY_SHOOTER_LPWM_PIN_NAME "PortX-11"
#endif

#ifndef PRIMARY_SHOOTER_LPWM_IO_PORT
#define PRIMARY_SHOOTER_LPWM_IO_PORT PORTX
#endif

#ifndef PRIMARY_SHOOTER_LPWM_IO_BIT
#define PRIMARY_SHOOTER_LPWM_IO_BIT PIN11
#endif

#ifndef PRIMARY_SHOOTER_ENABLE_PORT
#define PRIMARY_SHOOTER_ENABLE_PORT PORTX
#endif

#ifndef PRIMARY_SHOOTER_REN_ENABLE_BIT
#define PRIMARY_SHOOTER_REN_ENABLE_BIT PIN3
#endif

#ifndef PRIMARY_SHOOTER_REN_ENABLE_NAME
#define PRIMARY_SHOOTER_REN_ENABLE_NAME "PortX-03"
#endif

#ifndef PRIMARY_SHOOTER_LEN_ENABLE_BIT
#define PRIMARY_SHOOTER_LEN_ENABLE_BIT PIN4
#endif

#ifndef PRIMARY_SHOOTER_LEN_ENABLE_NAME
#define PRIMARY_SHOOTER_LEN_ENABLE_NAME "PortX-04"
#endif

#ifndef SHOOTER_PWM_FREQUENCY
#define SHOOTER_PWM_FREQUENCY PWM_1KHZ
#endif

#ifndef PRIMARY_SHOOTER_DUTY
#define PRIMARY_SHOOTER_DUTY MAX_PWM
#endif

#ifndef PRIMARY_SHOOTER_DRIVE_PWM_PIN
#define PRIMARY_SHOOTER_DRIVE_PWM_PIN PRIMARY_SHOOTER_RPWM_PIN
#endif

#ifndef PRIMARY_SHOOTER_DRIVE_PWM_PIN_NAME
#define PRIMARY_SHOOTER_DRIVE_PWM_PIN_NAME PRIMARY_SHOOTER_RPWM_PIN_NAME
#endif

#ifndef PRIMARY_SHOOTER_IDLE_PWM_PIN
#define PRIMARY_SHOOTER_IDLE_PWM_PIN PRIMARY_SHOOTER_LPWM_PIN
#endif

#ifndef PRIMARY_SHOOTER_IDLE_PWM_PIN_NAME
#define PRIMARY_SHOOTER_IDLE_PWM_PIN_NAME PRIMARY_SHOOTER_LPWM_PIN_NAME
#endif

#ifndef SECONDARY_SHOOTER_PWM_PIN
#define SECONDARY_SHOOTER_PWM_PIN PWM_PORTZ06
#endif

#ifndef SECONDARY_SHOOTER_PWM_PIN_NAME
#define SECONDARY_SHOOTER_PWM_PIN_NAME "PortZ-06"
#endif

#ifndef SECONDARY_SHOOTER_PWM_IO_PORT
#define SECONDARY_SHOOTER_PWM_IO_PORT PORTZ
#endif

#ifndef SECONDARY_SHOOTER_PWM_IO_BIT
#define SECONDARY_SHOOTER_PWM_IO_BIT PIN6
#endif

#ifndef SECONDARY_SHOOTER_PWM_IO_TRIS
#define SECONDARY_SHOOTER_PWM_IO_TRIS PORTZ06_TRIS
#endif

#ifndef SECONDARY_SHOOTER_DUTY_CYCLE
#define SECONDARY_SHOOTER_DUTY_CYCLE MAX_PWM
#endif

#ifndef SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS
#define SECONDARY_SHOOTER_SOFT_PWM_PERIOD_MS 20
#endif

uint8_t InitShooterService(uint8_t priority);
uint8_t PostShooterService(ES_Event thisEvent);
ES_Event RunShooterService(ES_Event thisEvent);

uint8_t StartPrimaryShooterMotor(void);
uint8_t StopPrimaryShooterMotor(void);
uint8_t StartShooterMotor(void);
uint8_t StopShooterMotor(void);
uint8_t StartSecondaryShooterMotor(void);
uint8_t StopSecondaryShooterMotor(void);

#endif /* SHOOTER_SERVICE_H */
