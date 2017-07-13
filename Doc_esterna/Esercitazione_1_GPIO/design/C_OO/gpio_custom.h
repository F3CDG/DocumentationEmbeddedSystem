/**
  ******************************************************************************
  * @file    	gpio_header.h
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	12-May-2017
  * @brief   	Header file della libreria HAL driver per gestione GPIO custom
  ******************************************************************************
*/


#ifndef SRC_GPIO_CUSTOM_H_
#define SRC_GPIO_CUSTOM_H_

#include "xparameters.h"
#include "my_gpio.h"
#include <assert.h>

/**
  * @brief  Definizione struttura dati per la gestione del GPIO
  */
typedef struct{
	uint32_t *base_address; /*!< GPIO Base address */
	uint8_t pad_out_offset; /*!< GPIO offset registro di pad_out */
	uint8_t pad_rw_n_offset; /*!< GPIO offset registro modalità read/write */
	uint8_t pad_en_offset; /*!< GPIO offset registro abilitazione pin */
	uint8_t pad_in_offset; /**!< GPIO offset registro pad_in*/
}gpio_custom_TypeDef;

/**
  * @brief Configurazione dell'indirizzo base
  */
#define GPIO_BASE XPAR_MY_GPIO_0_S00_AXI_BASEADDR

/**
  * @brief Spiazzamenti registri periferica
  */
#define gpio_custom_PAD_OUT MY_GPIO_S00_AXI_SLV_REG0_OFFSET /*!< 0x0 */
#define gpio_custom_PAD_RW_N MY_GPIO_S00_AXI_SLV_REG1_OFFSET /*!< 0x4 */
#define gpio_custom_PAD_EN MY_GPIO_S00_AXI_SLV_REG2_OFFSET /*!< 0x8 */
#define gpio_custom_PAD_IN MY_GPIO_S00_AXI_SLV_REG3_OFFSET /*!< 0xC */

/**
  * @brief Definizione dei valori alto e basso
  */
typedef enum{
	LOW,
	HIGH
}gpio_custom_ValueType;

/**
  * @brief Posizionamento su GPIO di LED e Switch
  */
typedef enum{
	SET_SWITCH = 0x0,
	SET_LED = 0x4
}gpio_custom_SetType;


/**
  * @brief Impostazione della modalità su ogni pin di GPIO
  */
typedef enum{
	INIT_CONFIG_EN = 0x0,
	INIT_CONFIG_RWN = 0xF
}gpio_custom_InitType;

/******************Funzioni utente per gestione registri************/
void gpio_custom_Init(gpio_custom_TypeDef *gpio, uint32_t base_address);
void gpio_custom_Set_value_reg(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t mask,uint32_t mask_value);
void gpio_custom_Write_mask(gpio_custom_TypeDef *gpio,uint32_t reg, uint8_t set, uint32_t mask, uint8_t value);
void gpio_custom_Write_one(gpio_custom_TypeDef *gpio,uint32_t reg, uint8_t set, uint32_t position, uint8_t value);
uint32_t gpio_custom_Read_mask(gpio_custom_TypeDef *gpio,uint32_t reg, uint8_t set, uint32_t mask);
uint32_t gpio_custom_Read_one(gpio_custom_TypeDef *gpio, uint32_t reg, uint8_t set, uint32_t position);
void gpio_custom_Toggle_one(gpio_custom_TypeDef *gpio, uint32_t reg,uint8_t set,uint8_t position);
void gpio_custom_Toggle_mask(gpio_custom_TypeDef *gpio, uint32_t reg, uint8_t set,uint32_t mask);

#endif /* SRC_GPIO_CUSTOM_H_ */
