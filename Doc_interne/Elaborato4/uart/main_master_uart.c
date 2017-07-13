/**
  ******************************************************************************
  * @file      Elaborato4/uart/main_master_uart.c
  * @authors  Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @version   V1.0
  * @date      20-June-2017
  * @brief     main che descrive il master di una trasmissione uart
  * @note	 Si è in attesa che si preme il push button, per poter generare una trasmissione.Se la trasmissione è completata con successo, si fa un toggle sul LED6, e si procede al polling di una ricezione. Se anche la ricezione è completata con successo, si accende il led associato al dato ricevuto in receive_data.
  *******************************************************************************   
*/


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef huart2;

/* Buffers pointers */
uint8_t *send_data, *receive_data;
uint16_t LEDS[4] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void SetupUART(void);
static void setup_led(void);
void setup_pushbutton(void);

void loop(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_Init();
  
  /* Configure the BSP */
  setup_led();
  setup_pushbutton();

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /*##-1- Configure the UART peripheral ######################################*/
  SetupUART();
  
  send_data = (uint8_t*)malloc(sizeof(uint8_t)*TXSIZE);
  receive_data = (uint8_t*)malloc(sizeof(uint8_t)*RXSIZE);

  send_data[0]=0x00;
  receive_data[0]=0x00;



  while (1)
  {
	  loop();
  }
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
static void SystemClock_Config(void){
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 		= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 	= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 		= 8;
  RCC_OscInitStruct.PLL.PLLN 		= 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK){
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001){
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  Funzione Callback richiamata quando c'è stato un'errore durante il trasferimento
  * @param  huart2 gestore uart
  * @note	Si esegue un'operazione di Toggle sul PD13(LED3)
  * @retval None
  * **/void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart2){
  /* Si accende il LED3 quando*/
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
}

/**
  * @brief  Funzione in cui si configura i LED.
  * @note   Pin PD12:	LED4
  * @note	Pin PD13:	LED3
  * @note	Pin PD14:	LED5
  * @note	Pin PD15:	LED6
  * @param  None
  * @retval None
  *  **/
void setup_led(void){
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
  * @brief  Funzione in cui si configura il push button come GPIO_INPUT.
  * @note	Il push button è associato al pin PA0, quindi alla porta GPIOA
  * */
void setup_pushbutton(void){
	  GPIO_InitTypeDef GPIO_InitStruct;

	  /* Si abilita la porta associata al PA0 */
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  GPIO_InitStruct.Pin = GPIO_PIN_0;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief  Configurazione UART
  *         Si utilizza una USART Asincrona, nello specifico si sceglie USART2
  *         che è connessa ai pin PA2(USART2_TX) e PA3(USART2 RX)
  *
  *         Si sceglie di settare i seguenti valori :
  *            - Lunghezza = 8 Bits
  *            - Stop Bit = 1 bit
  *            - Parity = None
  *            - BaudRate = 9600 baud
  *            - Hardware flow control disabled (RTS and CTS signals)
  * @param  None
  * @retval None
  */
static void SetupUART(void){
	huart2.Instance          = USARTx;

	huart2.Init.BaudRate     = 9600;
	huart2.Init.WordLength   = UART_WORDLENGTH_8B;
	huart2.Init.StopBits     = UART_STOPBITS_1;
	huart2.Init.Parity       = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart2.Init.Mode         = UART_MODE_TX_RX;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	 {
	  Error_Handler();
	 }

}


/**
  * @brief  Loop per la trasmissione/ricezione di un messaggio attraverso il bus UART.
  * @param  None
  * @retval None
  */
void loop(void){

	  /* Si attende che sia premuto il push button */
	  while (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != GPIO_PIN_SET) {

		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
	    HAL_Delay(40);
	  }
	  /* Si attende che il push button sia rilasciato */
	  while (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != GPIO_PIN_RESET);

	  /*Si spegne il led3 */
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);

		/*Si trasmette il messaggio da inviare (send_data) con un timeout di 5 secondi*/

	  if(HAL_UART_Transmit(&huart2, (uint8_t*)send_data, TXSIZE, 5000)!= HAL_OK)
	  {
	    Error_Handler();
	  }

	  /* Si accende il LED6 quando il trasferimento è completato */
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);

	  /*Si pone il master in polling per una ricezione*/
	  if(HAL_UART_Receive(&huart2, (uint8_t *)receive_data, RXSIZE, 5000) != HAL_OK)
	  {
	    Error_Handler();
	  }


	  /* Si accende il led associato a arxBuffer quando la ricezione è stata completata */
	  HAL_GPIO_WritePin(GPIOD,LEDS[send_data[0]],GPIO_PIN_RESET);
	  send_data[0] = receive_data[0];
	  HAL_GPIO_WritePin(GPIOD,LEDS[send_data[0]],GPIO_PIN_SET);

}

/**
  * @brief  Questa funzione è chiama in caso di errore.
  * @note	Se richiamata,si entra in un loop (vuoto) infinito
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Si accende il LED5*/
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
  while(1)
  {
  }
}
