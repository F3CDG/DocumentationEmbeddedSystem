/**
  **********************************************************************************************
  * @file    	main.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele
  *		Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	16-June-2017
  * @brief   	Esempio di utilizzo delle interruzioni BARE metal su board STM32
  **********************************************************************************************
*/

/* Librerie Incluse ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_hal_gpio.h"

/* variabili globali ------------------------------------------------------------------*/
short int count=0;

/* Prototipi funzioni ------------------------------------------------------------------*/
void setup();
void loop();

/**
  * @brief  Interrupt Service Routine lanciata da EXTI0.
  * @note   Una volta invocata la routine essa implementa una logica custom, in particolare
  *         una volta schicciato il bottone utente si provvede ad incrementare di 1 una variabile
  *         e mostrato il conteggio sui led.
  * @param  None.
  * @retval None.
  */

void EXTI0_IRQHandler(){
	/*L'NVIC segnala alla periferica su EXTI0 che è stata servita */
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	while(GPIOA->IDR&1);
	  count++;
	  GPIOD->ODR=count<<12;
	  if(count==15)
	    count=0;
	/*Una volta servita l'interrupt, viene segnala la linea di EXTI da pulire
	 * in modo tale da ritornare nel main*/
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
}

int main(void)
{
	setup();
	for(;;) loop();
	return 1;
}
/**
  * @brief  Funzione per inizializzare le periferiche da utilizzare.
  * @param  None.
  * @retval None.
  */
void setup(){
	HAL_Init();
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
}

void loop(){

}

