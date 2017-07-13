/**
  **********************************************************************************************
  * @file    	Elaborato5/Elaborato5_2_Mouse_HID/mouse_position.h
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	05-July-2017
  * @brief   	Libreria per gestione puntatore di un mouse
  **********************************************************************************************
*/

#ifndef MOUSE_POSITION_H_
#define MOUSE_POSITION_H_

/* Librerie Incluse ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/**
  * @brief Definizione tipo enumerativo per valori degli assi X e Y
  */
typedef enum{
	X,
	Y
}mouse_position_AxisType;

/* Prototipi funzioni -------------------------------------------------------*/
void init_mouse(SPI_HandleTypeDef); /*!< Inizializzazione dispositivo*/
uint8_t x_position(SPI_HandleTypeDef); /*!< Calcolo posizione su asse X*/
uint8_t y_position(SPI_HandleTypeDef); /*!< Calcolo posizione su asse Y*/
void position_on_led(uint8_t position,mouse_position_AxisType); /*!< Illuminazione led in base al movimento*/

#endif /* MOUSE_POSITION_H_ */
