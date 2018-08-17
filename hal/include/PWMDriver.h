/*
 * PWMDriver.h
 *
 *  Created on: Aug 16, 2018
 *      Author: brianvuong
 */

#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#include <stdint.h>

/*
 * Typedefs
 */
typedef struct PwmDriver_S
{
    uint8_t dummy;
}PwmDriver_S;

typedef enum
{
    PWM_STEP_1,
    PWM_STEP_2,
    PWM_STEP_3,
    PWM_STEP_4,
    PWM_STEP_5,
    PWM_STEP_6,

    PWM_NUM_STEPS
}PwmDriver_CommutationStep_E;

/*
 * Global Variables
 */
extern PwmDriver_S PwmDriver;

/*
 * Private Functions
 */

/*
 * Public Functions
 */
extern void PwmDriver_init(void);

#endif /* PWMDRIVER_H_ */
