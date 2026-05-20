# BeaconDetector.X

MPLAB X project scaffold for reading an analog 2 kHz beacon detector strength
output with the ES Framework on the PIC32/Uno32 I/O protection board.

Default wiring:

- Detector analog output: `AD_PORTV3`
- ADC range: 10-bit, `0..1023`
- Default present threshold: `512`

To change the input pin or detection threshold, edit `BeaconEventChecker.h`:

```c
#define BEACON_DETECTOR_AD_PIN AD_PORTV3
#define BEACON_PRESENT_THRESHOLD 512
#define BEACON_LOST_HYSTERESIS 32
#define BEACON_STRENGTH_DELTA 8
```

The event checker posts:

- `BEACON_STRENGTH_CHANGED` when the ADC reading changes meaningfully
- `BEACON_DETECTED` when the ADC reading crosses the present threshold
- `BEACON_LOST` when the ADC reading drops below the hysteresis-adjusted threshold

All event parameters carry the raw ADC reading. The service currently prints
those readings over serial so you can tune the threshold with the real detector.
