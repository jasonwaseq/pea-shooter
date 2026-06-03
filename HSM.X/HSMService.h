/*
 * HSMService.h
 *
 * Public service interface for the HSM beacon alignment project.
 */

#ifndef HSM_SERVICE_H
#define HSM_SERVICE_H

#include <stdint.h>

#include "ES_Configure.h"
#include "ES_Events.h"

uint8_t InitHSMService(uint8_t priority);
uint8_t PostHSMService(ES_Event thisEvent);
ES_Event RunHSMService(ES_Event thisEvent);
uint8_t HSM_TapeEventsEnabled(void);

#endif /* HSM_SERVICE_H */
