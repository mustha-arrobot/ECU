/************************************************************
 * File: tempeture_test_main.c
 *
 * ECU COMPLETE SENSOR + OUTPUT TEST
 *
 * MCU:
 *     PIC18LF2520
 *
 * SENSORS:
 *     Temperature
 *     Throttle
 *     Pump Voltage
 *     Battery Voltage
 *     Battery Current
 *     RPM
 *
 * OUTPUTS:
 *     Fuel Valve
 *     Gas Valve
 *     Fuel Pump
 *     Glow Plug
 *     Starter
 *     Starter Enable
 *     Status LED
 *
 * TEST:
 *     1. Display all sensor values
 *     2. Test each output ON/OFF
 *     3. Test starter PWM
 ************************************************************/

#include <xc.h>
#include <stdint.h>

#include "config.h"

/*==========================================================
 * SENSOR DRIVERS
 *==========================================================*/

#include "adc_driver.h"
#include "temperature.h"
#include "throttle.h"
#include "pump_voltage.h"
#include "battery_current.h"
#include "rpm.h"

/*==========================================================
 * OUTPUT DRIVERS
 *==========================================================*/

#include "gpio_driver.h"
#include "output.h"
#include "fuel_pump.h"
#include "starter_pwm.h"

/*==========================================================
 * SYSTEM DRIVERS
 *==========================================================*/

#include "uart.h"
#include "timer0.h"


/*==========================================================
 * MAIN
 *==========================================================*/

void main(void)
{
    /*------------------------------------------------------
     * Sensor variables
     *------------------------------------------------------*/

    uint16_t temperature_adc;
    float temperature;

    uint16_t throttle_adc;
    float throttle_voltage;
    uint8_t throttle_percent;

    uint16_t pump_voltage_adc;
    float pump_voltage;

    uint16_t battery_voltage_adc;
    float battery_voltage;

    uint16_t battery_current_adc;
    float battery_current;

    uint32_t rpm;


    /*======================================================
     * INITIALIZATION
     *======================================================*/

    UART_Init();

    UART_WriteLine("");
    UART_WriteLine("========================================");
    UART_WriteLine("       ECU SENSOR + OUTPUT TEST");
    UART_WriteLine("========================================");
    UART_WriteLine("");

    /*------------------------------------------------------
     * GPIO
     *------------------------------------------------------*/

    GPIO_Driver_Init();

    /*------------------------------------------------------
     * ADC
     *------------------------------------------------------*/

    ADC_Driver_Init();

    /*------------------------------------------------------
     * Sensors
     *------------------------------------------------------*/

    Temperature_Init();

    Throttle_Init();

    PumpVoltage_Init();

    Battery_Init();

    /*------------------------------------------------------
     * Timer0
     *------------------------------------------------------*/

    Timer0_Init();

    /*------------------------------------------------------
     * Starter PWM
     *------------------------------------------------------*/

    StarterPWM_Init();

    /*------------------------------------------------------
     * RPM
     *------------------------------------------------------*/

    RPM_Init();

    /*------------------------------------------------------
     * Output driver
     *------------------------------------------------------*/

    OUTPUT_Init();

    FuelPump_Init();


    /*======================================================
     * SAFETY
     *======================================================*/

    OUTPUT_AllOff();

    FuelPump_Stop();

    StarterPWM_Stop();

    GPIO_StarterEnable_Set(OFF);

    GPIO_LED_Set(OFF);


    /*======================================================
     * INITIALIZATION COMPLETE
     *======================================================*/

    UART_WriteLine("System Initialized");
    UART_WriteLine("All Outputs OFF");
    UART_WriteLine("");

    UART_WriteLine("Sensor Inputs:");
    UART_WriteLine("Temperature : AN0 / RA0");
    UART_WriteLine("Pump Voltage: AN1 / RA1");
    UART_WriteLine("Battery Volt: AN2 / RA2");
    UART_WriteLine("Battery Curr: AN4 / RA5");
    UART_WriteLine("Throttle    : AN12 / RB0");
    UART_WriteLine("RPM         : RB2 / INT2");

    UART_WriteLine("");

    UART_WriteLine("Output Test Starting...");
    UART_WriteLine("");

    __delay_ms(1000);


    /*======================================================
     * MAIN LOOP
     *======================================================*/

    while(1)
    {

        /*==================================================
         * READ TEMPERATURE
         *==================================================*/

        temperature_adc =
            Temperature_ReadADC();

        temperature =
            Temperature_ReadCelsius();


        /*==================================================
         * READ THROTTLE
         *==================================================*/

        throttle_adc =
            Throttle_ReadADC();

        throttle_voltage =
            Throttle_ReadVoltage();

        throttle_percent =
            Throttle_ReadPercent();


        /*==================================================
         * READ PUMP VOLTAGE
         *==================================================*/

        pump_voltage_adc =
            PumpVoltage_ReadADC();

        pump_voltage =
            PumpVoltage_ReadVoltage();


        /*==================================================
         * READ BATTERY VOLTAGE
         *==================================================*/

        battery_voltage_adc =
            BatteryVoltage_ReadADC();

        battery_voltage =
            BatteryVoltage_ReadVoltage();


        /*==================================================
         * READ BATTERY CURRENT
         *==================================================*/

        battery_current_adc =
            BatteryCurrent_ReadADC();

        battery_current =
            BatteryCurrent_ReadCurrent();


        /*==================================================
         * READ RPM
         *==================================================*/

        rpm =
            RPM_Read();


        /*==================================================
         * DISPLAY SENSOR DATA
         *==================================================*/

        UART_WriteLine("");
        UART_WriteLine("========================================");
        UART_WriteLine("             SENSOR DATA");
        UART_WriteLine("========================================");


        /*--------------------------------------------------
         * Temperature
         *--------------------------------------------------*/

        UART_WriteString("Temperature ADC : ");
        UART_WriteUInt(temperature_adc);
        UART_WriteLine("");

        UART_WriteString("Temperature     : ");
        UART_WriteFloat(temperature, 2);
        UART_WriteLine(" C");


        /*--------------------------------------------------
         * Throttle
         *--------------------------------------------------*/

        UART_WriteString("Throttle ADC    : ");
        UART_WriteUInt(throttle_adc);
        UART_WriteLine("");

        UART_WriteString("Throttle Voltage: ");
        UART_WriteFloat(throttle_voltage, 2);
        UART_WriteLine(" V");

        UART_WriteString("Throttle        : ");
        UART_WriteUInt(throttle_percent);
        UART_WriteLine(" %");


        /*--------------------------------------------------
         * Pump Voltage
         *--------------------------------------------------*/

        UART_WriteString("Pump ADC        : ");
        UART_WriteUInt(pump_voltage_adc);
        UART_WriteLine("");

        UART_WriteString("Pump Voltage    : ");
        UART_WriteFloat(pump_voltage, 2);
        UART_WriteLine(" V");


        /*--------------------------------------------------
         * Battery Voltage
         *--------------------------------------------------*/

        UART_WriteString("Battery V ADC   : ");
        UART_WriteUInt(battery_voltage_adc);
        UART_WriteLine("");

        UART_WriteString("Battery Voltage : ");
        UART_WriteFloat(battery_voltage, 2);
        UART_WriteLine(" V");


        /*--------------------------------------------------
         * Battery Current
         *--------------------------------------------------*/

        UART_WriteString("Battery I ADC   : ");
        UART_WriteUInt(battery_current_adc);
        UART_WriteLine("");

        UART_WriteString("Battery Current : ");
        UART_WriteFloat(battery_current, 2);
        UART_WriteLine(" A");


        /*--------------------------------------------------
         * RPM
         *--------------------------------------------------*/

        UART_WriteString("RPM             : ");
        UART_WriteLong((long)rpm);
        UART_WriteLine(" RPM");


        /*==================================================
         * OUTPUT TEST
         *==================================================*/

        UART_WriteLine("");
        UART_WriteLine("----------------------------------------");
        UART_WriteLine("           OUTPUT TEST");
        UART_WriteLine("----------------------------------------");


        /*==================================================
         * 1. FUEL VALVE
         *==================================================*/

        UART_WriteLine("Fuel Valve ON");

        GPIO_FuelValve_Set(ON);

        __delay_ms(1000);

        UART_WriteLine("Fuel Valve OFF");

        GPIO_FuelValve_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * 2. GAS VALVE
         *==================================================*/

        UART_WriteLine("Gas Valve ON");

        GPIO_GasValve_Set(ON);

        __delay_ms(1000);

        UART_WriteLine("Gas Valve OFF");

        GPIO_GasValve_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * 3. FUEL PUMP
         *==================================================*/

        UART_WriteLine("Fuel Pump ON");

        GPIO_FuelPump_Set(ON);

        __delay_ms(1000);

        UART_WriteLine("Fuel Pump OFF");

        GPIO_FuelPump_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * 4. GLOW PLUG
         *==================================================*/

        UART_WriteLine("Glow Plug ON");

        GPIO_Glow_Set(ON);

        __delay_ms(1000);

        UART_WriteLine("Glow Plug OFF");

        GPIO_Glow_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * 5. STARTER ENABLE
         *==================================================*/

        UART_WriteLine("Starter Enable ON");

        GPIO_StarterEnable_Set(ON);

        __delay_ms(1000);

        UART_WriteLine("Starter Enable OFF");

        GPIO_StarterEnable_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * 6. STARTER PWM
         *==================================================*/

        UART_WriteLine("Starter PWM 25%");

        GPIO_StarterEnable_Set(ON);

        StarterPWM_SetDuty(25);

        __delay_ms(2000);


        UART_WriteLine("Starter PWM 50%");

        StarterPWM_SetDuty(50);

        __delay_ms(2000);


        UART_WriteLine("Starter PWM 75%");

        StarterPWM_SetDuty(75);
        __delay_ms(2000);


        UART_WriteLine("Starter PWM OFF");

        StarterPWM_Stop();

        GPIO_StarterEnable_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * 7. STATUS LED
         *==================================================*/

        UART_WriteLine("Status LED ON");

        GPIO_LED_Set(ON);

        __delay_ms(1000);

        UART_WriteLine("Status LED OFF");

        GPIO_LED_Set(OFF);

        __delay_ms(500);


        /*==================================================
         * ALL OUTPUTS OFF
         *==================================================*/

        OUTPUT_AllOff();

        FuelPump_Stop();

        StarterPWM_Stop();

        GPIO_StarterEnable_Set(OFF);

        GPIO_LED_Set(OFF);


        UART_WriteLine("");
        UART_WriteLine("ALL OUTPUTS OFF");
        UART_WriteLine("----------------------------------------");

        __delay_ms(2000);
    }
}