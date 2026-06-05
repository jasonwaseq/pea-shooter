/*
 * File: FollowEdgeSubHSM.h
 * Split from TemplateSubHSM using the ECE118 SubHSM template.
 */

#ifndef FOLLOWEDGESUBHSM_H
#define FOLLOWEDGESUBHSM_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitFollowEdgeSubHSM(void);
ES_Event RunFollowEdgeSubHSM(ES_Event ThisEvent);

#endif /* FOLLOWEDGESUBHSM_H */
