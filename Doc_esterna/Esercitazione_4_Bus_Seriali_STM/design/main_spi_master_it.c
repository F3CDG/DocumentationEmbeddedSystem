/******************************************************************************
  * @name         : main.c
  * @authors	  :Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @date    	  :19-June-2017
  * @brief        : Main program body
  ******************************************************************************
  *	 In questo programma è configurato il main di una trasmissione SPI.
  *	 Quando è premuto il push-button si genera una interrupt che fa eseguire la
  *	 trasmissione di un dato allo slave.
  *
  *
  * 		SPI2 MASTER SLAVE connection configuration
  * 		PB13 (SPI2_SCK) ------> (SLAVE_SCK)
  * 		PB14 (SPI2_MISO)------> (SLAVE_MISO)
  * 		PB15 (SPI2_MOSI)------> (SLAVE_MOSI)
  * 		PB12 (SPI2_NSS) ------> (SLAVE_NSS)
  *
  ******************************************************************************
  */

/* Handler del spi2 */
#include "main.h"

SPI_HandleTypeDef SpiHandle;
/* Buffer usati for transmission */
uint8_t aTxBuffer[] ="****MESSAGGIO SPI****";



/*Si definiscono delle macro per controllare via software NSS*/
#define SPI2_NSS_ENABLE   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)
#define SPI2_NSS_DISABLE   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_SET)


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
void setup_LED();
void setup_SPI();
void setup_pushbutton_EXTI0();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void loop();

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  setup_LED();
  setup_SPI();
  setup_pushbutton_EXTI0();
  /* Si attende che l'utente premi il bottone per poter avviare la comunicazione*/
  /* Infinite loop */
for(;;){
	loop();
  	  }
}
/**
  * @brief  Loop Function
  * @note	Toggle del Led4 ogni 0.4 s
  * @retval None
  *
  *    **/
void loop(){
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	HAL_Delay(400);
}

/**
  * @brief  Funzione in cui si settano i parametri SPI
  * @note
  * @retval None
  *    **/
void setup_SPI(){
	  SpiHandle.Instance               = SPIx;
	  SpiHandle.Init.Mode 				= SPI_MODE_MASTER;
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
  * @brief  Funzione in cui si configura il push button come EXTI0.
  * @note	Il push button è associato al pin PA0
  * @note   Configure pins as
  * @note	Output: PD12, PD13,PD14,PD15 (LED), PB12(NSS)
  * @note	EXTI: PA0
*/
void setup_pushbutton_EXTI0(){
	  GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin : PA0 */
	  GPIO_InitStruct.Pin = GPIO_PIN_0;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /* EXTI interrupt init*/
	  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 2);
	  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/**
  * @brief Questa funzione è eseguita in caso di errore.
  * @note  In caso di errore si accende il led rosso: LED5
  * @retval None
  * **/
static void Error_Handler(void)
{
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

/**
  * @brief  Funzione Callback richiamata quando il trasferimento è completato
  * @param  hspi: gestore spi
  * @note	Si esegue un'operazione di Toggle sul PD12(LED4)
  * @retval None
  * **/
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi->Instance==SpiHandle.Instance){
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
		SPI2_NSS_DISABLE;
	}
}

/**
  * @brief  Funzione Callback richiamata quando c'è stato un'errore durante il trasferimento
  * @param  hspi: gestore spi
  * @note	Si esegue un'operazione di Toggle sul PD15(LED6)
  * @retval None
  * **/
 void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  /* Turn LED5 on: Transfer error in reception/transmission process */
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_15);
}


/**
  * @brief Questa funzione è eseguita quando scatta l'interruzione di EXTI0,
  * 	   ovvero quando viene premuto il push button
  * @param GPIO_Pin: si indica il pin associato al push button.
  * @note  si esegue la transmit.
  * @retval None
  * **/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//si attende che si rilascia il bottone
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
	//si abilita il pin not slave select
	SPI2_NSS_ENABLE;
	while(HAL_SPI_Transmit_IT(&SpiHandle, (uint8_t *)aTxBuffer, sizeof(aTxBuffer)) != HAL_OK)
	while (HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_READY);
}
