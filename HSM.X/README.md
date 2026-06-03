# HSM.X

MPLAB X ES Framework project for the HOME hierarchical state machine.

On `ES_INIT`, the top-level HSM enters `HOME` and runs these substates in order:

1. `SWEEP_DATA`: slow non-blocking tank turn for one calibrated 360 degree sweep while sampling Beacon Detector 1 on `AD_PORTV3`.
2. `LOCK_ON`: slow tank turn until the filtered beacon ADC reading reaches the stored sweep maximum minus the configured offset.
3. `ON_FIELD`: reverse until all three tape sensors are off tape.
4. `LOCATE_TAPE`: drive forward, with `FIX_RIGHT` and `FIX_LEFT` correction substates, until all three tape sensors detect tape.
5. `TURN_TO_CORNER`: tank turn until only left and middle tape sensors detect tape.
6. `DRIVE_TO_CORNER`: drive forward on left+middle tape, with `FIX_RIGHT` and `FIX_LEFT` correction substates, until all three tape sensors detect tape.
7. `TURN_TO_EDGE`: tank turn until only left and middle tape sensors detect tape, then stop and emit `HOME_COMPLETE`.

Default hardware assumptions:

- Beacon detector 1 analog output: `AD_PORTV3`.
- Left, middle, and right tape sensors match `Movement.X/peashooter.c`.
- Left and right motor PWM/direction pins match `Movement.X/peashooter.c`.

Tune these constants in `HSMService.c`:

- `HSM_FULL_ROTATION_TIME_MS`
- `HSM_BEACON_AVG_WINDOW`
- `HSM_BEACON_LOCK_OFFSET`
- `HSM_SWEEP_TURN_POWER`
- `HSM_LOCK_TURN_POWER`
- `HSM_TAPE_TURN_POWER`
- `HSM_DRIVE_POWER`
- `HSM_CORRECT_POWER`
