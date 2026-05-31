/*
 * ShooterEventChecker.c
 *
 * The shooter project has no local sensor event checks. Beacon detector logic
 * lives in BeaconDetector.X.
 */

#include "ShooterEventChecker.h"

#include "BOARD.h"

uint8_t InitShooterEventChecker(void)
{
    return TRUE;
}

uint8_t CheckShooterEvents(void)
{
    return FALSE;
}
