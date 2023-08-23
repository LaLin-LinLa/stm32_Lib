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
#include "bsp_spi.h"
#include "bsp_key.h"
#include "drv_W25Qx.h"
#include "drv_LCD.h"
#include "drv_ps2_uart.h"
#include "drv_usb_uart.h"

/*遥控解包任务*/
void RC_Task(void *p_arg);
TaskHandle_t RC_Task_Handler;

static void usart1_rx_call_back(uint8_t *buf, uint16_t len);
static void PS2_uart_call_back(uint8_t *buf, uint16_t len);

/**
	* @brief  系统硬件层初始化
	* @retval None
	*/
void System_Driver_Init(void)
{
	drv_USB_Init();
	bsp_Timer_Init(1);	//定时器初始化
	bsp_spi_Init(SPI1);
	bsp_usart1_Init(Usart1_Rx_Buff[0], Usart1_Rx_Buff[1], USART1_RX_BUFFER_SIZE, usart1_rx_call_back);	//串口1初始化
	bsp_usart6_Init(Usart6_Rx_Buff, USART6_RX_BUFFER_SIZE, PS2_uart_call_back);	//串口6初始化
	//bsp_I2C_soft_Init(&DRV_I2C1, I2C1_SCL_S_GPIO_Port, I2C1_SDA_S_GPIO_Port, I2C1_SCL_S_Pin, I2C1_SDA_S_Pin, 8, 9);	//模拟I2C1初始化配置
	LL_TIM_EnableAllOutputs(TIM4);
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2);
	LL_TIM_OC_SetCompareCH1(TIM4, 15);	
	LL_TIM_OC_SetCompareCH2(TIM4, 15);	
	drv_LCD_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE;	
	drv_W25Qx_Init();
	LCD_ShowString(50,50,210,24,24,"Welcome!");
	delay_ms_nos(5000);
	
}

/**
	* @brief  系统硬件层循环
	* @retval None
	*/
void System_Driver_Loop(void)
{
	/*串口遥控任务*/
	xTaskCreate(RC_Task, "rc_task", Small_Stack_Size, NULL, PrioritySuperHigh, &RC_Task_Handler); 
}

/*=========================================================中断回调函数=========================================================*/
/**
	* @brief  串口1中断回调函数
	* @retval None
	*/
static void usart1_rx_call_back(uint8_t *buf, uint16_t len)
{
	
}

/**
	* @brief  串口6中断回调函数
	* @retval None
	*/
static void PS2_uart_call_back(uint8_t *buf, uint16_t len)
{
	static BaseType_t* Usart6_TaskWoken;
	static USART_COB	PS2_pack = {0};
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
  {
		if(!PS2_Pack_Check(buf, len))
		{
			PS2_pack.port_num = 6;
			PS2_pack.address = buf;
			PS2_pack.len = len;
			xTaskNotifyFromISR(RC_Task_Handler, (uint32_t) &PS2_pack, eSetValueWithOverwrite, Usart6_TaskWoken);
		}
	}
}

/*=========================================================任务函数=========================================================*/
/** 
	* @brief 遥控解包任务 
 **/
void RC_Task(void *p_arg)
{
	static USART_COB* RCPack;
	while(1)
	{
		if(xTaskNotifyWait(0x00000000, 0xFFFFFFFF, (uint32_t *) &RCPack, portMAX_DELAY) == pdTRUE)
		{
			if(RCPack->port_num == 6)
			{
				PS2_DatePrcess(RCPack->address);
			}
		}
	}
}
