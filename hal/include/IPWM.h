/*
 * IPWM.h
 *
 *  Created on: May 26, 2018
 *      Author: brianvuong
 */

#ifndef IPWM_H_
#define IPWM_H_

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

    struct{
        uint8_t enabled:1;
    }bit;
}PWM_S;

typedef enum
{
    PWM_PHASE_A,
    PWM_PHASE_B,
    PWM_PHASE_C,

    PWM_NUM_PHASES
}PWM_phase_E;

typedef enum
{
    PWM_SIDE_HIGH,
    PWM_SIDE_LOW,

    PWM_NUM_SIDES
}PWM_side_E;
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
extern void PWM_init(void);
extern void PWM_enable(void);
extern void PWM_disable(void);

#endif /* IPWM_H_ */
