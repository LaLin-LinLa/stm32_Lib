/**
  ******************************************************************************
  * @file    bsp_usart.c/h
  * @brief   Code for UART driver in STM32 series MCU	 
	@verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
			-#以串口1为例：
				-# 调用bsp_usart1_Init初始化串口1相关参数(接收缓存区0、1，接收数据长度，中断回调函数)
				-# 编写相应的中断回调函数处理接收数据
				-# 调用Usart1_DMA_Transmit以DMA方式发送数据
    @attention
			-# 用户视情况自行选择配置串口使用
      -# 本模块仅支持LL库
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "bsp_usart.h"
#include <stdio.h>
#include <string.h>

static usart_manage_obj_t usart_manage_obj[8] = {NULL};

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}

/**
  * @brief  串口1初始化
	* @param	rx_buffr0		接收缓存区0
	* @param	len					接收长度
	* @param	fun					中断回调函数
 **/
void bsp_usart1_Init(uint8_t* rx_buffr0, uint16_t rx_len, usart_call_back fun)
{
	usart_manage_obj[1].rx_buffer_size = rx_len;
	usart_manage_obj[1].rx_buffer0 = rx_buffr0;
	usart_manage_obj[1].call_back_f = fun;
	
	/*DMA配置*/
	/*DMA_rx*/
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)(&USART1->DR));
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)rx_buffr0);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, rx_len);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
	/*DMA_tx*/
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)(&USART1->DR));
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	/*USART配置*/
	LL_USART_EnableDMAReq_RX(USART1);
	LL_USART_EnableDMAReq_TX(USART1);
	LL_USART_EnableIT_IDLE(USART1);
	LL_USART_ClearFlag_IDLE(USART1);
	LL_USART_Enable(USART1);
}

/*串口1中断服务函数*/
void usart1_IQR_Service(void)
{
	static uint16_t this_time_rx_len = 0;
	if(LL_USART_IsActiveFlag_RXNE(USART1) == SET)
	{
		LL_USART_ClearFlag_RXNE(USART1);
	}
	else if(LL_USART_IsActiveFlag_IDLE(USART1) == SET)
	{
		LL_USART_ClearFlag_IDLE(USART1);
		
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
		while(LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_5)){
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);}
		LL_DMA_ClearFlag_TC5(DMA1); LL_DMA_ClearFlag_HT5(DMA1);
		this_time_rx_len = usart_manage_obj[1].rx_buffer_size - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_5);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, usart_manage_obj[1].rx_buffer_size);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
		
		if(usart_manage_obj[1].call_back_f != NULL)
		usart_manage_obj[1].call_back_f(usart_manage_obj[1].rx_buffer0, this_time_rx_len);
	}
}

/**
  * @brief  串口1DMA传输
 **/
void Usart1_DMA_Transmit(uint32_t data, uint16_t ndtr)
{
	while(LL_USART_IsActiveFlag_TC(USART1) == 0){}
	LL_USART_ClearFlag_TC(USART1);
	LL_DMA_ClearFlag_TC4(DMA1);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	while(LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_4)){
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);}
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, data);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, ndtr);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}
