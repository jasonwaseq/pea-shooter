# HSM.X

MPLAB X ES Framework project for startup beacon alignment.

On `ES_INIT`, the top-level HSM enters a beacon-alignment sub-HSM:

1. Tank-turn left for one calibrated 360 degree rotation.
2. While rotating, sample the beacon detector ADC pin using the same raw ADC read pattern as `BeaconDetector.X`.
3. Store the maximum ADC value and the elapsed time in the 360 degree sweep when it occurred.
4. Tank-turn left again for that stored elapsed time so the robot returns to the strongest beacon heading.
5. Stop both drive motors.

Default hardware assumptions:

- Beacon detector analog output: `AD_PORTV3`.
- Left motor PWM/direction pins match `Movement.X/peashooter.c`.
- Right motor PWM/direction pins match `Movement.X/peashooter.c`.

Tune these constants in `HSMService.c`:

- `HSM_TANK_TURN_POWER`
- `HSM_FULL_ROTATION_TIME_MS`
