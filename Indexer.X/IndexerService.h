/*
 * IndexerService.h
 */

#ifndef INDEXER_SERVICE_H
#define INDEXER_SERVICE_H

#include <stdint.h>

#include "ES_Configure.h"
#include "ES_Events.h"
#include "pwm.h"

/*
 * PWM-capable board outputs from include/pwm.h:
 *   PWM_PORTZ06, PWM_PORTY12, PWM_PORTY10, PWM_PORTY04, PWM_PORTX11
 */
#ifndef INDEXER_PWM_PIN
#define INDEXER_PWM_PIN PWM_PORTZ06
#endif

#ifndef INDEXER_PWM_PIN_NAME
#define INDEXER_PWM_PIN_NAME "PortZ-06"
#endif

#ifndef INDEXER_PWM_FREQUENCY
#define INDEXER_PWM_FREQUENCY PWM_1KHZ
#endif

#ifndef INDEXER_STARTUP_DUTY_CYCLE
#define INDEXER_STARTUP_DUTY_CYCLE MAX_PWM
#endif

#ifndef INDEXER_RUN_DUTY_CYCLE
#define INDEXER_RUN_DUTY_CYCLE 250
#endif

#ifndef INDEXER_STARTUP_TIME_MS
#define INDEXER_STARTUP_TIME_MS 500
#endif

uint8_t InitIndexerService(uint8_t priority);
uint8_t PostIndexerService(ES_Event thisEvent);
ES_Event RunIndexerService(ES_Event thisEvent);

uint8_t StartIndexerSystem(void);
uint8_t StopIndexerSystem(void);

#endif /* INDEXER_SERVICE_H */
