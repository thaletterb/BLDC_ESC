/*
 * MotorControl.h
 *
 *  Created on: Jun 16, 2018
 *      Author: brianvuong
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include <stdint.h>

/*
 * Typedefs
 */
typedef enum
{
    MC_ESC_DISABLED,
    MC_ESC_ARMING,
    MC_ESC_ARMED,

    MC_NUM_STATES
} MC_states_e;

typedef struct MC_s
{
    MC_states_e state;
}MC_S;

/*
 * Global Variables
 */
extern MC_S MC;

/*
 * Private Functions
 */

/*
 * Public Functions
 */
void MC_init(void);

void MC_setPulseWidth(float timeOnMS);

// Periodics
void MC_50Hz_CLK(void);

#endif /* MOTORCONTROL_H_ */
