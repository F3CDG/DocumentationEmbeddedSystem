/**
  ******************************************************************************
  * @file    	Elaborato1/1_3_Driver_C_OO_Int/gpio_custom.h
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	28-Jun-2017
  * @brief   	Header file della libreria HAL driver per gestione GPIO custom
  ******************************************************************************
*/

#ifndef SRC_GPIO_CUSTOM_H_
#define SRC_GPIO_CUSTOM_H_

#include "xparameters.h"
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
  * @brief  Definizione struttura dati per la gestione del GPIO
  */

typedef struct{
	uint32_t *base_address; /*!< GPIO Base address*/
	uint8_t pad_out_offset; /*!< GPIO offset registro di pad_out */
	uint8_t pad_rw_n_offset; /*!< GPIO offset registro modalità read/write */
	uint8_t pad_en_offset; /*!< GPIO offset registro abilitazione pin */
	uint8_t pad_in_offset; /*!< GPIO offset registro pad_in*/
	uint8_t imr_offset; /*!< GPIO offset registro interrupt mask*/
	uint8_t sr_offset; /*!< GPIO offset registro di stato*/
	uint8_t iack_offset; /*!< GPIO offset registro interrupt ack*/
	uint8_t gie_offset; /*!< GPIO offset registro global interrupt enable*/
}gpio_custom_TypeDef;

/**
  * @brief Configurazione dell'indirizzo base
  */
#define GPIO_BASE XPAR_MY_GPIO_0_S00_AXI_BASEADDR

/**
  * @brief Spiazzamenti registri periferica
  */
#define gpio_custom_PAD_OUT 0x00
#define gpio_custom_PAD_RW_N 0x04
#define gpio_custom_PAD_EN 0x08
#define gpio_custom_PAD_IN 0x0C
#define gpio_custom_IMR 0x10
#define gpio_custom_SR 0x14
#define gpio_custom_IACK 0x18
#define gpio_custom_GIE 0x1C

/**
  * @brief Definizione dei valori alto e basso
  */
typedef enum{
	LOW,
	HIGH
}gpio_custom_ValueType;



void gpio_custom_Init(gpio_custom_TypeDef *gpio, uint32_t base_address);

/*************************Funzioni private per gestione registri generici*********************/
static void gpio_custom_Set_value_reg(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t mask,uint32_t mask_value);
static void gpio_custom_Write_mask(gpio_custom_TypeDef *gpio,uint32_t reg, uint32_t mask, gpio_custom_ValueType value);
static void gpio_custom_Write_one(gpio_custom_TypeDef *gpio,uint32_t reg, uint32_t position, gpio_custom_ValueType value);
static uint32_t gpio_custom_Read_mask(gpio_custom_TypeDef *gpio,uint32_t reg, uint32_t mask);
static uint32_t gpio_custom_Read_one(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t position);
static void gpio_custom_Toggle_one(gpio_custom_TypeDef *gpio, uint32_t reg,uint8_t position);
static void gpio_custom_Toggle_mask(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t mask);

/***************************Funzioni utente per gestione specifici registri************/

void gpio_custom_SetEnable(gpio_custom_TypeDef *gpio,uint32_t mask,gpio_custom_ValueType value);
uint32_t gpio_custom_GetEnable(gpio_custom_TypeDef *gpio,uint32_t mask);

void gpio_custom_SetValue(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value);
uint32_t gpio_custom_GetValue(gpio_custom_TypeDef *gpio, uint32_t mask);

void gpio_custom_SetMode(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType mvalue);
uint32_t gpio_custom_GetMode(gpio_custom_TypeDef *gpio, uint32_t mask);

void gpio_custom_SetInterruptMask(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value);
uint32_t gpio_custom_GetInterruptMask(gpio_custom_TypeDef *gpio, uint32_t mask);

void gpio_custom_SetStatusInterrupt(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value);
uint32_t gpio_custom_GetStatusInterrupt(gpio_custom_TypeDef *gpio, uint32_t mask);

void gpio_custom_SetAckInterrupt(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value);

void gpio_custom_SetGlobalInterrupt(gpio_custom_TypeDef *gpio, gpio_custom_ValueType value);
uint32_t gpio_custom_GetGlobalInterrupt(gpio_custom_TypeDef *gpio);

#endif /* SRC_GPIO_CUSTOM_H_ */
