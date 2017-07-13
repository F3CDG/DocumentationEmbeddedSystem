/******************************************************************************
  * @name         : main.c
  * @authors	  :Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @date    	  :18-June-2017
  * @brief        : Main program body
  ******************************************************************************
  *	 In questo programma è configurato il main di una trasmissione I2C.
  *	 Quando è premuto il push-button si genera una interrupt che fa eseguire la
  *	 trasmissione di un dato allo slave di indirizzo ADDRESS_B.
  *
  *
  * 		I2C2 MASTER  configuration
  * 		PB6 (I2C1_SDA) ------> (SLAVE_SDA)
  * 		PB7 (I2C1_SCL) ------> (SLAVE_SCL)
  ******************************************************************************
*/


#include "main.h"
#include "stm32f3xx_hal.h"

/*Definizione di due macro per stabilire gli indirizzi di master(a) e slave(b)*/
#define ADDRESS_A      0x18
#define ADDRESS_B      0x11

/*definizione del gestore del bus*/
I2C_HandleTypeDef hi2c1;

/*prototipi delle funzioni*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void loop(void);
/*ridefinizione dei prototipi per le callback di EXTI0 e I2CTxClp*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef *hi2c);

/*definizione e inizializzazione del messaggio da inviare allo slave*/
uint8_t send_data[] ="****MESSAGGIO I2C****";


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();




  while (1)
  {
	  loop();
  }

}


/**
  * @brief  Loop infinito richiamato dalla funzione main.c.
  * @param  None
  * @retval None
  */
static void loop(void){
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_10);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_12);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_13);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_14);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_15);
	HAL_Delay(150);
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}
/**
  * @brief  Configurazione I2C
  * @note	utilizzando un valore di Timing pari a 004C4092A ,partendo da un clock a 72MHz, si ha un timing a 1MHz
  * @param  None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
	hi2c1.Instance             = I2C1;
	hi2c1.Init.Timing          = 0x00C4092A;
	hi2c1.Init.OwnAddress1     = ADDRESS_A;
	hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) 	!= HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
*/
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/**
  * @brief  Funzione in cui si configurano i GPIO.
  * @param  None
  * @retval None
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE9 PE10 PE11 
                           PE12 PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 3);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
/**
  * @brief Questa funzione è eseguita quando scatta l'interruzione di EXTI0,
  * 	   ovvero quando viene premuto il push button
  * @param GPIO_Pin: si indica il pin associato al push button.
  * @note  si esegue la Master Transmit in no blocking mode verso lo slave di indirizzo ADDRESS_B.
  * @retval None
  * **/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//si attende che si rilascia il bottone
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)!=0);
	//si abilita il pin not slave select
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);

	  while(HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)ADDRESS_B, (uint8_t*)send_data, TXSIZE)!= HAL_OK)
	  while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);


}
/**
  * @brief  Funzione Callback richiamata quando il Master completa il trasferimento  verso lo slave
  * @param  hspi: gestore spi
  * @note	Si accendono tutti i LED della board
  * @retval None
  * **/
void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance==hi2c1.Instance){
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
		}

}

/**
  * @brief  Funzione Callback richiamata quando c'è stato un'errore durante il trasferimento
  * @param  hspi: gestore spi
  * @note	Sulla board saranno accesi solo i LED 6 e 7, configurati nei pin PE15 e PE11, tutti gli altri saranno spenti
  * @retval None
  * **/void HAL_I2C_ErrorCallback (I2C_HandleTypeDef * hi2c){
	if(hi2c->Instance==hi2c1.Instance)
				{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
				while(1);
				}

}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {

	  HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
	  HAL_Delay(200);
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

