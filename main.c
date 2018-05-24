/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty_min.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>


/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Board Header file */
#include "Board.h"

#include <msp430.h>
#include <timer_a.h>
#include <timer_b.h>

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/*
 *  ======== sendPulseToESC ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void sendPulseToESC(void)
{
    while (1) {
        Semaphore_pend(semaphore0, BIOS_WAIT_FOREVER);

        WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
        P1DIR |= BIT2+BIT3;                       // P1.2 and P1.3 output
        P1SEL |= BIT2+BIT3;                       // P1.2 and P1.3 options select
        TA0CCR0 = 20972-1;                          // PWM Period
        TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
        TA0CCR1 = 1050;                            // CCR1 PWM duty cycle
        TA0CTL = TASSEL_2 + MC_1 + TACLR + ID_3;

        GPIO_toggle(Board_LED0);
    }
}

void PWM_init(void);

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    PWM_init();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    taskParams.instance->name = "heartBeat";
    Task_construct(&task0Struct, (Task_FuncPtr)sendPulseToESC, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

    return (0);
}

uint32_t count = 0;
void CLK_50Hz()
{
    if(count++ == 250)
    {
        Semaphore_post(semaphore0);
    }

}


void PWM_init(void)
{
//    // Start TIMER_A
//    Timer_A_outputPWMParam timerA_params = {0};
//    timerA_params.clockSource           = TIMER_A_CLOCKSOURCE_SMCLK;
//    timerA_params.clockSourceDivider    = TIMER_A_CLOCKSOURCE_DIVIDER_2;
//    timerA_params.compareOutputMode     = TIMER_A_OUTPUTMODE_RESET_SET;
//    timerA_params.compareRegister       = TIMER_A_CAPTURECOMPARE_REGISTER_1;
//    timerA_params.dutyCycle             = 2097;
//    timerA_params.timerPeriod           = 20972;
//    Timer_A_outputPWM(__MSP430_BASEADDRESS_T0A5__, &timerA_params);

    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P1DIR |= BIT2+BIT3;                       // P1.2 and P1.3 output
    P1SEL |= BIT2+BIT3;                       // P1.2 and P1.3 options select
    TA0CCR0 = 20972-1;                          // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = 525;                            // CCR1 PWM duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TACLR + ID_3;
}
