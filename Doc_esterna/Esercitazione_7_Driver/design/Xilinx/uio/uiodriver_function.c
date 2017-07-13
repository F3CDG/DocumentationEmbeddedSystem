/**
  ******************************************************************************
  * @file    	uiodriver_function.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	24-June-2017
  * @brief   	Functions used for 'uiodriver' GPIO
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "uiodriver_header.h"

/**
  * @brief  Describes how 'uiodriver' must be used, specifing all supported options.
  * @param  [in]	name: Specifies uiodriver name.
  * @retval None.
  */
void usage(char *name){
	printf("The right way to use this driver is\n");
	printf(BOLDWHITE"%s -d <UIO_DEV_FILE> -c <CHANNEL> -i|-o <VALUE>\n"RESET,name);
	printf("Please, pay attention:\n");
	printf(BOLDWHITE"\t-d"RESET GREEN"\t<UIO_DEV_FILE>"RESET"\n\t\tThe mandatory parameter <UIO_DEV_FILE>\n\t\tspecifies the uio device file corrisponding\n\t\t to GPIO, for example /dev/uio0.\n");
	printf(BOLDWHITE"\n\t-c"RESET GREEN"\t<CHANNEL>"RESET"\n\t\tThe mandatory parameter <CHANNEL> specify\n\t\tthe GPIO channel to use \n\t\tThis number must be 1 or 2");
	printf(BOLDWHITE"\n\t-i"RESET"\n\t\tSpecifies the input value of GPIO \n\t\tcorrisponding to <UIO_DEV_FILE> \n");
	printf(BOLDWHITE"\n\t-o"RESET GREEN"\t<VALUE>"RESET"\n\t\tThe mandatory parameter <VALUE> specify\n\t\tthe value must be write on GPIO output \n");
	return;
}

/**
  * @brief  Parses uiodriver arguments.
  * @param  [in]	argc: number of parameters, passed to main function.
  * @param  [in]	argv: parameters passed to main function
  * @param  [out]	uiod: uio device file corrisponding to GPIO
  * @param  [out]	channel: number corrisponding to GPIO channel
  * @param  [out]	r_w: specifies if the operation on GPIO is a read or a write
  * @param  [out]	value: contains the read value (if the operation on GPIO is a read) 
  *		  	 or the value to write (if the operation on GPIO is a write)
  * @retval Integer status:
  *         -1		An error occurred;
  *	     1		Help function is called;		
  * 	     0		Everything is ok.
  */
int parse_command(int argc,char **argv,char** uiod,int* channel,int* r_w,int* value){
	int index=0;
	int man_opt_d=-1; 	/* Keep track if the mandatory option 'd' is parsed */
	int man_opt_c=-1; 	/* Keep track if the mandatory option 'c' is parsed */
	int i_or_o=0;		/* Keep track if an i|o operation is requested */
	static char *optstring = ":d:c:io:h";

		while((index = getopt(argc, argv, optstring)) != -1) {
				switch(index) {
				case 'd':
					man_opt_d=atoi(optarg);	/* Change mand_opt_d value to memorize that the option 'd' is parsed */
					*uiod=optarg;						
					break;
				case 'c':
					man_opt_c=atoi(optarg);	/* Change mand_opt_c value to memorize that the option 'c' is parsed */
					/* Check if before 'c' option was passed the mandatory option 'd' */
					if(man_opt_d!=-1){
						*channel=atoi(optarg);
					}
					else{
						printf("Missing mandatory parameter 'd'\n");
						usage(argv[0]);
						return -1;
					}
					break;
				case 'i':
					/* Check if before 'i' option was passed the mandatory option 'd' */
					if(man_opt_d!=-1){
						/* Check if before 'i' option was passed the mandatory option 'c' */
						if(man_opt_c!=-1){
							*r_w=READ; /* Set READ operation for GPIO */
							i_or_o=1;
						}
						else{
							printf("Missing mandatory parameter 'c'\n");
							usage(argv[0]);
							return -1;
						}		
					}
					else{
						printf("Missing mandatory parameter 'd'\n");
						usage(argv[0]);
						return -1;
					}
					break;
				case 'o':
					/* Check if before 'o' option was passed the mandatory option 'd' */
					if(man_opt_d!=-1){
						/* Check if before 'o' option was passed the mandatory option 'c' */
						if(man_opt_c!=-1){
							*r_w=WRITE; /* Set WRITE operation for GPIO */
							*value=strtoul(optarg, NULL, 0);
							i_or_o=1;
						}
						else{
							printf("Missing mandatory parameter 'c'\n");
							usage(argv[0]);
							return -1;
						}	
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
	/* Check if channel number is right*/
	if(*channel!=GPIO_CHANNEL_1 && *channel!=GPIO_CHANNEL_2){
		printf("Wrong channel number inserted\n");
		usage(argv[0]);
		return -1;
	}
	
	/* Check if an i|o operation is requested */	
	if(i_or_o==0){
		printf("Can't use "BOLDWHITE"uiodriver "RESET"without specify i|o option\n");
		usage(argv[0]);
		return -1;
	}
	return 0;	
}

/**
  * @brief  Opens uio device file in order to access to it by its virtual address.
  * @param  [out]	fd: file descriptor name.
  * @param  [in]	uiod: uio device file corrisponding to GPIO.
  * @param  [out]	virtual_address: uio device file's virtual address.
  * @retval Integer status:
  *         -1		An error occurred.
  */
int open_device(int* fd, char* uiod, void** virtual_address){
	*fd = open (uiod, O_RDWR);
		if (*fd < 1) {
			perror("Error to open uio device file");
			return -1;
		}
		/* mmap system call returns virtual address of <uiod> file */
		*virtual_address = mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, *fd, 0);
	return 0;
}

/**
  * @brief  Reads the <value> in input to GPIO port.
  * @param  [out]	value: read value.
  * @param  [in]	virtual_address: uio device file's virtual address.
  * @param  [in]	channel: number corrisponding to GPIO channel
  * @retval None.
  */
void read_gpio(int* value,void* virtual_address,int channel){
	/* Set the offsets to access to right gpio channel (1 or 2) */
	int gpio_tri_off;
	int gpio_data_off;
	if(channel==GPIO_CHANNEL_1){
		gpio_tri_off=GPIO_TRI_OFFSET;
		gpio_data_off=GPIO_DATA_OFFSET;
	}
	else{
		gpio_tri_off=GPIO2_TRI_OFFSET;
		gpio_data_off=GPIO2_DATA_OFFSET;
	}	
	/* Set GPIO_TRI register values to configure the GPIO port as input */
	*((unsigned *)(virtual_address + gpio_tri_off)) = 255;
	/* Memorize the value from GPIO port in <value> */
	*value = *((unsigned *)(virtual_address + gpio_data_off));
	printf("The value on channel %d GPIO input is: %08x\n",channel,*value);
}

/**
  * @brief  Writes <value> in output to GPIO port.
  * @param  [in]	value: write value.
  * @param  [in]	uiod: uio device file corrisponding to GPIO
  * @param  [in]	virtual_address: uio device file's virtual address.
  * @param  [in]	channel: number corrisponding to GPIO channel
  * @retval None.
  */
void write_gpio(int value,char* uiod,void* virtual_address, int channel){
	/* Set the offsets to access to right gpio channel (1 or 2) */
	int gpio_tri_off;
	int gpio_data_off;
	if(channel==GPIO_CHANNEL_1){
		gpio_tri_off=GPIO_TRI_OFFSET;
		gpio_data_off=GPIO_DATA_OFFSET;
	}
	else{
		gpio_tri_off=GPIO2_TRI_OFFSET;
		gpio_data_off=GPIO2_DATA_OFFSET;
	}
	/* Set GPIO_TRI register values to configure the GPIO port as output */
	*((unsigned *)(virtual_address + gpio_tri_off)) = 0;
	printf("Value %08x is going to write onto %s (channel %d)\n", value, uiod, channel);
	/* Write <value> in GPIO register */
	*((unsigned *)(virtual_address + gpio_data_off)) = value;

}

/**
  * @brief  Closes /dev/uiox file.
  * @param  [in]	fd: file descriptor name of uio device
  * @param  [in]	virtual_address: uio device's virtual address.
  * @retval None.
  */
void close_device(int fd,void* virtual_address){
	munmap(virtual_address,GPIO_MAP_SIZE);
	close(fd);
}

