#include "system.h"

static uint32_t fac_us=0;							//us延时倍乘数

/**
  * @brief  系统滴答定时器 SysTick 初始化
  * @param  ticks	
	*					SystemFrequency / 1000    1ms中断一次
	*					SystemFrequency / 100000	 10us中断一次
	*					SystemFrequency / 1000000 1us中断一次
  * @retval 无
  */
void bsp_systick_init(uint8_t SYSCLK)
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

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
