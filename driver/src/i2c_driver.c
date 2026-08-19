#include "i2c_driver.h"


/*==========================================================
 * I2C INITIALIZATION
 *==========================================================*/

void I2C_Driver_Init(void)
{
    /*
     * RC3 = SCL
     * RC4 = SDA
     *
     * MSSP I2C pins are released as inputs.
     */
    I2C_SCL_TRIS = 1;
    I2C_SDA_TRIS = 1;


    /*
     * SSPCON1 = 0x28
     *
     * SSPEN = 1
     * I2C Master Mode
     */
    SSPCON1 = 0x28;


    /*
     * Clear SSPCON2.
     */
    SSPCON2 = 0x00;


    /*
     * Slew-rate disabled for 100 kHz operation.
     */
    SSPSTAT = 0x80;


    /*
     * 100 kHz I2C clock.
     *
     * SSPADD = (Fosc / (4 * Fscl)) - 1
     */
    SSPADD =
        (uint8_t)((_XTAL_FREQ / 4UL) /
                  100000UL - 1UL);
}


/*==========================================================
 * WAIT FOR BUS
 *==========================================================*/

void I2C_Driver_Wait(void)
{
    while((SSPCON2 & 0x1FU) ||
          SSPSTATbits.R_W)
    {
        ;
    }
}


/*==========================================================
 * START
 *==========================================================*/

bool I2C_Driver_Start(void)
{
    I2C_Driver_Wait();

    SSPCON2bits.SEN = 1;

    while(SSPCON2bits.SEN)
    {
        ;
    }

    return true;
}


/*==========================================================
 * REPEATED START
 *==========================================================*/

bool I2C_Driver_RepeatedStart(void)
{
    I2C_Driver_Wait();

    SSPCON2bits.RSEN = 1;

    while(SSPCON2bits.RSEN)
    {
        ;
    }

    return true;
}


/*==========================================================
 * STOP
 *==========================================================*/

bool I2C_Driver_Stop(void)
{
    I2C_Driver_Wait();

    SSPCON2bits.PEN = 1;

    while(SSPCON2bits.PEN)
    {
        ;
    }

    return true;
}


/*==========================================================
 * WRITE ONE BYTE
 *==========================================================*/

bool I2C_Driver_Write(uint8_t data)
{
    I2C_Driver_Wait();

    SSPBUF = data;

    /*
     * Wait for transmission to finish.
     */
    while(SSPSTATbits.BF)
    {
        ;
    }

    I2C_Driver_Wait();

    /*
     * Check ACK status.
     *
     * ACKSTAT = 0 ? ACK received
     * ACKSTAT = 1 ? NACK received
     */
    if(SSPCON2bits.ACKSTAT)
    {
        return false;
    }

    return true;
}


/*==========================================================
 * READ ONE BYTE
 *==========================================================*/

uint8_t I2C_Driver_Read(bool ack)
{
    uint8_t data;


    I2C_Driver_Wait();


    /*
     * Enable receive mode.
     */
    SSPCON2bits.RCEN = 1;


    /*
     * Wait for received byte.
     */
    while(!SSPSTATbits.BF)
    {
        ;
    }


    data = SSPBUF;


    I2C_Driver_Wait();


    /*
     * ACKDT:
     *
     * 0 = ACK
     * 1 = NACK
     *
     * Our API uses:
     *
     * ack = true  ? ACK
     * ack = false ? NACK
     */
    SSPCON2bits.ACKDT =
        (ack ? 0U : 1U);


    SSPCON2bits.ACKEN = 1;

    while(SSPCON2bits.ACKEN)
    {
        ;
    }


    return data;
}


/*==========================================================
 * BUS IDLE
 *==========================================================*/

bool I2C_Driver_IsBusIdle(void)
{
    if((SSPCON2 & 0x1FU) ||
       SSPSTATbits.R_W)
    {
        return false;
    }

    return true;
}


/*==========================================================
 * ACK STATUS
 *==========================================================*/

bool I2C_Driver_IsAcknowledged(void)
{
    return (SSPCON2bits.ACKSTAT == 0U);
}