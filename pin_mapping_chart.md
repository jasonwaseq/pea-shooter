# Pin Mapping Chart

Simple current robot wiring map.

| Function | Board pin | Software definition |
| --- | --- | --- |
| Beacon detector 1 | `V3` | `AD_PORTV3` |
| Beacon detector 2 | `V5` | `AD_PORTV5` |
| Left drive PWM | `Y10` | `PWM_PORTY10` |
| Right drive PWM | `Y12` | `PWM_PORTY12` |
| Indexer PWM | `Z6` | `PWM_PORTZ06` |
| Left tape | `W3` | `PORTW03_BIT` |
| Switch sensor | `W4` | `PORTW04_BIT` |
| Right tape | `W5` | `PORTW05_BIT` |
| Mid tape | `W6` | `PORTW06_BIT` |
| HC-SR04 trigger | `X10` | `PORTX`, `PIN10` |
| HC-SR04 echo | `Y11` | `PORTEbits.RE5` |
| Left motor IN1 | `Y6` | `PORTY06_LAT` |
| Left motor IN2 | `Y5` | `PORTY05_LAT` |
| Right motor IN1 | `Y8` | `PORTY08_LAT` |
| Right motor IN2 | `Y7` | `PORTY07_LAT` |
| Upper shooter RPWM | `Y4` | `PWM_PORTY04` |
| Upper shooter R_EN | `X3` | `PORTX`, `PIN3` |
| Lower shooter LPWM | `X11` | `PWM_PORTX11` |
| Lower shooter L_EN | `X4` | `PORTX`, `PIN4` |

## Notes

- `Z6`, `Y12`, `Y10`, `Y4`, and `X11` are the hardware PWM-capable pins used by this codebase.
- The shooter firmware also starts the indexer on `Z6`: 100% PWM for 500 ms, then 50% PWM.
- HC-SR04 `ECHO` must go through a 5 V to 3.3 V divider before reaching `Y11`.
