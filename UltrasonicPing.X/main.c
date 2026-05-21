/*
 * HC-SR04 ultrasonic ping sensor demo for the Uno32 / PIC32MX320F128H board.
 *
 * Wiring:
 *   HC-SR04 VCC  -> board 5V
 *   HC-SR04 GND  -> board GND
 *   HC-SR04 TRIG -> PortX-10 / RD7
 *   HC-SR04 ECHO -> PortX-11 / RD4 through a 5V-to-3.3V divider
 */

#include <BOARD.h>
#include <IO_Ports.h>
#include <serial.h>
#include <stdint.h>
#include <stdio.h>
#include <xc.h>

#define HCSR04_TRIG_PORT PORTX
#define HCSR04_TRIG_PIN PIN10
#define HCSR04_TRIG_BIT BIT_7
#define HCSR04_TRIG_NAME "PortX-10 / RD7"
#define HCSR04_ECHO_PORT_NAME "PortX-11 / RD4 / J5-01 / chipKIT D33"

#define HCSR04_TRIG_TRIS TRISDbits.TRISD7
#define HCSR04_TRIG_LAT LATDbits.LATD7
#define HCSR04_ECHO_TRIS TRISDbits.TRISD4
#define HCSR04_ECHO_PORT PORTDbits.RD4

#define HCSR04_TIMEOUT_US 60000u
#define PING_INTERVAL_US 100000u

#define DISTANCE_FILTER_WINDOW 8u
#define REFERENCE_SAMPLE_COUNT 16u
#define REFERENCE_MAX_ATTEMPTS 64u
#define REALLY_CLOSE_DELTA_MM 100u
#define REALLY_FAR_DELTA_MM 100u

typedef enum {
    HCSR04_OK = 0,
    HCSR04_ECHO_STUCK_HIGH_BEFORE_TRIGGER,
    HCSR04_TIMEOUT_WAITING_FOR_RISE,
    HCSR04_TIMEOUT_WAITING_FOR_FALL
} HCSR04_Status;

typedef struct {
    uint32_t samples[DISTANCE_FILTER_WINDOW];
    uint32_t sum;
    uint8_t index;
    uint8_t count;
} DistanceFilter;

static DistanceFilter DistanceAverage;
static uint32_t ReferenceDistanceMm;
static uint8_t ReferenceValid;

static inline uint32_t CoreTimerNow(void)
{
    uint32_t count;
    __asm__ volatile("mfc0 %0,$9" : "=r"(count));
    return count;
}

static uint32_t CoreTimerTicksPerUs(void)
{
    return BOARD_GetPBClock() / 1000000u;
}

static void DelayUs(uint32_t delayUs)
{
    const uint32_t targetTicks = delayUs * CoreTimerTicksPerUs();
    const uint32_t start = CoreTimerNow();

    while ((uint32_t)(CoreTimerNow() - start) < targetTicks) {
        ;
    }
}

static uint8_t EchoIsHigh(void)
{
    return HCSR04_ECHO_PORT != 0;
}

static void TriggerLow(void)
{
    IO_PortsClearPortBits(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN);
    LATDCLR = HCSR04_TRIG_BIT;
    HCSR04_TRIG_LAT = 0;
}

static void TriggerHigh(void)
{
    IO_PortsSetPortBits(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN);
    LATDSET = HCSR04_TRIG_BIT;
    HCSR04_TRIG_LAT = 1;
}

static uint8_t WaitForEchoState(uint8_t expectedHigh, uint32_t timeoutUs)
{
    const uint32_t timeoutTicks = timeoutUs * CoreTimerTicksPerUs();
    const uint32_t start = CoreTimerNow();

    while (EchoIsHigh() != expectedHigh) {
        if ((uint32_t)(CoreTimerNow() - start) >= timeoutTicks) {
            return FALSE;
        }
    }
    return TRUE;
}

static void HCSR04_Init(void)
{
    TriggerLow();
    IO_PortsSetPortOutputs(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN);
    HCSR04_TRIG_TRIS = 0;
    HCSR04_ECHO_TRIS = 1;
    TriggerLow();
    DelayUs(10000u);
}

static HCSR04_Status HCSR04_ReadEchoUs(uint32_t *echoUs)
{
    uint32_t start;
    uint32_t elapsedTicks;

    /* If ECHO is still high from a previous bad ping, give it one interval to settle. */
    if (!WaitForEchoState(FALSE, HCSR04_TIMEOUT_US)) {
        return HCSR04_ECHO_STUCK_HIGH_BEFORE_TRIGGER;
    }

    TriggerLow();
    DelayUs(2u);
    TriggerHigh();
    DelayUs(10u);
    TriggerLow();

    if (!WaitForEchoState(TRUE, HCSR04_TIMEOUT_US)) {
        return HCSR04_TIMEOUT_WAITING_FOR_RISE;
    }

    start = CoreTimerNow();
    if (!WaitForEchoState(FALSE, HCSR04_TIMEOUT_US)) {
        return HCSR04_TIMEOUT_WAITING_FOR_FALL;
    }

    elapsedTicks = CoreTimerNow() - start;
    *echoUs = elapsedTicks / CoreTimerTicksPerUs();
    return HCSR04_OK;
}

static uint32_t EchoUsToMillimeters(uint32_t echoUs)
{
    return (echoUs * 343u + 1000u) / 2000u;
}

static void DistanceFilter_Reset(DistanceFilter *filter)
{
    uint8_t i;

    for (i = 0; i < DISTANCE_FILTER_WINDOW; i++) {
        filter->samples[i] = 0;
    }
    filter->sum = 0;
    filter->index = 0;
    filter->count = 0;
}

static void DistanceFilter_Add(DistanceFilter *filter, uint32_t distanceMm)
{
    if (filter->count < DISTANCE_FILTER_WINDOW) {
        filter->samples[filter->index] = distanceMm;
        filter->sum += distanceMm;
        filter->count++;
    } else {
        filter->sum -= filter->samples[filter->index];
        filter->samples[filter->index] = distanceMm;
        filter->sum += distanceMm;
    }

    filter->index++;
    if (filter->index >= DISTANCE_FILTER_WINDOW) {
        filter->index = 0;
    }
}

static uint8_t DistanceFilter_GetAverage(const DistanceFilter *filter, uint32_t *averageMm)
{
    if (filter->count == 0) {
        return FALSE;
    }

    *averageMm = (filter->sum + (filter->count / 2u)) / filter->count;
    return TRUE;
}

static uint8_t HCSR04_ReadDistanceMm(uint32_t *distanceMm)
{
    uint32_t echoUs;

    if (HCSR04_ReadEchoUs(&echoUs) != HCSR04_OK) {
        return FALSE;
    }

    *distanceMm = EchoUsToMillimeters(echoUs);
    return TRUE;
}

static uint8_t HCSR04_ZeroReference(uint32_t *referenceMm)
{
    uint8_t samples = 0;
    uint8_t attempts = 0;
    DistanceFilter referenceFilter;

    DistanceFilter_Reset(&referenceFilter);

    while ((samples < REFERENCE_SAMPLE_COUNT) && (attempts < REFERENCE_MAX_ATTEMPTS)) {
        uint32_t distanceMm;

        attempts++;
        if (HCSR04_ReadDistanceMm(&distanceMm)) {
            DistanceFilter_Add(&referenceFilter, distanceMm);
            samples++;
        }
        DelayUs(PING_INTERVAL_US);
    }

    return DistanceFilter_GetAverage(&referenceFilter, referenceMm);
}

static uint8_t HCSR04_IsReallyClose(uint32_t distanceMm, uint32_t referenceMm)
{
    if (referenceMm <= REALLY_CLOSE_DELTA_MM) {
        return FALSE;
    }
    return (distanceMm + REALLY_CLOSE_DELTA_MM) <= referenceMm;
}

static uint8_t HCSR04_IsReallyFar(uint32_t distanceMm, uint32_t referenceMm)
{
    return distanceMm >= (referenceMm + REALLY_FAR_DELTA_MM);
}

static const char *HCSR04_ObjectStateName(uint32_t distanceMm, uint32_t referenceMm)
{
    if (HCSR04_IsReallyClose(distanceMm, referenceMm)) {
        return "really_close";
    }
    if (HCSR04_IsReallyFar(distanceMm, referenceMm)) {
        return "really_far";
    }
    return "near_reference";
}

static void PrintDistanceLine(uint32_t rawMm, uint32_t averageMm)
{
    const uint32_t averageInchesTenths = (averageMm * 100u + 127u) / 254u;

    if (ReferenceValid) {
        printf("raw_cm=%lu.%lu avg_cm=%lu.%lu avg_in=%lu.%lu ref_cm=%lu.%lu state=%s\r\n",
                rawMm / 10u, rawMm % 10u,
                averageMm / 10u, averageMm % 10u,
                averageInchesTenths / 10u, averageInchesTenths % 10u,
                ReferenceDistanceMm / 10u, ReferenceDistanceMm % 10u,
                HCSR04_ObjectStateName(averageMm, ReferenceDistanceMm));
    } else {
        printf("raw_cm=%lu.%lu avg_cm=%lu.%lu avg_in=%lu.%lu state=unzeroed\r\n",
                rawMm / 10u, rawMm % 10u,
                averageMm / 10u, averageMm % 10u,
                averageInchesTenths / 10u, averageInchesTenths % 10u);
    }
}

static void TryZeroReference(void)
{
    uint32_t referenceMm;

    printf("zeroing_reference samples=%u\r\n", REFERENCE_SAMPLE_COUNT);
    if (HCSR04_ZeroReference(&referenceMm)) {
        ReferenceDistanceMm = referenceMm;
        ReferenceValid = TRUE;
        DistanceFilter_Reset(&DistanceAverage);
        printf("reference_cm=%lu.%lu\r\n", ReferenceDistanceMm / 10u, ReferenceDistanceMm % 10u);
    } else {
        ReferenceValid = FALSE;
        printf("reference_error=no_valid_samples\r\n");
    }
}

int main(void)
{
    DistanceFilter_Reset(&DistanceAverage);

    BOARD_Init();
    HCSR04_Init();

    printf("\r\nHC-SR04 ultrasonic ping demo\r\n");
    printf("TRIG: %s\r\n", HCSR04_TRIG_NAME);
    printf("ECHO: %s\r\n", HCSR04_ECHO_PORT_NAME);
    printf("Use a voltage divider on ECHO before it reaches the PIC32 input.\r\n\r\n");
    printf("Type z in the serial terminal to zero the current object as reference.\r\n");

    TryZeroReference();

    while (1) {
        const char command = GetChar();
        uint32_t rawMm;
        uint32_t averageMm;

        if ((command == 'z') || (command == 'Z')) {
            TryZeroReference();
        }

        if (HCSR04_ReadDistanceMm(&rawMm)) {
            DistanceFilter_Add(&DistanceAverage, rawMm);
            if (DistanceFilter_GetAverage(&DistanceAverage, &averageMm)) {
                PrintDistanceLine(rawMm, averageMm);
            }
        } else {
            printf("distance_cm=ERR no_valid_echo\r\n");
        }

        DelayUs(PING_INTERVAL_US);
    }
}
