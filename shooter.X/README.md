# shooter

MPLAB X ES Framework project for printing the raw beacon detector ADC value
from the PIC32MX320F128H / Uno32 I/O protection stack.

Default firmware behavior:

- Beacon detector analog output: `AD_PORTV3`
- Serial output: `beacon_adc=<raw 10-bit ADC value>`
- Shooter motor auto-start: disabled

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

With the default code, the motor driver is initialized but left disabled. The
event checker only prints the beacon detector ADC reading.

To change the beacon ADC pin or print rate, edit `ShooterEventChecker.h`.
