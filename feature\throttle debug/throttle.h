#ifndef THROTTLE_H
#define THROTTLE_H

#include <stdint.h>
#include "config.h"


/*==========================================================
 * THROTTLE INITIALIZATION
 *==========================================================*/

void Throttle_Init(void);


/*==========================================================
 * INT0 HANDLER
 *
 * Called from the global ISR when RB0/INT0 changes state.
 *==========================================================*/

void Throttle_INT0_Handler(void);


/*==========================================================
 * THROTTLE TASK
 *
 * Processes the latest CCPM pulse and updates
 * the ramped throttle value.
 *==========================================================*/

void Throttle_Task(void);


/*==========================================================
 * READ CCPM PULSE WIDTH
 *
 * Returns:
 *     Pulse width in microseconds.
 *
 * Example:
 *     1000 us
 *     1500 us
 *     2000 us
 *==========================================================*/

uint16_t Throttle_ReadPulseWidth(void);


/*==========================================================
 * READ THROTTLE PERCENTAGE
 *
 * Returns:
 *     0 - 100 %
 *==========================================================*/

uint8_t Throttle_ReadPercent(void);


#endif /* THROTTLE_H */