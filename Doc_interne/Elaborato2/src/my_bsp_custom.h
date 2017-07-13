/**
  **********************************************************************************************
  * @file    	Elaborato2/src/my_bsp_custom.h
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	12-June-2017
  * @brief   	Libreria per BSP custom STM32F4 Discovery
  **********************************************************************************************
*/

#ifndef MY_BSP_CUSTOM_H_
#define MY_BSP_CUSTOM_H_

/* Librerie Incluse ------------------------------------------------------------------*/
#include "stm32f407xx.h"

/* Prototipi funzioni -------------------------------------------------------*/
void init_led_user(); /*!< inizializza i led utente */
void init_button_user(); /*!< inizializza il bottone utente*/

void write_led_user(); /*!< scrittura valori su led utente */
uint8_t read_button_user(); /*!< lettura bottone utente */

void init_timer2(uint32_t delay); /*!< inizializzazione timer */
void start_timer2(); /*!< start conteggio*/
uint8_t is_timer2_stopped(); /*!< interroga se il timer ha terminato il conteggio*/



#endif /* MY_BSP_CUSTOM_H_ */
