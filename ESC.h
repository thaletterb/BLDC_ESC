/*
 * ESC.h
 *
 *  Created on: Jul 25, 2018
 *      Author: brianvuong
 */

#ifndef ESC_H_
#define ESC_H_

#include <stdint.h>

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
void ESC_init(void);

// SWI

// Periodics
void ESC_50Hz_CLK(void);

#endif /* ESC_H_ */
