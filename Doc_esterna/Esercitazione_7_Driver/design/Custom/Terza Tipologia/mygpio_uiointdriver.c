/**
  ******************************************************************************
  * @file    	mygpio_uiodriver.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	7-July-2017
  * @brief   	Functions used for mygpio_uiodriver gpio
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mygpio_uiointdriver.h"

/**
  * @brief  Describes how 'mygpio_uiodriver' must be used, specifing all supported options.
  * @param  [in]	name: Specifies mygpio_nodriver name.
  * @retval None.
  */
void mygpio_usage(char *name){
	printf("The right way to use this driver is\n");
	printf(BOLDWHITE"%s -d <UIO_DEV_FILE> -i|-o <VALUE>\n"RESET,name);
	printf("Please, pay attention:\n");
	printf(BOLDWHITE"\t-d"RESET GREEN"\t<UIO_DEV_FILE>"RESET"\n\t\tThe mandatory parameter <UIO_DEV_FILE>\n\t\tspecifies the uio device file corrisponding\n\t\t to GPIO, for example /dev/uio0.\n");
	printf(BOLDWHITE"\n\t-i"RESET"\n\t\tSpecifies the input value of GPIO \n\t\tcorrisponding to <UIO_DEV_FILE> \n");
	printf(BOLDWHITE"\n\t-o"RESET GREEN"\t<VALUE>"RESET"\n\t\tThe mandatory parameter <VALUE> specify\n\t\tthe value must be write on GPIO output \n");
	return;
	return;
}

/**
  * @brief  Parses nodriver arguments.
  * @param  [in]	argc: number of parameters, passed to main function.
  * @param  [in]	argv: parameters passed to main function
  * @param  [out] 	mygpio: data stucture that contains GPIO data
  * @retval Integer status:
  *	    -1		An error occurred;
  *	     1		Help function is called;		
  * 	     0		Everything is ok.
  */
int mygpio_parse_command(int argc,char **argv,mygpio_TypeDef* mygpio){
	int index=0;
	int mandatory_opt=-1; 			/* Keep track if the mandatory option 'g' is parsed */
	int i_or_o=0;				/* Keep track if an i|o operation is requested */
	static char *optstring = ":d:io:h";	/* Allowed parameters */

		while((index = getopt(argc, argv, optstring)) != -1) {
				switch(index) {
				case 'd':
					mandatory_opt=atoi(optarg);	/* Change mandatory_opt value to memorize that the mandatory option is parsed */
					mygpio->uiod=optarg;		/* Saves device uio path name */					
					break;
				case 'i':
					/* Check if before 'i' option was passed the mandatory option 'g' */
					if(mandatory_opt!=-1){
						mygpio->r_w=READ; /* Set READ operation for GPIO */
						i_or_o=1;
					}
					else{
						printf("Missing mandatory parameter 'g'\n");
						mygpio_usage(argv[0]);
						return -1;
					}
					break;
				case 'o':
					/* Check if before 'o' option was passed the mandatory option 'g' */
					if(mandatory_opt!=-1){
						mygpio->r_w=WRITE; /* Set WRITE operation for GPIO */
						mygpio->r_or_w_value=strtoul(optarg, NULL, 0);
						i_or_o=1;
					}
					else{
						printf("Missing mandatory parameter 'g'\n");
						mygpio_usage(argv[0]);
						return -1;
					}
					break;
				case 'h':
					mygpio_usage(argv[0]);
					return 1;
					break;
				case ':':
					printf("Missing argument for '%c' option\n", optopt);
					mygpio_usage(argv[0]);
					return -1;
					break;
				case '?':
					printf("Option '%c' not recognized\n", optopt);
					mygpio_usage(argv[0]);
					return -1;
					break;
				default:
					mygpio_usage(argv[0]);
					return -1;
					break;
				}
		
			}
	/* Check if an i|o operation is requested */	
	if(i_or_o==0){
		printf("Can't use "BOLDWHITE"nodriver "RESET"without specify i|o option\n");
		mygpio_usage(argv[0]);
		return -1;
	}
	return 0;
}

/**
  * @brief  Opens /dev/mem file in order to access custom GPIO address and calculates page offset and virtual address of GPIO file.
  * @param  [inout]	mygpio: data stucture that contains GPIO data
  * @retval Integer status:
  *         -1		An error occurred.
  */
int mygpio_open_memory(mygpio_TypeDef* mygpio){
	
	mygpio->fd = open (mygpio->uiod, O_RDWR);
		if (mygpio->fd < 1) {
			perror("Error to open uio device file");
			return -1;
		}
		/* mmap system call returns virtual page address of GPIO device uio */
		void* vrt_add = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mygpio->fd, 0);
		mygpio->gpio_custom->base_address=vrt_add; // virtual base address extracted from mmap
	return 0;
}

/**
  * @brief  Reads the <mygpio->r_or_w_value> in input to GPIO port.
  * @param  [inout]	mygpio: data stucture that contains GPIO data
  * @retval None.
  */
int mygpio_read_gpio(mygpio_TypeDef* mygpio){

	/* number of bytes read */
	ssize_t num_byte;
	/* to test if read operation is correct */
	uint32_t test_read=1;

	/* Set PAD_EN register to enable read operation from GPIO port */
	gpio_custom_SetEnable(mygpio->gpio_custom, GPIO_PORT, HIGH);
	/* Set PAD_RW_N register of GPIO to configure the GPIO port as input */	
	gpio_custom_SetMode(mygpio->gpio_custom, GPIO_PORT, READ);
	
	while(1){
		/* Enable Global Interrupt of GPIO */
		gpio_custom_SetGlobalInterrupt(mygpio->gpio_custom, HIGH);
		

		/* Enable Interrupt on all port of GPIO*/		
		gpio_custom_SetInterruptMask(mygpio->gpio_custom, GPIO_PORT, HIGH);		

		/* to suspend the process on GPIO peripheral */
		num_byte=read(mygpio->fd,&test_read,sizeof(test_read));
		if(num_byte!=sizeof(test_read)){
			printf("It occurs an error on read opeartion\n");
			return -1;
		}
		else{
			/* Disable Global Interrupt of GPIO */
			gpio_custom_SetGlobalInterrupt(mygpio->gpio_custom, LOW);
			/* Disable Interrupt on all port of GPIO*/
			gpio_custom_SetInterruptMask(mygpio->gpio_custom, GPIO_PORT, LOW);	

			/* Memorize the value from GPIO port in <mygpio->r_or_w_value> */
			mygpio->r_or_w_value = gpio_custom_GetValue(mygpio->gpio_custom, GPIO_PORT);
			printf("The value in input to GPIO %s is: %08x\n",mygpio->uiod,mygpio->r_or_w_value);			
		}

		/* In this case the driver don't read the register until the input that cause the interrupt is set to default low position */
	//	while(*(unsigned*)gpio_custom_GetValue(mygpio->gpio_custom, GPIO_PORT)!=0);
		
		/* test reenable interrupt */
		ssize_t re_enable = 1;
		/* to clean interrupt */
		re_enable = write(mygpio->fd, &re_enable, sizeof(re_enable)); 

		if (re_enable < sizeof(re_enable)) {
			perror("write");
			close(mygpio->fd);
			exit(EXIT_FAILURE);
		}
		/* Sent ack to GPIO peripheral */
		
		gpio_custom_SetAckInterrupt(mygpio->gpio_custom,gpio_custom_GetStatusInterrupt(mygpio->gpio_custom, GPIO_PORT), HIGH);
	}
	return 1;
}

/**
  * @brief  Writes <mygpio->r_or_w_value> in output to GPIO port.
  * @param  [inout]	mygpio: data stucture that contains GPIO data
  * @retval None.
  */
void mygpio_write_gpio(mygpio_TypeDef* mygpio){
	gpio_custom_SetEnable(mygpio->gpio_custom, GPIO_PORT, HIGH);
	gpio_custom_SetMode(mygpio->gpio_custom, GPIO_PORT, WRITE);
	printf("Going to write onto GPIO %s the value %08x\n", mygpio->uiod, mygpio->r_or_w_value);
	gpio_custom_SetValue(mygpio->gpio_custom, GPIO_PORT, LOW);  // Cleans previous values
	gpio_custom_SetValue(mygpio->gpio_custom, mygpio->r_or_w_value, HIGH);
}

/**
  * @brief  Closes /dev/mem file.
  * @param  [in]	mygpio: data stucture that contains GPIO data
  * @retval None.
  */
void mygpio_close_memory(mygpio_TypeDef* mygpio){
	munmap((void*)(mygpio->gpio_custom->base_address),PAGE_SIZE);
	close(mygpio->fd);
}
