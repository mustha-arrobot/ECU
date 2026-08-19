#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * EEPROM DRIVER
 *==========================================================
 *
 * External I2C EEPROM
 *
 * Device address:
 *     EEPROM_DEVICE_ADDR
 *
 * Address range:
 *     0x0000 - 0x7FFF
 *
 * Responsibilities:
 *     - EEPROM initialization
 *     - Byte write
 *     - Byte read
 *     - Buffer write
 *     - Buffer read
 *
 * This driver does NOT:
 *     - Know ECU_Record
 *     - Manage log records
 *     - Print through UART
 *     - Decide where ECU data is stored
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void EEPROM_Driver_Init(void);


/*----------------------------------------------------------
 * Single Byte
 *----------------------------------------------------------*/

bool EEPROM_Driver_WriteByte(uint16_t address,
                             uint8_t data);

bool EEPROM_Driver_ReadByte(uint16_t address,
                            uint8_t *data);


/*----------------------------------------------------------
 * Buffer
 *----------------------------------------------------------*/

bool EEPROM_Driver_WriteBuffer(uint16_t address,
                               const uint8_t *buffer,
                               uint16_t length);

bool EEPROM_Driver_ReadBuffer(uint16_t address,
                              uint8_t *buffer,
                              uint16_t length);


/*----------------------------------------------------------
 * Address Validation
 *----------------------------------------------------------*/

bool EEPROM_Driver_IsAddressValid(uint16_t address);

bool EEPROM_Driver_IsRangeValid(uint16_t address,
                                uint16_t length);

#endif /* EEPROM_DRIVER_H */