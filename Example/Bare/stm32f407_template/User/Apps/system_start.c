#include "system_start.h"
#include "system.h"
#include "bsp_timer.h"
#include "bsp_usart.h"
#include "bsp_i2c_soft.h"
#include "drv_oled.h"

#define USART1_RX_BUFFER_SIZE 32
uint8_t Usart1_Rx_Buff[2][USART1_RX_BUFFER_SIZE];

static void usart1_rx_call_back(uint8_t *buf, uint16_t len);

/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{
	bsp_Timer_Init(1);	//定时器初始化
	bsp_usart1_Init(Usart1_Rx_Buff[0], Usart1_Rx_Buff[1], USART1_RX_BUFFER_SIZE, usart1_rx_call_back);	//串口初始化
	bsp_I2C_Init(&DRV_I2C1, I2C1_SCL_S_GPIO_Port, I2C1_SDA_S_GPIO_Port, I2C1_SCL_S_Pin, I2C1_SDA_S_Pin, 8, 9);	//模拟I2C1初始化配置
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
	* @brief  系统任务循环
	* @retval None
	*/
void System_Loop(void)
{
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
	delay_ms_nos(300);
}

/*=========================================================中断回调函数=========================================================*/

/**
	* @brief  串口1中断回调函数
	* @retval None
	*/
static void usart1_rx_call_back(uint8_t *buf, uint16_t len)
{
	
}


