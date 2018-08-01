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
#include "ADC.h"
#include "Throttle.h"

#include <string.h>

/*
 * Typedefs
 */
#define FULL_THROTTLE_VALUE_MS_ON   2.0
#define ZERO_THROTTLE_VALUE_MS_ON   1.0

/*
 * Global Variables
 */
Throttle_S THROTTLE;


/*
 * Local Variables
 */


/*
 * Private Functions
 */
// Converts the input ADC to a pulse width
// Pulse Width = (1.0 ms) + (input/ADC12_FULL_RESOLUTION) * ((2.0ms - 1.0ms) * input)
static double convertInputToPulseWidth(uint16_t input)
{
    return ZERO_THROTTLE_VALUE_MS_ON + ((FULL_THROTTLE_VALUE_MS_ON - ZERO_THROTTLE_VALUE_MS_ON) * ((double)input/(double)ADC12_FULL_RESOLUTION));
}


/*
 * Public Functions
 */
void THROTTLE_init(void)
{
    ADC_init();

    memset(&THROTTLE, 0, sizeof(Throttle_S));

}

void THROTTLE_50Hz_CLK(void)
{
    // Scale the input on P1.6 accordingly
    double pulseWidth = convertInputToPulseWidth(ADC.throttleInput);

    // Send the pulse width to motor control
    MC_setPulseWidth(pulseWidth);

}
