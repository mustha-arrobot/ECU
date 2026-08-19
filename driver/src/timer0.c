#include "timer0.h"


void Timer0_Init(void)
{
    /*
     * Stop Timer0 during configuration.
     */
    T0CONbits.TMR0ON = 0;


    /*
     * 16-bit Timer0.
     */
    T0CONbits.T08BIT = 0;


    /*
     * Internal instruction clock:
     *
     * Fosc / 4
     */
    T0CONbits.T0CS = 0;


    /*
     * Enable prescaler.
     */
    T0CONbits.PSA = 0;


    /*
     * Prescaler = 1:8
     */
    T0CONbits.T0PS = 0b010;


    /*
     * Existing working reload value.
     *
     * Keep unchanged until the actual Starter PWM
     * timing is verified.
     */
    TMR0H = 0xFF;
    TMR0L = 0xF3;


    /*
     * Clear Timer0 interrupt flag.
     */
    INTCONbits.TMR0IF = 0;


    /*
     * Enable Timer0 interrupt.
     */
    INTCONbits.TMR0IE = 1;


    /*
     * Enable peripheral and global interrupts.
     */
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


    /*
     * Start Timer0.
     */
    T0CONbits.TMR0ON = 1;
}