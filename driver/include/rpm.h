#ifndef RPM_H
#define RPM_H

#include <stdint.h>

#include "config.h"

/*
 *==========================================================
 * RPM DRIVER
 *==========================================================
 *
 * Hardware input:
 *     RB2 / INT2
 *
 * Schematic:
 *     MCU_20_RPM
 *
 * Configuration:
 *     RPM_TRIS
 *     RPM_PORT
 *
 * Responsibilities:
 *     - Initialize RPM input
 *     - Detect RPM pulses
 *     - Measure pulse timing
 *     - Calculate RPM
 *     - Provide current RPM value
 *
 * Timer hardware is handled through the Timer Driver.
 *
 * This driver does NOT:
 *     - Control the starter
 *     - Control fuel
 *     - Implement ECU state-machine logic
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void RPM_Init(void);


/*----------------------------------------------------------
 * RPM Processing Task
 *----------------------------------------------------------*/

/*
 * Process RPM measurement.
 *
 * Should be called periodically by the application/scheduler
 * or from the appropriate timing mechanism.
 */
void RPM_Task(void);


/*----------------------------------------------------------
 * RPM Reading
 *----------------------------------------------------------*/

/*
 * Return calculated RPM.
 */
uint32_t RPM_Read(void);

#endif /* RPM_DRIVER_H */