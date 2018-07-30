/*
 * Throttle.h
 *
 *  Created on: Jul 29, 2018
 *      Author: brianvuong
 */

#ifndef THROTTLE_H_
#define THROTTLE_H_

#include <stdint.h>

/*
 * Typedefs
 */
typedef struct Throttle_S
{
    uint16_t    position;   // Potentiometer
    struct
    {
        bool armed:1;
    }bit ;
} Throttle_S;


/*
 * Global Variables
 */
extern Throttle_S THROTTLE;

/*
 * Private Functions
 */

/*
 * Public Functions
 */
void THROTTLE_init(void);

// SWI

// Periodics
void THROTTLE_50Hz_CLK(void);

#endif /* THROTTLE_H_ */
