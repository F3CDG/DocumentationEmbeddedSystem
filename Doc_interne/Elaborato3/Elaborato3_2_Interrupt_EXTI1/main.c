/**
  *************************************************************************************************
  * @file    	Elaborato3/Elaborato3_2_Interrupt_EXTI1/main.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	16-June-2017
  * @brief   	Esempio di utilizzo delle interruzioni BARE metal su board STM32 e periferica EXTI1
  *************************************************************************************************
*/

/* Librerie Incluse ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/* Prototipi funzione ------------------------------------------------------------------*/
void setup();
void loop();

/* Variabili globali ------------------------------------------------------------------*/
short int count=0;

/**
  * @brief  Interrupt Service Routine lanciata da EXTI1.
  * @note   Una volta invocata la routine essa implementa una logica custom, in particolare schicciato il bottone utente si provvede ad incrementare di 1 una variabile e mostrato il conteggio sui led.
  * @param  None.
  * @retval None.
  */
void EXTI1_IRQHandler(){
	count++;
	GPIOD->ODR=count<<12;
	if(count==15)
		count=0;
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
}

int main(void)
{

	setup();
	for(;;)loop();
}

void setup(){

	init_pin1C();
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

}

void loop(){}

/**
  * @brief  Inizializzazione del pin PC1.
  * @note   PC1 è settato in modo tale da lanciare interruzioni
  * @param  None.
  * @retval None.
  */
void init_pin1C(){

	/*Abilitazione GPIOC*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	/*Si indica che da GPIOC, PC1, arriva un interrupt*/
	SYSCFG->EXTICR[0] &= 0xFF0F;
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PC;

	/*Interupt da GPIOC non mascherato*/
	EXTI->IMR = (1<<1);

	/*Interrupt su fronte di salita*/
	EXTI->RTSR=(1<<1);

	/*Settaggio priorità su NVIC*/
	NVIC_SetPriority(EXTI1_IRQn,15);

	/*Abilitazione interrupt su NVIC*/
	NVIC_EnableIRQ(EXTI1_IRQn);
}
