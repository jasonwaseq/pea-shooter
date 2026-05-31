# Shooter.X

MPLAB X ES Framework project for driving the primary and secondary shooter
motor outputs from the PIC32MX320F128H / Uno32 I/O protection stack.

Default firmware behavior:

- Primary shooter motor auto-start: disabled
- Secondary shooter software PWM output: `PortZ-04`
- Secondary shooter duty cycle: `500 / 1000`, or 50.0%
- Hardware PWM frequency: `PWM_1KHZ`
- Secondary software PWM period: `20 ms`

## Wiring

Power everything off before wiring.

Uno32 stack control wiring:

- Uno32 `PortY-04` -> IBT-2 `RPWM`
- Uno32 `PortX-11` -> IBT-2 `LPWM`
- Uno32 `PortX-03` -> IBT-2 `R_EN`
- Uno32 `PortX-04` -> IBT-2 `L_EN`
- Uno32 `PortZ-04` -> secondary shooter motor driver PWM/control input
- Uno32 `GND` -> IBT-2 logic `GND`
- Uno32 `5V` -> IBT-2 logic `VCC`

Motor power wiring:

- Motor supply positive -> IBT-2 `B+`
- Motor supply negative -> IBT-2 `B-`
- Motor lead 1 -> IBT-2 `M+`
- Motor lead 2 -> IBT-2 `M-`

Important notes:

- The Uno32/IBT-2 ground and the motor supply negative must share common ground.
- Do not power the motor from the Uno32 `5V` pin.
- Put a fuse or current-limited supply on the motor `B+` lead while testing.
- Keep the motor mechanically unloaded for the first 20% duty-cycle test.
- Do not power either motor directly from an Uno32 pin.
- The local hardware PWM module only supports
  `PWM_PORTZ06`, `PWM_PORTY12`, `PWM_PORTY10`, `PWM_PORTY04`, and
  `PWM_PORTX11`. The secondary shooter uses software PWM so it can run on
  digital `PortZ-04` instead of hardware PWM `PortZ-06`.

With the default code, the primary shooter motor driver is initialized but left
disabled. The secondary shooter PWM output starts at the fixed duty cycle after
initialization.

Beacon detector ADC logic is intentionally kept in `BeaconDetector.X`.

To change the shooter PWM pins or duty cycles, edit `ShooterService.h`.
