/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "usb_device.h"


/* MACRO per gestire la priorità dei vari Task---------------------------------*/
#define PRIORITY1 1
#define PRIORITY2 6
#define PRIORITY3 3
#define PRIORITY4 6
/* MACRO per gestire il Delay dei vari Task---------------------------------*/
#define DELAY1 350
#define DELAY2 450
#define DELAY3 250
#define DELAY4 150
/*MACRO che indica la dimensione dello Stack da assegnare al task------------*/
#define STACK_DIM 200

/* Gestore dei Task ---------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* Prototipi delle funzioni private--------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

static void Task1(void *pvParameters) ;
static void Task2(void *pvParameters) ;
static void Task3(void *pvParameters) ;
static void Task4(void *pvParameters) ;

/* utili per la definizione dei task -----------------------------------------------*/
static const char *pcTextForTask1 ="Task 1 is running\r\n";
static const char *pcTextForTask2 ="Task 2 is running\r\n";
static const char *pcTextForTask3 ="Task 3 is running\r\n";
static const char *pcTextForTask4 ="Task 4 is running\r\n";



/* main */
/**
  * @brief  main in cui sono creati e schedulati i task definiti successivamente
  * @note	None
  * @retval None
  *    **/
int main(void)
{
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /*Initialize USB*/
  MX_USB_DEVICE_Init();
  /* definition and creation dei 4 Task */
	xTaskCreate(Task1,"Task1",STACK_DIM,(void*)pcTextForTask1,PRIORITY1,NULL);
	xTaskCreate(Task2,"Task2",STACK_DIM,(void*)pcTextForTask2,PRIORITY2,NULL);
	xTaskCreate(Task3,"Task3",STACK_DIM,(void*)pcTextForTask3,PRIORITY3,NULL);
	xTaskCreate(Task4,"Task4",STACK_DIM,(void*)pcTextForTask4,PRIORITY4,NULL);


  	vTaskStartScheduler();

  /* Start scheduler */
  osKernelStart();

  while (1){
  }

}


/*Task 1 */
/**
  * @brief  Task 1, in cui si fa un Toggle del LED6 ogni 450 ms
  * 		Ogni volta che è richiamato questo Task, si invia una CDC Transmit
  * 		"TASK ---1 che è utile per un debug da terminale
  * @note	pvParameters
  * @retval None
  *    **/
static void Task1(void *pvParameters) {
	char testo[]="Task---1\n";
    for (;;) {
		  osDelay(DELAY1);
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_15);
		  CDC_Transmit_FS((uint8_t*)testo,strlen(testo));
			}
}
/*Task 2 */
/**
  * @brief  Task 2, in cui si fa un Toggle del LED4 ogni DELAY2 ms
  * 		Ogni volta che è richiamato questo Task, si invia una CDC Transmit
  * 		"TASK *******2 che è utile per un debug da terminale
  * @note	pvParameters
  * @retval None
  *    **/
static void Task2(void *pvParameters) {

	char testo[]="Task**********2\n";
	for (;;) {
		  osDelay(DELAY2);
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
		  CDC_Transmit_FS((uint8_t*)testo,strlen(testo));
			}
}

/**
  * @brief  Task 1, in cui si fa un Toggle del LED5 ogni 450 ms
  * 		Ogni volta che è richiamato questo Task, si invia una CDC Transmit
  * 		"TASK-----------------3 che è utile per un debug da terminale
  * @note	pvParameters
  * @retval None
  *    **/
/*Task 3 */

static void Task3(void *pvParameters) {
	char testo[]="Task-----------------3\n";
	for (;;) {
		  osDelay(DELAY3);
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
		  CDC_Transmit_FS((uint8_t*)testo,strlen(testo));
			}
}
/*Task 4 */
/**
  * @brief  Task 1, in cui si fa un Toggle del LED3 ogni 450 ms
  * 		Ogni volta che è richiamato questo Task, si invia una CDC Transmit
  * 		"TASK***********************4 che è utile per un debug da terminale
  * @note	pvParameters
  * @retval None
  *    **/


static void Task4(void *pvParameters) {

	char testo[]="Task************************4\n";
	for (;;) {
		  osDelay(DELAY4);
		  HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
		  CDC_Transmit_FS((uint8_t*)testo,strlen(testo));
			}
}
/** System Clock Configuration
*/

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
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

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
