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
typedef struct s_ADC
{
    uint16_t    throttleInput;
}ADC_s;


/*
 * Global Variables
 */
extern ADC_s ADC;


/*
 * Private Functions
 */


/*
 * Public Functions
 */
void ADC_init(void);
void ADC_initPeripheral(void);

void ADC_enable(void);
void ADC_disable(void);
void ADC_configureMemoryParameters(ADC12_A_configureMemoryParam params);

void ADC_50Hz_CLK(void);
uint16_t ADC_getOneReading(void);


#endif /* HAL_INCLUDE_ADC_H_ */
