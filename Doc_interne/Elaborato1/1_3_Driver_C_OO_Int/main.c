/**
  ******************************************************************************
  * @file    	Elaborato1/1_3_Driver_C_OO_Int/main.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	28-Jun-2017
  * @brief   	Programma principale che contiene al suo interno l'implemetazione di un particolare driver che pilota la periferica
  ******************************************************************************
*/


/*
 * main.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "gpio_custom.h"


int main()
{
    init_platform();
    uint32_t ret=0x0;
    gpio_custom_TypeDef gpio;

    gpio_custom_Init(&gpio, GPIO_BASE);
    while(1){
    gpio_custom_SetEnable(&gpio,0xF,HIGH);
    ret = gpio_custom_GetEnable(&gpio,0xF);

    if(ret!=0){
    	printf("OK");
    	ret=0;
    }

    gpio_custom_SetMode(&gpio, 0xF, HIGH);
    ret = gpio_custom_GetMode(&gpio,0xF);

    if(ret!=0){
        	printf("OK");
        	ret=0;
    }

    //gpio_custom_SetValue(&gpio, 0xA, HIGH);
    ret = gpio_custom_GetValue(&gpio,0xF);

    if(ret!=0){
        	printf("OK");
        	ret=0;
        }

    gpio_custom_SetGlobalInterrupt(&gpio);
    ret = gpio_custom_GetGlobalInterrupt(&gpio);


    if(ret!=0){
        	printf("OK");
        	ret=0;
        }

    gpio_custom_SetInterruptMask(&gpio, 0x2, HIGH);
    ret = gpio_custom_GetInterruptMask(&gpio,0xF);

    if(ret!=0){
        	printf("OK");
        	ret=0;
        }

    //gpio_custom_SetStatusInterrupt(&gpio, 0x2, HIGH);
    ret = gpio_custom_GetStatusInterrupt(&gpio, 0xF);

    if(ret!=0){
        	printf("OK");
        	ret=0;
        }
    gpio_custom_SetAckInterrupt(&gpio, 0x2, HIGH);
    }

    cleanup_platform();
    return 0;
}
