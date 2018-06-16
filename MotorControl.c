/*
 * MotorControl.c
 *
 *  Created on: Jun 16, 2018
 *      Author: brianvuong
 */

#include "MotorControl.h"
#include "PWM.h"

#include <string.h>

/*
 * Typedefs
 */
typedef struct mc_s
{
    double  pulseTimeOnMS;
    int     stateTimer50hz;
}mc_s;

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
            if(mc.stateTimer50hz++ == 500)
            {
                MC.state = MC_ESC_DISABLED;

                mc.stateTimer50hz = 0;
                mc.pulseTimeOnMS = 1.00;
                mc_sendPulseToESC();
            }
            break;
        case MC_ESC_DISABLED:
            if(mc.stateTimer50hz++ == 250)
            {
                MC.state = MC_ESC_ARMED;

                mc.stateTimer50hz = 0;
                mc.pulseTimeOnMS = 1.10;
                mc_sendPulseToESC();
            }
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

void MC_50Hz_CLK(void)
{
    // Process state machine
    mc_processStates_50Hz();
    // Send appropriate pulse
}
