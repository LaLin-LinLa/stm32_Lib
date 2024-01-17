/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t led_tick = 0;
_Bool led_flag = 0;
uint8_t key_tick = 0;

uint16_t ADC1_Value;
uint16_t ADC2_Value;

uint8_t lcd_str[20];

uint32_t Sine12bit[32] = {
	2048	, 2460	, 2856	, 3218	, 3532	, 3786	, 3969	, 4072	,
	4093	, 4031	, 3887	, 3668	, 3382	, 3042	,2661	, 2255	, 
	1841	, 1435	, 1054	, 714	, 428	, 209	, 65	, 3		,
	24		, 127	, 310	, 564	, 878	, 1240	, 1636	, 2048
};
uint16_t DualSine12bit[100];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void led_proc(void);
void adc_proc(void);
void key_scan(void);
void lcd_proc(void);
void dac_proc(void);
void usart_proc(void);
void SineWave_Data( u16 num,u16 *D,float U);
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
//	uint32_t Idx;        //定义变量
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
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC1_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	led_init();
	LCD_Init();
	
	/*使能定时器4中断*/
	HAL_TIM_Base_Start_IT(&htim4);
	
//	for (Idx = 0; Idx <100; Idx++)  
//	{  
//		DualSine12bit[Idx] = (Sine12bit[Idx] << 16)+ (Sine12bit[Idx]);  
//	}
	HAL_TIM_Base_Start(&htim6);
	// 在配置DAC的DMA的时候，必须将数据类型配置为32位
  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)Sine12bit, 32, DAC_ALIGN_12B_R);
//	HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t *)DualSine12bit,100,DAC_ALIGN_12B_R);
	
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);    //ADC1校准
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);    //ADC2校准
	/*ADC DMA的CIRCULAR模式，须将MX_DMA_Init()函数中的中断注释掉，否则程序会阻塞*/
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC1_Value, 1);
	
	LCD_Clear(Blue);
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);
	
	LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line1, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line2, (uint8_t *)"      LCD Test      ");
	LCD_DisplayStringLine(Line3, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line4, (uint8_t *)"                    ");
	
	LCD_SetBackColor(White);
	LCD_SetTextColor(Blue);

	LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line6, (uint8_t *)"       HAL LIB      ");
	LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line8, (uint8_t *)"         @80        ");
	LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		led_proc();
		adc_proc();
		lcd_proc();
		dac_proc();
		usart_proc();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == (&htim4))
	{
		if(++led_tick == 500)
		{
			led_tick = 0;
			led_flag = 1;
		}
		if(++key_tick == 10)
		{
			key_tick = 0;
			key_scan();
		}
	}
}

void led_proc(void)
{
	if(led_flag)
	{
		led_flag = 0;
		led_toggle(LD8);
	}
}

void adc_proc(void)
{
//	HAL_ADC_Start(&hadc1);																											//启动ADC装换
//	HAL_ADC_PollForConversion(&hadc1, 50);																			//等待转换完成， 第二个参数表示超时时间，单位ms.
//	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)){
//		ADC1_Value = HAL_ADC_GetValue(&hadc1);																		//读取ADC转换数据， 数据为12位
//	}
//	HAL_ADC_Stop(&hadc1);
	
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC1_Value, 1);
//	HAL_ADC_Stop_DMA(&hadc1);
	
	HAL_ADC_Start(&hadc2);																											//启动ADC装换
	HAL_ADC_PollForConversion(&hadc2, 50);																			//等待转换完成， 第二个参数表示超时时间，单位ms.
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC)){
		ADC2_Value = HAL_ADC_GetValue(&hadc2);																		//读取ADC转换数据， 数据为12位
	}
	HAL_ADC_Stop(&hadc2);
}

void lcd_proc(void)
{
	memset(lcd_str,0,sizeof(lcd_str));
	sprintf((char*)lcd_str,"       ADC1: %.2f   ",ADC1_Value / 4095.0 * 3.3 + 0.005);
//	sprintf((char*)lcd_str,"       ADC: %d   ",ADC_Value);
	LCD_DisplayStringLine(Line6,lcd_str);
	
	memset(lcd_str,0,sizeof(lcd_str));
	sprintf((char*)lcd_str,"       ADC2: %.2f   ",ADC2_Value / 4095.0 * 3.3 + 0.005);
//	sprintf((char*)lcd_str,"       ADC: %d   ",ADC_Value);
	LCD_DisplayStringLine(Line8,lcd_str);
}

void dac_proc(void)
{
//	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ADC1_Value);
}

void usart_proc(void)
{
//	printf("HELLO WORLD!!! \r\n");
//	HAL_Delay(1000);
	printf("%.2f\r\n",ADC1_Value / 4095.0 * 3.3 + 0.005);
}

void key_scan(void)
{
	key_refresh();	// 刷新一次按键的状态信息
	if(key_falling == B1)
	{
		led_toggle(LD1);
	}
	else if(key_falling == B2)
	{
		led_toggle(LD2);
	}
	else if(key_falling == B3)
	{
		led_toggle(LD3);
	}
	else if(key_falling == B4)
	{
		led_toggle(LD4);
	}
}

/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
