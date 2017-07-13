/**
  **********************************************************************************************
  * @file    	Elaborato2/src/main.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	12-June-2017
  * @brief   	Main per pilotaggio periferiche
  **********************************************************************************************
 */

/* Librerie Incluse ------------------------------------------------------------------*/
#include "driver.h"

/* MACRO per selezione driver ------------------------------------------------------*/
#define TOGGLEALL 0
#define COUNTER 1
#define SUPERCAR 2

/* Prototipi funzioni ------------------------------------------------------------------*/
void loop(short int);
void init();

int main(void)
{

	init();
	for(;;) loop(SUPERCAR);
	return 1;
}

/**
  * @brief  Questa funzione inizializza led, bottone utente e timer2.
  * @param  None.
  * @retval None.
  */

void init(){
	init_led_user();
	init_button_user();
	init_timer2(8000000);
}

/**
  * @brief  Questa funzione permette di selezionare ed eseguire un driver tra quelli implementati.
  * @param  None.
  * @retval None.
  */

void loop(short int f){

	switch(f){

		case TOGGLEALL:
			toggleAll();
			break;

		case COUNTER:
			counter();
			break;

		case SUPERCAR:
			supercar();
			break;

		default:
			break;
	}

}
