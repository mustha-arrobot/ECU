#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * EXTERNAL EEPROM DRIVER
 *==========================================================
 *
 * Device:
 *     CAT24C256 / compatible 24LC256-class EEPROM
 *
 * Interface:
 *     I2C
 *
 * Responsibilities:
 *     - EEPROM initialization
 *     - Byte read
 *     - Byte write
 *     - Sequential read
 *     - Sequential write
 *
 * This driver handles EEPROM hardware access only.
 *
 * It does NOT:
 *     - Define ECU records
 *     - Decide what data to save
 *     - Manage ECU parameters
 *     - Implement ECU control logic
 *==========================================================
 */


/*----------------------------------------------------------
 * EEPROM Configuration
 *----------------------------------------------------------*/

#define EEPROM_I2C_ADDRESS      EEPROM_DEVICE_ADDR

#define EEPROM_PAGE_SIZE        64U

#define EEPROM_SIZE_BYTES       32768UL

#define EEPROM_LAST_ADDRESS     0x7FFFU


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void EEPROM_Driver_Init(void);


/*----------------------------------------------------------
 * Single Byte Operations
 *----------------------------------------------------------*/

bool EEPROM_Driver_WriteByte(uint16_t address,
                             uint8_t data);

bool EEPROM_Driver_ReadByte(uint16_t address,
                            uint8_t *data);


/*----------------------------------------------------------
 * Sequential Operations
 *----------------------------------------------------------*/

bool EEPROM_Driver_Write(uint16_t address,
                         const uint8_t *data,
                         uint16_t length);

bool EEPROM_Driver_Read(uint16_t address,
                        uint8_t *data,
                        uint16_t length);


/*----------------------------------------------------------
 * EEPROM Status
 *----------------------------------------------------------*/

bool EEPROM_Driver_IsAddressValid(uint16_t address);

bool EEPROM_Driver_IsRangeValid(uint16_t address,
                                uint16_t length);

#endif /* EEPROM_DRIVER_H */