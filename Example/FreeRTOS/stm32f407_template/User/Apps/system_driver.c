/**
  ******************************************************************************
  * @file    system_communication.c/h
  * @brief   系统驱动层文件
  * @version 1.0
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/	
#include "system_driver.h"
#include "system_os.h"
#include "system.h"
#include "system_datapool.h"
#include "bsp_timer.h"
#include "bsp_usart.h"
#include "bsp_i2c_soft.h"

static void usart1_rx_call_back(uint8_t *buf, uint16_t len);

/**
	* @brief  系统硬件层初始化
	* @retval None
	*/
void System_Driver_Init(void)
{
	bsp_Timer_Init(1);	//定时器初始化
	bsp_usart1_Init(Usart1_Rx_Buff[0], Usart1_Rx_Buff[1], USART1_RX_BUFFER_SIZE, usart1_rx_call_back);	//串口初始化
	bsp_I2C_Init(&DRV_I2C1, I2C1_SCL_S_GPIO_Port, I2C1_SDA_S_GPIO_Port, I2C1_SCL_S_Pin, I2C1_SDA_S_Pin, 8, 9);	//模拟I2C1初始化配置
}

/**
	* @brief  系统硬件层循环
	* @retval None
	*/
void System_Driver_Loop(void)
{
	
}

/*=========================================================中断回调函数=========================================================*/
/**
	* @brief  串口1中断回调函数
	* @retval None
	*/
static void usart1_rx_call_back(uint8_t *buf, uint16_t len)
{
	
}

