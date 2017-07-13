/**
  ******************************************************************************
  * @file    	Elaborato7/Driver_GPIO_Xilinx/Terza_Tipologia/uiointdriver_function.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	3-July-2017
  * @brief   	Functions used for 'uiointdriver' GPIO
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "uiointdriver_header.h"

/**
  * @brief  Describes how 'uiointdriver' must be used, specifing all supported options.
  * @param  [in]	name: Specifies uiointdriver name.
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
  * @brief  Parses uiointdriver arguments.
  * @param  [in]	argc: number of parameters, passed to main function.
  * @param  [in]	argv: parameters passed to main function
  * @param  [out]	uiod: uio device file corrisponding to GPIO
  * @param  [out]	channel: number corrisponding to GPIO channel
  * @param  [out]	r_w: specifies if the operation on GPIO is a read or a write
  * @param  [out]	value: contains the read value (if the operation on GPIO is a read) 
  *		  	or the value to write (if the operation on GPIO is a write)
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
		printf("Can't use "BOLDWHITE"uiointdriver "RESET"without specify i|o option\n");
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
  * @param  [in]	fd: file descriptor name.
  * @param  [out]	value: read value.
  * @param  [in]	uiod: uio device file corrisponding to GPIO
  * @param  [in]	virtual_address: uio device file's virtual address.
  * @param  [in]	channel: number corrisponding to GPIO channel
  * @retval None.
  */
int read_gpio(int fd,int* value,char* uiod, void* virtual_address, int channel){
	/* number of bytes read */
	ssize_t num_byte;
	/* to test if read operation is correct */
	uint32_t test_read=1;

	/* Set the offsets to access to right gpio channel (1 or 2) */
	int gpio_tri_off;
	int gpio_data_off;
	unsigned long gpio_cha_interrupt_en;
	unsigned long gpio_cha_interrupt_dis;
	unsigned long gpio_cha_isr;
	if(channel==GPIO_CHANNEL_1){
		gpio_tri_off=GPIO_TRI_OFFSET;
		gpio_data_off=GPIO_DATA_OFFSET;
		gpio_cha_interrupt_en=CHANNEL_1_IER_ENABLE;
		gpio_cha_interrupt_dis=CHANNEL_1_IER_DISABLE;
		gpio_cha_isr=CHANNEL_1_ISR;
	}
	else{
		gpio_tri_off=GPIO2_TRI_OFFSET;
		gpio_data_off=GPIO2_DATA_OFFSET;
		gpio_cha_interrupt_en=CHANNEL_2_IER_ENABLE;
		gpio_cha_interrupt_dis=CHANNEL_2_IER_DISABLE;
		gpio_cha_isr=CHANNEL_2_ISR;
	}
	/*----------------------------------------------------------*/

	/* Set GPIO_TRI register values to configure the GPIO port as input */
	*((unsigned *)(virtual_address + gpio_tri_off)) = 255;

	while(1){

		/* Enable Global Interrupt of GPIO */
		GPIO_Interrupt(virtual_address,GPIO_GIER_OFFSET,GPIO_GIER_ENABLE);
		/* Enable Channel Interrut of GPIO */
		GPIO_Interrupt(virtual_address,GPIO_IP_IER_OFFSET,gpio_cha_interrupt_en);

		/* to suspend the process on gpio peripheral */
		num_byte=read(fd,&test_read,sizeof(test_read));
		if(num_byte!=sizeof(test_read)){
			printf("It occurs an error on read opeartion\n");
			return -1;
		}
		else{
			/* Disable Global Interrupt of GPIO */
			GPIO_Interrupt(virtual_address,GPIO_GIER_OFFSET,GPIO_GIER_DISABLE);
			/* Disable Channel Interrut of GPIO */
			GPIO_Interrupt(virtual_address,GPIO_IP_IER_OFFSET,gpio_cha_interrupt_dis);
	
			/* Memorize the value from GPIO port in <value> */
			*value = *((unsigned *)(virtual_address + gpio_data_off));
			printf("The value on channel %d of %s input is: %08x\n",channel,uiod,*value);
		}
	
/* In this case the driver don't read the register until the input that cause the interrupt is set to default low position */
//		while(*((unsigned *)(virtual_address + gpio_data_off))!=0);

		/* test reenable interrupt */
		ssize_t re_enable = 1;
		/* to clean interrupt */
		re_enable = write(fd, &re_enable, sizeof(re_enable)); 
		if (re_enable < sizeof(re_enable)) {
			perror("write");
			close(fd);
			exit(EXIT_FAILURE);
		}
	
		/* Sent ack to GPIO peripheral */
		GPIO_Interrupt(virtual_address, GPIO_IP_ISR_OFFSET, gpio_cha_isr);
	}
	
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
	printf("The value %08x is going to write onto %s (channel %d)\n", value, uiod, channel);
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

/**
  * @brief  According to the offset <reg>, modifies the values of enable registers of GPIO custom peripheral
  * @param  [in]	base_add: virtual base address of GPIO peripheral
  * @param  [in]	reg: offset value of selected register
  * @param  [in]	mask: register value
  * @retval 1.
  */
int GPIO_Interrupt(long* base_add,int reg, unsigned long mask)
{
	*((unsigned *)(base_add + (reg/4)))=mask;	
	return 1;
}
