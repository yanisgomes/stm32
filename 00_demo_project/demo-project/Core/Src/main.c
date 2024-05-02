/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dac.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdio.h"
#include "HorombeRGB565.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RX_BUFFER_SIZE 100

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t joystick_v;
uint32_t joystick_h;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 	char text[50]={};

	static TS_StateTypeDef  TS_State;

	char str_time[50] = {};
	RTC_DateTypeDef sDate = {0};
	RTC_TimeTypeDef sTime = {0};

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

	HAL_StatusTypeDef status;
	char tx_msg[50];
	char rx_msg[10];
	uint8_t rx_char;

	int xMove = 0;
	int yMove = 0;

	int vxy = 5;

	int xPos = 50;
	int yPos = 50;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC3_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_RTC_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_UART7_Init();
  /* USER CODE BEGIN 2 */
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS+ BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4);
  BSP_LCD_DisplayOn();
  BSP_LCD_SelectLayer(0);
  BSP_LCD_Clear(LCD_COLOR_RED);
  BSP_LCD_DrawBitmap(0,0,(uint8_t*)HorombeRGB565_bmp);

  BSP_LCD_SelectLayer(1);
  BSP_LCD_Clear(00);

  BSP_LCD_SetFont(&Font24); // Définit la police à Font16
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN); // Définit la couleur du texte en vert
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK); // Définit la couleur de fond du texte en noir

  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_WritePin(LED13_GPIO_Port,LED13_Pin,HAL_GPIO_ReadPin(BP1_GPIO_Port,BP1_Pin));
	  HAL_GPIO_WritePin(LED14_GPIO_Port,LED14_Pin,HAL_GPIO_ReadPin(BP2_GPIO_Port,BP2_Pin));

//	  sConfig.Channel = ADC_CHANNEL_7;
//	  HAL_ADC_ConfigChannel(&hadc3, &sConfig);
//	  while(HAL_ADC_PollForConversion(&hadc3, 100)!=HAL_OK);
//	  vxy = (int)HAL_ADC_GetValue(&hadc3)/200;

	  sConfig.Channel = ADC_CHANNEL_8;
	  HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	  HAL_ADC_Start(&hadc1);
	  while(HAL_ADC_PollForConversion(&hadc1, 100)!=HAL_OK);
	  joystick_h = HAL_ADC_GetValue(&hadc1);

	  HAL_ADC_Start(&hadc3);
	  while(HAL_ADC_PollForConversion(&hadc3, 100)!=HAL_OK);
	  joystick_v = HAL_ADC_GetValue(&hadc3);

	  // Formatage des valeurs du joystick dans une chaîne de caractères
	  sprintf(tx_msg, "h=%d | v=%d\n\r", joystick_h, joystick_v);

	  // Envoi de la chaîne de caractères via UART
	  HAL_UART_Transmit(&huart1, (uint8_t*)tx_msg, strlen(tx_msg), HAL_MAX_DELAY);

	  status = HAL_UART_Receive(&huart1, &rx_char,1, 500);

	  // Background update
	  BSP_LCD_DrawBitmap(0,0,(uint8_t*)HorombeRGB565_bmp);

	  BSP_LCD_SetFont(&Font12);
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

	  sprintf(text, ">> %d", rx_char);
	  BSP_LCD_DisplayStringAt(5, 5, (uint8_t*)text, LEFT_MODE);

	  sprintf(text, "vxy = %d", vxy);
	  BSP_LCD_DisplayStringAt(400, 5, (uint8_t*)text, LEFT_MODE);


//	  sprintf(text, ">>> %d", rx_char);
//
//	  BSP_LCD_DisplayStringAt(row*2, col*4, (uint8_t*)text, LEFT_MODE);

	  if (status == HAL_TIMEOUT) {
		  xMove = 0;
		  yMove = 0;
	  } else {
		  switch(rx_char) {

		  case 122 :
			  xMove = 0;
			  yMove = -vxy;
			  break;
		  case 119 :
			  xMove = 0;
			  yMove = vxy;
			  break;
		  case 115 :
			  xMove = vxy;
			  yMove = 0;
			  break;
		  case 113 :
			  xMove = -vxy;
			  yMove = 0;
			  break;
		  }
	  }

	  xPos += xMove;
	  yPos += yMove;

	  if (xPos < 0) {
		  xPos = 0;
	  } else if (xPos > BSP_LCD_GetXSize()) {
		  xPos = BSP_LCD_GetXSize();
	  }

	  if (yPos < 0) {
		  yPos = 0;
	  } else if (yPos > BSP_LCD_GetYSize()) {
		  yPos = BSP_LCD_GetYSize();
	  }

	  BSP_LCD_SetFont(&Font24);
	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	  BSP_LCD_DisplayStringAt(xPos, yPos, (uint8_t*)"o", LEFT_MODE);


//	  // Récupération de l'heure et de la date
//	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//	  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); // Toujours après HAL_RTC_GetTime
//
//	  // Formatage de l'heure et de la date en chaîne de caractères
//	  sprintf(str_time, "%02d:%02d:%02d.%02d", sTime.Hours, sTime.Minutes, sTime.Seconds, sTime.SubSeconds);
//	  // Affichage sur l'écran LCD
//	  BSP_LCD_DisplayStringAt(row, col, (uint8_t*)str_time, LEFT_MODE);
//	  // Dans la boucle principale ou la fonction de mise à jour
//	  //UpdateBackgroundColor(joystick_h, joystick_v);


	  BSP_TS_GetState(&TS_State);
	  if(TS_State.touchDetected){
		  BSP_LCD_FillCircle(TS_State.touchX[0],TS_State.touchY[0],4);
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
