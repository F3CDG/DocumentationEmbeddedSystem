/**
  ******************************************************************************
  * @file    	nodriver_function.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	22-June-2017
  * @brief   	Functions used for nodriver gpio
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "nodriver_header.h"

/**
  * @brief  Describes how 'nodriver' must be used, specifing all supported options.
  * @param  name: Specifies nodriver name.
  * @retval None.
  */
void usage(char *name){
	printf("The right way to use this driver is\n");
	printf(BOLDWHITE"%s -g <GPIO_ADDRESS> -i|-o <VALUE>\n"RESET,name);
	printf("Please, pay attention:\n");
	printf(BOLDWHITE"\t-g"RESET GREEN"\t<GPIO_ADDR>"RESET"\n\t\tThe mandatory parameter <GPIO_ADDR>\n\t\tspecifies the physical address of GPIO.\n\t\t<GPIO_ADDR> can be expressed in\n\t\tdecimal representation (no prefix),\n\t\toctal representation (0 prefix)\n\t\tor hexadecimal representation\n\t\t(0x or 0X prefix)\n");
	printf(BOLDWHITE"\n\t-i"RESET"\n\t\tSpecifies the input value of GPIO at\n\t\t<GPIO_ADDR> physical address \n");
	printf(BOLDWHITE"\n\t-o"RESET GREEN"\t<VALUE>"RESET"\n\t\tThe mandatory parameter <VALUE> specify\n\t\tthe value must be write on GPIO output \n");
	return;
}

/**
  * @brief  Parses nodriver arguments.
  * @param  argc: 	number of parameters, passed to main function.
  * @param  argv: 	parameters passed to main function
  * @param  address: 	physical address of GPIO component
  * @param  r_w: 	specifies if the operation on GPIO is a read or a write
  * @param  value: 	contains the read value (if the operation on GPIO is a read) 
  *		   or the value to write (if the operation on GPIO is a write)
  * @retval Integer status.
  *         -1		An error occurred.
  *	     1		Help function is called.		
  * 	     0		Everything is ok.
  */
int parse_command(int argc,char **argv,int* address,int* r_w,int* value){
	int index=0;
	int mandatory_opt=-1; /*!< Keep track if the mandatory option 'g' is parsed */
	int i_or_o=0;	/*!< Keep track if an i|o operation is requested */
	static char *optstring = ":g:io:h";

		while((index = getopt(argc, argv, optstring)) != -1) {
				switch(index) {
				case 'g':
					mandatory_opt=atoi(optarg);	/*!< Change mandatory_opt value to memorize that the mandatory option is parsed */
					*address=strtoul(optarg,NULL, 0);
					/*! Check if address passed is not 0x0 */
					if(*address==0){
						printf("Wrong physical address inserted!\n");
						return -1;
					}						
					break;
				case 'i':
					/* Check if before 'i' option was passed the mandatory option 'g' */
					if(mandatory_opt!=-1){
						*r_w=READ; /*!< Set READ operation for GPIO */
						i_or_o=1;
					}
					else{
						printf("Missing mandatory parameter 'g'\n");
						usage(argv[0]);
						return -1;
					}
					break;
				case 'o':
					/*! Check if before 'o' option was passed the mandatory option 'g' */
					if(mandatory_opt!=-1){
						*r_w=WRITE; /*!< Set WRITE operation for GPIO */
						*value=strtoul(optarg, NULL, 0);
						i_or_o=1;
					}
					else{
						printf("Missing mandatory parameter 'g'\n");
						usage(argv[0]);
						return -1;
					}
					break;
				case 'h':
					usage(argv[0]);
					return 1;
					break;
				case ':':
					printf("Missing argument for '%c' option\n", optopt);
					usage(argv[0]);
					return -1;
					break;
				case '?':
					printf("Option '%c' not recognized\n", optopt);
					usage(argv[0]);
					return -1;
					break;
				default:
					usage(argv[0]);
					return -1;
					break;
				}
		
			}
	/*! Check if an i|o operation is requested */	
	if(i_or_o==0){
		printf("Can't use "BOLDWHITE"nodriver "RESET"without specify i|o option\n");
		usage(argv[0]);
		return -1;
	}
	return 0;
}

/**
  * @brief  Opens /dev/mem file in order to access GPIO address and calculates page offset and virtual address of GPIO file.
  * @param  phy_address: 	GPIO's physical address.
  * @param  page_offset: 	page offset which GPIO finds.
  * @param  virtual_address: 	GPIO's virtual address.
  * @retval Integer status.
  *         -1		An error occurred.
  */
int open_memory(int* fd, int phy_address, int* page_offset,void** virtual_address){
	*fd = open ("/dev/mem", O_RDWR);
		if (*fd < 1) {
			perror("Error to open /dev/mem file");
			return -1;
		}
		int page_addr = phy_address & MASK_SIZE;
		/*! Calculates page offset */
		*page_offset = phy_address - page_addr;
		/*! mmap system call returns virtual address of GPIO */
		*virtual_address = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, *fd, page_addr);
	return 0;
}

/**
  * @brief  Reads the <value> in input to GPIO port.
  * @param  value: 		read value.
  * @param  virtual_address: 	GPIO's virtual address.
  * @param  page_offset: 	page offset which GPIO finds.
  * @retval None.
  */
void read_gpio(int* value,void* virtual_address,int page_offset){
	*value = *((unsigned *)(virtual_address + page_offset));
	printf("The value on GPIO input is: %08x\n",*value);
}

/**
  * @brief  Writes <value> in output to GPIO port.
  * @param  value: 		write value.
  * @param  phy_address: 	GPIO's physical address.
  * @param  virtual_address: 	GPIO's virtual address.
  * @param  page_offset: 	page offset which GPIO finds.
  * @retval None.
  */
void write_gpio(int value,int phy_address,void* virtual_address, int page_offset){
	printf("Going to write onto %08x the value %08x\n", phy_address, value );
	*((unsigned *)(virtual_address + page_offset)) = value;
}

/**
  * @brief  Closes /dev/mem file.
  * @param  virtual_address: 	GPIO's virtual address.
  * @retval None.
  */
void close_memory(int fd,void* virtual_address){
	munmap(virtual_address,PAGE_SIZE);
	close(fd);
}

