#include "mod_OLED.h"
#include "system.h"
#include "drv_oled.h"

/**
	* @brief  OLED模块初始化
	* @retval None
	*/
void mod_OLED_Init(void)
{
	OLED_Init();
	OLED_Clear();
	OLED_WR_Byte(0x2E,OLED_CMD);        //关闭滚动
	OLED_WR_Byte(0x26,OLED_CMD);        //水平向左或者右滚动 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);        //起始页 0
	OLED_WR_Byte(0x07,OLED_CMD);        //滚动时间间隔
	OLED_WR_Byte(0x07,OLED_CMD);        //终止页 7
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0xFF,OLED_CMD);        //虚拟字节
	OLED_ShowString(17,20,"Welcome",L);
	OLED_Refresh_Gram();
	delay_ms_nos(1000);
	OLED_Clear();
	OLED_ShowString(10,20,"Sneek2022",L);
	OLED_Refresh_Gram();
	OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动
}

/**
	* @brief  OLED模块循环
	* @retval None
	*/
void mod_OLED_Loop(void)
{
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
	
}
