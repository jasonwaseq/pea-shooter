# shooter

MPLAB X ES Framework project for driving one DC motor through an IBT-2/BTS7960
motor driver from the PIC32MX320F128H / Uno32 I/O protection stack.

Default firmware command:

- Direction: forward
- Duty cycle: `1000 / 1000`, or 100.0%
- PWM frequency: `PWM_1KHZ`

## Wiring

Power everything off before wiring.

Uno32 stack control wiring:

- Uno32 `PortY-12` -> IBT-2 `RPWM`
- Uno32 `PortY-10` -> IBT-2 `LPWM`
- Uno32 `PortY-09` -> IBT-2 `R_EN`
- Uno32 `PortY-11` -> IBT-2 `L_EN`
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

With the default code, `R_EN` and `L_EN` are driven high, `RPWM` is driven at
100% duty, and `LPWM` is held at 0% duty. If the motor spins the wrong direction,
swap `M+` and `M-`, or swap the `RPWM` and `LPWM` behavior in
`ShooterService.c`.

To change the pins, frequency, or duty cycle, edit `ShooterService.h`.
