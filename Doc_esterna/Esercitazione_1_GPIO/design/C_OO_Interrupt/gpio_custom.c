/**
  ******************************************************************************
  * @file    	gpio_custom.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	28-Jun-2017
  * @brief   	Questo file mette a disposizione un firmware per gestire il
  * 			comportamento della periferica
  ******************************************************************************
*/

#include "gpio_custom.h"

/**
  * @brief  Questa funzione permette di inizializzare correttamente un GPIO
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  base_addr: puntatore ad un intero di 32 bit per il corretto indirizzamento della periferica
  *
  * @retval none
  */
void gpio_custom_Init(gpio_custom_TypeDef *gpio, uint32_t base_address){

	assert(gpio!=NULL && "Errore allocazione di memoria per tipo GPIO");
	assert(base_address%4==0 && "Base address non allineato a 4 byte");
	gpio->base_address=(uint32_t*)base_address;
	gpio->pad_en_offset=gpio_custom_PAD_EN;
	gpio->pad_in_offset=gpio_custom_PAD_IN;
	gpio->pad_out_offset=gpio_custom_PAD_OUT;
	gpio->pad_rw_n_offset=gpio_custom_PAD_RW_N;
	gpio->gie_offset=gpio_custom_GIE;
	gpio->iack_offset=gpio_custom_IACK;
	gpio->imr_offset=gpio_custom_IMR;
	gpio->sr_offset=gpio_custom_SR;

}

/**
  * @brief  Questa funzione permette di settare all'interno di uno specifico
  * 		registro, dei valori in base ad una determinata maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *        se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *        scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param mask_value: intero a 32 bit da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_Set_value_reg(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t mask,uint32_t mask_value){

	assert(gpio!=NULL && "Errore allocazione di memoria per tipo GPIO");
	assert(gpio->base_address!=NULL && "Errore allocazione base address GPIO");
	gpio->base_address[reg/4]=(gpio->base_address[reg/4] & ~mask) | (mask & mask_value);
}

/**
  * @brief  Questa funzione permette di scrivere all'interno di uno specifico
  * 		registro in base ad una determinata maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *        se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *        scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_Write_mask(gpio_custom_TypeDef *gpio,uint32_t reg, uint32_t mask, gpio_custom_ValueType value){

	assert(gpio!=NULL && "Errore allocazione di memoria per tipo GPIO");
	assert(gpio->base_address!=NULL && "Errore allocazione base address GPIO");
	if((0x0000000F & mask) == mask)
		value == HIGH ? gpio_custom_Set_value_reg(gpio,reg,mask,0xF) : gpio_custom_Set_value_reg(gpio,reg,mask,0x0);
}

/**
  * @brief  Questa funzione permette di scrivere un unico valore all'interno di uno
  * 		specifico registro in base ad una determinata posizione
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param position: intero a 32 bit che specifica la posizione in cui scrivere
  * @param value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_Write_one(gpio_custom_TypeDef *gpio,uint32_t reg, uint32_t position, gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,reg, 0x1<<position, value);
}

/**
  * @brief  Questa funzione permette di leggere all'interno di uno specifico
  *         registro in base ad una determinata maschera di bit
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *        se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *        leggere in quella posizione, viceversa se 0
  *
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_Read_mask(gpio_custom_TypeDef *gpio,uint32_t reg, uint32_t mask){

	assert(gpio!=NULL && "Errore allocazione di memoria per tipo GPIO");
	assert(gpio->base_address!=NULL && "Errore allocazione base address GPIO");
	if((0x0000000F & mask) == mask){
		return (gpio->base_address[reg/4] & mask);
	}
	return -1;
}

/**
  * @brief  Questa funzione permette di leggere un unico valore all'interno di uno
  * 		specifico registro in base ad una determinata posizione
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param position: intero a 32 bit che specifica la posizione in cui leggere
  *
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_Read_one(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t position){
	return gpio_custom_Read_mask(gpio,reg,0x1<<position) == (0x1<<position);
}

/**
  * @brief  Questa funzione permette il toggle di un singolo valore all'interno di
  * 		uno specifico registro in base ad una determinata posizione
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui effettuare il toggle
  * @param position: intero a 8 bit che specifica la posizione in cui effettuare
  *        il toggle
  *
  * @retval none
  */

void gpio_custom_Toggle_one(gpio_custom_TypeDef *gpio, uint32_t reg,uint8_t position){
	gpio_custom_Read_one(gpio,reg,position) == HIGH ? gpio_custom_Write_one(gpio,reg ,position,LOW) : gpio_custom_Write_one(gpio,reg,position,HIGH);
}

/**
  * @brief  Questa funzione permette il toggle di un gruppo di bit all'interno di
  * 		uno specifico registro in base ad una determinata masschera
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui effettuare il toggle
  * @param position: intero a 32 bit che specifica la maschera di bit, in
  *        particolare se il valore i-esimo è pari a 1 allora in quella posizione
  *        si è abilitati ad effettuare il toggle, viceversa non si effettua tale
  *        operazione
  *
  * @retval none
  */

void gpio_custom_Toggle_mask(gpio_custom_TypeDef *gpio, uint32_t reg, uint32_t mask){
	int i;
	for(i=0;i<4;i++){
		if((0x1<<i) == ((0x1<<i) & mask)){
			gpio_custom_Toggle_one(gpio,reg,i);
		}
	}
}

/**
  * @brief  Questa funzione permette di settare il registro di abilitazione in base ad una
  * 		maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param  value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_SetEnable(gpio_custom_TypeDef *gpio,uint32_t mask,gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,gpio_custom_PAD_EN, mask, value);
	printf("Settaggio registro abilitazione\n");
}

/**
  * @brief  Questa funzione permette di leggere il registro di abilitazione in base ad una
  * 		maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_GetEnable(gpio_custom_TypeDef *gpio, uint32_t mask){
	printf("Lettura da registro abilitazione\n");
	return gpio_custom_Read_mask(gpio,gpio_custom_PAD_EN, mask);
}

/**
  * @brief  Questa funzione permette di settare il registro pad_out in base ad una
  * 		maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param  value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_SetValue(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,gpio_custom_PAD_OUT, mask, value);
	printf("Scrittura valori su periferica\n");

}

/**
  * @brief  Questa funzione permette di leggere il registro pad_in in base ad una
  * 		maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_GetValue(gpio_custom_TypeDef *gpio, uint32_t mask){
	printf("Lettura valori da periferica\n");
	return gpio_custom_Read_mask(gpio,gpio_custom_PAD_IN, mask);
}

/**
  * @brief  Questa funzione permette di settare il registro read/write_n in base ad una
  * 		maschera di bit, ossia il modo di funzionamento del GPIO se di input o di output
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param  value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_SetMode(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,gpio_custom_PAD_RW_N, mask, value);
	printf("Definita modalità periferica\n");
}

/**
  * @brief  Questa funzione permette di leggere il registro read/write_n in base ad una
  * 		maschera di bit, ossia di capire la modalità di funzionamento del GPIO
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_GetMode(gpio_custom_TypeDef *gpio, uint32_t mask){
	printf("Lettura tipologia periferica\n");
	return gpio_custom_Read_mask(gpio,gpio_custom_PAD_RW_N, mask);
}

/**
  * @brief  Questa funzione permette di settare il registro interrupt mascherate in base ad una
  * 		maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param  value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_SetInterruptMask(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,gpio_custom_IMR, mask, value);
	printf("Scrittura in registro delle interrupt mascherate\n");
}

/**
  * @brief  Questa funzione permette di leggere il registro delle interrupt mascherate in base ad una
  * 		maschera di bit.
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_GetInterruptMask(gpio_custom_TypeDef *gpio, uint32_t mask){
	printf("Lettura tipologia periferica\n");
	return gpio_custom_Read_mask(gpio,gpio_custom_IMR, mask);
}

/**
  * @brief  Questa funzione permette di settare il registro di stato delle interrupt in base
  *         ad una maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param  value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_SetStatusInterrupt(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,gpio_custom_SR, mask, value);
	printf("Scrittura in registro degli interrupt pendenti\n");
}

/**
  * @brief  Questa funzione permette di leggere il registro di stato delle interrupt in base ad una
  * 		maschera di bit.
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_GetStatusInterrupt(gpio_custom_TypeDef *gpio, uint32_t mask){
	printf("Lettura tipologia periferica\n");
	return gpio_custom_Read_mask(gpio,gpio_custom_SR, mask);
}

/**
  * @brief  Questa funzione permette di settare il registro di ack delle interrupt in base
  *         ad una maschera di bit
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @param  mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *         se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *         scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param  value: valore di tipo gpio_custom_ValueType da scrivere all'interno del registro
  *
  * @retval none
  */

void gpio_custom_SetAckInterrupt(gpio_custom_TypeDef *gpio, uint32_t mask, gpio_custom_ValueType value){
	gpio_custom_Write_mask(gpio,gpio_custom_IACK, mask, value);
	printf("Inviato interrupt ack alla periferica\n");
}

/**
  * @brief  Questa funzione permette di settare il registro delle interrupt globali
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @retval none
  */

void gpio_custom_SetGlobalInterrupt(gpio_custom_TypeDef *gpio, gpio_custom_ValueType value){
	gpio_custom_Write_one(gpio,gpio_custom_GIE, 0x0, value);
	printf("Scrittura in registro interrupt globale\n");
}

/**
  * @brief  Questa funzione permette di leggere il registro delle interrupt globali
  *
  * @param  gpio: puntatore alla struttura gpio_custom_TypeDef che contiene le informazioni
  * 		di configurazione della specifica GPIO
  * @retval intero a 32 bit
  */

uint32_t gpio_custom_GetGlobalInterrupt(gpio_custom_TypeDef *gpio){
	printf("Lettura interrupt globale");
	return gpio_custom_Read_one(gpio, gpio_custom_GIE, 0x0);

}





