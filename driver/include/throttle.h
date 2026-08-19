#ifndef THROTTLE_H
#define THROTTLE_H

#include <stdint.h>

#include "config.h"

/*
 *==========================================================
 * THROTTLE DRIVER
 *==========================================================
 *
 * MCU Input:
 *     RB0 / AN12
 *
 * Configuration:
 *     THROTTLE_CHANNEL
 *
 * Responsibilities:
 *     - Initialize throttle input
 *     - Read raw ADC value
 *     - Convert ADC value to voltage
 *     - Convert throttle signal to percentage
 *
 * ADC hardware access is handled by the ADC Driver.
 *
 * This driver does NOT:
 *     - Control the fuel valve
 *     - Control the gas valve
 *     - Decide engine operating state
 *     - Implement ECU control logic
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void Throttle_Init(void);


/*----------------------------------------------------------
 * Raw ADC Value
 *----------------------------------------------------------*/

uint16_t Throttle_ReadADC(void);


/*----------------------------------------------------------
 * ADC Input Voltage
 *----------------------------------------------------------*/

float Throttle_ReadVoltage(void);


/*----------------------------------------------------------
 * Throttle Position
 *
 * Return:
 *     0 - 100 %
 *----------------------------------------------------------*/

uint8_t Throttle_ReadPercent(void);


#endif /* THROTTLE_H */