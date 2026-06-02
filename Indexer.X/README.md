# Indexer.X

MPLAB X project scaffold for running the indexer output with the ES
Framework on the PIC32MX320F128H / Uno32 I/O protection board.

Default output:

- RC_Servo pin: `RC_PORTZ04` (`PortZ-04`)
- Pulse period: RC_Servo default, `20 ms`
- Startup pulse: `2000 us`
- Startup time: `500 ms`
- Running pulse: `1250 us`

Beacon detector ADC logic is intentionally kept in `BeaconDetector.X`.

To change the output pin or pulse widths, edit
`IndexerService.h`.

The public startup function is:

```c
StartIndexerSystem();
```

It posts an `INDEXER_START` event to the service. The service drives the
RC_Servo output with a 2000 us pulse, starts ES timer 0 for 500 ms, and then
switches to a 1250 us pulse when that timer expires. `StopIndexerSystem()`
posts an `INDEXER_STOP` event and removes the RC_Servo output.
