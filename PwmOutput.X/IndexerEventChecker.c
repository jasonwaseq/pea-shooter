/*
 * IndexerEventChecker.c
 *
 * Reads the beacon detector analog voltage and posts strength updates.
 */

#include "IndexerEventChecker.h"

#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "IndexerService.h"

static uint16_t LastBeaconReading;
static uint16_t RawBeaconReading;
static uint16_t FilteredBeaconReading;
static uint16_t BaselineBeaconReading;
static uint16_t ExcessBeaconReading;
static uint16_t ReportSampleCounter;
static uint32_t CalibrationAccumulator;
static uint16_t CalibrationSampleCount;
static uint8_t BeaconScore;
static uint8_t BeaconCalibrated;
static uint8_t LastBeaconPresent;

static uint16_t ReadBeaconStrength(void)
{
    return AD_ReadADPin(BEACON_DETECTOR_AD_PIN);
}

static uint8_t IsBeaconPresent(void)
{
    if (LastBeaconPresent == TRUE) {
        return (BeaconScore > BEACON_SCORE_LOST) ? TRUE : FALSE;
    }
    return (BeaconScore >= BEACON_SCORE_DETECT) ? TRUE : FALSE;
}

static void UpdateExcess(void)
{
    ExcessBeaconReading = (FilteredBeaconReading > BaselineBeaconReading)
            ? (FilteredBeaconReading - BaselineBeaconReading)
            : 0;
}

uint8_t InitIndexerEventChecker(void)
{
    if ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
        if (AD_AddPins(BEACON_DETECTOR_AD_PIN) != SUCCESS) {
            return FALSE;
        }
        while ((AD_ActivePins() & BEACON_DETECTOR_AD_PIN) == 0) {
            ;
        }
    }

    RawBeaconReading = ReadBeaconStrength();
    if (RawBeaconReading == (uint16_t) ERROR) {
        return FALSE;
    }

    FilteredBeaconReading = RawBeaconReading;
    BaselineBeaconReading = RawBeaconReading;
    ExcessBeaconReading = 0;
    LastBeaconReading = ExcessBeaconReading;
    ReportSampleCounter = 0;
    CalibrationAccumulator = 0;
    CalibrationSampleCount = 0;
    BeaconScore = 0;
    BeaconCalibrated = FALSE;
    LastBeaconPresent = FALSE;
    return TRUE;
}

uint8_t CheckIndexerEvents(void)
{
    ES_Event thisEvent;
    uint16_t readingDelta;
    uint8_t currentBeaconPresent;

    if (AD_IsNewDataReady() != TRUE) {
        return FALSE;
    }

    RawBeaconReading = ReadBeaconStrength();
    if (RawBeaconReading == (uint16_t) ERROR) {
        return FALSE;
    }

    FilteredBeaconReading = FilteredBeaconReading
            + ((int16_t) RawBeaconReading - (int16_t) FilteredBeaconReading)
            / (1 << BEACON_FILTER_SHIFT);

    if (BeaconCalibrated != TRUE) {
        CalibrationAccumulator += RawBeaconReading;
        CalibrationSampleCount++;

        if (CalibrationSampleCount >= BEACON_CALIBRATION_SAMPLES) {
            BaselineBeaconReading =
                    CalibrationAccumulator / BEACON_CALIBRATION_SAMPLES;
            FilteredBeaconReading = BaselineBeaconReading;
            RawBeaconReading = BaselineBeaconReading;
            ExcessBeaconReading = 0;
            LastBeaconReading = 0;
            BeaconScore = 0;
            BeaconCalibrated = TRUE;

            thisEvent.EventType = BEACON_SAMPLE;
            thisEvent.EventParam = RawBeaconReading;
            PostIndexerService(thisEvent);
            return TRUE;
        }
        return FALSE;
    }

    UpdateExcess();

    if (ExcessBeaconReading >= BEACON_DETECT_DELTA_THRESHOLD) {
        if (BeaconScore < (BEACON_SCORE_MAX - 4)) {
            BeaconScore += 4;
        } else {
            BeaconScore = BEACON_SCORE_MAX;
        }
    } else if (ExcessBeaconReading <= BEACON_LOST_DELTA_THRESHOLD) {
        if (BeaconScore > 0) {
            BeaconScore--;
        } else {
            BeaconScore = 0;
        }
    }

    currentBeaconPresent = IsBeaconPresent();
    if (currentBeaconPresent != LastBeaconPresent) {
        LastBeaconPresent = currentBeaconPresent;
        LastBeaconReading = ExcessBeaconReading;
        thisEvent.EventType = currentBeaconPresent ? BEACON_DETECTED : BEACON_LOST;
        thisEvent.EventParam = ExcessBeaconReading;
        PostIndexerService(thisEvent);
        return TRUE;
    }

    ReportSampleCounter++;
    if (ReportSampleCounter >= BEACON_REPORT_INTERVAL_SAMPLES) {
        ReportSampleCounter = 0;
        thisEvent.EventType = BEACON_SAMPLE;
        thisEvent.EventParam = RawBeaconReading;
        PostIndexerService(thisEvent);
        return TRUE;
    }

    readingDelta = (ExcessBeaconReading > LastBeaconReading)
            ? (ExcessBeaconReading - LastBeaconReading)
            : (LastBeaconReading - ExcessBeaconReading);
    if (readingDelta >= BEACON_STRENGTH_DELTA) {
        LastBeaconReading = ExcessBeaconReading;
        thisEvent.EventType = BEACON_STRENGTH_CHANGED;
        thisEvent.EventParam = ExcessBeaconReading;
        PostIndexerService(thisEvent);
        return TRUE;
    }

    return FALSE;
}

uint16_t BeaconRawReading(void)
{
    return RawBeaconReading;
}

uint16_t BeaconFilteredReading(void)
{
    return FilteredBeaconReading;
}

uint16_t BeaconBaselineReading(void)
{
    return BaselineBeaconReading;
}

uint16_t BeaconExcessReading(void)
{
    return ExcessBeaconReading;
}

uint8_t BeaconConfidenceScore(void)
{
    return BeaconScore;
}
