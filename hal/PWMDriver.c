/*
 * PWMDriver.c
 *
 *  Created on: Aug 16, 2018
 *      Author: brianvuong
 */

#include "IPWM.h"

#include "PWMDriver.h"

#include <msp430.h>
#include <timer_a.h>

#include <string.h>
/*
 * Defines
 */
#define SMCLK_FREQUENCY_HZ          1048576UL   // 1Mhz
#define TIMERA_20MS_PERIOD_TICKS    20972U

/*
 * Global Variables
 */
PWM_S PWM;              // PWM interface global variable
PwmDriver_S PwmDriver;  // PwmDriver

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
// Enables the PWM module
void PWM_enable(void)
{
    // Disables any overiding states
    PWM.bit.enabled = 1U;
}

// Disables the PWM module
void PWM_disable(void)
{
    // Overrides PWM outputs into all off
    PWM.bit.enabled = 0U;
}


void PWM_init(void)
{
    PwmDriver_init();

    memset(&PWM, 0, sizeof(PWM));
    memset(&PwmDriver, 0, sizeof(PwmDriver));
}

// Initializes TimerA in compare mode to create a PWM output on TA0.1
// P1.2 with period of 20ms and an initial duty cycle of 2.5% (0.5ms period)
void PwmDriver_init(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P1DIR |= BIT2+BIT3;                       // P1.2 and P1.3 output
    P1SEL |= BIT2+BIT3;                       // P1.2 and P1.3 options select
    TA0CCR0 = TIMERA_20MS_PERIOD_TICKS-1;     // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = 525;                            // CCR1 PWM duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TACLR + ID_3;
}

// Reconfigures Timer A's PWM duty cycle
void PwmDriver_setDutyCycle(double dutyCycle)
{
    TA0CCR1 = pwm_dutyCycleToTicks(dutyCycle);
}

// Reconfigures Timer A's PWM duty cycle in terms on Time On
void PwmDriver_setDutyCycleTimeOn(double timeOnMS)
{
    TA0CCR1 = pwm_timeOnToTicks(timeOnMS);
}
