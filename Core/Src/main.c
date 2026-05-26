/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "math.h"
#include <stdio.h>
#define TABLE_SIZE 128
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {Sawtooth, Square, Sine, Trapezoidal} Waveform_t;
uint16_t sawTable[TABLE_SIZE];
uint16_t squareTable[TABLE_SIZE];
uint16_t sineTable[TABLE_SIZE];
uint16_t trapezoidTable[TABLE_SIZE];
uint16_t *currentTable =sawTable;
Waveform_t currentWave = Sawtooth;//initial waveform = Sawtooth
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac_ch1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//================================**Sawtooth wave callback**=========================================================//
//float DAC_RANGE =4095;   //Numerical mapping 1.0(float)=(2^12)-1=3.3v
//float j=0.0;
//uint32_t DACData;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	DACData=((uint32_t)(j*DAC_RANGE)); //float to DAC conversion
//	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DACData);
//	j=j+0.1;
//	if(j>1.0)j=0.0;
//}

//=================================** square wave callback **=========================================================//
//float j=0.0;
//uint32_t DACData;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(j < 0.5)
//        DACData = 4095;
//    else
//        DACData = 0;
//    HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,DACData);
//    j = j + 0.1;
//    if(j > 1.0)
//        j = 0.0;
//}

//=================================** sine wave callback**=========================================================//
//uint16_t indexWave=0;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,sineTable[indexWave]);
//	indexWave++;
//	if(indexWave >=TABLE_SIZE)
//		indexWave=0;
//}

//=================================** trapzoid wave callback**=========================================================//
//uint16_t trapzoidTable[TABLE_SIZE];
//uint16_t indexWave=0;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,trapzoidTable[indexWave]);
//	indexWave++;
//	if(indexWave >=TABLE_SIZE)
//		indexWave=0;
//}

//=================================>>  Callback  methoed <<========================================================================//
//=================================**  changing waveform callback**=========================================================//
//uint16_t indexWave=0;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,currentTable[indexWave]);
//	indexWave++;
//	if(indexWave >=TABLE_SIZE)
//		indexWave=0;
//}

//=================================**external button interrup callback function **=========================================================//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
//	if(GPIO_PIN==GPIO_PIN_13){
//
//		currentWave++;
//		if(currentWave > Trapezoidal)
//				currentWave = Sawtooth;
//
//		switch(currentWave)
//		{
//			case Sawtooth:
//				currentTable = sawTable;
//				break;
//
//			case Square:
//				currentTable = squareTable;
//				break;
//
//			case Sine:
//				currentTable = sineTable;
//				break;
//
//			case Trapezoidal:
//				currentTable = trapezoidTable;
//				break;
//		}
//
//	}
//
//}
//=================================>>  Callback method <<========================================================================//
//=================================**********************========================================================================//
//=================================>>     DMA   method <<========================================================================//

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
	if(GPIO_PIN==GPIO_PIN_13){

		currentWave++;
		if(currentWave > Trapezoidal)
				currentWave = Sawtooth;

		switch(currentWave)
		{
			case Sawtooth:
				currentTable = sawTable;
				break;

			case Square:
				currentTable = squareTable;
				break;

			case Sine:
				currentTable = sineTable;
				break;

			case Trapezoidal:
				currentTable = trapezoidTable;
				break;
		}

		HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);

		HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t*)currentTable,TABLE_SIZE,DAC_ALIGN_12B_R);

	}

}


//=================================>>     DMA   method <<========================================================================//



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_DAC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //================================**initial Sawtooth wave table**=========================================================//
  for (int i=0; i<TABLE_SIZE; i++){
  	sawTable[i]=i*32;
  }

  //================================**initial Square wave table**=========================================================//
  for (int i=0; i<TABLE_SIZE; i++){
  	if(i < TABLE_SIZE/2)
  		squareTable[i]=4095;
  	else
  		squareTable[i]=0;
  }

  //=================================**initial sine wave table**=========================================================//
  for(int i =0; i<TABLE_SIZE;i++){
	  sineTable[i]=(sin(2*3.14159*i/TABLE_SIZE)+1)*2027;
  }

  //=================================**initial trapzoid wave table**=========================================================//
   for(int i =0; i<TABLE_SIZE;i++){
	   //Rising edge
	   if(i<32){
		   trapezoidTable[i]=i*128;
	   }
	   //High level
	   else if(i<64){
		   trapezoidTable[i]=4095;
	   }
	   //Fallinh edge
	   else if(i<96){
		   trapezoidTable[i]=4095-((i-64)*128);
	   }
	   //Low level
	   else {
		   trapezoidTable[i]=0;
	   }

   }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
   //=================================>>  Callback method <<========================================================================//
//  HAL_TIM_Base_Start_IT(&htim2);
//  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

//=================================>>     DMA   method <<========================================================================//
  //DMA
  HAL_TIM_Base_Start(&htim2);
  HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t*)currentTable,TABLE_SIZE,DAC_ALIGN_12B_R);

  lcd_Init();
  lcd_Puts("Waveform style: ");
  char buff[32];
  while (1)
  {
	  lcd_Goto(0,1);
	  switch(currentWave)
	  {
	      case Sawtooth:
	          sprintf(buff,"Sawtooth   ");
	          break;

	      case Square:
	          sprintf(buff,"Squarewave ");
	          break;

	      case Sine:
	          sprintf(buff,"Sinewave   ");
	          break;

	      case Trapezoidal:
	          sprintf(buff,"Trapezoidal");
	          break;
	  }
	  lcd_Puts(buff);

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 3999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim2.Init.Period = 19;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 PA8 PA9
                           PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
#ifdef USE_FULL_ASSERT
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
