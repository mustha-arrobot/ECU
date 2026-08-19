#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/*
 *==========================================================
 * UART DRIVER
 *==========================================================
 *
 * MCU:
 *     PIC18LF2520
 *
 * Hardware:
 *     RC6 / TX
 *     RC7 / RX
 *
 * Configuration:
 *     UART_BAUDRATE
 *
 * Responsibilities:
 *     - UART initialization
 *     - Character transmission
 *     - String transmission
 *     - Character reception
 *     - Receive-data availability
 *     - Numeric formatting
 *     - Basic terminal control
 *
 * This driver handles UART hardware and communication
 * primitives only.
 *
 * It does NOT:
 *     - Interpret ECU commands
 *     - Control actuators
 *     - Implement ECU state logic
 *==========================================================
 */


/*----------------------------------------------------------
 * Initialization
 *----------------------------------------------------------*/

void UART_Init(void);


/*----------------------------------------------------------
 * Transmit
 *----------------------------------------------------------*/

void UART_WriteChar(char data);

void UART_WriteString(const char *str);

void UART_WriteLine(const char *str);


/*----------------------------------------------------------
 * Receive
 *----------------------------------------------------------*/

char UART_ReadChar(void);

bool UART_DataReady(void);


/*----------------------------------------------------------
 * Number Printing
 *----------------------------------------------------------*/

void UART_WriteInt(int value);

void UART_WriteUInt(unsigned int value);

void UART_WriteLong(unsigned long value);

void UART_WriteFloat(float value,
                            uint8_t decimalPlaces);


/*----------------------------------------------------------
 * Terminal Functions
 *----------------------------------------------------------*/

void UART_ClearScreen(void);

void UART_Home(void);


#endif /* UART_DRIVER_H */