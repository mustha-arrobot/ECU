#ifndef FUEL_PUMP_H
#define FUEL_PUMP_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * FUEL PUMP PWM DRIVER
 *==========================================================
 *
 * Hardware:
 *     PIC18LF2520
 *
 * PWM Peripheral:
 *     CCP1
 *
 * Output:
 *     RC2 / CCP1
 *
 * Responsibilities:
 *     - Initialize CCP1 hardware PWM
 *     - Set PWM duty cycle
 *     - Stop PWM output
 *     - Return current PWM duty
 *
 * This driver does NOT:
 *     - Read throttle
 *     - Decide required pump duty
 *     - Implement ECU state logic
 *     - Control engine operating modes
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void FuelPump_Init(void);


/*----------------------------------------------------------
 * PWM Duty
 *
 * Valid range:
 *     0 - 100 %
 *----------------------------------------------------------*/

void FuelPump_SetDuty(uint8_t duty);


/*----------------------------------------------------------
 * Stop Fuel Pump
 *----------------------------------------------------------*/

void FuelPump_Stop(void);


/*----------------------------------------------------------
 * Get Current Duty
 *----------------------------------------------------------*/

uint8_t FuelPump_GetDuty(void);


/*----------------------------------------------------------
 * Driver Status
 *----------------------------------------------------------*/

bool FuelPump_IsRunning(void);

#endif /* FUEL_PUMP_H */