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
 *  ======== MSP_EXP430F5529LP.c ========
 *  This file is responsible for setting up the board specific items for the
 *  MSP_EXP430F5529LP board.
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>

#include <msp430.h>
#include <inc/hw_memmap.h>
#include <dma.h>
#include <usci_a_uart.h>
#include <usci_b_i2c.h>
#include <usci_b_spi.h>
#include <usci_a_spi.h>
#include <ucs.h>
#include <gpio.h>
#include <pmm.h>
#include <wdt_a.h>

#include "MSP_EXP430F5529LP.h"

#include <ti/drivers/SPI.h>

const SPI_Config SPI_config[];

/*
 *  =============================== General ===============================
 */
/*
 *  ======== MSP_EXP430F5529LP_initGeneral ========
 */
void MSP_EXP430F5529LP_initGeneral(void) {
}

/*
 *  =============================== DMA ===============================
 */
/*
 *  ======== MSP_EXP430F5529LP_isrDMA ========
 *  This is a application defined DMA ISR. This ISR must map and call the
 *  appropriate Driver_event(handle) API to indicate completed DMA transfers.
 */
Void MSP_EXP430F5529LP_isrDMA(UArg arg)
{
    /* Call the SPI DMA function, passing the SPI handle used for WiFi */
    SPI_serviceISR((SPI_Handle) &(SPI_config[0]));
}

/*
 *  =============================== GPIO ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPIOMSP430_config, ".const:GPIOMSP430_config")
#endif

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP430.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP430F5529LP.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Input pins */
    /* MSP_EXP430F5529LP_S1 */
    GPIOMSP430_P2_1 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,
    /* MSP_EXP430F5529LP_S2 */
    GPIOMSP430_P1_1 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,

    /* Output pins */
    /* MSP_EXP430F5529LP_LED1 */
    GPIOMSP430_P1_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* MSP_EXP430F5529LP_LED2 */
    GPIOMSP430_P4_7 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP430F5529LP.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,  /* MSP_EXP430F5529LP_S1 */
    NULL   /* MSP_EXP430F5529LP_S2 */
};

/* The device-specific GPIO_config structure */
const GPIOMSP430_Config GPIOMSP430_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn)
};

/*
 *  ======== MSP_EXP430F5529LP_initGPIO ========
 */
void MSP_EXP430F5529LP_initGPIO(void)
{
    /* Initialize peripheral and pins */
    GPIO_init();
}

/*
 *  =============================== I2C ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cUSCIBHWAttrs, ".const:i2cUSCIBHWAttrs")
#endif

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CUSCIB.h>

I2CUSCIB_Object i2cUSCIBObjects[MSP_EXP430F5529LP_I2CCOUNT];

const I2CUSCIB_HWAttrs i2cUSCIBHWAttrs[MSP_EXP430F5529LP_I2CCOUNT] = {
    {
        .baseAddr = USCI_B0_BASE,
        .clockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK
    }
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CUSCIB_fxnTable,
        .object = &i2cUSCIBObjects[0],
        .hwAttrs = &i2cUSCIBHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430F5529LP_initI2C ========
 */
void MSP_EXP430F5529LP_initI2C(void)
{
    /*
     * NOTE: TI-RTOS examples configure USCIB0 as either SPI or I2C.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P3,
        GPIO_PIN0 | GPIO_PIN1);

    I2C_init();
}

/*
 *  =============================== SDSPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SDSPI_config, ".const:SDSPI_config")
#pragma DATA_SECTION(sdspiUSCIBHWAttrs, ".const:sdspiUSCIBHWAttrs")
#endif

#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPIUSCIB.h>

SDSPIUSCIB_Object sdspiUSCIBObjects[MSP_EXP430F5529LP_SDSPICOUNT];

const SDSPIUSCIB_HWAttrs sdspiUSCIBHWAttrs[MSP_EXP430F5529LP_SDSPICOUNT] = {
    {
        .baseAddr = USCI_B0_BASE,
        .clockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK,

        .portSCK = GPIO_PORT_P3,
        .pinSCK = GPIO_PIN2,
        .portMISO = GPIO_PORT_P3,
        .pinMISO = GPIO_PIN1,
        .portMOSI = GPIO_PORT_P3,
        .pinMOSI = GPIO_PIN0,
        .portCS = GPIO_PORT_P2,
        .pinCS = GPIO_PIN7
    }
};

const SDSPI_Config SDSPI_config[] = {
    {
        .fxnTablePtr = &SDSPIUSCIB_fxnTable,
        .object = &sdspiUSCIBObjects[0],
        .hwAttrs = &sdspiUSCIBHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430F5529LP_initSDSPI ========
 */
void MSP_EXP430F5529LP_initSDSPI(void)
{
    SDSPI_init();
}

/*
 *  =============================== SPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiUSCIBDMAHWAttrs, ".const:spiUSCIBDMAHWAttrs")
#endif

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIUSCIBDMA.h>

SPIUSCIBDMA_Object spiUSCIBDMAObjects[MSP_EXP430F5529LP_SPICOUNT];
uint8_t spiUSCIBDMAscratchBuf[MSP_EXP430F5529LP_SPICOUNT];

const SPIUSCIBDMA_HWAttrs spiUSCIBDMAHWAttrs[MSP_EXP430F5529LP_SPICOUNT] = {
    {
        .baseAddr = USCI_B0_BASE,
        .clockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK,
        .bitOrder = USCI_B_SPI_MSB_FIRST,
        .scratchBufPtr = &spiUSCIBDMAscratchBuf[0],
        .defaultTxBufValue = 0,

        /* DMA */
        .dmaBaseAddr = DMA_BASE,
        /* Rx Channel */
        .rxDMAChannelIndex = DMA_CHANNEL_1,
        .rxDMASourceTrigger = DMA_TRIGGERSOURCE_18,
        /* Tx Channel */
        .txDMAChannelIndex = DMA_CHANNEL_0,
        .txDMASourceTrigger = DMA_TRIGGERSOURCE_19
    }
};

const SPI_Config SPI_config[] = {
    {
        .fxnTablePtr = &SPIUSCIBDMA_fxnTable,
        .object = &spiUSCIBDMAObjects[0],
        .hwAttrs = &spiUSCIBDMAHWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP430F5529LP_initSPI ========
 */
void MSP_EXP430F5529LP_initSPI(void)
{
    /*
     * NOTE: TI-RTOS examples configure USCIB0 as either SPI or I2C.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */

    /* SPI CLK */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    /* MOSI/SIMO */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    /* MISO/SOMI */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN1);

    SPI_init();
}

/*
 *  =============================== UART ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartUSCIAHWAttrs, ".const:uartUSCIAHWAttrs")
#endif

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTUSCIA.h>

UARTUSCIA_Object uartUSCIAObjects[MSP_EXP430F5529LP_UARTCOUNT];

/*
 * The baudrate dividers were determined by using the MSP430 baudrate
 * calculator
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const UARTUSCIA_BaudrateConfig uartUSCIABaudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    {
        .outputBaudrate = 115200,
        .inputClockFreq = 8192000,
        .prescalar = 4,
        .hwRegUCBRFx = 7,
        .hwRegUCBRSx = 0,
        .oversampling = 1
    },
    {9600, 8192000, 53, 5, 0, 1},
    {9600,   32768,  3, 0, 3, 0},
};

const UARTUSCIA_HWAttrs uartUSCIAHWAttrs[MSP_EXP430F5529LP_UARTCOUNT] = {
    {
        .baseAddr = USCI_A1_BASE,
        .clockSource = USCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = USCI_A_UART_LSB_FIRST,
        .numBaudrateEntries = sizeof(uartUSCIABaudrates)/sizeof(UARTUSCIA_BaudrateConfig),
        .baudrateLUT = uartUSCIABaudrates
    },
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTUSCIA_fxnTable,
        .object = &uartUSCIAObjects[0],
        .hwAttrs = &uartUSCIAHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430F5529LP_initUART ========
 */
void MSP_EXP430F5529LP_initUART(void)
{
    /* P4.4,5 = USCI_A1 TXD/RXD */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
        GPIO_PIN4 | GPIO_PIN5);

    /* Initialize the UART driver */
    UART_init();
}

/*
 *  =============================== USB ===============================
 */
/*
 *  ======== MSP_EXP430F5529LP_initUSB ========
 */
void MSP_EXP430F5529LP_initUSB(unsigned int arg)
{
    PMM_setVCore(PMM_CORE_LEVEL_3);

}

/*
 *  =============================== Watchdog ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogMSP430HWAttrs, ".const:watchdogMSP430HWAttrs")
#endif

#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP430.h>

WatchdogMSP430_Object watchdogMSP430Objects[MSP_EXP430F5529LP_WATCHDOGCOUNT];

const WatchdogMSP430_HWAttrs watchdogMSP430HWAttrs[MSP_EXP430F5529LP_WATCHDOGCOUNT] = {
    {
        .baseAddr = WDT_A_BASE,
        .sfrAddr = SFR_BASE,
        .clockSource = WDT_A_CLOCKSOURCE_SMCLK,
        .clockDivider = WDT_A_CLOCKDIVIDER_8192K
    },
};

const Watchdog_Config Watchdog_config[] = {
    {
        .fxnTablePtr = &WatchdogMSP430_fxnTable,
        .object = &watchdogMSP430Objects[0],
        .hwAttrs = &watchdogMSP430HWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== MSP_EXP430F5529LP_initWatchdog ========
 */
void MSP_EXP430F5529LP_initWatchdog(void)
{
    /* Initialize the Watchdog driver */
    Watchdog_init();
}

/*
 *  =============================== WiFi ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(WiFi_config, ".const:WiFi_config")
#pragma DATA_SECTION(wiFiCC3100HWAttrs, ".const:wiFiCC3100HWAttrs")
#endif

#include <ti/drivers/WiFi.h>
#include <ti/drivers/wifi/WiFiCC3100.h>

WiFiCC3100_Object wiFiCC3100Objects[MSP_EXP430F5529LP_WIFICOUNT];

const WiFiCC3100_HWAttrs wiFiCC3100HWAttrs[MSP_EXP430F5529LP_WIFICOUNT] = {
    {
        .irqPort = GPIO_PORT_P2,
        .irqPin = GPIO_PIN0,
        .irqIntNum = NULL, /* IRQ port interrupt not used */

        .csPort = GPIO_PORT_P2,
        .csPin = GPIO_PIN2,

        .enPort = GPIO_PORT_P1,
        .enPin = GPIO_PIN6
    }
};

const WiFi_Config WiFi_config[] = {
    {
        .fxnTablePtr = &WiFiCC3100_fxnTable,
        .object = &wiFiCC3100Objects[0],
        .hwAttrs = &wiFiCC3100HWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== MSP_EXP430F5529LP_initWiFi ========
 */
void MSP_EXP430F5529LP_initWiFi(void)
{
    /* Configure EN & CS pins to disable CC3100 */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);

    /* Configure SPI */
    /* SPI CLK */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    /* MOSI/SIMO */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    /* MISO/SOMI */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN1);

    /* Configure IRQ pin */
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0,
        GPIO_LOW_TO_HIGH_TRANSITION);

    /* Initialize SPI and WiFi drivers */
    SPI_init();
    WiFi_init();
}
