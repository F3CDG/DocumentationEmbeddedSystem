/**
  ******************************************************************************
  * @file    	Elaborato7/Driver_GPIO_Custom/Prima_Tipologia/mygpio_nodriver.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version 	V1.0
  * @date    	7-July-2017
  * @brief   	Functions used for mygpio_nodriver gpio
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mygpio_nodriver.h"

/**
  * @brief  Describes how 'mygpio_nodriver' must be used, specifing all supported options.
  * @param  [in]	name: Specifies mygpio_nodriver name.
  * @retval None.
  */
void mygpio_usage(char *name){
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
	static char *optstring = ":g:io:h";	/* Allowed parameters */

		while((index = getopt(argc, argv, optstring)) != -1) {
				switch(index) {
				case 'g':
					mandatory_opt=atoi(optarg);	/* Change mandatory_opt value to memorize that the mandatory option is parsed */
					mygpio->gpio_phy_address=strtoul(optarg,NULL, 0);
					/* Check if address passed is not 0x0 */
					if(mygpio->gpio_phy_address==0){
						printf("Wrong physical address inserted!\n");
						return -1;
					}						
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
	
	mygpio->fd = open ("/dev/mem", O_RDWR);
		if (mygpio->fd < 1) {
			perror("Error to open /dev/mem file");
			return -1;
		}
		int page_phy_addr = (mygpio->gpio_phy_address) & MASK_SIZE;
		printf("page_phy_addr %d\n",page_phy_addr); //debug
		/* Calculates page offset */
		off_t page_offset = (off_t)((mygpio->gpio_phy_address) - page_phy_addr);
		printf("page_offset %lu\n",page_offset); //debug
		/* mmap system call returns virtual page address of GPIO */
		void* vrt_add = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mygpio->fd, page_phy_addr);
		mygpio->gpio_custom->base_address=vrt_add + page_offset; // base address includes page offset
	return 0;
}

/**
  * @brief  Reads the <mygpio->r_or_w_value> in input to GPIO port.
  * @param  [inout]	mygpio: data stucture that contains GPIO data
  * @retval None.
  */
void mygpio_read_gpio(mygpio_TypeDef* mygpio){
	gpio_custom_SetEnable(mygpio->gpio_custom, GPIO_PORT, HIGH);
	gpio_custom_SetMode(mygpio->gpio_custom, GPIO_PORT, READ);
	mygpio->r_or_w_value=gpio_custom_GetValue(mygpio->gpio_custom,GPIO_PORT);
	printf("The value on GPIO %08x input is: %08x\n",mygpio->gpio_phy_address,mygpio->r_or_w_value);
}

/**
  * @brief  Writes <mygpio->r_or_w_value> in output to GPIO port.
  * @param  [inout]	mygpio: data stucture that contains GPIO data
  * @retval None.
  */
void mygpio_write_gpio(mygpio_TypeDef* mygpio){
	gpio_custom_SetEnable(mygpio->gpio_custom, GPIO_PORT, HIGH);
	gpio_custom_SetMode(mygpio->gpio_custom, GPIO_PORT, WRITE);
	printf("Going to write onto GPIO %08x the value %08x\n", mygpio->gpio_phy_address, mygpio->r_or_w_value);
	gpio_custom_SetValue(mygpio->gpio_custom, GPIO_PORT, LOW);
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
