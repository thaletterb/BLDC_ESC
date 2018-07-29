/*
 * ADC.h
 *
 *  Created on: Jul 27, 2018
 *      Author: brianvuong
 */

#ifndef HAL_INCLUDE_ADC_H_
#define HAL_INCLUDE_ADC_H_


#include <stdint.h>

#include <adc12_a.h>

/*
 * Defines
 */
#define ADC12_FULL_RESOLUTION   4096U


/*
 * Typedefs
 */


/*
 * Global Variables
 */


/*
 * Private Functions
 */


/*
 * Public Functions
 */
void ADC_init(void);
void ADC_enable(void);
void ADC_disable(void);
void ADC_configureMemoryParameters(ADC12_A_configureMemoryParam params);

uint16_t ADC_getOneReading(void);


#endif /* HAL_INCLUDE_ADC_H_ */
