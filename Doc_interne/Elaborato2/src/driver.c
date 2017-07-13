/**
  **********************************************************************************************
  * @file    	Elaborato2/src/driver.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	12-June-2017
  * @brief   	Implementazione funzioni per pilotaggio periferiche
  **********************************************************************************************
*/

/* Librerie incluse ------------------------------------------------------------------*/
#include "driver.h"

/* Variabili globali ------------------------------------------------------------------*/
short int on=0;
short int dx=0;
short int count=0;
uint8_t val_shift=1;

/**
  * @brief  Questa funzione permette il toggle di tutti e 4 i led utente.
  * @param  None.
  * @retval None.
  */
void toggleAll(){

	/*Si attende pressione bottone*/
	if(read_button()){
		/*Si attende rilascio bottone*/
		while(read_button());
		/*Se i led sono spenti allora si accendono*/
		if(!on){
			write_led(0xF);
			on=1;
		}
		else{
			/*Altrimenti si spengono*/
			write_led(0x0);
			on=0;
		}
	}

}

/**
  * @brief  Questa funzione permette di implementare un contatore modulo 16 e di visualizzare il
  * 		contenuto sui led.
  * @note   Il conteggio avviene premendo e successivamente rilasciando il bottone utente
  * @param  None.
  * @retval None.
  */
void counter(){
	/*Se viene premuto il bottone si computa il conteggio*/
	if(read_button()){
		/*Si attende che il bottone sia rilasciato*/
		while(read_button());
		count++;
		/*Stampa conteggio sui led*/
		write_led(count);
		/*Se il conteggio è arrivato a 15 si resetta la variabile associata*/
		if(count==15)
			count=0;
	}

}

/**
  * @brief  Questa funzione permette di shiftare da destra verso sinistra e viceversa, un bit
  * 		pari a 1 sui led.
  * @note   Lo shift avviene una volta ogni 500 ms.
  * @param  None.
  * @retval None.
  */
void supercar(){
	/*Inizio conteggio*/
	start_timer2();
	/*Se il conteggio è terminato allora inizia lo shift*/
	if(is_timer2_stopped()){
		/*Se non è stato illuminato il led più a sinistra si shifta da destra a sinistra*/
		if(!dx){
				val_shift=val_shift<<1;
				write_led(val_shift);
				/*Se il led illuminato è quello più a sinistra si setta una variabile pari a 1
				 * per iniziare lo shift verso destra*/
				if(val_shift==0x8)
					dx=1;
		}
		else{
			/*Computazione dello shift verso destra*/
			val_shift=val_shift>>1;
			write_led(val_shift);
			/*Se il led illuminato è quello più a destra si setta una variabile pari a 0
			 * per iniziare lo shift verso sinistra*/
			if(val_shift==0x1)
				dx=0;
		}
	}
}


