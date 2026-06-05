# HSM.X

MPLAB X ES Framework project for the HOME hierarchical state machine.

On `ES_INIT`, the top-level HSM enters `HOME` and runs these substates in order:

1. `SWEEP_DATA`: keep motors off for `HSM_SWEEP_SETTLE_TIME_MS` while sampling Beacon Detector 1 on `AD_PORTV3`; if beacon is detected during that wait, go straight to `ON_FIELD`, otherwise start the sweep tank turn.
2. `LOCK_ON`: slow tank turn until the raw beacon ADC value reaches `HSM_BEACON_DETECT_MIN`.
3. `ON_FIELD`: reverse until all three tape sensors are off tape.
4. `LOCATE_TAPE`: drive forward until left, middle, and right tape sensors all detect tape, then transition to `TURN_TO_CORNER` on the tape change.
5. `TURN_TO_CORNER`: timed 180 degree tank turn for just over two seconds, ignoring tape events while turning.
6. `DriveState`: drive forward until a right tape event enters `HitLeftState`, a left tape event enters `HitRightState`, or the W4 switch sensor posts `BUMPER_TRIPPED` and enters `BumpState`.
7. `HitLeftState`: keep the left motor at drive speed and boost the right motor in half-second pulses until all tape sensors are off, then return to `DriveState`. A W4 switch sensor trip interrupts into `BumpState`.
8. `HitRightState`: keep the right motor at drive speed and boost the left motor in half-second pulses until all tape sensors are off, then return to `DriveState`. A W4 switch sensor trip interrupts into `BumpState`.
9. `BumpState`: reverse both motors for one second, tank turn right for one second, drive forward for 1.5 seconds, tank turn left for one second, then return to `DriveState`.
10. `TRAVEL`: lower motor power and follow tape slowly. If one or zero tape sensors detect tape, stop immediately. If the left sensor comes off tape, drive the right motor to correct; if the right sensor comes off tape, drive the left motor to correct.

Default hardware assumptions:

- Beacon detector 1 analog output: `AD_PORTV3`.
- Left, middle, and right tape sensors match `Movement.X/peashooter.c`.
- Left and right motor PWM/direction pins match `Movement.X/peashooter.c`.

Tune these constants in `HSMService.c`:

- `HSM_FULL_ROTATION_TIME_MS`
- `HSM_SWEEP_SETTLE_TIME_MS`
- `HSM_BEACON_AVG_WINDOW`
- `HSM_BEACON_DETECT_MIN`
- `HSM_SWEEP_TURN_POWER`
- `HSM_LOCK_TURN_POWER`
- `HSM_TURN_TO_CORNER_POWER`
- `HSM_TURN_TO_CORNER_180_TIME_MS`
- `HSM_DRIVE_POWER`
- `HSM_HIT_PIVOT_TIME_MS`
- `HSM_HIT_FAST_POWER`
- `HSM_HIT_SLOW_POWER`
- `HSM_BUMP_REVERSE_TIME_MS`
- `HSM_BUMP_TURN_TIME_MS`
- `HSM_BUMP_FORWARD_TIME_MS`
- `HSM_BUMP_POWER`
- `HSM_TRAVEL_POWER`
- `HSM_TRAVEL_CORRECT_POWER`
