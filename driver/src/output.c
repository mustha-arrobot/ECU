/******************************************************************************
 * File: output.c
 *
 * ECU Digital Output Driver
 *
 * Hardware:
 *     Fuel Valve       -> RB4  Active HIGH
 *     Gas Valve        -> RB5  Active HIGH
 *     Glow Plug        -> RC1  Active LOW
 *     Starter Enable   -> RC5  Active HIGH
 *
 * PWM outputs:
 *     Fuel Pump        -> RC2 / CCP1
 *     Starter          -> RC0
 *
 ******************************************************************************/

#include "output.h"
#include "gpio_driver.h"


/*==========================================================
 * INITIALIZE OUTPUTS
 *==========================================================*/

void OUTPUT_Init(void)
{
    /*
     * GPIO Driver is responsible for configuring
     * the actual PIC GPIO directions.
     */

    GPIO_Driver_Init();


    /*
     * Put all digital outputs into their safe OFF state.
     */

    GPIO_FuelValve_Set(GPIO_LOW);

    GPIO_GasValve_Set(GPIO_LOW);

    GPIO_GlowPlug_Set(GPIO_LOW);

    GPIO_StarterEnable_Set(GPIO_LOW);
}


/*==========================================================
 * FUEL VALVE
 *
 * Active HIGH
 *==========================================================*/

void OUTPUT_FuelValve(uint8_t state)
{
    if(state == ON)
    {
        GPIO_FuelValve_Set(GPIO_HIGH);
    }
    else
    {
        GPIO_FuelValve_Set(GPIO_LOW);
    }
}


/*==========================================================
 * GAS VALVE
 *
 * Active HIGH
 *==========================================================*/

void OUTPUT_GasValve(uint8_t state)
{
    if(state == ON)
    {
        GPIO_GasValve_Set(GPIO_HIGH);
    }
    else
    {
        GPIO_GasValve_Set(GPIO_LOW);
    }
}


/*==========================================================
 * GLOW PLUG
 *
 * Active LOW
 *
 * Logical ON  -> physical LOW
 * Logical OFF -> physical HIGH
 *==========================================================*/

void OUTPUT_GlowPlug(uint8_t state)
{
    if(state == ON)
    {
        GPIO_GlowPlug_Set(GPIO_LOW);
    }
    else
    {
        GPIO_GlowPlug_Set(GPIO_HIGH);
    }
}


/*==========================================================
 * STARTER ENABLE
 *
 * Active HIGH
 *==========================================================*/

void OUTPUT_StarterEnable(uint8_t state)
{
    if(state == ON)
    {
        GPIO_StarterEnable_Set(GPIO_HIGH);
    }
    else
    {
        GPIO_StarterEnable_Set(GPIO_LOW);
    }
}


/*==========================================================
 * ALL OUTPUTS OFF
 *==========================================================*/

void OUTPUT_AllOff(void)
{
    /*
     * Fuel Valve OFF
     */
    GPIO_FuelValve_Set(GPIO_LOW);


    /*
     * Gas Valve OFF
     */
    GPIO_GasValve_Set(GPIO_LOW);


    /*
     * Glow Plug OFF
     *
     * Active LOW ? physical HIGH
     */
    GPIO_GlowPlug_Set(GPIO_HIGH);


    /*
     * Starter Enable OFF
     */
    GPIO_StarterEnable_Set(GPIO_LOW);
}