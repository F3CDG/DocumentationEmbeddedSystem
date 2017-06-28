/**
  ******************************************************************************
  * @file    	nodriver.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	22-June-2017
  * @brief   	driver "nodriver" to control gpio
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "nodriver_header.h"

int main(int argc, char *argv[]){
	
	int file_descriptor; /*!< Memory file descriptor */
	int r_w=READ;	/*!< Saves operation's type to do with GPIO */
	int r_or_w_value = 0;	/*!< Saves read/write value from/on GPIO */
	int ret_parse;	/*!< Returned value from parse_command function */

	unsigned phy_addr, page_off;	/*!< Respectively physical address of GPIO and page offset */
	void *virt_addr;	/*!< Virtual address of GPIO */

	/*! Check if parse_command function returns error */
	if((ret_parse=parse_command(argc,argv,&phy_addr,&r_w,&r_or_w_value))==1)
		return 0;
	else
		if(ret_parse==-1)
			return -1;
	
	/*! Check if open_memory function returns error */
	if(open_memory(&file_descriptor,phy_addr,&page_off,&virt_addr)==-1){
		printf("nodriver aborted!\n");
		return -1;
	}
	
	/*! Calls conveniently read_gpio or write_gpio */
	if (r_w == READ) read_gpio(&r_or_w_value,virt_addr,page_off);
	else write_gpio(r_or_w_value,phy_addr,virt_addr,page_off);
	
	close_memory(file_descriptor,virt_addr);

	return 0;
}
