#ifndef HSM_EVENT_CHECKERS_H
#define HSM_EVENT_CHECKERS_H

#include "BeaconEventChecker.h"
#include "HSMService.h"
#include "SensorsEventChecker.h"

static uint8_t CheckHSMEvents(void) {
    if (HSM_BeaconAlignFastStopEnabled() == TRUE) {
        if (CheckBeaconDetector() == TRUE) {
            return TRUE;
        }
    }

    if (TemplateCheckBattery() == TRUE) {
        return TRUE;
    }

    if (TemplateCheckSwitch() == TRUE) {
        return TRUE;
    }

    if (HSM_TapeEventsEnabled() == TRUE) {
        if (TemplateCheckTape() == TRUE) {
            return TRUE;
        }
    }

    return CheckBeaconDetector();
}

#endif /* HSM_EVENT_CHECKERS_H */
