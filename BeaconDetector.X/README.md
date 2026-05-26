# BeaconDetector.X

MPLAB X project scaffold for printing the raw beacon detector ADC reading with
the ES Framework on the PIC32/Uno32 I/O protection board.

Default wiring:

- Detector analog output: `AD_PORTV3`
- ADC range: 10-bit, `0..1023`
- Serial output: raw ADC value, one reading per line

To change the input pin or print rate, edit `BeaconEventChecker.h`:

```c
#define BEACON_DETECTOR_AD_PIN AD_PORTV3
#define BEACON_ADC_PRINT_DIVIDER 100
```
