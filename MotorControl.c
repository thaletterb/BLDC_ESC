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
#include "board.h"

// TODO!
#include <msp430.h>
#include <timer_a.h>

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
    TA0CCR1     = 8U;
    TA0CCR2     = 0U;
    TA0CCR3     = 0U;

    GPIO_write(Board_PH1_EN, 1);
    GPIO_write(Board_PH2_EN, 1);

    GPIO_write(Board_PH1_IN, 1);
    GPIO_write(Board_PH2_IN, 0);
}

// PH1 HI, PH3 LO
static void step1(void)
{
    phaseTwoOff();

    // Disable PWM on IN2/IN3
    TA0CCR1     = 8U;
    TA0CCR2     = 0U;
    TA0CCR3     = 0U;

    GPIO_write(Board_PH1_EN, 1);
    GPIO_write(Board_PH3_EN, 1);

    GPIO_write(Board_PH1_IN, 1);
    GPIO_write(Board_PH3_IN, 0);
}

// PH2 HI, PH3 LO
static void step2(void)
{
    phaseOneOff();

    // Disable PWM on IN1/IN3
    TA0CCR1     = 0U;
    TA0CCR2     = 8U;
    TA0CCR3     = 0U;

    GPIO_write(Board_PH2_EN, 1);
    GPIO_write(Board_PH3_EN, 1);

    GPIO_write(Board_PH2_IN, 1);
    GPIO_write(Board_PH3_IN, 0);
}

// PH2 HI, PH1 LO
static void step3(void)
{
    phaseThreeOff();

    // Disable PWM on IN1/IN3
    TA0CCR1     = 0U;
    TA0CCR2     = 8U;
    TA0CCR3     = 0U;

    GPIO_write(Board_PH2_EN, 1);
    GPIO_write(Board_PH1_EN, 1);

    GPIO_write(Board_PH2_IN, 1);
    GPIO_write(Board_PH1_IN, 0);
}

// PH3 HI, PH1 LO
static void step4(void)
{
    phaseTwoOff();

    // Disable PWM on IN1/IN2
    TA0CCR1     = 0U;
    TA0CCR2     = 0U;
    TA0CCR3     = 8U;

    GPIO_write(Board_PH3_EN, 1);
    GPIO_write(Board_PH1_EN, 1);

    GPIO_write(Board_PH3_IN, 1);
    GPIO_write(Board_PH1_IN, 0);
}

// PH3 HI, PH2 LO
static void step5(void)
{
    phaseOneOff();

    // Disable PWM on IN1/IN2
    TA0CCR1     = 0U;
    TA0CCR2     = 0U;
    TA0CCR3     = 8U;

    GPIO_write(Board_PH3_EN, 1);
    GPIO_write(Board_PH2_EN, 1);

    GPIO_write(Board_PH3_IN, 1);
    GPIO_write(Board_PH2_IN, 0);
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
}

void MC_setPulseWidth(float timeOnMS)
{
    mc.pulseTimeOnMS = timeOnMS;
}

void MC_50Hz_CLK(void)
{
    // Process state machine
    //mc_processStates_50Hz(); // TODO: Still needed?
    // Send appropriate pulse
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
    if(++mc.index > 5U)
    {
        mc.index = 0U;
    }
    GPIO_toggle(Board_LED0);
}

