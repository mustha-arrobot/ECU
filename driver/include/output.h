#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * ECU OUTPUT DRIVER
 *==========================================================
 *
 * Hardware outputs:
 *
 *     Fuel Valve
 *     Gas Valve
 *     Glow Plug
 *     Starter Enable
 *
 * This module provides a hardware-oriented interface for
 * ECU digital outputs.
 *
 * GPIO register access is handled by gpio_driver.c.
 *
 * This module does NOT contain ECU control decisions.
 *==========================================================
 */


/*----------------------------------------------------------
 * Output Initialization
 *----------------------------------------------------------*/

void OUTPUT_Init(void);


/*----------------------------------------------------------
 * Fuel Valve
 *
 * Hardware:
 *     RB4
 *
 * Active:
 *     HIGH
 *----------------------------------------------------------*/

void OUTPUT_FuelValve(uint8_t state);


/*----------------------------------------------------------
 * Gas Valve
 *
 * Hardware:
 *     RB5
 *
 * Active:
 *     HIGH
 *----------------------------------------------------------*/

void OUTPUT_GasValve(uint8_t state);


/*----------------------------------------------------------
 * Glow Plug
 *
 * Hardware:
 *     RC1
 *
 * Active:
 *     LOW
 *----------------------------------------------------------*/

void OUTPUT_GlowPlug(uint8_t state);


/*----------------------------------------------------------
 * Starter Enable
 *
 * Hardware:
 *     RC5
 *----------------------------------------------------------*/

void OUTPUT_StarterEnable(uint8_t state);


/*----------------------------------------------------------
 * All Outputs OFF
 *----------------------------------------------------------*/

void OUTPUT_AllOff(void);


/*----------------------------------------------------------
 * Output Status
 *----------------------------------------------------------*/

uint8_t OUTPUT_GetFuelValveState(void);

uint8_t OUTPUT_GetGasValveState(void);

uint8_t OUTPUT_GetGlowPlugState(void);

uint8_t OUTPUT_GetStarterEnableState(void);

#endif /* OUTPUT_H */