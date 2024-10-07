/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <math.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//-----------------------------------------------------------------------------------------------------------*/
// structure de distortion
typedef struct{
	int disto_mode;
	float clipping;
	float gain ;
	float clipping_coef;
}distortion;

//structure flanger
typedef struct{
	int flanger_mode;
	float rate;
	float depth;
	float delay;
    int effect_num;
}flanger;

//structure delay
typedef struct{
    int delay_mode;
    int depth;
    float delay;
    int effect_num;
}delay;

typedef struct{
    int wahwah_mode;
    float rate;
    float depth;
    float minFreq;
    float maxFreq;
    int effect_num;
} wahwah;

typedef struct{
    int tremolo_mode;
    float rate;
    float depth;
    int effect_num;
} tremolo;

typedef struct{
    int vibrato_mode;
    float rate;
    float depth;
    float delay;
    int effect_num;
} vibrato;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//define -----------------------------------------------------------------------------------------------------------*/
#define N 500
#define halfN 250
#define NBR_SAMPLES 50
#define OFFSET_SIGNAL 1980//was 1983
#define PI 3.1415

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac1_ch1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// création des buffers
uint32_t adc_buffer[N];
uint32_t dac_buffer[N];

uint32_t pot_values[N];


int Temps, frequency;
int cnt = 0;

unsigned int t = 0;
int counter_cos = 0, internal_counter_cos = 0;

int ADC_counter = 0;

int pot1;

float test;
//int cos_frequency = 50;

float cos_values[360];
float sin_values[360];
//ADC_ChannelConfTypeDef ADC_config;

volatile uint32_t last_clock = 0;
volatile uint32_t current_clock = 0;
volatile uint8_t i = 0;
volatile uint8_t calcul_open =0;
volatile int difference = 0;
volatile int capacite_pF = 0;
volatile int frequence = 0;
volatile int periode_ns = 0;

//int pot_values[0] = 0, potd_value = 0;
int pot_selector = false;


// initialisation des effets -----------------------------------------------------------------------------------------------------------*/
distortion Disto = {1, 10.0f, 150.0f,1000.0f};
flanger Flanger = {1, 0.5f, 20.0f, 1.0f, 2};
delay Delay = {1, 50, 500.0f, 3};
wahwah Wahwah = {1, 4.0f, 70.0f, 100.0f, 1000.0f, 4};
tremolo Tremolo = {1, 50.0f, 20.0f, 5}; //{1, 5.0f, 50.0f, 5};
vibrato Vibrato = {1, 5.0f, 50.0f, 5.0f, 6};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_DAC1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM15_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM6_Init(void);
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_DAC1_Init();
  MX_ADC1_Init();
  MX_TIM15_Init();
  MX_ADC2_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  // initialisation de l'effet
  //distortion_init();

  // démarrage des buffers et des timers
  HAL_ADC_Start_DMA(&hadc1, adc_buffer, N);
  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, dac_buffer, N, DAC_ALIGN_12B_R);
  //HAL_TIM_Base_Start(&htim2);

  for(int i = 0; i < 360; i++)
  {
	  cos_values[i] = cosf(i*2*PI/360);
	  sin_values[i] = sinf(i*2*PI/360);
  }

  HAL_TIM_Base_Start_IT(&htim15);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim6);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  //HAL_GPIO_TogglePin(TEST1_GPIO_Port, TEST1_Pin);
	  //t += fmodf(cosfrequency*0.002270f, 6.28f);
	  // = cosfrequency*2270;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T15_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  //ADC_config = sConfig;
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 2;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

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
  sConfig.DAC_Trigger = DAC_TRIGGER_T15_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
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
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 600;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 64000;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 0;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 1557;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TEST1_GPIO_Port, TEST1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : TEST1_Pin */
  GPIO_InitStruct.Pin = TEST1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TEST1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// -------------------------------------------------------------------------------------------------------------------*/
//disto
void distortion_processBuffer(uint32_t* inputData, uint32_t* outputData){

	// si la disto est activée
	if(Disto.disto_mode)
	{
		//float gain = Disto.gain/100;
		//float clipping = Disto.clipping;

		//int16_t high_threshold = 100.0f; //10000.0f - ((clipping/100) * Disto.clipping_coef);
		//int16_t high_threshold = 100 - capacite_pF;
		//int16_t low_threshold = - high_threshold; //1950.0f; // 2900.0f pour amp500mV
		int16_t min_threshold = 20;
		int16_t max_threshold = 50 + (4096 - pot_values[1])*200/4096;//seuil max entre 50 et 250
		int16_t high_threshold = max_threshold - capacite_pF*(max_threshold-min_threshold)/100;
		int16_t low_threshold = - high_threshold; //1950.0f; // 2900.0f pour amp500mV

		for(int i = 0; i < N; i++)
		{
			// get current sample
			int32_t sample = (int32_t)inputData[i];

			int32_t AC_signal = sample - OFFSET_SIGNAL;

			//on applique le gains
			int16_t AC_signal_amplified = (int16_t)((float)AC_signal * (Disto.gain / 100.0f));

			 // clipping
			 if (AC_signal_amplified > high_threshold)
			 {
				 AC_signal_amplified = high_threshold;
			  }
			  else if (AC_signal_amplified < low_threshold) //
			  {
				  AC_signal_amplified = low_threshold;
			  }

			// send to output
			outputData[i] = (int32_t)AC_signal_amplified + OFFSET_SIGNAL;
		}
	}
}

void directloopback_processBuffer(uint32_t* inputData, uint32_t* outputData){
		for(int i = 0; i < N; i++)
		{
			outputData[i] = inputData[i];
		}
}
//flanger--------------------------------------------------------------------------------
/*void flanger_processBuffer(uint32_t* inputData, uint32_t* outputData, int offset){

	if(Flanger.flanger_mode)
	{
		static float phase = 0;

		uint16_t maxDelay  = (Flanger.delay*fe)/1000;
		float lfoFreq = Flanger.rate;
		float lfoDepth = (Flanger.depth/100.0f);

		for(int i = offset; i < offset+halfN; i++)
		{
			// indice courant
			int32_t sample = (int32_t)inputData[i];

			int32_t AC_signal = sample - OFFSET_SIGNAL;

			// delay modulé
			uint16_t delaySamples = (uint16_t)(1+(maxDelay/2)*(1-(lfoDepth * cosf(2*PI*phase)))); //arm_cos_f32 ?

			// indice delay
			int16_t prevSample = (int16_t)inputData[(i + N - delaySamples) % N];

			// envoi sortie
			int16_t AC_transformed = (AC_signal+prevSample)/2;

			outputData[i] = (uint32_t) ( AC_transformed + OFFSET_SIGNAL);

			// maj phase
			phase =  fmodf(phase + lfoFreq / fe,1);
		}
	}
}*/

//delay --------------------------------------------------------------------------------
/*void delay_processBuffer(uint32_t* inputData, uint32_t* outputData, int offset)
{
	uint16_t delaySamples = (Delay.delay*fe)/1000;//fe = 44100
	float feedbackGain = (Delay.depth/100.0f);

	if(Delay.delay_mode)
	{
		for(int i = offset; i < offset +halfN; i++)
		{
			// current sample
			int32_t sample =  (int32_t)inputData[i];

			int32_t AC_signal = sample - OFFSET_SIGNAL;

			// previous sample
			int32_t prevSample = (int32_t)inputData[(i + N - delaySamples) % N];

			// apply feedback gain
			prevSample *= feedbackGain;

            int32_t AC_transformed = (AC_signal + prevSample);

			// mix samples together
			outputData[i] = (uint32_t) (AC_transformed + OFFSET_SIGNAL);
		}
	}
}*/

//tremolo --------------------------------------------------------------------------------
void tremolo_processBuffer(uint32_t* inputData, uint32_t* outputData)
{
	if(Tremolo.tremolo_mode)
	{
		//float phase = 0; // à mettre dans la structure
		float lfoFreq = 1 + (4095.0f - pot_values[1])*100.0f/4096.0f;//Tremolo.rate;
		float lfoDepth = 0.4f + (100.0f-capacite_pF)/200.0f;//(Tremolo.depth/100.0f);
		if(lfoDepth<0.5f)
			lfoDepth = 0.5f;
		//static float phase = 0.0f;
		//float before_sample;
		float sample;//, cosvalue;
		//cosfrequency = lfoFreq;
		for(int i = 0; i < N ; i++) //offset, offset+halfN
		{
			// get current sample
			//float sample = (int32_t)inputData[i];

			// modulate sample volume with lfo
			//sample *= (1.0f-lfoDepth) + lfoDepth * cosf(2.0f*3.14f*t*lfoFreq); //arm_cos_f32 // cosf(2.0f*PI*phase)

			//outputData[i] = sample*(uint32_t)(1.0f-lfoDepth) + lfoDepth * cosf(t) + OFFSET_SIGNAL;
			//outputData[i]
			//counter_cos = fmodf(counter_cos + cosfrequency*0.0000002270f, 6.28f);
			//cosvalue = cosf(counter_cos);
			//float sample = inputData[i];

			//cosvalue = cos_values[counter_cos];
			if(internal_counter_cos >= (int)(1050/lfoFreq))//21 need to be the center, slowest is
			{
				counter_cos++;
				internal_counter_cos = 0;
			}
			internal_counter_cos++;

			//la variable a tendance à passer à une valeur très grande
			if(counter_cos == 360)
				counter_cos = 0;

			sample = ((float)inputData[i] - (float)OFFSET_SIGNAL);
			sample *= ((1.0f-lfoDepth) + lfoDepth * cos_values[counter_cos]);
			//outputData[i] = 50*cosvalue + 1000;//
			outputData[i] = (uint32_t)(sample + OFFSET_SIGNAL);//(uint32_t)(500*cosvalue + 1000);
			//outputData[i] = (uint32_t)(cosvalue*500  + OFFSET_SIGNAL);//(uint32_t)(500*cosvalue + 1000);
			//HAL_GPIO_TogglePin(TEST1_GPIO_Port, TEST1_Pin);
			// update phase
			//t =  fmodf(t + (lfoFreq / ((float)fe)), 3.14f);

		}

	}
}


//vibrato --------------------------------------------------------------------------------

/*void vibrato_processBuffer(uint32_t* inputData, uint32_t* outputData, int offset)
{
	if(Vibrato.vibrato_mode)
	{
		static float phase = 0;
		uint16_t maxDelay  = ((Vibrato.delay)*fe)/1000;
		float lfoFreq = Vibrato.rate;
		float lfoDepth = (Vibrato.depth/100.0f);

		for(int i = offset; i < offset+halfN; i++)
		{
			// get the modulated delay
			uint16_t delaySamples = (uint16_t)(1+(maxDelay/2)*(1-(lfoDepth * cosf(2*PI*phase)))); //arm_cos_f32

			// get the delayed sample
			int16_t prevSample = (int16_t)inputData[(i + N - delaySamples) % N];

			// send to output
			outputData[i] = (uint32_t) prevSample;

			// update phase
			phase =  fmodf(phase + lfoFreq / fe,1);
		}
	}
}*/

//wah wah --------------------------------------------------------------------------------
//coeffs passe bande actuels
static float bp_a0, bp_a1, bp_a2, bp_b0, bp_b1, bp_b2;

// coeffs passe bande précédents
static float bp_x1, bp_x2, bp_y1, bp_y2;

static float currentCutoff = 440.0f; // fréquence de coupure du filtre
static float qFactor = 2.3f; // facteur de qualité du filtre


void new_bandpass()
{
    //int omega_deg = 360.0f*PI * currentCutoff / 44100.0f;//simplif du 2 dans 360/2Pi * 2.0f
    float omega = 2.0f*(float)PI*currentCutoff / 44100.0f;
	float cosomega = (float)cosf((float)omega); //arm_cos_f32
    //float cosomega = (float)cos_values[omega_deg];//reste sinus à faire
    //PROBLEME Vis à vis de cette ligne, le cos fait planter le compteur.
    //Le mieux serait d'utiliser la matrice de cos pour le calculer

   // float cosomega = cos_values[360*omega/(2*PI)] //arm_cos_f32

    //float alpha = sin_values[omega_deg] / (2.0f * qFactor); //arm_sin_f32
	float alpha = (float)sinf((float)omega) / (2.0f * qFactor);

    bp_b0 = alpha;
    bp_b1 = 0;
    bp_b2 = -alpha;
    bp_a0 = 1 + alpha;
    bp_a1 = -2 * cosomega;
    bp_a2 = 1 - alpha;
}

int16_t apply_bandpass(int16_t inSample)
{
    float x0 = (float)inSample;
    float result =
        (bp_b0 / bp_a0) * x0 +
        (bp_b1 / bp_a0) * bp_x1 +
        (bp_b2 / bp_a0) * bp_x2 -
        (bp_a1 / bp_a0) * bp_y1 -
        (bp_a2 / bp_a0) * bp_y2;

    // shift x1 to x2, sample to x1
    // shift y1 to y2, result to y1
    // simulate delay!!
    bp_x2 = bp_x1;
    bp_x1 = x0;
    bp_y2 = bp_y1;
    bp_y1 = result;

    return (int16_t)result;
}

void wahwah_processBuffer(uint32_t* inputData, uint32_t* outputData)
{
	if(Wahwah.wahwah_mode)
	{
		//static float phase = 0;
		//float lfoFreq = Wahwah.rate;
		//float lfoDepth = Wahwah.depth/100.0f;
		float G = (float)(4096 - pot_values[1])*5.0f/4096.0f;//gain entre 0 et 5

		for(int i = 0; i < N; i++)
		{
			// update phase
			//phase = fmodf((phase + lfoFreq / fe), 1);

			// get triangle lfo sample
			//float lfoSample = phase < 0.5 ? phase * 4 - 1 : 3 - 4 * phase;

			// modulate bandpass cutoff
			//currentCutoff = (float)((lfoSample * lfoDepth * centreFreq) + centreFreq);
			currentCutoff = (float)(Wahwah.minFreq + (Wahwah.maxFreq - Wahwah.minFreq)*capacite_pF / 100.0f);//T

			// update bandpass filter
			new_bandpass();

			// get current sample
			int16_t sample = (int16_t) (inputData[i] - OFFSET_SIGNAL);

			// apply bandpass filter to current sample
			sample = apply_bandpass(sample);

			// send to output
			outputData[i] = (uint32_t) ((float)sample*G + OFFSET_SIGNAL);

		}
	}
}



//-----------------------------------------------------------------------------------------------------------------------------*/
// le buffer est à moitié plein
/*void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	//distortion_processBuffer(adc_buffer, dac_buffer, 0);
	//tremolo_processBuffer(adc_buffer, dac_buffer, 0);

	for( int n=0; n<halfN;n++)
	{
	dac_buffer[n] = adc_buffer[n];	// disto
	}


}*/

// le buffer plein
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc1)
	{
		HAL_GPIO_TogglePin(TEST1_GPIO_Port, TEST1_Pin);
		//distortion_processBuffer(adc_buffer, dac_buffer);
		//wahwah_processBuffer(adc_buffer, dac_buffer);//ATTENTION, bloque le compteur pour le conditionneur
		//tremolo_processBuffer(adc_buffer, dac_buffer);
		//directloopback_processBuffer(adc_buffer, dac_buffer);

		if( pot_values[0] >= 3750)//
		{
			/*for( int n=0; n<N;n++)
			{
				 dac_buffer[n] = adc_buffer[n];

			}*/
			directloopback_processBuffer(adc_buffer, dac_buffer);
		}
		else if(3750 > pot_values[0] && pot_values[0] >= 3295)
		{
			distortion_processBuffer(adc_buffer, dac_buffer);
		}
		else if(3295 > pot_values[0] && pot_values[0] >= 2470)
		{
			tremolo_processBuffer(adc_buffer, dac_buffer);
		}
		else if(2470 >= pot_values[0] && pot_values[0] >= 1700)
		{
			wahwah_processBuffer(adc_buffer, dac_buffer);
		}
		else if(1700 > pot_values[0])
		{
			//autres effets à ajouter ici
			directloopback_processBuffer(adc_buffer, dac_buffer);
		}

	}
	/*if(hadc == &hadc2)
	{
		if(!pot_selector)//first passage
			potd_value = HAL_ADC_GetValue(&hadc2);
		else
			pot_values[0] = HAL_ADC_GetValue(&hadc2);

		pot_selector = !pot_selector;
	}*/

}

int calculate_capacity(int period) {
    // Calcul de la capacité pF en fonction de la période ms
	//int temp_capa = 0.000975619084 * period - 28.122;
	int temp_capa = 0.000975619084 * period - 49;
	if(temp_capa<0)
		temp_capa = 0;
	temp_capa = temp_capa*100 / 240;
	if(temp_capa>100)
			temp_capa = 100;//On mets entre 0 et 100 ici
    return temp_capa ;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim2)
	{
		if(i){
			last_clock = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			i=0;
		}
		else if(!i){
			current_clock =	HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			difference = current_clock - last_clock;
			if(difference != 0)
				frequence = 32000000/difference;
			else
				return;
			periode_ns = difference*32;
			i=1;
			calcul_open = 1;
		}
		if (calcul_open){
			int temp_capa;
			temp_capa = calculate_capacity(periode_ns);
			if(temp_capa < 500 && temp_capa > -100)
				capacite_pF = temp_capa;
			else
			{
				HAL_GPIO_TogglePin(TEST1_GPIO_Port, TEST1_Pin);
			}
			calcul_open = 0;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6)
	{
		//HAL_ADC_Start_IT(&hadc2);
		  HAL_ADC_Start_DMA(&hadc2, pot_values, 2);
	}
}


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
