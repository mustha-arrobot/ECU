#include "throttle.h"
#include "config.h"


/*==========================================================
 * CCPM THROTTLE
 *==========================================================
 *
 * RB0 / INT0
 *
 * Timer3 measures HIGH pulse width.
 *
 * 20 MHz clock
 * Fosc/4 = 5 MHz
 * Timer3 prescaler = 1:8
 * Timer3 tick = 1.6 us
 *
 * 1000 us = 0%
 * 1500 us = 50%
 * 2000 us = 100%
 *==========================================================*/


/*==========================================================
 * FILTER SETTINGS
 *==========================================================*/

/*
 * Small pulse deadband.
 *
 * Changes smaller than this are ignored.
 *
 * 3 us is approximately:
 *
 * 3 / 10 = 0.3%
 *
 * This removes tiny measurement fluctuations.
 */
#define THROTTLE_DEADBAND_US     4U


/*
 * Lightweight filter.
 *
 * filtered =
 *
 *     (old * 3 + new) / 4
 *
 * This is intentionally light.
 *
 * It does NOT create the large delay
 * of the previous filter.
 */
#define FILTER_OLD_WEIGHT        3UL
#define FILTER_NEW_WEIGHT        1UL
#define FILTER_DIVISOR           4UL


/*==========================================================
 * VARIABLES
 *==========================================================*/

static volatile uint16_t throttlePulseUs = 0;

static volatile uint8_t throttleMeasuring = 0;


/*
 * Last accepted pulse.
 */
static uint16_t throttleStablePulseUs = 0;


/*
 * Filtered pulse.
 */
static uint16_t throttleFilteredPulseUs = 0;


/*==========================================================
 * INITIALIZATION
 *==========================================================*/

void Throttle_Init(void)
{
    /*
     * RB0 / INT0 input.
     */
    THROTTLE_TRIS = 1;


    /*======================================================
     * TIMER3
     *======================================================*/

    /*
     * Stop Timer3.
     */
    T3CONbits.TMR3ON = 0;


    /*
     * 16-bit Timer3.
     */
    T3CONbits.RD16 = 1;


    /*
     * Prescaler = 1:8.
     */
    T3CONbits.T3CKPS1 = 1;
    T3CONbits.T3CKPS0 = 1;


    /*
     * Internal clock = Fosc/4.
     */
    T3CONbits.TMR3CS = 0;


    /*
     * Timer3 interrupt disabled.
     */
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 0;


    /*
     * Clear Timer3.
     */
    TMR3H = 0;
    TMR3L = 0;


    /*======================================================
     * INT0
     *======================================================*/

    /*
     * First edge = rising.
     */
    THROTTLE_INT_EDGE = 1;


    /*
     * Clear INT0 flag.
     */
    THROTTLE_INT_FLAG = 0;


    /*
     * Enable INT0.
     */
    THROTTLE_INT_ENABLE = 1;


    /*======================================================
     * VARIABLES
     *======================================================*/

    throttlePulseUs = 0;

    throttleStablePulseUs = 0;

    throttleFilteredPulseUs = 0;

    throttleMeasuring = 0;
}


/*==========================================================
 * INT0 HANDLER
 *==========================================================*/

void Throttle_INT0_Handler(void)
{
    uint16_t ticks;
    uint16_t pulse;


    /*======================================================
     * RISING EDGE
     *======================================================*/

    if (THROTTLE_PORT)
    {
        /*
         * Stop Timer3.
         */
        T3CONbits.TMR3ON = 0;


        /*
         * Clear Timer3.
         */
        TMR3H = 0;
        TMR3L = 0;


        /*
         * Start Timer3.
         */
        T3CONbits.TMR3ON = 1;


        /*
         * Start measuring.
         */
        throttleMeasuring = 1;


        /*
         * Next edge = falling.
         */
        THROTTLE_INT_EDGE = 0;
    }


    /*======================================================
     * FALLING EDGE
     *======================================================*/

    else
    {
        if (throttleMeasuring)
        {
            /*
             * Stop Timer3.
             */
            T3CONbits.TMR3ON = 0;


            /*
             * Read Timer3.
             */
            {
                uint8_t lowByte;
                uint8_t highByte;

                lowByte = TMR3L;
                highByte = TMR3H;

                ticks =
                    ((uint16_t)highByte << 8)
                    |
                    lowByte;
            }


            /*
             * Convert ticks to microseconds.
             *
             * 1 tick = 1.6 us
             *
             * pulse = ticks * 8 / 5
             */
            pulse =
                (uint16_t)
                (
                    ((uint32_t)ticks *
                     THROTTLE_TIMER_TICK_NUM)
                    /
                    THROTTLE_TIMER_TICK_DEN
                );


            /*
             * Accept valid CCPM pulse.
             */
            if (
                (pulse >= THROTTLE_VALID_MIN_US) &&
                (pulse <= THROTTLE_VALID_MAX_US)
               )
            {
                throttlePulseUs = pulse;
            }


            /*
             * Measurement finished.
             */
            throttleMeasuring = 0;


            /*
             * Next edge = rising.
             */
            THROTTLE_INT_EDGE = 1;
        }
        else
        {
            /*
             * Invalid falling edge.
             *
             * Return to rising edge.
             */
            THROTTLE_INT_EDGE = 1;
        }
    }
}


/*==========================================================
 * THROTTLE TASK
 *==========================================================*/

void Throttle_Task(void)
{
    /*
     * No timing-dependent work here.
     *
     * Pulse measurement is performed
     * completely inside INT0.
     */
}


/*==========================================================
 * READ PULSE WIDTH
 *==========================================================*/

uint16_t Throttle_ReadPulseWidth(void)
{
    uint16_t pulse;


    /*
     * Disable INT0 while copying 16-bit value.
     */
    THROTTLE_INT_ENABLE = 0;


    pulse = throttlePulseUs;


    /*
     * Re-enable INT0.
     */
    THROTTLE_INT_ENABLE = 1;


    /*
     * First valid pulse.
     */
    if (throttleStablePulseUs == 0)
    {
        throttleStablePulseUs = pulse;

        throttleFilteredPulseUs = pulse;

        return pulse;
    }


    /*
     * Small changes are ignored.
     */
    if (pulse > throttleStablePulseUs)
    {
        if (
            (pulse - throttleStablePulseUs)
            <= THROTTLE_DEADBAND_US
           )
        {
            pulse = throttleStablePulseUs;
        }
    }
    else
    {
        if (
            (throttleStablePulseUs - pulse)
            <= THROTTLE_DEADBAND_US
           )
        {
            pulse = throttleStablePulseUs;
        }
    }


    /*
     * Update stable value.
     */
    throttleStablePulseUs = pulse;


    /*
     * Lightweight filter.
     */
    throttleFilteredPulseUs =
        (uint16_t)
        (
            (
                ((uint32_t)
                 throttleFilteredPulseUs *
                 FILTER_OLD_WEIGHT)
                +
                ((uint32_t)
                 pulse *
                 FILTER_NEW_WEIGHT)
            )
            /
            FILTER_DIVISOR
        );


    return throttleFilteredPulseUs;
}


/*==========================================================
 * READ THROTTLE %
 *==========================================================*/

uint8_t Throttle_ReadPercent(void)
{
    uint16_t pulse;

    uint32_t percentage;


    /*
     * Get filtered pulse.
     */
    pulse = Throttle_ReadPulseWidth();


    /*
     * No pulse.
     */
    if (pulse == 0)
    {
        return 0;
    }


    /*
     * Minimum.
     */
    if (pulse <= THROTTLE_MIN_US)
    {
        return 0;
    }


    /*
     * Maximum.
     */
    if (pulse >= THROTTLE_MAX_US)
    {
        return 100;
    }


    /*
     * Convert pulse to percentage.
     */
    percentage =
        (
            (uint32_t)
            (pulse - THROTTLE_MIN_US)
            * 100UL
        )
        /
        (
            THROTTLE_MAX_US -
            THROTTLE_MIN_US
        );


    return (uint8_t)percentage;
}
