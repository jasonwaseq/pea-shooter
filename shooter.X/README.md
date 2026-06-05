# Shooter.X

MPLAB X ES Framework project for driving the upper and lower shooter
motor outputs from the PIC32MX320F128H / Uno32 I/O protection stack.

Default firmware behavior:

- Upper shooter hardware PWM: `RPWM`/`PortY-04`
- Upper shooter enable: `R_EN`/`PortX-03`
- Upper shooter duty cycle: `850 / 1000`, or 85.0%
- Lower shooter hardware PWM: `LPWM`/`PortX-11`
- Lower shooter enable: `L_EN`/`PortX-04`
- Lower shooter duty cycle: `350 / 1000`, or 35.0%
- Hardware PWM frequency: `PWM_1KHZ`

## Wiring

Power everything off before wiring.

Uno32 stack control wiring:

- Upper shooter: Uno32 `PortY-04` -> motor driver `RPWM`
- Upper shooter: Uno32 `PortX-03` -> motor driver `R_EN`
- Lower shooter: Uno32 `PortX-11` -> motor driver `LPWM`
- Lower shooter: Uno32 `PortX-04` -> motor driver `L_EN`
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
  `PWM_PORTX11`.

With the default code, both shooter outputs initialize disabled. When the
shooter service starts, the upper shooter runs at 85% and the lower shooter
runs at 35%.

Beacon detector ADC logic is intentionally kept in `BeaconDetector.X`.

To change the shooter PWM pins or duty cycles, edit `ShooterService.h`.
