/*
 * ADC.c
 *
 *  Created on: Jul 27, 2018
 *      Author: brianvuong
 */

#include <gpio.h>
#include <xdc/std.h>
#include <ti/drivers/GPIO.h>

#include "ADC.h"

#include <string.h>

/*
 * Defines
 */


/*
 * Global Variables
 */
ADC_s ADC;

/*
 * Private Functions
 */
static void sample50HzSignals(void)
{
    // Sample
    ADC.throttleInput = ADC_getOneReading();
}


/*
 * Public Functions
 */
// ADC_init()
// Initializes the ADC module
void ADC_init(void)
{
    memset(&ADC, 0, sizeof(ADC_s));
}


// ADC_initPeripheral():
// Initializes ADC12 peripheral into the following default state:
//  - P6.1, connected to ADC channel 1
//  - Internal Vref+ = Vcc, Vref- = Vss (GND)
//  - Clocked by ADC12OSC
//  - Stores conversion in memory buffer 0
void ADC_initPeripheral(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1);

    ADC12_A_init(
            __MSP430_BASEADDRESS_ADC12_PLUS__,
            ADC12_A_SAMPLEHOLDSOURCE_SC,
            ADC12_A_CLOCKSOURCE_ADC12OSC,
            ADC12_A_CLOCKDIVIDER_1);

    ADC12_A_setupSamplingTimer(
            __MSP430_BASEADDRESS_ADC12_PLUS__,
            ADC12_A_CYCLEHOLD_64_CYCLES,
            ADC12_A_CYCLEHOLD_64_CYCLES,
            ADC12_A_MULTIPLESAMPLESDISABLE);

    ADC12_A_configureMemoryParam config_param;
    config_param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    config_param.inputSourceSelect = ADC12_A_INPUT_A1;
    config_param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC; // Vref+
    config_param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS; // Vref-
    config_param.endOfSequence = ADC12_A_ENDOFSEQUENCE;

    ADC12_A_configureMemory(
            __MSP430_BASEADDRESS_ADC12_PLUS__,
            &config_param);

    ADC12_A_setResolution(
            __MSP430_BASEADDRESS_ADC12_PLUS__,
            ADC12_A_RESOLUTION_12BIT);

    ADC12_A_enable(__MSP430_BASEADDRESS_ADC12_PLUS__);
}

// ADC_enable():
// Enables ADC12 block for usage by writing 1 to bit ADC12ON in ADC12CTL0
void ADC_enable(void)
{
    ADC12_A_enable(__MSP430_BASEADDRESS_ADC12_PLUS__);
}

// ADC_disable():
// Disables ADC12 block
void ADC_disable(void)
{
    ADC12_A_disable(__MSP430_BASEADDRESS_ADC12_PLUS__);
    ADC12_A_disableConversions(__MSP430_BASEADDRESS_ADC12_PLUS__, true);
}

// ADC_configureMemoryParameters()
// Reconfigures ADC core's memory parameters (i.e channel, pin etc)
void ADC_configureMemoryParameters(ADC12_A_configureMemoryParam params)
{
    ADC12_A_configureMemory(
            __MSP430_BASEADDRESS_ADC12_PLUS__,
            &params);
}

void ADC_50Hz_CLK(void)
{
    sample50HzSignals();
}

// ADC_getOneReading():
// Returns the results of one single channel conversion
uint16_t ADC_getOneReading(void)
{
    uint16_t result;

    ADC12_A_startConversion(
            __MSP430_BASEADDRESS_ADC12_PLUS__,
            ADC12_A_MEMORY_0,
            ADC12_A_SINGLECHANNEL);

    while(ADC12_A_isBusy(__MSP430_BASEADDRESS_ADC12_PLUS__) == ADC12_A_BUSY);

    result = ADC12_A_getResults(
                __MSP430_BASEADDRESS_ADC12_PLUS__,
                ADC12_A_MEMORY_0);

    return result;
}
