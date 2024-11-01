/**
  ******************************************************************************
  * @file    bsp_systick.c/h
  * @brief   Code for SysTick driver in STM32 series MCU	 
	@verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# 调用bsp_systick_Init初始化系统滴答定时器
			-# 调用delay_us以实现微秒级软件堵塞延时
			-# 调用delay_ms以实现毫秒级软件堵塞延时
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

static uint32_t fac_us=0;							//us延时倍乘数

/**
  * @brief  系统滴答定时器 SysTick 初始化
  * @param  ticks	
	*					SystemFrequency / 1000    1ms中断一次
	*					SystemFrequency / 100000	 10us中断一次
	*					SystemFrequency / 1000000 1us中断一次
  * @retval 无
  */
void bsp_systick_Init(uint8_t SYSCLK)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SYSCLK;
}

/**
  * @brief   ms延时程序,1ms为一个单位
  * @param  
  * @retval  无
  */
void delay_ms(__IO uint16_t nms)
{ 
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

/**
  * @brief   us延时程序,1us为一个单位
  * @param  
  * @retval  无
  */
void delay_us(__IO uint32_t nus)
{ 
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}
