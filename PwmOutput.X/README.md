# Indexer PWM Project

MPLAB X project scaffold for running the indexer PWM output with the ES
Framework on the PIC32MX320F128H / Uno32 I/O protection board.

Default output:

- PWM pin: `PWM_PORTY12` (`PortY-12`)
- Frequency: `PWM_1KHZ`
- Startup duty cycle: `1000 / 1000`, or 100.0%
- Startup time: `500 ms`
- Running duty cycle: `250 / 1000`, or 25.0%

Default beacon analog input:

- Beacon detector voltage output: `AD_PORTV3` (`PortV-3`)
- ADC range: `0..1023`
- Detection is based on ADC value above the calibrated no-beacon baseline, not
  an absolute ADC number.
- Startup calibration uses the first `BEACON_CALIBRATION_SAMPLES` readings as
  the no-beacon baseline. Keep the beacon away from the detector while the board
  boots.
- After calibration, the baseline is held fixed so normal no-beacon variation is
  not mistaken for a slowly approaching beacon.
- Detection uses a confidence score, so repeated weak samples above the detect
  threshold accumulate instead of being reset by one low/noisy sample.
- Current far-field starting point: samples above about `20` counts over
  baseline increase confidence quickly, and samples below about `6` counts
  reduce it slowly.

`PortX-3` is a digital I/O pin, not an ADC input. If your detector outputs a
voltage proportional to beacon strength, connect it to one of the analog `V` or
`W` pins and update `IndexerEventChecker.h` if you do not use `V3`.

Use the serial output to compare `raw`, `filt`, `base`, and `excess`.
`excess = filt - base` is the value used for detection. If close and far
positions only differ by a few ADC counts in `excess`, increase detector gain or
lower `BEACON_DETECT_DELTA_THRESHOLD`.

The local PWM module supports these board outputs:

- `PWM_PORTZ06`
- `PWM_PORTY12`
- `PWM_PORTY10`
- `PWM_PORTY04`
- `PWM_PORTX11`

To change the output pin, frequency, or duty cycle, edit
`IndexerService.h`.

The public startup function is:

```c
StartIndexerSystem();
```

It posts an `INDEXER_START` event to the service. The service drives the PWM
pin at 100% duty cycle, starts ES timer 0 for 500 ms, and then switches the PWM
pin to 25% duty cycle when that timer expires. `StopIndexerSystem()` posts an
`INDEXER_STOP` event and drops the output to 0%.
