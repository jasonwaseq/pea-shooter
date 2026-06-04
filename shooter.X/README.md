# Shooter.X

MPLAB X ES Framework project for driving the upper and lower shooter
motor outputs from the PIC32MX320F128H / Uno32 I/O protection stack.

Default firmware behavior:

- Upper shooter motor auto-start: enabled
- Upper shooter output: drives `RPWM`/`PortY-04` with `R_EN`/`PortX-03`
- Upper shooter duty cycle: `700 / 1000`, or 70.0%
- Lower shooter output: drives `LPWM`/`PortX-11` with `L_EN`/`PortX-04`
- Lower shooter duty cycle: `700 / 1000`, or 70.0%
- Indexer output: drives `PWM`/`PortZ-06`
- Indexer startup duty cycle: `1000 / 1000`, or 100.0%, for `500 ms`
- Indexer running duty cycle: `500 / 1000`, or 50.0%
- Hardware PWM frequency: `PWM_1KHZ`

## Wiring

Power everything off before wiring.

Uno32 stack control wiring:

- Upper shooter: Uno32 `PortY-04` -> IBT-2 `RPWM`
- Upper shooter: Uno32 `PortX-03` -> IBT-2 `R_EN`
- Lower shooter: Uno32 `PortX-11` -> IBT-2 `LPWM`
- Lower shooter: Uno32 `PortX-04` -> IBT-2 `L_EN`
- Indexer: Uno32 `PortZ-06` -> motor driver PWM/control input
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

With the default code, the upper and lower shooter outputs start at the fixed
duty cycle after initialization. The indexer starts at full PWM duty for 500 ms,
then drops to 50% duty and stays there.

When the upper shooter is started, the service drives `RPWM` and enables
`R_EN`. The lower shooter uses only `LPWM` and `L_EN`. `PortZ-06` drives the
indexer.

Beacon detector ADC logic is intentionally kept in `BeaconDetector.X`.

To change the shooter PWM pins or duty cycles, edit `ShooterService.h`.
