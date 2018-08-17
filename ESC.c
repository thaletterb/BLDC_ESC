/*
 * ESC.c
 *
 *  Created on: Jul 25, 2018
 *      Author: brianvuong
 */

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include "board.h"

#include <IPWM.h>
#include "ADC.h"

#include "MotorControl.h"
#include "Throttle.h"

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
    // Peripheral Init
    ADC_initPeripheral();
    PWM_init();

    // Module Init
    ADC_init();
    THROTTLE_init();
    MC_init();
}

void ESC_50Hz_CLK(void)
{
    ADC_50Hz_CLK();
    THROTTLE_50Hz_CLK();
    MC_50Hz_CLK();
}
