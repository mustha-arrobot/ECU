/******************************************************************************
 * File: throttle.c
 *
 * Throttle Position Measurement Driver
 *
 * Hardware:
 *     Throttle signal -> AN12 / RB0
 *
 ******************************************************************************/

#include "throttle.h"
#include "adc_driver.h"


/*==========================================================
 * ADC PARAMETERS
 *==========================================================*/

#define ADC_REF_VOLTAGE       5.0f
#define ADC_MAX_COUNT         1023.0f


/*==========================================================
 * THROTTLE CALIBRATION
 *==========================================================*/

/*
 * Calibration:
 *
 * ADC = 0       -> 0 %
 * ADC = 900     -> 100 %
 *
 * ADC values >= 900 are limited to 100 %.
 */
#define THROTTLE_FULL_SCALE_ADC    900U


/*==========================================================
 * INITIALIZATION
 *==========================================================*/

void Throttle_Init(void)
{
    /*
     * ADC initialization is handled by ADC_Driver_Init().
     *
     * THROTTLE_CHANNEL is defined in config.h:
     *
     * #define THROTTLE_CHANNEL 12
     *
     * AN12 = RB0
     */
}


/*==========================================================
 * READ RAW ADC
 *==========================================================*/

uint16_t Throttle_ReadADC(void)
{
    return ADC_Driver_Read(THROTTLE_CHANNEL);
}


/*==========================================================
 * READ THROTTLE VOLTAGE
 *==========================================================*/

float Throttle_ReadVoltage(void)
{
    uint16_t adc;

    adc = Throttle_ReadADC();

    return ((float)adc * ADC_REF_VOLTAGE)
           / ADC_MAX_COUNT;
}


/*==========================================================
 * READ THROTTLE PERCENTAGE
 *==========================================================*/

uint8_t Throttle_ReadPercent(void)
{
    uint16_t adc;

    adc = Throttle_ReadADC();

    /*
     * Limit maximum throttle value.
     */
    if (adc >= THROTTLE_FULL_SCALE_ADC)
    {
        return 100U;
    }

    /*
     * Convert ADC value to percentage.
     *
     * ADC = 0   -> 0 %
     * ADC = 900 -> 100 %
     */
    return (uint8_t)
           (((uint32_t)adc * 100UL)
            / THROTTLE_FULL_SCALE_ADC);
}