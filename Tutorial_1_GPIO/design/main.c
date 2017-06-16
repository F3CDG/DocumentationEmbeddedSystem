/*
 * main.c
 *
 *  Created on: 	07 giu 2017
 *  Company:		Gruppo IV - Sistemi Embedded 2016-17
 *  Authors: 		Colella Gianni, Guida Ciro, Lombardi Daniele
 */


#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "gpio_header.h"
#include <unistd.h>

void init();
void supercar();

int main()
{
    init_platform();
    init();
    supercar();
    cleanup_platform();
    return 0;
}

void init(){
	gpio_init(GPIO_ADDRESS);
	gpio_write_mask(PAD_EN, SET_LED, _F_, HIGH);
	gpio_write_mask(PAD_OUT,SET_LED,_F_,LOW);
}

void supercar(){
	int position=ZERO;
	short int dx=0;
	useconds_t usec = 500000;
	while(1){
		usleep(usec);
		if(!dx){
			gpio_toggle_one(PAD_OUT, SET_LED, position);
			usleep(usec);
			gpio_toggle_one(PAD_OUT,SET_LED, position);
			if(position==THREE)
				dx=1;
			else
				position++;
		}
		else{
			position--;
			gpio_toggle_one(PAD_OUT, SET_LED, position);
			usleep(usec);
			gpio_toggle_one(PAD_OUT, SET_LED, position);
			if(position==ZERO){
				dx=0;
				position++;
			}
		}
	}


}
