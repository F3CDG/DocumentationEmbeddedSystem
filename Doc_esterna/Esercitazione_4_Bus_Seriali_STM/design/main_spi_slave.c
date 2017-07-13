/**
  ******************************************************************************
  * @file    	main.c
  * @authors	Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @date    	19-June-2017
  * @brief   	MAIN SLAVE
   ******************************************************************************
  *	 In questo programma è configurato il main di una periferica che si comporta da SLAVE
  *	 in una comunicazione attraverso il bus SPI.
  *
  *
  * 		SPI2 SLAVE connection configuration
  * 		(MASTER_SCK ) ------> PB13 (SLAVE_SCK)
  * 		(MASTER_MISO) ------> PB14 (SLAVE_MISO)
  * 		(MASTER_MOSI) ------> PB15 (SPI2_MOSI)
  * 		(MASTER_NSS ) ------> PB12 (SPI2_NSS)
  *
  ******************************************************************************
  */
#include "main.h"

/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;
/* Buffer utilizzati per la comunicazione */
/* Dato è il buffer che contiene ciò che si trasmette(utile per fare un confronto con ciò che si riceve) */
uint8_t dato[] ="****MESSAGGIO SPI****";
uint8_t receive_data[SIZE];

/* Buffer used for reception */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
/* Private functions ---------------------------------------------------------*/
void setup_LED();
void setup_SPI();


int main(void)
{

  HAL_Init();
  setup_LED();
  setup_SPI();

  SystemClock_Config();
	  /* Infinite loop */
  while (1)
  {
	  if(HAL_SPI_Receive_IT(&SpiHandle, (uint8_t *)receive_data,SIZE) != HAL_OK)

		  {
			Error_Handler();
		  }
		  while (HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_READY);

  }
}


/**
  * @brief  Funzione in cui si configura i LED.
  * @note   Pin PD12:	LED4
  * @note	Pin PD13:	LED3
  * @note	Pin PD14:	LED5
  * @note	Pin PD15:	LED6
  * @retval None
  *  **/
void setup_LED(){
	  /* Configure LED3, LED4, LED5 and LED6 */
		GPIO_InitTypeDef GPIO_InitStruct;

	  /* Abilitazione del clock alla porta GPIOD */
	  __HAL_RCC_GPIOD_CLK_ENABLE();

	  /*Configurazione dei 4 LED: PD12,PD13,PD14,PD15 */
	   GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	   GPIO_InitStruct.Pull = GPIO_NOPULL;
	   GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	   HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
}

/**
  * @brief  Funzione in cui si settano i parametri SPI
  * @note	Si nota che la modalità è SLAVE
  * @retval None
  *    **/

void setup_SPI(){

	  SpiHandle.Instance               = SPI2;
	  SpiHandle.Init.Mode 			   = SPI_MODE_SLAVE;
	  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	  SpiHandle.Init.CRCPolynomial     = 7;
	  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

	  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }
}

/**
  * @brief  Funzione Callback richiamata quando la ricezione è completata
  * @param [in] hspi: gestore spi
  * @note	Si esegue un'operazione di Toggle sul PD15(LED6)
  * @retval None
  * **/
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi->Instance==SpiHandle.Instance)
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_15);
}

/**
  * @brief  Funzione Callback richiamata quando c'è stato un'errore durante il trasferimento
  * @param  hspi: gestore spi
  * @note	Si esegue un'operazione di Toggle sul PD14(LED5)
  * @retval None
  * **/
 void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi){
	if(hspi->Instance==SpiHandle.Instance)
	 HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_15);
}

/**
  * @brief Questa funzione è eseguita in caso di errore.
  * @note  In tal caso si accende il led rosso: LED5
  * @retval None
  * **/
static void Error_Handler(void){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}
