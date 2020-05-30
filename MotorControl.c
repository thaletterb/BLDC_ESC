/*
 * MotorControl.c
 *
 *  Created on: Jun 16, 2018
 *      Author: brianvuong
 */

#include "MotorControl.h"
#include "PWM.h"

#include <string.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/cfg/global.h>


#include "board.h"

// TODO!
#include <msp430.h>
#include <timer_a.h>
#include "Utility.h"

/*
 * Typedefs
 */
typedef struct mc_s
{
    double  pulseTimeOnMS;
    int     stateTimer50hz;

    uint8_t index;
}mc_s;

typedef enum
{
    MC_STEP_0,
    MC_STEP_1,
    MC_STEP_2,
    MC_STEP_3,
    MC_STEP_4,
    MC_STEP_5,

    MC_NUM_STEPS
} mc_steps_t;

/*
 * Global Variables
 */
MC_S MC;

/*
 * Local Variables
 */
mc_s mc;

/*
 * Private Functions
 */
static void zeroCrossingComparatorInit(void)
{
    // TODO: Move this somewhere more appropriate
    // void COMPB_init();
    {
        CBCTL0 |= (CBIMEN | CBIPEN);                                        // Enable (-) and (+) terminal to the comparator
        CBCTL0 &= ~(CBIPSEL3 | CBIPSEL2 | CBIPSEL1 | CBIPSEL0);             // (+) to CB0/P6.0
        CBCTL0 |= (CBIMSEL0);                                               // (-) to CB1/P6.1

        //CBCTL1 |= (CBF);                                                    // Filter CBOUT
        CBCTL1 &= ~(CBIES);                                                 // Rising edge for CBIFG

        CBINT &= ~(CBIFG);                                                  // Clear any errant flags

        CBCTL1 |= (CBON);                                                   // Turn comparator on
    }
}
/*
 * Commutation functions
 * Step:            Active Switches:
 * -----            ----------------
 * 0                PH1 HI, PH2 LO
 * 1                PH1 HI, PH3 LO
 * 2                PH2 HI, PH3 LO
 * 3                PH2 HI, PH1 LO
 * 4                PH3 HI, PH1 LO
 * 5                PH3 HI, PH2 LO
 */
static void phaseOneOff(void)
{
    GPIO_write(Board_PH1_EN, 0);
    GPIO_write(Board_PH1_IN, 0);
}

static void phaseTwoOff(void)
{
    GPIO_write(Board_PH2_EN, 0);
    GPIO_write(Board_PH2_IN, 0);
}

static void phaseThreeOff(void)
{
    GPIO_write(Board_PH3_EN, 0);
    GPIO_write(Board_PH3_IN, 0);
}

static void allOff(void)
{
    GPIO_write(Board_PH1_EN, 0);
    GPIO_write(Board_PH2_EN, 0);
    GPIO_write(Board_PH3_EN, 0);

    GPIO_write(Board_PH1_IN, 0);
    GPIO_write(Board_PH2_IN, 0);
    GPIO_write(Board_PH3_IN, 0);
}

// PH1 HI, PH2 LO
static void step0(void)
{
    phaseThreeOff();

    // Disable PWM on IN2/IN3
    TA0CCR1     = timer2_periodUsToCounts(PWM_DUTY_CYCLE);
    TA0CCR2     = 0xFFFF;
    TA0CCR3     = 0x0U;

    GPIO_write(Board_PH1_EN, 1);
    GPIO_write(Board_PH2_EN, 1);

    GPIO_write(Board_PH1_IN, 1);
    GPIO_write(Board_PH2_IN, 0);

    // C Falling -> CBOUT rising edge
    CBCTL0 &= ~(CBIMSEL3 | CBIMSEL2 | CBIMSEL1 | CBIMSEL0);             // (-) to CB3/P6.3
    CBCTL0 |= (CBIMSEL1 | CBIMSEL0);

    CBCTL1 &= ~(CBIES);                                                 // Rising edge for CBIFG

    CBINT &= ~(CBIFG);                                                  // Clear any errant flags
}

// PH1 HI, PH3 LO
static void step1(void)
{
    phaseTwoOff();

    // Disable PWM on IN2/IN3
    TA0CCR1     = timer2_periodUsToCounts(PWM_DUTY_CYCLE);
    TA0CCR2     = 0x0U;
    TA0CCR3     = 0xFFFF;

    GPIO_write(Board_PH1_EN, 1);
    GPIO_write(Board_PH3_EN, 1);

    GPIO_write(Board_PH1_IN, 1);
    GPIO_write(Board_PH3_IN, 0);

    // B Rising - CBOUT Falling edge
    CBCTL0 &= ~(CBIMSEL3 | CBIMSEL2 | CBIMSEL1 | CBIMSEL0);             // (-) to CB2/P6.2
    CBCTL0 |= (CBIMSEL1);

    CBCTL1 |= (CBIES);                                                  // Falling edge for CBIFG

    CBINT &= ~(CBIFG);                                                  // Clear any errant flags
}

// PH2 HI, PH3 LO
static void step2(void)
{
    phaseOneOff();

    // Disable PWM on IN1/IN3
    TA0CCR1     = 0x0;
    TA0CCR2     = timer2_periodUsToCounts(PWM_DUTY_CYCLE);
    TA0CCR3     = 0xFFFF;

    GPIO_write(Board_PH2_EN, 1);
    GPIO_write(Board_PH3_EN, 1);

    GPIO_write(Board_PH2_IN, 1);
    GPIO_write(Board_PH3_IN, 0);

    // A Falling - CBOUT rising edge
    CBCTL0 &= ~(CBIMSEL3 | CBIMSEL2 | CBIMSEL1 | CBIMSEL0);             // (-) to CB1/P6.1
    CBCTL0 |= (CBIMSEL0);

    CBCTL1 &= ~(CBIES);                                                 // Rising edge for CBIFG

    CBINT &= ~(CBIFG);                                                  // Clear any errant flags
}

// PH2 HI, PH1 LO
static void step3(void)
{
    phaseThreeOff();

    // Disable PWM on IN1/IN3
    TA0CCR1     = 0xFFFF;
    TA0CCR2     = timer2_periodUsToCounts(PWM_DUTY_CYCLE);
    TA0CCR3     = 0x0U;

    GPIO_write(Board_PH2_EN, 1);
    GPIO_write(Board_PH1_EN, 1);

    GPIO_write(Board_PH2_IN, 1);
    GPIO_write(Board_PH1_IN, 0);

    // C Rising - CBOUT Falling edge
    CBCTL0 &= ~(CBIMSEL3 | CBIMSEL2 | CBIMSEL1 | CBIMSEL0);             // (-) to CB3/P6.3
    CBCTL0 |= (CBIMSEL1 | CBIMSEL0);

    CBCTL1 |= (CBIES);                                                  // Falling edge for CBIFG

    CBINT &= ~(CBIFG);                                                  // Clear any errant flags
}

// PH3 HI, PH1 LO
static void step4(void)
{
    phaseTwoOff();

    // Disable PWM on IN1/IN2
    TA0CCR1     = 0xFFFF;
    TA0CCR2     = 0x0U;
    TA0CCR3     = timer2_periodUsToCounts(PWM_DUTY_CYCLE);

    GPIO_write(Board_PH3_EN, 1);
    GPIO_write(Board_PH1_EN, 1);

    GPIO_write(Board_PH3_IN, 1);
    GPIO_write(Board_PH1_IN, 0);

    // B Falling - CBOUT rising edge
    CBCTL0 &= ~(CBIMSEL3 | CBIMSEL2 | CBIMSEL1 | CBIMSEL0);             // (-) to CB2/P6.2
    CBCTL0 |= (CBIMSEL1);

    CBCTL1 &= ~(CBIES);                                                 // Rising edge for CBIFG

    CBINT &= ~(CBIFG);                                                  // Clear any errant flags
}

// PH3 HI, PH2 LO
static void step5(void)
{
    phaseOneOff();

    // Disable PWM on IN1/IN2
    TA0CCR1     = 0x0U;
    TA0CCR2     = 0xFFFF;
    TA0CCR3     = timer2_periodUsToCounts(PWM_DUTY_CYCLE);

    GPIO_write(Board_PH3_EN, 1);
    GPIO_write(Board_PH2_EN, 1);

    GPIO_write(Board_PH3_IN, 1);
    GPIO_write(Board_PH2_IN, 0);

    // A Rising - CBOUT Falling edge
    CBCTL0 &= ~(CBIMSEL3 | CBIMSEL2 | CBIMSEL1 | CBIMSEL0);             // (-) to CB1/P6.1
    CBCTL0 |= (CBIMSEL0);

    CBCTL1 |= (CBIES);                                                  // Falling edge for CBIFG

    CBINT &= ~(CBIFG);                                                  // Clear any errant flags
}

// Should only be called on transition between MC.states
static void mc_sendPulseToESC(void)
{
    PWM_setDutyCycleTimeOn(mc.pulseTimeOnMS);
}


// Process state machine
static void mc_processStates_50Hz(void)
{
    switch(MC.state)
    {
        case MC_ESC_ARMING:
            if(mc.stateTimer50hz++ == 250)
            {
                MC.state = MC_ESC_ARMED;

                mc.stateTimer50hz = 0;
                mc.pulseTimeOnMS = 1.10;
                mc_sendPulseToESC();
            }
            break;
        case MC_ESC_ARMED:
                mc_sendPulseToESC();
            break;
        case MC_ESC_DISABLED:
        default:
            break;
    }
}


/*
 * Public Functions
 */
void MC_init(void)
{
    memset(&MC, 0, sizeof(MC));
    MC.state = MC_ESC_ARMING;
    zeroCrossingComparatorInit();
}

void MC_setPulseWidth(float timeOnMS)
{
    mc.pulseTimeOnMS = timeOnMS;
}
static void bldc_move(void)
{
    switch(mc.index)
    {
        case 0:
            step0();
            break;
        case 1:
            step1();
            break;
        case 2:
            step2();
            break;
        case 3:
            step3();
            break;
        case 4:
            step4();
            break;
        case 5:
            step5();
            break;
    }
    mc.index++;
    mc.index %=6;

}

void MC_50Hz_CLK(void)
{
}

void TA2_CCR0_ISR(void)
{
    static uint32_t rampStepUs = 10000;

    if(rampStepUs > 100)
    {
        bldc_move();
        TA2CCR0 = timer2_periodUsToCounts(rampStepUs);
        rampStepUs -= 200;
    }
    else
    {
        //bldc_move();
        CBINT |= (CBIE);
    }
    GPIO_toggle(Board_LED1);
}

void zero_crossing_ISR(void)
{
    uint8_t i;

    for(i = 0; i < 150; i++) {
      if(mc.index & 1){  // Odd steps look for falling BEMF
        if(!(CBCTL1 & CBOUT)) i -= 1;    // CBOUT not set
      }
      else {  // Even steps look for rising BEMF
        if((CBCTL1 & CBOUT))  i -= 1;    // CBOUT set
      }
    }
    bldc_move();
    GPIO_toggle(Board_LED0);
    CBINT &= ~(CBIFG);
}

