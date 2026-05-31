# Indexer.X

MPLAB X project scaffold for running the indexer PWM output with the ES
Framework on the PIC32MX320F128H / Uno32 I/O protection board.

Default output:

- PWM pin: `PWM_PORTZ06` (`PortZ-06`)
- Frequency: `PWM_1KHZ`
- Startup duty cycle: `1000 / 1000`, or 100.0%
- Startup time: `500 ms`
- Running duty cycle: `250 / 1000`, or 25.0%

Beacon detector ADC logic is intentionally kept in `BeaconDetector.X`.

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
