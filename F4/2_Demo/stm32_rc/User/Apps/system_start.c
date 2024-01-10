#include "system_start.h"
#include "system.h"
#include "bsp_timer.h"
#include "bsp_spi.h"
#include "drv_24l01.h"
#include "drv_LCD.h"

uint8_t Rx_buf[33];

/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{	     
	bsp_Timer_Init(1);
	bsp_spi_Init(SPI1);
	drv_LCD_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"NRF24L01 TEST");	
	drv_24l01_Init();
	while(NRF24L01_Check())
	{
		LCD_ShowString(30,90,200,16,16,"NRF24L01 Error");
		delay_ms_nos(200);
		LCD_Fill(30,90,239,130+16,WHITE);
 		delay_ms_nos(200);
	}
	LCD_ShowString(30,90,200,16,16,"NRF24L01 OK");
}

/**
	* @brief  系统任务循环
	* @retval None
	*/
void System_Loop(void)
{
	NRF24L01_RX_Mode();
	while(1)
	{
//		memset(Tx_buf,0,sizeof(Tx_buf));
//		snprintf((char*)Tx_buf,20,"NO.1");
		if(NRF24L01_RxPacket(Rx_buf)==0)
		{
			Rx_buf[32]=0;//加入字符串结束符
			LCD_ShowString(30,190,lcddev.width-1,32,16,Rx_buf);
		}
	}
}


