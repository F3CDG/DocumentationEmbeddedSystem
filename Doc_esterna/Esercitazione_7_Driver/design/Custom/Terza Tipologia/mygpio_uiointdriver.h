/**
  ******************************************************************************
  * @file    	mygpio_uiodriver.h
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	7-July-2017
  * @brief   	library mygpio_uiodriver gpio
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "gpio_custom.h"

/**
  * @brief  Data Structure to manage data associated to GPIO custom peripheral
  */
typedef struct{
	gpio_custom_TypeDef* gpio_custom;	/*!< Contains virtual address (page offset included) and offset registers value of GPIO device uio*/
	char* uiod;				/*!< uio device file corrisponding to GPIO */
	int fd;					/*!< Memory file descriptor */
	int r_w;				/*!< Saves operation's type to do with GPIO */
	uint32_t r_or_w_value;			/*!< Saves read/write value from/on GPIO */
}mygpio_TypeDef;

/* GPIO port MACRO -----------------------------------------------------------*/
#define GPIO_PORT 0xF 				/*!< GPIO ports selected to read/write */

/* Colour MACRO --------------------------------------------------------------*/
#define RESET		"\033[0m"		/*!< Reset colour */
#define GREEN   	"\033[32m"     		/*!< Green colour */
#define BOLDWHITE	"\033[1m\033[37m"	/*!< Bold White colour */

/* GPIO operation MACRO ------------------------------------------------------*/
#define READ 	1				/*!< Read operation is set */
#define WRITE 	0				/*!< Write operation is set */

/* GPIO page MACRO -----------------------------------------------------------*/
#define PAGE_SIZE sysconf(_SC_PAGESIZE) 	/*!< Page size used by SO */

/* Function prototypes -------------------------------------------------------*/
void mygpio_usage(char *name); /*!< usage function is called to help the user */
int mygpio_parse_command(int argc,char **argv,mygpio_TypeDef* mygpio);	/*!< parse_command function is called to parse arguments passed to driver */
int mygpio_open_memory(mygpio_TypeDef* mygpio);	/*!< mygpio_open_memory function is called to open memory file */
int mygpio_read_gpio(mygpio_TypeDef* mygpio); 	/*!< mygpio_read_gpio function is called to do read operation from GPIO */
void mygpio_write_gpio(mygpio_TypeDef* mygpio); /*!< mygpio_write_gpio function is called to do write operation on GPIO */
void mygpio_close_memory(mygpio_TypeDef* mygpio);/*!< mygpio_close_memory function is called to close memory file */

