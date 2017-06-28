/**
  ******************************************************************************
  * @file    	nodriver.h
  * @author	Colella Gianni - Guida Ciro - Lombardi Daniele
  *		Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	22-June-2017
  * @brief   	library nodriver gpio
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
#define READ 0
#define WRITE 1

/* GPIO page MACRO ------------------------------------------------------*/
#define PAGE_SIZE sysconf(_SC_PAGESIZE) 	/*!< Page size used by SO */
#define MASK_SIZE (~(PAGE_SIZE-1))		/*!< Bit mask to extract page address, without offset */

/* Function prototypes -------------------------------------------------------*/
void usage(char *name);
int parse_command(int argc,char **argv,int* address,int* r_w,int* value);
int open_memory(int* fd,int phy_address, int* page_offset,void** virtual_address);
void read_gpio(int* value,void* virtual_address,int page_offset);
void write_gpio(int value,int phy_address,void* virtual_address, int page_offset);
void close_memory(int fd, void* virtual_address);
