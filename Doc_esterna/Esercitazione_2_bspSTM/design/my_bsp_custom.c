/**
  **********************************************************************************************
  * @file    	my_led_button_bsp.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele
  *		Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	12-June-2017
  * @brief   	Implementazione funzioni BSP custom STM32F4 Discovery
  **********************************************************************************************
*/

/* Librerie incluse ------------------------------------------------------------------*/
#include "my_bsp_custom.h"

/**
  * @brief  Questa funzione inizializza i 4 led utente in modo tale da poter essere
  *         utilizzati come display di valori a 4 bit.
  * @param  None.
  * @retval None.
  */

void init_led_user(){

	/*Abilitazione a GPIOD*/
	RCC->AHB1ENR |= (1<<3);

	/*Si indica ai pin 15-14-13-12 di PD di funzionare come output*/
	GPIOD->MODER|=(0x55<<24);

	/*Si spengono eventualmente i led accesi da un programma eseguito in precedenza*/
	GPIOD->ODR=0x0;
}

/**
  * @brief  Questa funzione inizializza il bottone utente.
  * @param  None.
  * @retval None.
  */

void init_button_user(){

	/*Abilitazione GPIOA*/
	RCC->AHB1ENR |= 1;

	/*Valore di reset al moder di GPIOA, si indica a PA0 di funzionare da input*/
	GPIOA->MODER=0xA8000000;
}

/**
  * @brief  Questa funzione inizializza il timer2.
  * @param [in] clock: intero a 32 bit che indica il numero di colpi di clock da contare
  *             prima del segnale di timeout
  * @retval None.
  */

void init_timer2(uint32_t clock){

	/*Abilitazione al Timer2*/
	RCC->APB1ENR|=1;

	/*Si indica al Timer2 che solo un overflow/underflow può generare l'interruzione*/
	TIM2->CR1|=(1<<2);

	/*Numero di colpi di clock da contare*/
	TIM2->ARR=clock;

	/*Non si utilizza il prescaler (divisione per 1)*/
	TIM2->PSC=0;

	/*Azzeramento conteggio*/
	TIM2->CNT=0;
}


/**
  * @brief  Questa funzione permette di scrivere sui led utente.
  * @param [in] value: intero a 8 bit che indica il valore da mostrare sui led
  *
  * @retval None.
  */

void write_led(uint8_t value){
	GPIOD->ODR|=value<<12;
}

/**
  * @brief  Questa funzione permette di leggere lo stato del bottone utente.
  * @param [in] value: intero a 8 bit che indica il valore da mostrare sui led
  *
  * @retval Intero a 8 bit:
  * 		1 bottone premuto;
  * 		0 bottone non premuto.
  */

uint8_t read_button(){
	return GPIOA->IDR&1;
}

/**
  * @brief  Questa funzione indica al timer2 l'inizio di un countdown.
  *
  * @retval None.
  */
void start_timer2(){
	TIM2->CR1=1;
}

/**
  * @brief  Questa funzione permette di interrogare il timer2 se il countdown è avvenuto.
  *
  * @retval Intero a 8 bit:
  * 		1 countdown avvenuto;
  * 		0 countdown non avvenuto.
  */

uint8_t is_timer2_stopped(){

	/*Se l'ultimo bit dello status register è pari a 1 allora il conteggio è terminato*/
	if(TIM2->SR&1){
			/*Si resetta l'ultimo bit per il prossimo conteggio*/
			TIM2->SR=0;
			return 1;
	}
	return 0;
}



