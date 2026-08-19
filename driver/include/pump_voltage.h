#ifndef PUMP_VOLTAGE_H
#define PUMP_VOLTAGE_H

#include <stdint.h>

#include "config.h"

/*
 *==========================================================
 * PUMP VOLTAGE DRIVER
 *==========================================================
 *
 * Hardware input:
 *     RA1 / AN1
 *
 * Configuration:
 *     PUMP_VOLT_CHANNEL
 *
 * Responsibilities:
 *     - Initialize pump-voltage input
 *     - Read raw ADC value
 *     - Convert ADC input to voltage
 *
 * ADC peripheral access should be handled by the common
 * ADC Driver.
 *
 * This module does NOT:
 *     - Control the fuel pump
 *     - Calculate pump PWM
 *     - Implement ECU control logic
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void PumpVoltage_Init(void);


/*----------------------------------------------------------
 * Raw ADC
 *----------------------------------------------------------*/

uint16_t PumpVoltage_ReadADC(void);


/*----------------------------------------------------------
 * ADC Input Voltage
 *----------------------------------------------------------*/

float PumpVoltage_ReadVoltage(void);


/*----------------------------------------------------------
 * Actual Pump Voltage
 *----------------------------------------------------------*/

float PumpVoltage_Read(void);


#endif /* PUMP_VOLTAGE_H */