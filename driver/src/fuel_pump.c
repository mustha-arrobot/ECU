#include "fuel_pump.h"
#include "adc_driver.h"

static uint8_t FuelPumpDuty = 0;


/*==========================================================
 * Initialize CCP1 Hardware PWM
 *==========================================================*/

void FuelPump_Init(void)
{
    /*
     * RC2 / CCP1
     */
    PUMP_TRIS = 0;


    /*
     * Timer2 ON
     */
    T2CON = 0x04;


    /*
     * CCP1 PWM mode
     */
    CCP1CON = 0x0C;


    /*
     * PWM period
     *
     * Existing working configuration:
     * approximately 20 kHz
     */
    PR2 = 249;


    /*
     * Start with pump OFF
     */
    FuelPump_SetDuty(0);
}


/*==========================================================
 * Set PWM Duty
 *
 * Range:
 *     0 - 100 %
 *==========================================================*/

void FuelPump_SetDuty(uint8_t duty)
{
    uint16_t pwm;


    /*
     * Limit duty to 100%.
     */
    if(duty > 100U)
    {
        duty = 100U;
    }


    /*
     * Store logical duty.
     */
    FuelPumpDuty = duty;


    /*
     * Convert:
     *
     * 0 - 100 %
     *
     * to:
     *
     * 0 - 1023
     *
     * CCP1 uses 10-bit PWM duty.
     */
    pwm =
        ((uint32_t)duty * 1023UL) / 100UL;


    /*
     * Upper 8 bits
     */
    CCPR1L =
        (uint8_t)(pwm >> 2);


    /*
     * Lower 2 bits
     */
    CCP1CONbits.DC1B =
        (uint8_t)(pwm & 0x03U);
}


/*==========================================================
 * Stop Fuel Pump
 *==========================================================*/

void FuelPump_Stop(void)
{
    FuelPump_SetDuty(0U);
}


/*==========================================================
 * Get Current Duty
 *==========================================================*/

uint8_t FuelPump_GetDuty(void)
{
    return FuelPumpDuty;
}