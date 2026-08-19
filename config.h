#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#include <xc.h>
#include <stdint.h>

/*==========================================================
 * MCU
 *==========================================================*/

#define _XTAL_FREQ          20000000UL


/*==========================================================
 * CONFIGURATION BITS
 *==========================================================*/

#pragma config OSC     = HS
#pragma config WDT     = OFF
#pragma config LVP     = OFF
#pragma config PBADEN  = OFF
#pragma config MCLRE   = ON
#pragma config BOREN   = OFF
#pragma config PWRT    = ON
#pragma config DEBUG   = OFF
#pragma config CCP2MX  = PORTC
#pragma config STVREN  = ON

#pragma config CP0     = OFF
#pragma config CP1     = OFF
#pragma config CPB     = OFF

#pragma config WRT0    = OFF
#pragma config WRT1    = OFF
#pragma config WRTB    = OFF

#pragma config EBTR0   = OFF
#pragma config EBTR1   = OFF


/*==========================================================
 * LOGIC LEVELS
 *==========================================================*/

#define ON              1U
#define OFF             0U

#define GPIO_HIGH       1U
#define GPIO_LOW        0U


/*==========================================================
 * ADC INPUTS
 *==========================================================*/

/* Temperature : RA0 / AN0 */
#define TEMP_CHANNEL        0U

/* Pump Voltage : RA1 / AN1 */
#define PUMP_VOLT_CHANNEL   1U

/* Battery Voltage : RA2 / AN2 */
#define BATT_VOLT_CHANNEL   2U

/* Battery Current : RA5 / AN4 */
#define BATT_CURR_CHANNEL   4U

/* Throttle : RB0 / AN12 */
#define THROTTLE_CHANNEL    12U


/*==========================================================
 * RPM INPUT
 * RB2 / INT2
 *==========================================================*/

#define RPM_TRIS            TRISBbits.TRISB2
#define RPM_PORT            PORTBbits.RB2


/*==========================================================
 * FUEL VALVE
 * RB4
 * Active HIGH
 *==========================================================*/

#define FUEL_VALVE_TRIS     TRISBbits.TRISB4
#define FUEL_VALVE_LAT      LATBbits.LATB4


/*==========================================================
 * GAS VALVE
 * RB5
 * Active HIGH
 *==========================================================*/

#define GAS_VALVE_TRIS      TRISBbits.TRISB5
#define GAS_VALVE_LAT       LATBbits.LATB5


/*==========================================================
 * FUEL PUMP
 * RC2
 * Active LOW
 *==========================================================*/

#define PUMP_TRIS           TRISCbits.TRISC2
#define PUMP_LAT            LATCbits.LATC2


/*==========================================================
 * GLOW PLUG
 * RC1
 * Active LOW
 *==========================================================*/

#define GLOW_TRIS           TRISCbits.TRISC1
#define GLOW_LAT            LATCbits.LATC1


/*==========================================================
 * STARTER
 * RC0
 *==========================================================*/

#define STARTER_TRIS        TRISCbits.TRISC0
#define STARTER_LAT         LATCbits.LATC0


/*==========================================================
 * STARTER ENABLE
 * RC5
 * Active HIGH
 *==========================================================*/

#define STARTER_EN_TRIS     TRISCbits.TRISC5
#define STARTER_EN_LAT      LATCbits.LATC5


/*==========================================================
 * STATUS LED
 * RA4
 * Active LOW
 *==========================================================*/

#define LED_TRIS            TRISAbits.TRISA4
#define LED_LAT             LATAbits.LATA4


/*==========================================================
 * UART
 *==========================================================*/

#define UART_BAUDRATE       115200UL

#define UART_TX_TRIS        TRISCbits.TRISC6
#define UART_RX_TRIS        TRISCbits.TRISC7


/*==========================================================
 * I2C
 *==========================================================*/

#define I2C_SCL_TRIS        TRISCbits.TRISC3
#define I2C_SDA_TRIS        TRISCbits.TRISC4


/*==========================================================
 * EXTERNAL EEPROM
 *==========================================================*/

#define EEPROM_DEVICE_ADDR  0xA0


#endif /* HARDWARE_CONFIG_H */