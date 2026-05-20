/*
 * HC-SR04 ultrasonic ping sensor demo for the Uno32 / PIC32MX320F128H board.
 *
 * Wiring:
 *   HC-SR04 VCC  -> board 5V
 *   HC-SR04 GND  -> board GND
 *   HC-SR04 TRIG -> PortY-03 / RD11
 *   HC-SR04 ECHO -> PortY-04 / RD3 through a 5V-to-3.3V divider
 */

#include <BOARD.h>
#include <IO_Ports.h>
#include <stdint.h>
#include <stdio.h>
#include <xc.h>

#define HCSR04_PORT PORTY
#define HCSR04_TRIG_PIN PIN3
#define HCSR04_ECHO_PIN PIN4

#define HCSR04_TIMEOUT_US 60000u
#define PING_INTERVAL_US 100000u

typedef enum {
    HCSR04_OK = 0,
    HCSR04_ECHO_STUCK_HIGH_BEFORE_TRIGGER,
    HCSR04_TIMEOUT_WAITING_FOR_RISE,
    HCSR04_TIMEOUT_WAITING_FOR_FALL
} HCSR04_Status;

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
    return (IO_PortsReadPort(HCSR04_PORT) & HCSR04_ECHO_PIN) != 0;
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
    IO_PortsSetPortOutputs(HCSR04_PORT, HCSR04_TRIG_PIN);
    IO_PortsSetPortInputs(HCSR04_PORT, HCSR04_ECHO_PIN);
    IO_PortsClearPortBits(HCSR04_PORT, HCSR04_TRIG_PIN);
    DelayUs(10000u);
}

static HCSR04_Status HCSR04_ReadEchoUs(uint32_t *echoUs)
{
    uint32_t start;
    uint32_t elapsedTicks;

    if (!WaitForEchoState(FALSE, HCSR04_TIMEOUT_US)) {
        return HCSR04_ECHO_STUCK_HIGH_BEFORE_TRIGGER;
    }

    IO_PortsClearPortBits(HCSR04_PORT, HCSR04_TRIG_PIN);
    DelayUs(2u);
    IO_PortsSetPortBits(HCSR04_PORT, HCSR04_TRIG_PIN);
    DelayUs(10u);
    IO_PortsClearPortBits(HCSR04_PORT, HCSR04_TRIG_PIN);

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

int main(void)
{
    uint32_t echoUs;
    HCSR04_Status status;

    BOARD_Init();
    HCSR04_Init();

    printf("\r\nHC-SR04 ultrasonic ping demo\r\n");
    printf("TRIG: PortY-03 / RD11, ECHO: PortY-04 / RD3\r\n");
    printf("Use a voltage divider on ECHO before it reaches the PIC32 input.\r\n\r\n");

    while (1) {
        status = HCSR04_ReadEchoUs(&echoUs);
        if (status == HCSR04_OK) {
            const uint32_t mm = EchoUsToMillimeters(echoUs);
            const uint32_t tenthsInch = (mm * 10u + 127u) / 254u;

            printf("Echo %lu us  Distance %lu.%lu cm  %lu.%lu in\r\n",
                    echoUs,
                    mm / 10u, mm % 10u,
                    tenthsInch / 10u, tenthsInch % 10u);
        } else if (status == HCSR04_ECHO_STUCK_HIGH_BEFORE_TRIGGER) {
            printf("ECHO is already high before trigger: unplug ECHO from PortY-04 and check for a short to 3.3V/5V. ECHO idle=%u\r\n",
                    EchoIsHigh());
        } else if (status == HCSR04_TIMEOUT_WAITING_FOR_RISE) {
            printf("No echo rising edge: sensor did not answer, or ECHO is not reaching PortY-04. ECHO idle=%u\r\n",
                    EchoIsHigh());
        } else {
            printf("ECHO rose but stayed high too long: no close target, weak reflection, or ECHO line problem. ECHO idle=%u\r\n",
                    EchoIsHigh());
        }

        DelayUs(PING_INTERVAL_US);
    }
}
