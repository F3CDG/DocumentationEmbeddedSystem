/**
  ******************************************************************************
  * @file    	Elaborato1/1_1_Driver_C_NoInt/gpio_functions.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	10-May-2017
  * @brief   	Questo file mette a disposizione un firmware per gestire il comportamento della periferica
  ******************************************************************************
*/
#include "gpio_header.h"

/*Variabile globale per memorizzare l'indirizzo del base address*/
uint32_t* my_gpio_pointer;

/**
  * @brief  Questa funzione permette di inizializzare correttamente un GPIO
  *
  * @param  base_addr: puntatore ad un intero di 32 bit per il corretto
  * 				   indirizzamento della periferica
  *
  * @retval none
  */
void gpio_init (uint32_t* base_addr){
	/*Verifica se l'indirizzo passato è allineato a 4 byte*/
	assert((uint32_t)base_addr%4==0 && "Base address non allineato a 4 byte");

	my_gpio_pointer=base_addr;

	/*Configurazione pin di GPIO*/
	*(my_gpio_pointer+PAD_EN/4)=INIT_CONFIG_EN;
	*(my_gpio_pointer+PAD_RW_N/4)=INIT_CONFIG_RWN;
}

/**
  * @brief  Questa funzione permette di settare all'interno di uno specifico
  * 		registro, dei valori in base ad una determinata maschera di bit
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *        se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *        scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param mask_value: intero a 32 bit da scrivere all'interno del registro
  * @retval none
  */
void set_value_reg(uint32_t reg, uint32_t mask,uint32_t mask_value){
	*(my_gpio_pointer+reg/4) = (*(my_gpio_pointer+reg/4) & ~mask) | (mask & mask_value);
}

/**
  * @brief  Questa funzione permette di scrivere all'interno di uno specifico
  * 		registro in base ad una determinata maschera di bit
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param set: intero a 8 bit che indica lo spiazzamento interno al registro
  * @param mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *        se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *        scrivere in quella posizione, se 0 allora il valore è mascherato
  * @param value: intero a 8 bit che indica il valore da scrivere
  *
  * @retval none
  */

void gpio_write_mask(uint32_t reg, uint8_t set, uint32_t mask, uint8_t value){
	assert((reg==0 || reg%12!=0) && "Scrittura in una locazione di memoria non consentita");
	mask = mask<<set;
	if(((0x0000000F<<set) & mask)==mask)
		/*Se value è pari a 1 allora si andrà a scrivere tanti 1 in base alla maschera */
		value == HIGH ? set_value_reg(reg,mask,0xF<<set) : set_value_reg(reg,mask,0x0);
}

/**
  * @brief  Questa funzione permette di scrivere un unico all'interno di uno
  * 		specifico registro in base ad una determinata posizione
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param set: intero a 8 bit che indica lo spiazzamento interno al registro
  * @param position: intero a 32 bit che specifica la posizione in cui scrivere
  * @param mask_value: intero a 8 bit che indica il valore da scrivere
  *
  * @retval none
  */

void gpio_write_one(uint32_t reg, uint8_t set, uint32_t position, uint8_t value){
	gpio_write_mask(reg, set, 0x1<<position, value);
}

/**
  * @brief  Questa funzione permette di leggere all'interno di uno specifico
  *         registro in base ad una determinata maschera di bit
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param set: intero a 8 bit che indica lo spiazzamento interno al registro
  * @param mask: intero a 32 bit che specifica una maschera di bit, in particolare
  *        se all'i-esima posizione vi è un valore pari a 1 allora si è abilitati a
  *        leggere in quella posizione, viceversa se 0
  *
  * @retval intero a 32 bit
  */

uint32_t gpio_read_mask(uint32_t reg,uint8_t set, uint32_t mask){
	mask = mask<<set;
	if(((0x0000000F<<set) & mask) == mask){
		return *(my_gpio_pointer + reg/4) & mask;
	}
	return -1;
}

/**
  * @brief  Questa funzione permette di leggere un unico valore all'interno di uno
  * 		specifico registro in base ad una determinata posizione
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui scrivere
  * @param set: intero a 8 bit che indica lo spiazzamento interno al registro
  * @param position: intero a 32 bit che specifica la posizione in cui leggere
  *
  * @retval intero a 32 bit
  */

uint32_t gpio_read_one(uint32_t reg, uint8_t set, uint32_t position){
	return gpio_read_mask(reg,set,0x1<<position) == (0x1<<(position+set));
}

/**
  * @brief  Questa funzione permette il toggle di un singolo valore all'interno di
  * 		uno specifico registro in base ad una determinata posizione
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui effettuare il toggle
  * @param set: intero a 8 bit che indica lo spiazzamento interno al registro
  * @param position: intero a 8 bit che specifica la posizione in cui effettuare
  *        il toggle
  *
  * @retval none
  */

void gpio_toggle_one(uint32_t reg, uint8_t set,uint8_t position){
	gpio_read_one(reg,set,position) == HIGH ? gpio_write_one(reg ,set,position,LOW) : gpio_write_one(reg,set,position,HIGH);
}

/**
  * @brief  Questa funzione permette il toggle di un gruppo di bit all'interno di
  * 		uno specifico registro in base ad una determinata masschera
  *
  * @param  reg: intero a 32 bit che indica lo spiazzamento rispetto all'indirizzo
  * 	    base del registro in cui effettuare il toggle
  * @param set: intero a 8 bit che indica lo spiazzamento interno al registro
  * @param position: intero a 32 bit che specifica la maschera di bit, in
  *        particolare se il valore i-esimo è pari a 1 allora in quella posizione
  *        si è abilitati ad effettuare il toggle, viceversa non si effettua tale
  *        operazione
  *
  * @retval none
  */

void gpio_toggle_mask(uint32_t reg, uint8_t set,uint32_t mask){
	int i;
	for(i=0;i<4;i++){
		if((0x1<<(set*4+i)) == ((0x1<<(set*4+i)) & mask)){
			gpio_toggle_one(reg, set,i+set*4);
		}
	}
}




