#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "config.h"

/* GPIO logic levels */
#define GPIO_HIGH   1
#define GPIO_LOW    0


/*==========================================================
 * GPIO INITIALIZATION
 *==========================================================*/

void GPIO_Driver_Init(void);


/*==========================================================
 * FUEL VALVE
 *==========================================================*/

void GPIO_FuelValve_Set(uint8_t state);
uint8_t GPIO_FuelValve_Get(void);


/*==========================================================
 * GAS VALVE
 *==========================================================*/

void GPIO_GasValve_Set(uint8_t state);
uint8_t GPIO_GasValve_Get(void);


/*==========================================================
 * FUEL PUMP
 *==========================================================*/

void GPIO_FuelPump_Set(uint8_t state);
uint8_t GPIO_FuelPump_Get(void);


/*==========================================================
 * GLOW PLUG
 *==========================================================*/

void GPIO_Glow_Set(uint8_t state);
uint8_t GPIO_Glow_Get(void);

/*
 * Compatibility name used by output.c
 */
#define GPIO_GlowPlug_Set   GPIO_Glow_Set


/*==========================================================
 * STARTER
 *==========================================================*/

void GPIO_Starter_Set(uint8_t state);
uint8_t GPIO_Starter_Get(void);


/*==========================================================
 * STARTER ENABLE
 *==========================================================*/

void GPIO_StarterEnable_Set(uint8_t state);
uint8_t GPIO_StarterEnable_Get(void);


/*==========================================================
 * STATUS LED
 *==========================================================*/

void GPIO_LED_Set(uint8_t state);
uint8_t GPIO_LED_Get(void);


/*==========================================================
 * RPM INPUT
 *==========================================================*/

uint8_t GPIO_RPM_Read(void);


#endif