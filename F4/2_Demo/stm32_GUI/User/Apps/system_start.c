#include "system_start.h"
#include "system.h"
#include "bsp_timer.h"
#include "drv_LCD.h"
#include "drv_sram.h"


u32 testsram[250000] __attribute__((at(0X68000000)));//测试用数组
//外部内存测试(最大支持1M字节内存测试)	    
void fsmc_sram_test(u16 x,u16 y)
{  
	u32 i=0;  	  
	u8 temp=0;	   
	u8 sval=0;	//在地址0读到的数据	  				   
  	LCD_ShowString(x,y,239,y+16,16,"Ex Memory Test:   0KB"); 
	//每隔4K字节,写入一个数据,总共写入256个数据,刚好是1M字节
	for(i=0;i<1024*1024;i+=4096)
	{
		FSMC_SRAM_WriteBuffer(&temp,i,1);
		temp++;
	}
	//依次读出之前写入的数据,进行校验		  
 	for(i=0;i<1024*1024;i+=4096) 
	{
  		FSMC_SRAM_ReadBuffer(&temp,i,1);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;//后面读出的数据一定要比第一次读到的数据大.	   		   
		LCD_ShowxNum(x+15*8,y,(u16)(temp-sval+1)*4,4,16,0);//显示内存容量  
 	}					 
}	

/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{	     
	u32 ts=0;
	bsp_Timer_Init(1);
	drv_LCD_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"SRAM TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/4/18");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Test Sram");
	LCD_ShowString(30,150,200,16,16,"KEY1:TEST Data");
 	POINT_COLOR=BLUE;//设置字体为蓝色  
	for(ts=0;ts<250000;ts++)testsram[ts]=ts;//预存测试数据	 
	
}

/**
	* @brief  系统任务循环
	* @retval None
	*/
void System_Loop(void)
{
	fsmc_sram_test(60,170);
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	delay_ms_nos(500);

}
