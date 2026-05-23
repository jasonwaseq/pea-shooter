/*
 * ShooterEventChecker.c
 *
 * The first shooter project only starts the motor from main.c. This hook keeps
 * the ES Framework event-checker list valid for later sensors or buttons.
 */

#include "ShooterEventChecker.h"

#include "BOARD.h"

uint8_t CheckShooterEvents(void)
{
    return FALSE;
}
