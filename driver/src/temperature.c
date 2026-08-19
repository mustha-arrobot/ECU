/************************************************************
 * File: temperature.c
 *
 * AD8495 + K-Type Thermocouple
 *
 * Hardware:
 *     AD8495 VOUT -> AN0 / RA0
 ************************************************************/

#include "temperature.h"
#include "adc_driver.h"
#include "config.h"


/*==========================================================
 * ADC PARAMETERS
 *==========================================================*/

#define ADC_REF_VOLTAGE       3.3f
#define ADC_MAX_COUNT         1023.0f


/*==========================================================
 * AD8495 PARAMETERS
 *==========================================================*/

#define AD8495_REF_VOLTAGE    1.275f
#define AD8495_SENSITIVITY    0.005f


/*==========================================================
 * INITIALIZATION
 *==========================================================*/

void Temperature_Init(void)
{
    /*
     * ADC initialization is handled by
     * ADC_Driver_Init().
     */
}


/*==========================================================
 * READ ADC
 *==========================================================*/

uint16_t Temperature_ReadADC(void)
{
    return ADC_Driver_Read(TEMP_CHANNEL);
}


/*==========================================================
 * READ SENSOR VOLTAGE
 *==========================================================*/

float Temperature_ReadVoltage(void)
{
    uint16_t adc;

    adc = Temperature_ReadADC();

    return ((float)adc * ADC_REF_VOLTAGE)
           / ADC_MAX_COUNT;
}


/*==========================================================
 * CONVERT TO TEMPERATURE
 *==========================================================*/

float Temperature_ReadCelsius(void)
{
    float voltage;

    voltage = Temperature_ReadVoltage();

    return (voltage - AD8495_REF_VOLTAGE)
           / AD8495_SENSITIVITY;
}