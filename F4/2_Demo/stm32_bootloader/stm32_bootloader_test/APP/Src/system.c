#include "system.h"

static uint32_t fac_us=0;							//us��ʱ������

/**
  * @brief  ϵͳ�δ�ʱ�� SysTick ��ʼ��
  * @param  ticks	
	*					SystemFrequency / 1000    1ms�ж�һ��
	*					SystemFrequency / 100000	 10us�ж�һ��
	*					SystemFrequency / 1000000 1us�ж�һ��
  * @retval ��
  */
void bsp_systick_init(uint8_t SYSCLK)
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

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
