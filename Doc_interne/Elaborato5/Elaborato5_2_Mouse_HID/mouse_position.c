/**
  **********************************************************************************************
  * @file    	Elaborato5/Elaborato5_2_Mouse_HID/mouse_position.c
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	05-July-2017
  * @brief   	Implementazione funzioni per gestione puntatore di un mouse
  **********************************************************************************************
*/

/* Librerie incluse ------------------------------------------------------------------*/
#include "mouse_position.h"

/**
  * @brief  Questa funzione permette di inizializzare la periferica HID mouse
  *
  * @note   L'inizializzazione avviene mediante l'utilizzo di SPI
  *
  * @param  hspi1: struttura SPI_HandleTypeDef che contiene le informazioni di configurazione
  * 			  della specifica SPI
  *
  * @retval none
  */

void init_mouse(SPI_HandleTypeDef hspi1){

	/*Selezione via software della perificerica accellerometro*/
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	/*Si invia l'indirizzo 0x20 su bus SPI per risvegliare il relativo registro */
	uint8_t address_Acc=0x20;
	HAL_SPI_Transmit(&hspi1,&address_Acc,1,50);
	/*Sul registro selezionato si indica che si vuole leggere il valore lungo gli assi X e Y*/
	uint8_t value_Ctrl_Reg1=0x67; /*< Valore da scrivere in Ctrl_Reg1*/
	HAL_SPI_Transmit(&hspi1,&value_Ctrl_Reg1,1,50);
	/*Deselezione via software della periferica accellerometro*/
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);

}

/**
  * @brief  Questa funzione permette di calcolare la posizione sull'asse X.
  *
  * @param  hspi1: struttura SPI_HandleTypeDef che contiene le informazioni
  * 		di configurazione della specifica SPI.
  *
  * @retval Intero senza segno ad 8 bit.
  */

uint8_t x_position(SPI_HandleTypeDef hspi1){
	uint8_t xval=0;
	/*Selezione via software della perificerica accellerometro*/
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	/*Si invia l'indirizzo 0x29+0x80 su bus SPI per risvegliare il relativo registro */
	/*Si aggiunge 0x80 per indicare alla periferica che voglio più letture dall'asse X */
    uint8_t address_Acc=0xA9;
	HAL_SPI_Transmit(&hspi1,&address_Acc,1,50);
	/*Lettura valore sull'asse X*/
	HAL_SPI_Receive(&hspi1,&xval,1,50);
	/*Deselezione via software della periferica accellerometro*/
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	return xval;
}

/**
  * @brief  Questa funzione permette di calcolare la posizione sull'asse Y
  *
  * @param  hspi1: struttura SPI_HandleTypeDef che contiene le informazioni
  * 		di configurazione della specifica SPI
  *
  * @retval Intero ad 8 bit.
  */

uint8_t y_position(SPI_HandleTypeDef hspi1){
	uint8_t yval;
	/*Selezione via software della perificerica accellerometro*/
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	/*Si invia l'indirizzo 0x2B+0x80 su bus SPI per risvegliare il relativo registro */
	/*Si aggiunge 0x80 per indicare alla periferica che voglio più letture dall'asse Y */
	uint8_t address_Acc=0xAB;
    HAL_SPI_Transmit(&hspi1,&address_Acc,1,50);
	/*Lettura valore sull'asse Y*/
	HAL_SPI_Receive(&hspi1,&yval,1,50);
	/*Deselezione via software della periferica accellerometro*/
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	return yval;
}

/**
  * @brief  Questa funzione in base al movimento della board permette di illuminare i led
  *         utente.
  * @param  [in] position: posizione attuale
  * @param  [in] axis: X per illuminare i led relativi alle ascisse, Y per illuminare quelli
  *              relativi alle ordinate.
  * @note   I valori X e Y di tipo mouse_position_AxisType sono rispettivamente pari a 0
  *         e 1.
  *
  * @retval None.
  */

void position_on_led(uint8_t position,mouse_position_AxisType axis){

	/*Se il valore di position è relativo all'asse delle ascisse allora si illuminano
	 * i led orizzontali di una board STM32F4 discovey*/
	if(axis==X){
		/*Se il movimento è verso sinistra si illumina il led LD4 e si spegne LD5*/
		if(position>127){
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
		}
		/*Altrimennti si illumina il led LD5 e si spegne LD4*/
		else{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		}
	}
	/*Allo stesso modo di prima si illuminano i led LD3 o LD6 relativi all'asse Y*/
	else{
		if(position>127){
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
		}
		else{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
		}

	}
}
