/**
  ******************************************************************************
  * @file    	uiodriver.h
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	24-June-2017
  * @brief   	library 'uiodriver' for GPIO
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

/* Colour MACRO --------------------------------------------------------------*/
#define RESET		"\033[0m"		/*!< Reset colour */
#define GREEN   	"\033[32m"     		/*!< Green colour */
#define BOLDWHITE	"\033[1m\033[37m"	/*!< Bold White colour */

/* GPIO operation MACRO ------------------------------------------------------*/
#define READ 0					/*!< Read operation is set */
#define WRITE 1					/*!< Write operation is set */

/* GPIO page MACRO -----------------------------------------------------------*/
#define GPIO_MAP_SIZE 		0x10000		/*!< Bit mask to extract page address, without offset */
#define GPIO_DATA_OFFSET	0x00		/*!< Offset of GPIO_DATA register */
#define GPIO_TRI_OFFSET 	0x04		/*!< Offset of GPIO_TRI register */
#define GPIO2_DATA_OFFSET 	0x08		/*!< Offset of GPIO2_DATA register */
#define GPIO2_TRI_OFFSET 	0x0C		/*!< Offset of GPIO2_TRI register */

/* GPIO channel MACRO --------------------------------------------------------*/
#define GPIO_CHANNEL_1 1	/*!< GPIO channel 1 selected */
#define GPIO_CHANNEL_2 2	/*!< GPIO channel 2 selected */

/* Function prototypes -------------------------------------------------------*/
void usage(char *name);/*!< usage function is called to help the user */
int parse_command(int argc,char **argv,char** uiod,int* channel,int* r_w,int* value);/*!< parse_command function is called to parse arguments passed to driver */
int open_device(int* fd, char* uiod, void** virtual_address);/*!< open_device function is called to open file associated to uio device */
void read_gpio(int* value,void* virtual_address, int channel);/*!< read_gpio function is called to do read operation from GPIO */
void write_gpio(int value,char* uiod,void* virtual_address, int channel);/*!< write_gpio function is called to do write operation on GPIO */
void close_device(int fd, void* virtual_address);/*!< close_device function is called to close uio file */
