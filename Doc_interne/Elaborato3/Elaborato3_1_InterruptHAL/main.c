/**
  **********************************************************************************************
  * @file    	Elaborato3/Elaborato3_1_InterruptHAL/main.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	16-June-2017
  * @brief   	Esempio di utilizzo delle interruzioni mediante HAL su board STM32
  **********************************************************************************************
*/

/* Librerie Incluse ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_hal.h"

/* Variabili globali ------------------------------------------------------------------*/
short int count=0;

void setup();
void loop();

/**
  * @brief  Interrupt Service Routine lanciata da EXTI0.
  * @note   Una volta invocata la routine essa implementa una logica custom, in particolare una volta schicciato il bottone utente si provvede ad incrementare di 1 una variabile e mostrato il conteggio sui led. Tale funzione reimplementa una callback
  * @param  [in] GPIO_Pin: intero a 16 bit che indica il pin di GPIO da dove è stata lanciata l'interruzione.
  * @retval None.
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	count++;
	GPIOD->ODR=count<<12;
	if(count==15)
		count=0;
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

	HAL_PWR_EnterSLEEPMode (PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

}

