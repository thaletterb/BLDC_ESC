/*
 * PWM.h
 *
 *  Created on: May 26, 2018
 *      Author: brianvuong
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

/*
 * Typedefs
 */
typedef struct PWM_s
{
    unsigned long   frequency;  // 1Mhz
    unsigned long   period;
    double          dutyCyle;
    uint8_t         mode;
}PWM_S;

/*
 * Global Variables
 */
extern PWM_S PWM;

/*
 * Private Functions
 */

/*
 * Public Functions
 */
void PWM_init(void);
void PWM_setDutyCycle(double dutyCycle);
void PWM_setDutyCycleTimeOn(double timeOnMS);

#endif /* PWM_H_ */
