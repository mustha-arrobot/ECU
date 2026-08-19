#ifndef ECU_PARAMETERS_H
#define ECU_PARAMETERS_H

#include <stdint.h>


/*==========================================================
 * RPM PARAMETERS
 *==========================================================*/

/* Number of pulses generated per revolution */
#define RPM_PULSES_PER_REV              1U


/*==========================================================
 * STARTER PARAMETERS
 *==========================================================*/

#define STARTER_PWM_MIN_DUTY            0U
#define STARTER_PWM_MAX_DUTY            100U

#define STARTER_PWM_PERIOD_US           100U


/*==========================================================
 * THROTTLE PARAMETERS
 *==========================================================*/

#define THROTTLE_MIN_PERCENT            0U
#define THROTTLE_MAX_PERCENT            100U


/*==========================================================
 * TEMPERATURE PARAMETERS
 *==========================================================*/

/*
 * Add the actual sensor-specific conversion
 * parameters here once finalized.
 *
 * Example:
 *
 * #define TEMPERATURE_MIN_C             ...
 * #define TEMPERATURE_MAX_C             ...
 * #define TEMPERATURE_COOLING_ON_C      ...
 */


/*==========================================================
 * PUMP VOLTAGE PARAMETERS
 *==========================================================*/

/*
 * Add actual pump-voltage limits/scaling here.
 *
 * Example:
 *
 * #define PUMP_VOLTAGE_MIN              ...
 * #define PUMP_VOLTAGE_MAX              ...
 */


/*==========================================================
 * BATTERY VOLTAGE PARAMETERS
 *==========================================================*/

/*
 * Add actual battery-voltage limits/scaling here.
 *
 * Example:
 *
 * #define BATTERY_VOLTAGE_MIN           ...
 * #define BATTERY_VOLTAGE_MAX           ...
 */


/*==========================================================
 * BATTERY CURRENT PARAMETERS
 *==========================================================*/

/*
 * Add actual current-sensor parameters here.
 *
 * Example:
 *
 * #define BATTERY_CURRENT_MIN           ...
 * #define BATTERY_CURRENT_MAX           ...
 */


/*==========================================================
 * SAFETY PARAMETERS
 *==========================================================*/

/*
 * ECU safety thresholds will be added here
 * after the actual required values are finalized.
 */


/*==========================================================
 * ECU TIMING PARAMETERS
 *==========================================================*/

/*
 * ECU sequence timing will be added here.
 *
 * Example:
 *
 * #define GLOW_PLUG_TIME_MS             ...
 * #define STARTER_TIMEOUT_MS            ...
 * #define FUEL_PUMP_PRIME_TIME_MS       ...
 */


#endif /* ECU_PARAMETERS_H */