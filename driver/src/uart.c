/******************************************************************************
 * File: uart_driver.c
 *
 * UART Driver
 *
 * MCU:
 *     PIC18LF2520
 *
 * Hardware:
 *     RC6 -> TX
 *     RC7 -> RX
 *
 * Baud rate:
 *     115200
 *
 ******************************************************************************/

#include "uart.h"
#include "config.h"
#include <stdio.h>
#include <stdint.h>


/*==========================================================
 * UART INITIALIZATION
 *==========================================================*/

void UART_Init(void)
{
    /*
     * RC6 = UART TX
     * RC7 = UART RX
     */

    UART_TX_TRIS = 0;
    UART_RX_TRIS = 1;


    /*
     * High-speed asynchronous UART.
     */

    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;


    /*
     * Enable 16-bit Baud Rate Generator.
     */

    BAUDCONbits.BRG16 = 1;


    /*
     * Baud-rate calculation:
     *
     * SPBRG = Fosc / (4 × Baud) - 1
     *
     * At:
     *
     * Fosc  = 20 MHz
     * Baud  = 115200
     *
     * SPBRG ? 42
     */

    SPBRGH = 0;
    SPBRG = 42;


    /*
     * Enable transmitter.
     */

    TXSTAbits.TXEN = 1;


    /*
     * Enable asynchronous serial port.
     */

    RCSTAbits.SPEN = 1;


    /*
     * Enable receiver.
     */

    RCSTAbits.CREN = 1;


    /*
     * Clear receive flag.
     */

    PIR1bits.RCIF = 0;
}


/*==========================================================
 * WRITE CHARACTER
 *==========================================================*/

void UART_WriteChar(char data)
{
    /*
     * Wait until TX register is available.
     */

    while(!PIR1bits.TXIF)
    {
        ;
    }

    TXREG = data;
}


/*==========================================================
 * WRITE STRING
 *==========================================================*/

void UART_WriteString(const char *str)
{
    if(str == 0)
    {
        return;
    }

    while(*str != '\0')
    {
        UART_WriteChar(*str);

        str++;
    }
}


/*==========================================================
 * WRITE LINE
 *==========================================================*/

void UART_WriteLine(const char *str)
{
    UART_WriteString(str);

    UART_WriteChar('\r');
    UART_WriteChar('\n');
}


/*==========================================================
 * CHECK RECEIVE DATA
 *==========================================================*/

bool UART_DataReady(void)
{
    return (PIR1bits.RCIF != 0);
}


/*==========================================================
 * READ CHARACTER
 *==========================================================*/

char UART_ReadChar(void)
{
    /*
     * Wait for received data.
     */

    while(!PIR1bits.RCIF)
    {
        ;
    }


    /*
     * Handle overrun condition.
     */

    if(RCSTAbits.OERR)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }


    return RCREG;
}


/*==========================================================
 * WRITE INTEGER
 *==========================================================*/

void UART_WriteInt(int value)
{
    char buffer[8];

    sprintf(buffer, "%d", value);

    UART_WriteString(buffer);
}


/*==========================================================
 * WRITE UNSIGNED INTEGER
 *==========================================================*/

void UART_WriteUInt(unsigned int value)
{
    char buffer[8];

    sprintf(buffer, "%u", value);

    UART_WriteString(buffer);
}


/*==========================================================
 * WRITE UNSIGNED LONG
 *==========================================================*/

void UART_WriteLong(unsigned long value)
{
    char buffer[16];

    sprintf(buffer, "%lu", value);

    UART_WriteString(buffer);
}


/*==========================================================
 * WRITE FLOAT
 *==========================================================*/

void UART_WriteFloat(float value,
                            uint8_t decimalPlaces)
{
    char buffer[32];

    /*
     * XC8 printf/sprintf floating-point support may require
     * additional compiler configuration.
     *
     * This implementation uses sprintf for simplicity.
     */

    switch(decimalPlaces)
    {
        case 0:
            sprintf(buffer, "%.0f", value);
            break;

        case 1:
            sprintf(buffer, "%.1f", value);
            break;

        case 2:
            sprintf(buffer, "%.2f", value);
            break;

        case 3:
            sprintf(buffer, "%.3f", value);
            break;

        default:
            sprintf(buffer, "%.2f", value);
            break;
    }

    UART_WriteString(buffer);
}


/*==========================================================
 * CLEAR TERMINAL
 *==========================================================*/

void UART_ClearScreen(void)
{
    UART_WriteString("\033[2J");
}


/*==========================================================
 * TERMINAL HOME
 *==========================================================*/

void UART_Home(void)
{
    UART_WriteString("\033[H");
}