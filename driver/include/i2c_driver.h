#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * I2C DRIVER
 *==========================================================
 *
 * MCU:
 *     PIC18LF2520
 *
 * Hardware:
 *     MSSP I2C Master
 *
 * Pins:
 *     RC3 -> SCL
 *     RC4 -> SDA
 *
 * Responsibilities:
 *     - Initialize MSSP I2C peripheral
 *     - Generate START
 *     - Generate REPEATED START
 *     - Generate STOP
 *     - Write one byte
 *     - Read one byte
 *     - ACK / NACK handling
 *
 * This driver does NOT:
 *     - Know EEPROM addresses
 *     - Know ECU records
 *     - Store ECU parameters
 *     - Implement EEPROM data structures
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void I2C_Driver_Init(void);


/*----------------------------------------------------------
 * Bus Control
 *----------------------------------------------------------*/

void I2C_Driver_Wait(void);

bool I2C_Driver_Start(void);

bool I2C_Driver_RepeatedStart(void);

bool I2C_Driver_Stop(void);


/*----------------------------------------------------------
 * Data Transfer
 *----------------------------------------------------------*/

/*
 * Write one byte onto the I2C bus.
 *
 * Returns:
 *     true  -> byte acknowledged
 *     false -> not acknowledged
 */
bool I2C_Driver_Write(uint8_t data);


/*
 * Read one byte from the I2C bus.
 *
 * ack = true:
 *     Send ACK after receiving byte.
 *
 * ack = false:
 *     Send NACK after receiving byte.
 */
uint8_t I2C_Driver_Read(bool ack);


/*----------------------------------------------------------
 * Bus Status
 *----------------------------------------------------------*/

bool I2C_Driver_IsBusIdle(void);

bool I2C_Driver_IsAcknowledged(void);

#endif /* I2C_DRIVER_H */