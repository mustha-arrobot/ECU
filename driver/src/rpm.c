#include <xc.h>
#include <stdint.h>

#include "rpm.h"

/*
 * ==========================================================
 * RPM SENSOR
 * ==========================================================
 *
 * MCU       : PIC18LF2520
 * RPM Input : RB2 / INT2
 *
 * 1 magnet = 1 pulse / revolution
 *
 * Fosc = 20 MHz
 * Instruction clock = 5 MHz
 *
 * Timer1 prescaler = 1:8
 *
 * Timer1 tick:
 *
 *     8 / 5 MHz = 1.6 us
 *
 *
 * RPM calculation:
 *
 *     RPM = 60 / period_seconds
 *
 *     RPM = 60,000,000 / (period_us)
 *
 * Since:
 *
 *     period_us = interval_ticks × 1.6
 *
 * Therefore:
 *
 *     RPM = 37,500,000 / interval_ticks
 *
 * ==========================================================
 */


/*----------------------------------------------------------
 * RPM Service
 *----------------------------------------------------------*/

static void RPM_Service(uint8_t event,
                        uint16_t timerValue,
                        uint8_t *newValue,
                        uint32_t *period,
                        uint32_t *rpm,
                        uint8_t *stopped)
{
    static uint16_t timerHigh = 0;
    static uint32_t previousTimestamp = 0;

    static uint8_t firstPulse = 1;
    static uint8_t waitingForPulse = 0;

    static uint16_t timeoutOverflows = 0;

    static uint32_t pendingPeriod = 0;
    static uint32_t pendingRPM = 0;

    static uint8_t pendingValue = 0;
    static uint8_t pendingStopped = 0;

    uint32_t timestamp;
    uint32_t interval;


    /*======================================================
     * TIMER1 OVERFLOW
     *======================================================*/

    if(event == 1)
    {
        timerHigh++;

        if(waitingForPulse)
        {
            timeoutOverflows++;

            /*
             * Timer1 overflow:
             *
             * 65536 × 1.6 us
             * = 104.8576 ms
             *
             * 20 overflows ? 2.1 seconds
             */

            if(timeoutOverflows >= 20)
            {
                firstPulse = 1;
                waitingForPulse = 0;

                timeoutOverflows = 0;

                pendingStopped = 1;
                pendingValue = 1;
            }
        }
    }


    /*======================================================
     * RPM PULSE
     *======================================================*/

    if(event == 2)
    {
        /*
         * Create 32-bit timestamp
         */

        timestamp =
            ((uint32_t)timerHigh << 16)
            | (uint32_t)timerValue;


        /*--------------------------------------------------
         * FIRST PULSE
         *--------------------------------------------------*/

        if(firstPulse)
        {
            firstPulse = 0;

            previousTimestamp = timestamp;

            waitingForPulse = 1;

            timeoutOverflows = 0;

            pendingStopped = 0;
        }


        /*--------------------------------------------------
         * FOLLOWING PULSES
         *--------------------------------------------------*/

        else
        {
            /*
             * Calculate time between pulses.
             */

            interval =
                timestamp - previousTimestamp;


            /*
             * Minimum valid interval:
             *
             * Maximum expected RPM = 150000
             *
             * 150000 RPM
             * = 250 timer ticks
             *
             * Pulses faster than this are ignored.
             */

            if(interval >= 250UL)
            {
                /*
                 * Valid RPM pulse
                 */

                pendingPeriod = interval;

                pendingRPM =
                    37500000UL / interval;

                pendingValue = 1;

                pendingStopped = 0;


                /*
                 * Current pulse becomes
                 * the new reference pulse.
                 */

                previousTimestamp = timestamp;

                timeoutOverflows = 0;
            }

            /*
             * If interval < 250:
             *
             * Ignore the pulse.
             *
             * Do not update previousTimestamp.
             */
        }
    }


    /*======================================================
     * SEND RESULT TO MAIN
     *======================================================*/

    if(event == 0)
    {
        if(pendingValue)
        {
            *newValue = 1;

            *period = pendingPeriod;

            *rpm = pendingRPM;

            *stopped = pendingStopped;

            pendingValue = 0;

            pendingStopped = 0;
        }
        else
        {
            *newValue = 0;
        }
    }
}


/*----------------------------------------------------------
 * RPM Initialization
 *----------------------------------------------------------*/

void RPM_Init(void)
{
    /*
     * RB2 = input
     */

    TRISBbits.TRISB2 = 1;


    /*
     * Make RB2 digital.
     */

    ADCON1 = 0x0F;


    /*
     * INT2 rising edge
     */

    INTCON2bits.INTEDG2 = 1;


    /*
     * Clear INT2 flag
     */

    INTCON3bits.INT2IF = 0;


    /*
     * Enable INT2
     */

    INTCON3bits.INT2IE = 1;


    /*
     * Timer1 16-bit mode
     */

    T1CONbits.RD16 = 1;


    /*
     * Timer1 prescaler = 1:8
     */

    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;


    /*
     * Timer1 clock = Fosc / 4
     */

    T1CONbits.TMR1CS = 0;


    /*
     * Start Timer1 from zero
     */

    TMR1H = 0;
    TMR1L = 0;


    /*
     * Clear Timer1 interrupt flag
     */

    PIR1bits.TMR1IF = 0;


    /*
     * Enable Timer1 interrupt
     */

    PIE1bits.TMR1IE = 1;


    /*
     * Start Timer1
     */

    T1CONbits.TMR1ON = 1;


    /*
     * Enable peripheral interrupts
     */

    INTCONbits.PEIE = 1;


    /*
     * Enable global interrupts
     */

    INTCONbits.GIE = 1;
}


/*----------------------------------------------------------
 * Interrupt Service Routine
 *----------------------------------------------------------*/

void __interrupt() ISR(void)
{
    uint16_t timerValue;


    /*======================================================
     * TIMER1 OVERFLOW
     *======================================================*/

    if(PIR1bits.TMR1IF)
    {
        /*
         * Clear interrupt flag.
         */

        PIR1bits.TMR1IF = 0;


        /*
         * Notify RPM service.
         */

        RPM_Service(
            1,
            0,
            0,
            0,
            0,
            0
        );
    }


    /*======================================================
     * RPM PULSE - RB2 / INT2
     *======================================================*/

    if(INTCON3bits.INT2IF)
    {
        /*
         * Read Timer1.
         */

        timerValue =
            (uint16_t)TMR1L;

        timerValue |=
            ((uint16_t)TMR1H << 8);


        /*
         * Process RPM pulse.
         */

        RPM_Service(
            2,
            timerValue,
            0,
            0,
            0,
            0
        );


        /*
         * Clear INT2 interrupt flag.
         */

        INTCON3bits.INT2IF = 0;
    }
}


/*----------------------------------------------------------
 * RPM Task
 *----------------------------------------------------------*/

void RPM_Task(void)
{
    uint8_t newValue;
    uint8_t stopped;

    uint32_t period;
    uint32_t rpm;


    newValue = 0;
    stopped = 0;

    period = 0;
    rpm = 0;


    RPM_Service(
        0,
        0,
        &newValue,
        &period,
        &rpm,
        &stopped
    );
}


/*----------------------------------------------------------
 * Read RPM
 *----------------------------------------------------------*/

uint32_t RPM_Read(void)
{
    static uint32_t lastRPM = 0;

    uint8_t newValue;
    uint8_t stopped;

    uint32_t period;
    uint32_t rpm;


    newValue = 0;
    stopped = 0;

    period = 0;
    rpm = 0;


    RPM_Service(
        0,
        0,
        &newValue,
        &period,
        &rpm,
        &stopped
    );


    if(newValue)
    {
        if(stopped)
        {
            lastRPM = 0;
        }
        else
        {
            lastRPM = rpm;
        }
    }


    return lastRPM;
}