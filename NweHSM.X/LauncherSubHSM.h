/*
 * File: LauncherSubHSM.h
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#ifndef LAUNCHERSUBHSM_H
#define LAUNCHERSUBHSM_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitLauncherSubHSM(void);
ES_Event RunLauncherSubHSM(ES_Event ThisEvent);

#endif /* LAUNCHERSUBHSM_H */
