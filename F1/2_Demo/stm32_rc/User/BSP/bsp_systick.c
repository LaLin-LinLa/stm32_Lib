/**
  ******************************************************************************
  * @file    bsp_systick.c/h
  * @brief   Code for SysTick driver in STM32 series MCU	 
	@verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# ����bsp_systick_Init��ʼ��ϵͳ�δ�ʱ��
			-# ����delay_us_sys��ʵ��΢�뼶���������ʱ
			-# ����delay_ms_sys��ʵ�ֺ��뼶���������ʱ
    @attention
			-# ��ģ��Ϊ��ʱ������ʽ��ʱ
      -# ��ģ���֧��LL��
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "bsp_systick.h"

static __IO uint32_t Time_cnt;


/**
  * @brief  ϵͳ�δ�ʱ�� SysTick ��ʼ��
  * @param  temp	
	*					SystemFrequency / 1000    1ms�ж�һ��
	*					SystemFrequency / 100000	 10us�ж�һ��
	*					SystemFrequency / 1000000 1us�ж�һ��
  * @retval ��
  */
void bsp_systick_Init(uint32_t ticks)
{
	if (HAL_SYSTICK_Config(SystemCoreClock / ticks))
	{  
		while (1);
	}
}

/**
  * @brief   us��ʱ����,10usΪһ����λ
  * @param  
  *	@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
  * @retval  ��
  */
void delay_us_sys(__IO uint32_t nTime)
{ 
	Time_cnt = nTime;	

	while(Time_cnt != 0);
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void Update_TimingDelay(void)
{
	if (Time_cnt != 0x00)
	{ 
		Time_cnt--;
	}
}
