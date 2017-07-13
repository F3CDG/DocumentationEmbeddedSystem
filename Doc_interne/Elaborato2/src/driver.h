/**
  **********************************************************************************************
  * @file    	Elaborato2/src/driver.h
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	12-June-2017
  * @brief   	Libreria per pilotaggio periferiche
  **********************************************************************************************
*/

#ifndef DRIVER_H_
#define DRIVER_H_

/* Librerie Incluse ------------------------------------------------------------------*/
#include "my_bsp_custom.h"

/* Prototipi funzioni -------------------------------------------------------*/
void toggleAll(); /*<! Toggle dei led utente*/
void counter(); /*<! Conteggio con output sui led*/
void supercar(); /*<! Shift a destra e sinistra di un bit e mostrato sui led*/

#endif /* DRIVER_H_ */
