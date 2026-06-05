# TapeSensor.X

Standalone tape sensor event monitor for the left, middle, and right tape
sensors.

The project prints the initial stable state and then prints each debounced tape
event transition:

- `LEFT_TAPE_DETECTED`
- `LEFT_TAPE_NOT_DETECTED`
- `MIDDLE_TAPE_DETECTED`
- `MIDDLE_TAPE_NOT_DETECTED`
- `RIGHT_TAPE_DETECTED`
- `RIGHT_TAPE_NOT_DETECTED`
Wiring matches the current robot pin map:

| Sensor | Board pin | Software input |
| --- | --- | --- |
| Left tape | `W3` | `PORTW03_BIT` |
| Middle tape | `W6` | `PORTW06_BIT` |
| Right tape | `W5` | `PORTW05_BIT` |

Serial output includes the event name, event parameter, stable tape mask, raw
tape mask, and decoded `L/M/R` values.
