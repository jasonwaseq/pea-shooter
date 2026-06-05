# Indexer.X

MPLAB X project scaffold for running the indexer output with the ES
Framework on the PIC32MX320F128H / Uno32 I/O protection board.

Default output:

- Hardware PWM pin: `PWM_PORTZ06` (`PortZ-06`)
- PWM frequency: `PWM_1KHZ`
- Startup duty: `1000 / 1000`, or 100.0%
- Startup time: `1000 ms`
- Running duty: `500 / 1000`, or 50.0%

Beacon detector ADC logic is intentionally kept in `BeaconDetector.X`.

To change the output pin or duty values, edit `IndexerService.h`.

The public startup function is:

```c
StartIndexerSystem();
```

It posts an `INDEXER_START` event to the service. The service drives the
hardware PWM output at 100%, starts ES timer 0 for 1000 ms, and then switches
to 50% duty when that timer expires. `StopIndexerSystem()` posts an
`INDEXER_STOP` event and sets the PWM duty to 0.
