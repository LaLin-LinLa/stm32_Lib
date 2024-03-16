/**
  ******************************************************************************
  * @file    bsp_timer.c/h
  * @brief   Code for Timer driver in STM32 series MCU	 
	@verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# ����bsp_Timer_Init��ʼ����ʱ��
			-# ����delay_us_nos��ʵ��΢�뼶���������ʱ
			-# ����delay_ms_nos��ʵ�ֺ��뼶���������ʱ
    @attention
			-# ��ģ��Ϊ��ʱ������ʽ��ʱ,Ĭ�϶�ʱ��Ϊ������ʱ��7
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
#include "bsp_timer.h"

#define TIMx TIM7

volatile uint32_t SystemTimerCnt;
static uint8_t delay_mode = 0;

/**
	* @brief  ��ʼ����ʱ��
	* @param  mode : ѡ��ʹ��ģ�鶨ʱ����HAL��ʱ
	*					0��ʹ��HAL����ʱ 1��ʹ�ö�ʱ��ģ����ʱ
	* @retval None
	*/
void bsp_Timer_Init(uint8_t mode)
{
	delay_mode = mode;
	LL_TIM_EnableCounter(TIMx);
	LL_TIM_EnableIT_UPDATE(TIMx);
	LL_TIM_ClearFlag_UPDATE(TIMx);
}


/**
	* @brief  ��ȡ��ʱ��ʱ��
	* @param  None
	* @retval ʵʱʱ��
	*/
uint32_t Get_SystemTimer(void)
{
	return TIMx->CNT + SystemTimerCnt * 0xffff;
}

/**
	* @brief  ���¶�ʱ��ʱ��
	* @not    ���˺������붨ʱ���жϷ�������
	* @param  None
	* @retval None
	*/
void Update_SystemTick(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIMx) == SET)
	{
		LL_TIM_ClearFlag_UPDATE(TIMx);
		SystemTimerCnt++;
	}
}

/**
* @brief  ΢�뼶���������ʱ
* @param  cnt : ��ʱʱ��
* @retval None
*/
void delay_us_nos(uint32_t cnt)
{
	uint32_t temp = cnt  + microsecond();

	while(temp >= microsecond());
}

/**
* @brief  ���뼶���������ʱ
* @param  cnt : ��ʱʱ��
* @retval None
*/
void delay_ms_nos(uint32_t cnt)
{
	if(delay_mode)
	{
		uint32_t temp = cnt * 1000 + microsecond();
		while(temp >= microsecond());
	}
	else
		HAL_Delay(cnt);
}


