/*
 * PWM.c
 *
 *  Created on: May 26, 2018
 *      Author: brianvuong
 */

#include "PWM.h"

#include <msp430.h>
#include <timer_a.h>

/*
 * Defines
 */
#define SMCLK_FREQUENCY_HZ          1048576UL   // 1Mhz
#define TIMERA_20MS_PERIOD_TICKS    20972U

/*
 * Global Variables
 */
PWM_S PWM;

/*
 * Private Functions
 */
// Turns input duty cycle into ticks
static uint16_t pwm_dutyCycleToTicks(const float dutyCycle)
{
    return (uint16_t)((TIMERA_20MS_PERIOD_TICKS)*(dutyCycle*0.1));
}

// Turns input time on into ticks
static uint16_t pwm_timeOnToTicks(const float timeOnMS)
{
    // time on = 1.0 ms ticks = 1050
    return (uint16_t) ((timeOnMS)*(0.001)*SMCLK_FREQUENCY_HZ);
}

/*
 * Public Functions
 */

// Initializes TimerA in compare mode to create a PWM output on TA0.1, TA0.2, TA0.3
// with period of 50us, and each "on pulse" of 8us
void PWM_init(void)
{
    WDTCTL      = WDTPW + WDTHOLD;              // Stop WDT
    P1DIR       |= BIT2+BIT3+BIT4;              // P1.2 and P1.3 output
    P1SEL       |= BIT2+BIT3+BIT4;              // P1.2 and P1.3 options select
    TA0CCR0     = 50-1;                         // PWM Period 50us (~8us on/~42us off)
    TA0CCTL1    = OUTMOD_7;                     // CCR1 reset/set
    TA0CCR1     = 8;                            // CCR1 PWM duty cycle
    TA0CCTL2    = OUTMOD_7;                     // CCR2 reset/set
    TA0CCR2     = 8;                            // CCR2 PWM duty cycle
    TA0CCTL3    = OUTMOD_7;                     // CCR3 reset/set
    TA0CCR3     = 8;                            // CCR3 PWM duty cycle
    TA0CTL      = TASSEL_2 + MC_1 + TACLR + ID_3;
}

// Reconfigures Timer A's PWM duty cycle
void PWM_setDutyCycle(double dutyCycle)
{
    TA0CCR1 = pwm_dutyCycleToTicks(dutyCycle);
}

// Reconfigures Timer A's PWM duty cycle in terms on Time On
void PWM_setDutyCycleTimeOn(double timeOnMS)
{
    TA0CCR1 = pwm_timeOnToTicks(timeOnMS);
}
