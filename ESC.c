/*
 * ESC.c
 *
 *  Created on: Jul 25, 2018
 *      Author: brianvuong
 */

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include "board.h"

#include "PWM.h"
#include "ADC.h"

#include "MotorControl.h"
#include "Throttle.h"

#include <ti/sysbios/knl/Clock.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

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

    {
        P1SEL |= (1<<6);
        P1DIR |= (1<<6);
    }
}

void ESC_50Hz_CLK(void)
{
    ADC_50Hz_CLK();
    THROTTLE_50Hz_CLK();
    MC_50Hz_CLK();
    //GPIO_toggle(Board_LED0);
}

void ESC_1Hz_CLK(void)
{
    // Heartbeat
    GPIO_toggle(Board_RED_LED);

}
