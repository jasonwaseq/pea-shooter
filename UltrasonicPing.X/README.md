# UltrasonicPing.X

Standalone MPLAB X project for reading an HC-SR04 ultrasonic ping sensor with
the PIC32MX320F128H / Uno32 I/O protection board.

## Exact Connections

Use these connections:

| HC-SR04 pin | Connect to |
| --- | --- |
| `VCC` | Board `5V` |
| `GND` | Board `GND` |
| `TRIG` | `PortY-03` (`RD11`) |
| `ECHO` | `PortY-04` (`RD3`) through the divider below |

Do not connect `ECHO` directly to the PIC32 input. The HC-SR04 drives `ECHO`
near 5 V, while the PIC32 I/O input should see about 3.3 V.

Use this divider:

```text
HC-SR04 ECHO --- 1.0 kOhm ---+--- PortY-04 / RD3
                             |
                           2.0 kOhm
                             |
                            GND
```

The sensor, divider, and Uno32 must share ground.

## What the Firmware Does

- Sends a 10 us trigger pulse on `PortY-03`.
- Measures the width of the returning `ECHO` pulse on `PortY-04`.
- Converts pulse width to distance using 343 m/s for speed of sound.
- Prints readings over the existing board serial port at 115200 baud.
- Pings every 100 ms.

Example output:

```text
HC-SR04 ultrasonic ping demo
TRIG: PortY-03 / RD11, ECHO: PortY-04 / RD3
Echo 1170 us  Distance 20.1 cm  7.9 in
```

If you see repeated `No echo` messages, check the divider, common ground, and
that the sensor is powered from 5 V.
