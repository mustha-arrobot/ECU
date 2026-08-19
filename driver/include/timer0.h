#ifndef TIMER0_H
#define TIMER0_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * TIMER0 DRIVER
 *==========================================================
 *
 * MCU:
 *     PIC18LF2520
 *
 * Purpose:
 *     Timer0 hardware time base
 *
 * Timer0 is used by:
 *     - Starter software PWM
 *
 * Responsibilities:
 *     - Configure Timer0
 *     - Start / stop Timer0
 *     - Reload Timer0
 *     - Handle overflow flag
 *
 * This driver does NOT:
 *     - Generate starter PWM duty logic
 *     - Control the starter
 *     - Implement ECU state logic
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void Timer0_Init(void);


/*----------------------------------------------------------
 * Timer Control
 *----------------------------------------------------------*/

void Timer0_Start(void);

void Timer0_Stop(void);


/*----------------------------------------------------------
 * Timer Reload
 *----------------------------------------------------------*/

void Timer0_Reload(void);


/*----------------------------------------------------------
 * Overflow
 *----------------------------------------------------------*/

bool Timer0_OverflowPending(void);

void Timer0_ClearOverflow(void);


/*----------------------------------------------------------
 * Current Timer Value
 *----------------------------------------------------------*/

uint16_t Timer0_Read(void);

#endif /* TIMER0_H */