/*
 * gpio_functions.c
 *
 *  Created on: 	07 giu 2017
 *  Company:		Gruppo IV - Sistemi Embedded 2016-17
 *  Authors: 		Colella Gianni, Guida Ciro, Lombardi Daniele
 */

#include "gpio_header.h"

uint32_t* my_gpio_pointer;

//inizializzazione gpio
void gpio_init (uint32_t* base_addr){
	assert((uint32_t)base_addr%4==0 && "Base address non allineato a 4 byte");
	my_gpio_pointer=base_addr;
	*(my_gpio_pointer+PAD_EN/4)=INIT_CONFIG_EN;
	*(my_gpio_pointer+PAD_RW_N/4)=INIT_CONFIG_RWN;
}

void set_value_reg(uint32_t reg, uint32_t mask,uint32_t mask_value){
	*(my_gpio_pointer+reg/4) = (*(my_gpio_pointer+reg/4) & ~mask) | (mask & mask_value);
}

void gpio_write_mask(uint32_t reg, uint8_t set, uint32_t mask, uint8_t value){
	assert((reg==0 || reg%12!=0) && "Scrittura in una locazione di memoria non consentita");
	mask = mask<<set;
	if(((0x0000000F<<set) & mask)==mask)
		value == HIGH ? set_value_reg(reg,mask,0xF<<set) : set_value_reg(reg,mask,0x0);
}

void gpio_write_one(uint32_t reg, uint8_t set, uint32_t position, uint8_t value){
	gpio_write_mask(reg, set, 0x1<<position, value);
}

uint32_t gpio_read_mask(uint32_t reg,uint8_t set, uint32_t mask){
	mask = mask<<set;
	if(((0x0000000F<<set) & mask) == mask){
		return *(my_gpio_pointer + reg/4) & mask;
	}
	return -1;
}

uint32_t gpio_read_one(uint32_t reg, uint8_t set, uint32_t position){
	return gpio_read_mask(reg,set,0x1<<position) == (0x1<<(position+set));
}

void gpio_toggle_one(uint32_t reg, uint8_t set,uint8_t position){
	gpio_read_one(reg,set,position) == HIGH ? gpio_write_one(reg ,set,position,LOW) : gpio_write_one(reg,set,position,HIGH);
	/*if(gpio_read_one(reg,set,position) == HIGH)
		gpio_write_one(reg,set,position,LOW);
	else
		gpio_write_one(reg,set,position,HIGH);*/
}

void gpio_toggle_mask(uint32_t reg, uint8_t set,uint32_t mask){
	int i;
	for(i=0;i<4;i++){
		if((0x1<<(set*4+i)) == ((0x1<<(set*4+i)) & mask)){
			gpio_toggle_one(reg, set,i+set*4);
		}
	}
}




