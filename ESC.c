/*
 * ESC.c
 *
 *  Created on: Jul 25, 2018
 *      Author: brianvuong
 */

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include "board.h"

#include "MotorControl.h"
#include "PWM.h"
#include "ADC.h"
/*
 * Typedefs
 */


/*
 * Global Variables
 */


/*
 * Local Variables
 */


/*
 * Private Functions
 */


/*
 * Public Functions
 */
void ESC_init(void)
{
    PWM_init();
    MC_init();
}

void ESC_50Hz_CLK(void)
{
    if(ADC_getOneReading() < ADC12_FULL_RESOLUTION/2U)
    {
        GPIO_toggle(Board_LED0);
    }
    MC_50Hz_CLK();
}
