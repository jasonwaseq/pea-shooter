# BeaconDetector.X

MPLAB X project scaffold for printing the raw beacon detector ADC reading with
the ES Framework on the PIC32/Uno32 I/O protection board.

Default wiring:

- Detector 1 analog output: `AD_PORTV3`
- Detector 2 analog output: `AD_PORTV5`
- ADC range: 10-bit, `0..1023`
- Serial output: `detector1_adc,detector2_adc`

To change the input pin or print rate, edit `BeaconEventChecker.h`:

```c
#define BEACON_1_DETECTOR_AD_PIN AD_PORTV3
#define BEACON_2_DETECTOR_AD_PIN AD_PORTV5
#define BEACON_ADC_PRINT_DIVIDER 100
```
