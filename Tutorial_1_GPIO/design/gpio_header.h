/*
 * gpio_header.h
 *
 *  Created on: 	07 giu 2017
 *  Company:		Gruppo IV - Sistemi Embedded 2016-17
 *  Authors: 		Colella Gianni, Guida Ciro, Lombardi Daniele
 */

#ifndef SRC_GPIO_HEADER_H_
#define SRC_GPIO_HEADER_H_


#include "xparameters.h"
#include "my_gpio.h"
#include <assert.h>

// macro indirizzo base my_gpio
#define GPIO_ADDRESS (uint32_t*) XPAR_MY_GPIO_0_S00_AXI_BASEADDR

//macro spiazzamenti per i registri del gpio
#define PAD_OUT MY_GPIO_S00_AXI_SLV_REG0_OFFSET
#define PAD_RW_N MY_GPIO_S00_AXI_SLV_REG1_OFFSET
#define PAD_EN MY_GPIO_S00_AXI_SLV_REG2_OFFSET
#define PAD_IN MY_GPIO_S00_AXI_SLV_REG3_OFFSET

//macro per valori
#define HIGH 1
#define LOW 0
#define SET_SWITCH 0
#define SET_LED 4

//macro valori esadecimali
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define _A_ 10
#define _B_ 11
#define _C_ 12
#define _D_ 13
#define _E_ 14
#define _F_ 15

//macro configurazione iniziale
#define INIT_CONFIG_RWN 0xF
#define INIT_CONFIG_EN 0x0


void gpio_init (uint32_t* base_addr);
void set_value_reg(uint32_t reg, uint32_t mask,uint32_t mask_value);
void gpio_write_mask(uint32_t reg, uint8_t set, uint32_t mask, uint8_t value);
void gpio_write_one(uint32_t reg, uint8_t set, uint32_t position, uint8_t value);
uint32_t gpio_read_mask(uint32_t reg, uint8_t set, uint32_t mask);
uint32_t gpio_read_one(uint32_t reg, uint8_t set, uint32_t position);
void gpio_toggle_one(uint32_t reg,uint8_t set,uint8_t position);
void gpio_toggle_mask(uint32_t reg, uint8_t set,uint32_t mask);



#endif /* SRC_GPIO_HEADER_H_ */
