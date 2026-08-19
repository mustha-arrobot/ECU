#ifndef STARTER_PWM_H
#define STARTER_PWM_H

#include <stdint.h>
#include "config.h"

/*
 *==========================================================
 * STARTER PWM DRIVER
 *==========================================================
 *
 * Hardware:
 *     RC0
 *
 * PWM:
 *     Software PWM
 *
 * Functions:
 *     StarterPWM_Init()
 *     StarterPWM_SetDuty()
 *     StarterPWM_GetDuty()
 *     StarterPWM_Task()
 *     StarterPWM_Stop()
 *
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void StarterPWM_Init(void);


/*----------------------------------------------------------
 * Set Duty Cycle
 *
 * Range:
 *     0 - 100 %
 *----------------------------------------------------------*/

void StarterPWM_SetDuty(uint8_t duty);


/*----------------------------------------------------------
 * Get Duty Cycle
 *----------------------------------------------------------*/

uint8_t StarterPWM_GetDuty(void);


/*----------------------------------------------------------
 * Software PWM Task
 *----------------------------------------------------------*/

void StarterPWM_Task(void);


/*----------------------------------------------------------
 * Stop Starter PWM
 *----------------------------------------------------------*/

void StarterPWM_Stop(void);

#endif