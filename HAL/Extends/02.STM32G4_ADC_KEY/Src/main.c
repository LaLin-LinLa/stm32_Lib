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
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

uint16_t adc_key_value;
uint8_t adc_key_tick = 0;
_Bool adc_key_flag;

uint8_t lcd_str[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void led_proc(void);
void key_scan(void);
void adc_key_scan(void);
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
  MX_TIM4_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	
	LCD_Init();
	
	led_init();
	
	HAL_TIM_Base_Start_IT(&htim4);
	
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);
	
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)&adc_key_value,1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
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
	LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		led_proc();
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
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void key_scan(void)
{
	key_refresh();
	if(key_falling == B1)
	{
		led_toggle(LD5);
	}
	else if(key_falling == B2)
	{
		led_toggle(LD6);
	}
	else if(key_falling == B3)
	{
		led_toggle(LD7);
	}
	else if(key_falling == B4)
	{
		led_toggle(LD8);
	}
}

void led_proc(void)
{
	if(led_flag)
	{
		led_flag = 0;
//		led_toggle(LD1);
	}
}

void adc_key_scan(void)
{
	static uint8_t adc_key_falling = 0;
	static uint8_t adc_key_state = 0;
	uint8_t adc_key_temp = 0x00;
	if(adc_key_value < 300)
		adc_key_temp |= 0x01;
	else if(adc_key_value < 800)
		adc_key_temp |= 0x02;
	else if(adc_key_value < 1500)
		adc_key_temp |= 0x04;
	else if(adc_key_value < 2000)
		adc_key_temp |= 0x08;
	else if(adc_key_value < 2500)
		adc_key_temp |= 0x10;
	else if(adc_key_value < 3200)
		adc_key_temp |= 0x20;
	else if(adc_key_value < 3500)
		adc_key_temp |= 0x40;
	else if(adc_key_value < 4000)
		adc_key_temp |= 0x80;
	adc_key_falling = adc_key_temp & (adc_key_state ^ adc_key_temp);
	adc_key_state = adc_key_temp;
	memset(lcd_str,0,sizeof(lcd_str));
	sprintf((char*)lcd_str,"Value:%d     ",adc_key_value);
	LCD_DisplayStringLine(Line8, lcd_str);
	if(adc_key_falling == 0x01)
	{
		led_toggle(LD1);
	}
	else if(adc_key_falling == 0x02)
	{
		led_toggle(LD2);
	}
	else if(adc_key_falling == 0x04)
	{
		led_toggle(LD3);
	}
	else if(adc_key_falling == 0x08)
	{
		led_toggle(LD4);
	}
	else if(adc_key_falling == 0x10)
	{
		led_toggle(LD5);
	}
	else if(adc_key_falling == 0x20)
	{
		led_toggle(LD6);
	}
	else if(adc_key_falling == 0x40)
	{
		led_toggle(LD7);
	}
	else if(adc_key_falling == 0x80)
	{
		led_toggle(LD8);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		if(++led_tick == 1000)
		{
			led_tick = 0;
			led_flag = 1;
		}
		if(++key_tick == 10)
		{
			key_tick = 0;
			key_scan();
		}
		if(++adc_key_tick == 10)
		{
			adc_key_tick = 0;
			adc_key_scan();
		}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/