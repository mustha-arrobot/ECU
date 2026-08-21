#include "rpm.h"
#include "starter_pwm.h"
#include "throttle.h"


/*
 * ==========================================================
 * RPM SENSOR
 * ==========================================================
 *
 * MCU:
 *     PIC18LF2520
 *
 * INPUT:
 *     RB2 / INT2
 *
 * SENSOR:
 *     1 pulse = 1 revolution
 *
 * CLOCK:
 *     Fosc = 20 MHz
 *
 * Instruction clock:
 *     Fosc / 4 = 5 MHz
 *
 * Timer1 prescaler:
 *     1:8
 *
 * Timer1 frequency:
 *     625 kHz
 *
 * Timer1 tick:
 *     1.6 us
 *
 * RPM:
 *
 *     RPM = 37,500,000 / interval_ticks
 *
 * ==========================================================
 */


/*==========================================================
 * RPM LIMITS
 *==========================================================*/

/*
 * Maximum RPM:
 *
 * 150000 RPM
 *
 * Period:
 *
 * 60 / 150000
 * = 400 us
 *
 * Timer ticks:
 *
 * 400 / 1.6
 * = 250 ticks
 */
#define RPM_MIN_INTERVAL_TICKS    250UL


/*
 * If there is no pulse for approximately 2 seconds,
 * consider the engine stopped.
 *
 * Timer1 overflow:
 *
 * 65536 × 1.6 us
 * = 104.8576 ms
 *
 * 20 overflows:
 *
 * ≈ 2.097 seconds
 */
#define RPM_TIMEOUT_OVERFLOWS      20U


/*==========================================================
 * RPM INTERNAL STATE
 *==========================================================*/

/*
 * Upper 16 bits of extended Timer1.
 *
 * Timer1 itself is the lower 16 bits.
 */
static volatile uint16_t rpmTimerHigh = 0;


/*
 * Previous complete 32-bit timestamp.
 */
static volatile uint32_t rpmPreviousTimestamp = 0;


/*
 * First pulse flag.
 */
static volatile uint8_t rpmFirstPulse = 1;


/*
 * Indicates that we are waiting for the next pulse.
 */
static volatile uint8_t rpmWaitingForPulse = 0;


/*
 * Timeout counter.
 */
static volatile uint16_t rpmTimeoutOverflows = 0;


/*==========================================================
 * PENDING RPM RESULT
 *==========================================================*/

static volatile uint32_t rpmPendingPeriod = 0;

static volatile uint32_t rpmPendingValue = 0;

static volatile uint8_t rpmPendingNew = 0;

static volatile uint8_t rpmPendingStopped = 0;


/*==========================================================
 * RPM SERVICE
 *==========================================================*/

static void RPM_Service(
        uint8_t event,
        uint16_t timerValue,
        uint8_t *newValue,
        uint32_t *period,
        uint32_t *rpm,
        uint8_t *stopped)
{
    uint32_t timestamp;
    uint32_t interval;


    /*======================================================
     * TIMER1 OVERFLOW
     *======================================================*/

    if(event == 1)
    {
        /*
         * Timer1 rolled over.
         *
         * Increase upper 16 bits.
         */
        rpmTimerHigh++;


        /*
         * Only count timeout while waiting
         * for another RPM pulse.
         */
        if(rpmWaitingForPulse)
        {
            rpmTimeoutOverflows++;


            /*
             * Engine stopped.
             */
            if(rpmTimeoutOverflows >=
               RPM_TIMEOUT_OVERFLOWS)
            {
                rpmFirstPulse = 1;

                rpmWaitingForPulse = 0;

                rpmTimeoutOverflows = 0;

                rpmPendingStopped = 1;

                rpmPendingNew = 1;
            }
        }
    }


    /*======================================================
     * RPM PULSE
     *======================================================*/

    if(event == 2)
    {
        /*
         * Create 32-bit timestamp.
         *
         * Upper 16 bits:
         *     rpmTimerHigh
         *
         * Lower 16 bits:
         *     timerValue
         */
        timestamp =
            ((uint32_t)rpmTimerHigh << 16)
            |
            timerValue;


        /*==================================================
         * FIRST PULSE
         *==================================================*/

        if(rpmFirstPulse)
        {
            /*
             * First pulse only establishes
             * the starting timestamp.
             */
            rpmFirstPulse = 0;

            rpmWaitingForPulse = 1;

            rpmPreviousTimestamp = timestamp;

            rpmTimeoutOverflows = 0;

            rpmPendingStopped = 0;
        }


        /*==================================================
         * FOLLOWING PULSES
         *==================================================*/

        else
        {
            /*
             * Calculate interval.
             *
             * Unsigned subtraction automatically
             * handles 32-bit timestamp rollover.
             */
            interval =
                timestamp -
                rpmPreviousTimestamp;


            /*
             * Check minimum valid interval.
             */
            if(interval >=
               RPM_MIN_INTERVAL_TICKS)
            {
                /*
                 * Save period.
                 */
                rpmPendingPeriod =
                    interval;


                /*
                 * Calculate RPM.
                 *
                 * 1 tick = 1.6 us
                 *
                 * RPM =
                 *
                 * 60,000,000 /
                 * (interval × 1.6)
                 *
                 * =
                 *
                 * 37,500,000 / interval
                 */
                rpmPendingValue =
                    37500000UL /
                    interval;


                /*
                 * New RPM result.
                 */
                rpmPendingNew = 1;


                /*
                 * Engine running.
                 */
                rpmPendingStopped = 0;


                /*
                 * IMPORTANT:
                 *
                 * Update previous timestamp
                 * only after a valid interval.
                 */
                rpmPreviousTimestamp =
                    timestamp;


                /*
                 * Reset timeout.
                 */
                rpmTimeoutOverflows = 0;
            }
        }
    }


    /*======================================================
     * RETURN RESULT TO MAIN
     *======================================================*/

    if(event == 0)
    {
        /*
         * Default.
         */
        *newValue = 0;

        *period = 0;

        *rpm = 0;

        *stopped = 0;


        /*
         * New result available.
         */
        if(rpmPendingNew)
        {
            *newValue = 1;

            *period =
                rpmPendingPeriod;

            *rpm =
                rpmPendingValue;

            *stopped =
                rpmPendingStopped;


            /*
             * Consume result.
             */
            rpmPendingNew = 0;

            rpmPendingStopped = 0;
        }
    }
}


/*==========================================================
 * RPM INITIALIZATION
 *==========================================================*/

void RPM_Init(void)
{
    /*
     * RB2 / INT2 input.
     */
    RPM_TRIS = 1;


    /*
     * Make RB2 digital.
     */
    ADCON1 = 0x0F;


    /*======================================================
     * INT2
     *======================================================*/

    /*
     * Rising edge.
     */
    INTCON2bits.INTEDG2 = 1;


    /*
     * Clear INT2 flag.
     */
    INTCON3bits.INT2IF = 0;


    /*
     * Enable INT2.
     */
    INTCON3bits.INT2IE = 1;


    /*======================================================
     * TIMER1
     *======================================================*/

    /*
     * Stop Timer1 during configuration.
     */
    T1CONbits.TMR1ON = 0;


    /*
     * 16-bit mode.
     */
    T1CONbits.RD16 = 1;


    /*
     * Internal clock = Fosc/4.
     */
    T1CONbits.TMR1CS = 0;


    /*
     * Prescaler = 1:8.
     */
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;


    /*
     * Clear Timer1.
     *
     * High byte first, low byte second.
     */
    TMR1H = 0;
    TMR1L = 0;


    /*
     * Clear overflow flag.
     */
    PIR1bits.TMR1IF = 0;


    /*
     * Enable Timer1 interrupt.
     */
    PIE1bits.TMR1IE = 1;


    /*
     * Reset software timer state.
     */
    rpmTimerHigh = 0;

    rpmPreviousTimestamp = 0;

    rpmFirstPulse = 1;

    rpmWaitingForPulse = 0;

    rpmTimeoutOverflows = 0;

    rpmPendingPeriod = 0;

    rpmPendingValue = 0;

    rpmPendingNew = 0;

    rpmPendingStopped = 0;


    /*
     * Start Timer1.
     */
    T1CONbits.TMR1ON = 1;


    /*======================================================
     * GLOBAL INTERRUPTS
     *======================================================*/

    INTCONbits.PEIE = 1;

    INTCONbits.GIE = 1;
}


/*==========================================================
 * SINGLE GLOBAL ISR
 *
 * IMPORTANT:
 *
 * ONLY ONE __interrupt() ISR MUST EXIST IN THE PROJECT.
 *
 * Handles:
 *
 *     Timer1 -> RPM
 *     INT2   -> RPM
 *     INT0   -> throttle
 *     Timer0  -> starter PWM
 *==========================================================*/

void __interrupt() ISR(void)
{
    uint16_t timerValue;


    /*======================================================
     * TIMER1 OVERFLOW
     *======================================================*/

    if(PIR1bits.TMR1IF)
    {
        /*
         * Clear flag first.
         */
        PIR1bits.TMR1IF = 0;


        /*
         * Update extended Timer1.
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
     * RPM SENSOR
     *
     * RB2 / INT2
     *======================================================*/

    if(INTCON3bits.INT2IF)
    {
        /*
         * Read Timer1 coherently.
         *
         * With RD16 enabled:
         *
         * Read low byte first.
         * Then read high byte.
         */
        timerValue =
            TMR1L;

        timerValue |=
            ((uint16_t)TMR1H << 8);


        /*
         * Clear INT2 flag before processing.
         */
        INTCON3bits.INT2IF = 0;


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
    }


    /*======================================================
     * CCPM THROTTLE
     *
     * RB0 / INT0
     *
     * Timer3 measures HIGH pulse width.
     *======================================================*/

    if(INTCONbits.INT0IF)
    {
        /*
         * Clear INT0 first.
         */
        INTCONbits.INT0IF = 0;


        /*
         * Process CCPM edge.
         */
        Throttle_INT0_Handler();
    }


    /*======================================================
     * TIMER0
     *
     * STARTER PWM
     *======================================================*/

    if(INTCONbits.TMR0IF)
    {
        /*
         * Reload Timer0.
         */
        TMR0H = 0xFF;
        TMR0L = 0xF3;


        /*
         * Clear flag.
         */
        INTCONbits.TMR0IF = 0;


        /*
         * Starter PWM service.
         */
        StarterPWM_Task();
    }
}


/*==========================================================
 * RPM TASK
 *==========================================================*/

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


/*==========================================================
 * RPM READ
 *==========================================================*/

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


    /*======================================================
     * UPDATE RPM
     *======================================================*/

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
