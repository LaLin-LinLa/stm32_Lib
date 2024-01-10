#include "system_start.h"
#include "system.h"
#include "bsp_timer.h"
#include "drv_LCD.h"
#include "drv_sram.h"


u32 testsram[250000] __attribute__((at(0X68000000)));//����������
//�ⲿ�ڴ����(���֧��1M�ֽ��ڴ����)	    
void fsmc_sram_test(u16 x,u16 y)
{  
	u32 i=0;  	  
	u8 temp=0;	   
	u8 sval=0;	//�ڵ�ַ0����������	  				   
  	LCD_ShowString(x,y,239,y+16,16,"Ex Memory Test:   0KB"); 
	//ÿ��4K�ֽ�,д��һ������,�ܹ�д��256������,�պ���1M�ֽ�
	for(i=0;i<1024*1024;i+=4096)
	{
		FSMC_SRAM_WriteBuffer(&temp,i,1);
		temp++;
	}
	//���ζ���֮ǰд�������,����У��		  
 	for(i=0;i<1024*1024;i+=4096) 
	{
  		FSMC_SRAM_ReadBuffer(&temp,i,1);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;//�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�.	   		   
		LCD_ShowxNum(x+15*8,y,(u16)(temp-sval+1)*4,4,16,0);//��ʾ�ڴ�����  
 	}					 
}	

/**
	* @brief  ϵͳ��ʼ��
	* @retval None
	*/
void System_Init(void)
{	     
	u32 ts=0;
	bsp_Timer_Init(1);
	drv_LCD_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"SRAM TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/4/18");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Test Sram");
	LCD_ShowString(30,150,200,16,16,"KEY1:TEST Data");
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ  
	for(ts=0;ts<250000;ts++)testsram[ts]=ts;//Ԥ���������	 
	
}

/**
	* @brief  ϵͳ����ѭ��
	* @retval None
	*/
void System_Loop(void)
{
	fsmc_sram_test(60,170);
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	delay_ms_nos(500);

}
