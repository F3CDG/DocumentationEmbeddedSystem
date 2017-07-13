/******************************************************************************
  * @name         : main.c
  * @authors	  :Colella Gianni - Guida Ciro - Lombardi Daniele / Group IV - Sistemi Embedded 2016-2017
  * @date    	  :19-June-2017
  * @brief   	  :In questo codice è configurato il main dello  di una trasmissione UART.
  *	 			   Si è in attesa di ricevere un dato dal master.
  *	 			   Se la ricezione è completata con successo, si fa un toggle sul led corrispondente
  *	 			   al valore specificato nel messaggio ricevuto.
  *	 			   Si incrementa di 1 modulo 4 il suddetto valore e si invia di nuovo al Master.
  *  			   Se anche la trasmissione è completata con successo, ritorna nel loop in cui si è in
  *  			   polling per la ricezione.
  ******************************************************************************
  * 		UART SLAVE connection configuration
  * 		PA2 (UART_TX) ------> (MASTER_UART RX)
  * 		PA3 (UART_RX) ------> (MASTER UART TX)
  *
  ******************************************************************************
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
void setup_pushbutton(void);
void setup_led(void);
static void loop(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the BSP */
  setup_pushbutton();
  setup_led();

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /*##-1- Configure the UART peripheral ######################################*/
  SetupUART();
  
  send_data = (uint8_t*)malloc(sizeof(uint8_t)*TXSIZE);
  receive_data = (uint8_t*)malloc(sizeof(uint8_t)*RXSIZE);

  send_data[0]=0x00;
  receive_data[0]=0x00;


  /* Transmission loop */
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
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();
  
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
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart2)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3);
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
static void loop(void){
	  /*Si è in polling di ricevere il messaggio dal master*/
	  if(HAL_UART_Receive(&huart2, (uint8_t *)receive_data, RXSIZE, 5000) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  HAL_GPIO_WritePin(GPIOD,LEDS[send_data[0]],GPIO_PIN_RESET);
	  /*Si incrementa di 1 modulo 4 il contenuto del messaggio ricevuto*/
	  send_data[0] = (receive_data[0]+1)%4;
	  HAL_GPIO_WritePin(GPIOD,LEDS[send_data[0]],GPIO_PIN_SET);


	  /*Si invia al master il valore appena aggiornato*/
	  if(HAL_UART_Transmit(&huart2, (uint8_t*)send_data, TXSIZE, 5000)!= HAL_OK)
	    {
	      Error_Handler();
	    }

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
