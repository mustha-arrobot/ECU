/******************************************************************************
 * File: pump_voltage.c
 *
 * Pump Voltage Measurement Driver
 *
 * Hardware:
 *     Pump Voltage -> AN1 / RA1
 *
 ******************************************************************************/

#include "pump_voltage.h"
#include "adc_driver.h"


/*==========================================================
 * ADC PARAMETERS
 *==========================================================*/

#define ADC_REF_VOLTAGE       5.0f
#define ADC_MAX_COUNT         1023.0f

/*
 * Voltage divider:
 *
 * Example:
 *     R1 = 30k
 *     R2 = 10k
 *
 *     Divider = 4
 *
 * Verify this value against the actual ECU hardware.
 */
#define PUMP_VOLTAGE_DIVIDER  4.0f


/*==========================================================
 * INITIALIZATION
 *==========================================================*/

void PumpVoltage_Init(void)
{
    /*
     * ADC hardware initialization is handled by
     * ADC_Driver_Init().
     *
     * This function is retained as the Pump Voltage
     * module initialization interface.
     */
}


/*==========================================================
 * READ RAW ADC
 *==========================================================*/

uint16_t PumpVoltage_ReadADC(void)
{
    return ADC_Driver_Read(PUMP_VOLT_CHANNEL);
}


/*==========================================================
 * READ PUMP VOLTAGE
 *==========================================================*/

float PumpVoltage_ReadVoltage(void)
{
    uint16_t adc;
    float voltage;


    /*
     * Read ADC through the common ADC Driver.
     */
    adc = PumpVoltage_ReadADC();


    /*
     * Convert ADC count to ADC input voltage.
     */
    voltage =
        ((float)adc * ADC_REF_VOLTAGE)
        / ADC_MAX_COUNT;


    /*
     * Convert divider output voltage to
     * actual pump voltage.
     */
    voltage *= PUMP_VOLTAGE_DIVIDER;


    return voltage;
}