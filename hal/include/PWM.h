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

    struct {
        uint8_t enabled:1;      // If PWM is initialized and enabled
        uint8_t ph1Disabled:1;  // PH1 is off (always LOW)
        uint8_t ph2Disabled:1;  // PH2 is off (always LOW)
        uint8_t ph3Disabled:1;  // PH3 is off (always LOW)
        uint8_t unused3:1;
        uint8_t unused2:1;
        uint8_t unused1:1;
        uint8_t unused0:1;
    }bit;
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
uint32_t timer2_periodUsToCounts(uint32_t periodInUs);

#endif /* PWM_H_ */
