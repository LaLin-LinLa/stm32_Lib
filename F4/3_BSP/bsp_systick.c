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
			-# ����delay_us��ʵ��΢�뼶���������ʱ
			-# ����delay_ms��ʵ�ֺ��뼶���������ʱ
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

static uint32_t fac_us=0;							//us��ʱ������

/**
  * @brief  ϵͳ�δ�ʱ�� SysTick ��ʼ��
  * @param  ticks	
	*					SystemFrequency / 1000    1ms�ж�һ��
	*					SystemFrequency / 100000	 10us�ж�һ��
	*					SystemFrequency / 1000000 1us�ж�һ��
  * @retval ��
  */
void bsp_systick_Init(uint8_t SYSCLK)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	fac_us=SYSCLK;
}

/**
  * @brief   ms��ʱ����,1msΪһ����λ
  * @param  
  * @retval  ��
  */
void delay_ms(__IO uint16_t nms)
{ 
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

/**
  * @brief   us��ʱ����,1usΪһ����λ
  * @param  
  * @retval  ��
  */
void delay_us(__IO uint32_t nus)
{ 
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}
