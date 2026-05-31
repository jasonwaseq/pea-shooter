# UltrasonicPing.X

Standalone MPLAB X project for reading an HC-SR04 ultrasonic ping sensor with
the PIC32MX320F128H / Uno32 I/O protection board.

## Exact Connections

Use these connections:

| HC-SR04 pin | Connect to |
| --- | --- |
| `VCC` | Board `5V` |
| `GND` | Board `GND` |
| `TRIG` | `PortX-10` / `RD7` |
| `ECHO` | `PortX-12` / `RD6`, through the divider below |

Do not connect `ECHO` directly to the PIC32 input. The HC-SR04 drives `ECHO`
near 5 V, while the PIC32 I/O input should see about 3.3 V.

Use this divider:

```text
HC-SR04 ECHO --- 1.2 kOhm ---+--- PortX-12 / RD6
                             |
                           2.2 kOhm
                             |
                            GND
```

The course I/O protection board labels are usually the least ambiguous path:
`TRIG` on `PortX-10`, `ECHO` on `PortX-12`.

The sensor, divider, and Uno32 must share ground.

## What the Firmware Does

- Sends a 10 us trigger pulse on `PortX-10`.
- Measures the returning `ECHO` pulse on `PortX-12`.
- Converts pulse width to distance using 343 m/s for speed of sound.
- Smooths readings with an 8-sample moving average.
- Captures a reference distance from 16 good samples at startup.
- Prints whether the filtered distance is `really_close`, `near_reference`, or
  `really_far` compared with the reference distance.
- Prints readings over the existing board serial port at 115200 baud.
- Pings every 100 ms.

The close/far thresholds are both 100 mm by default. Edit
`REALLY_CLOSE_DELTA_MM` and `REALLY_FAR_DELTA_MM` in `main.c` to tune them.

Type `z` in the serial terminal to zero the current object position as the new
reference distance.

Example output:

```text
HC-SR04 ultrasonic ping demo
TRIG: PortX-10 / RD7
ECHO: PortX-12 / RD6
reference_cm=20.1
raw_cm=18.9 avg_cm=19.5 avg_in=7.7 ref_cm=20.1 state=near_reference
raw_cm=8.7 avg_cm=9.8 avg_in=3.9 ref_cm=20.1 state=really_close
raw_cm=38.2 avg_cm=34.0 avg_in=13.4 ref_cm=20.1 state=really_far
```

If you see repeated `No echo` messages, check the divider, common ground, and
that the sensor is powered from 5 V.
