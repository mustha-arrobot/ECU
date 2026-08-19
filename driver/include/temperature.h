#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

#include "config.h"

/*
 *==========================================================
 * TEMPERATURE DRIVER
 *==========================================================
 *
 * Sensor:
 *     AD8495
 *
 * MCU input:
 *     RA0 / AN0
 *
 * Configuration:
 *     TEMP_CHANNEL
 *
 * Responsibilities:
 *     - Initialize temperature input
 *     - Read raw ADC value
 *     - Convert ADC value to sensor voltage
 *     - Convert sensor voltage to temperature
 *
 * ADC peripheral access is handled by the ADC Driver.
 *
 * This driver does NOT:
 *     - Decide cooling operation
 *     - Implement over-temperature protection
 *     - Control any actuator
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void Temperature_Init(void);


/*----------------------------------------------------------
 * Raw ADC
 *----------------------------------------------------------*/

uint16_t Temperature_ReadADC(void);


/*----------------------------------------------------------
 * Sensor Voltage
 *----------------------------------------------------------*/

float Temperature_ReadVoltage(void);


/*----------------------------------------------------------
 * Temperature
 *
 * Return:
 *     Temperature in degrees Celsius
 *----------------------------------------------------------*/

float Temperature_ReadCelsius(void);


#endif /* TEMPERATURE_H */