/**
  ******************************************************************************
  * @file    	main.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	24-June-2017
  * @brief   	driver "uiodriver" to control gpio
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "uiodriver_header.h"

int main(int argc, char *argv[]){
	/*!< First step */
	/*! Variables Declaration */
	int file_descriptor; /* uio file descriptor */
	int r_w=READ;	/* Saves operation's type to do with GPIO */
	int r_or_w_value = 0;	/* Saves read/write value from/on GPIO */
	int ret_parse;	/* Returned value from parse_command function */

	char* uio_dev;		/* uio device file name */
	int cha;		/* channel number of GPIO */
	void *virt_addr;	/* Virtual address of <uio_dev> */
	/*!< Second step */
	/*! parse_command function is called to parse all argument passed to driver uiodriver */
	/* Check if parse_command function returns error */
	if((ret_parse=parse_command(argc,argv,&uio_dev,&cha,&r_w,&r_or_w_value))==1)
		return 0;
	else
		if(ret_parse==-1)
			return -1;
	/*!< Third step */
	/*! open_device function is called to achieve gpio virtual address */
	/* Check if open_device function returns error */
	if(open_device(&file_descriptor,uio_dev,&virt_addr)==-1){
		printf("nodriver aborted!\n");
		return -1;
	}
	//
	/*!< Fourth */
	/*! read_gpio is called if r_w variable is READ, otherwise is called write_gpio function */
	/* Calls conveniently read_gpio or write_gpio function */
	if (r_w == READ) read_gpio(&r_or_w_value,virt_addr,cha);
	else write_gpio(r_or_w_value,uio_dev,virt_addr,cha);
	/*!< First step */
	/*! close_device function is called to close device file and delete file descriptor */
	close_device(file_descriptor,virt_addr);

	return 0;
  }
