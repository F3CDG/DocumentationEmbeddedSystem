/**
  ******************************************************************************
  * @file    	Elaborato1/1_1_Driver_C_NoInt/gpio_header.h
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	10-May-2017
  * @brief   	Header file della libreria HAL driver per gestione GPIO custom
  ******************************************************************************
*/

#ifndef SRC_GPIO_HEADER_H_
#define SRC_GPIO_HEADER_H_


#include "xparameters.h"
#include "my_gpio.h"
#include <assert.h>

/**
  * @brief Macro indirizzo base GPIO
  */
#define GPIO_ADDRESS (uint32_t*) XPAR_MY_GPIO_0_S00_AXI_BASEADDR

/**
  * @brief Spiazzamenti registri periferica
  */
#define PAD_OUT MY_GPIO_S00_AXI_SLV_REG0_OFFSET /*!< 0x0 */
#define PAD_RW_N MY_GPIO_S00_AXI_SLV_REG1_OFFSET /*!< 0x4 */
#define PAD_EN MY_GPIO_S00_AXI_SLV_REG2_OFFSET /*!< 0x8 */
#define PAD_IN MY_GPIO_S00_AXI_SLV_REG3_OFFSET /*!< 0xC */

/**
  * @brief Impostazione della modalità su ogni pin di GPIO
  */
#define INIT_CONFIG_RWN 0xF
#define INIT_CONFIG_EN 0x0

/**
  * @brief Definizione dei valori alto e basso
  */
#define HIGH 1
#define LOW 0

/**
  * @brief Posizionamento su GPIO di LED e Switch
  */
#define SET_SWITCH 0
#define SET_LED 4

/**
  * @brief Alias per alfabeto esadecimale
  */
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


void gpio_init (uint32_t* base_addr);
void set_value_reg(uint32_t reg, uint32_t mask,uint32_t mask_value);
void gpio_write_mask(uint32_t reg, uint8_t set, uint32_t mask, uint8_t value);
void gpio_write_one(uint32_t reg, uint8_t set, uint32_t position, uint8_t value);
uint32_t gpio_read_mask(uint32_t reg, uint8_t set, uint32_t mask);
uint32_t gpio_read_one(uint32_t reg, uint8_t set, uint32_t position);
void gpio_toggle_one(uint32_t reg,uint8_t set,uint8_t position);
void gpio_toggle_mask(uint32_t reg, uint8_t set,uint32_t mask);













#endif /* SRC_GPIO_HEADER_H_ */
