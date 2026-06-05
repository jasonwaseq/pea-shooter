/*
 * File: MainHSM.h
 */

#ifndef MAIN_HSM_H
#define MAIN_HSM_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitMainHSM(uint8_t Priority);
uint8_t PostMainHSM(ES_Event ThisEvent);
ES_Event RunMainHSM(ES_Event ThisEvent);

/* The shared ES_Timers.c still names PostTemplateHSM directly for timers. */
#define PostTemplateHSM PostMainHSM

/* AvoidObstacleSubHSM calls this after turning so ping can re-zero.
 * Keep this prototype here only if the function is implemented somewhere
 * in your project, usually SensorsEventChecker.c or MainHSM.c.
 */
void TemplateResetPingReference(void);

#endif /* MAIN_HSM_H */
