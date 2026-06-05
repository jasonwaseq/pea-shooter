/*
 * File: FollowTapeSubHSM.h
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#ifndef FOLLOWTAPESUBHSM_H
#define FOLLOWTAPESUBHSM_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitFollowTapeSubHSM(void);
ES_Event RunFollowTapeSubHSM(ES_Event ThisEvent);

#endif /* FOLLOWTAPESUBHSM_H */
