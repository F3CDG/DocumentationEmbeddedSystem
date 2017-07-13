/**
  ******************************************************************************
  * @file    	Elaborato1/1_1_Driver_C_NoInt/main.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	10-May-2017
  * @brief   	Programma principale che contiene al suo interno l'implemetazione di un particolare driver che pilota la periferica
  ******************************************************************************
*/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "gpio_header.h"
#include <unistd.h>


void init();
void supercar();

int main()
{
    init_platform();
    /*!<Primo passo*/
    /*Si inizializza correttamente la periferica*/
    init();
    /*!<Secondo passo*/
    /*Si richiama il relativo driver*/
    supercar();
    cleanup_platform();
    return 0;
}

/**
  * @brief  Questa funzione permette di inizializzare in maniera opportuna la
  * 		periferica
  * @note   Si può notare come la periferica di GPIO è inizializzata in modo tale
  * 		da utilizzare 8 pin: in accordo all'header file gpio_header.h sui
  * 		primi 4 pin, a partire dai meno significativi vengono mappati degli switch,
  * 		sui secondi 4 dei led
  * @retval none
  */

void init(){
	gpio_init(GPIO_ADDRESS);
	gpio_write_mask(PAD_EN, SET_LED, _F_, HIGH);
	gpio_write_mask(PAD_OUT,SET_LED,_F_,LOW);
}

/**
  * @brief  Implementazione del driver per il pilotaggio della periferica
  * @note   Il driver permette la simulazione del lampeggiamento dei led
  * 		posti sul muso anteriore di K.I.T.T., famosa auto del telefim
  * 		Anni '80 Supercar.
  * @retval none
  */

void supercar(){
	int position=ZERO;
	short int dx=0;
	useconds_t usec = 500000;
	while(1){
		usleep(usec);
		/*Il ramo then permette di scorrere l'illuminazione dei led da destra verso sinistra*/
		if(!dx){
			gpio_toggle_one(PAD_OUT, SET_LED, position);
			usleep(usec);
			gpio_toggle_one(PAD_OUT,SET_LED, position);
			/*Poiché il driver è specifico per la board Zybo Zynq-7000
			 * tale controllo prevede che all'illuminazione del quarto led da destra
			 * viene posta la variabile dx a 1 in modo tale che alla successiva
			 * iterazione lo scorrimento avvenga da sinistra a destra*/
			if(position==THREE)
				dx=1;
			else
				position++;
		}
		/*Il ramo else permette di scorrere l'illuminazione dei led da sinistra verso destra*/
		else{
			position--;
			gpio_toggle_one(PAD_OUT, SET_LED, position);
			usleep(usec);
			gpio_toggle_one(PAD_OUT, SET_LED, position);
			/*Se lo scorrimento ha raggiunto il led più a destra, allora viene posta
			 * la variabile dx a 0 in modo tale che alla successiva iterazione,
			 * avvenga da destra verso sinistra*/
			if(position==ZERO){
				dx=0;
				position++;
			}
		}
	}


}
