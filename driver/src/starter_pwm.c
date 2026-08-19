/******************************************************************************
 * File: starter_pwm.c
 *
 * Starter Software PWM Driver
 *
 * Hardware:
 *     RC0
 *
 * PWM:
 *     Software generated
 *
 ******************************************************************************/

#include "starter_pwm.h"


/*==========================================================
 * INTERNAL STATE
 *==========================================================*/

static uint8_t dutyCycle = 0U;
static uint8_t pwmCounter = 0U;


/*==========================================================
 * INITIALIZATION
 *==========================================================*/

void StarterPWM_Init(void)
{
    /*
     * RC0 = output
     */
    STARTER_TRIS = 0;


    /*
     * Starter OFF
     */
    STARTER_LAT = 0;


    /*
     * Initial PWM state
     */
    dutyCycle = 0U;
    pwmCounter = 0U;
}


/*==========================================================
 * SET DUTY CYCLE
 *
 * Valid range:
 *     0 - 100 %
 *==========================================================*/

void StarterPWM_SetDuty(uint8_t duty)
{
    if(duty > 100U)
    {
        duty = 100U;
    }

    dutyCycle = duty;
}


/*==========================================================
 * GET DUTY CYCLE
 *==========================================================*/

uint8_t StarterPWM_GetDuty(void)
{
    return dutyCycle;
}


/*==========================================================
 * STOP PWM
 *==========================================================*/

void StarterPWM_Stop(void)
{
    dutyCycle = 0U;
    pwmCounter = 0U;

    STARTER_LAT = 0;
}


/*==========================================================
 * SOFTWARE PWM TASK
 *==========================================================*/

void StarterPWM_Task(void)
{
    /*
     * Increment PWM counter.
     */
    pwmCounter++;


    /*
     * PWM period = 100 counts.
     */
    if(pwmCounter >= 100U)
    {
        pwmCounter = 0U;
    }


    /*
     * Duty = 0%
     */
    if(dutyCycle == 0U)
    {
        STARTER_LAT = 0;
    }


    /*
     * Duty > 0%
     */
    else if(pwmCounter < dutyCycle)
    {
        STARTER_LAT = 1;
    }


    /*
     * Remaining PWM period.
     */
    else
    {
        STARTER_LAT = 0;
    }
}