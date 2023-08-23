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
	* @param	rx_buffr1		接收缓存区1
	* @param	len					接收长度
	* @param	fun					中断回调函数
 **/
void bsp_usart1_Init(uint8_t* rx_buffr0, uint8_t* rx_buffr1, uint16_t rx_len, usart_call_back fun)
{
	usart_manage_obj[1].rx_buffer_size = rx_len;
	usart_manage_obj[1].rx_buffer0 = rx_buffr0;
	usart_manage_obj[1].rx_buffer1 = rx_buffr1;
	usart_manage_obj[1].call_back_f = fun;
	
	/*DMA配置*/
	/*DMA_rx*/
	LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_2, (uint32_t)(&USART1->DR));
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_2, (uint32_t)rx_buffr0);
	LL_DMA_SetMemory1Address(DMA2, LL_DMA_STREAM_2, (uint32_t)rx_buffr1);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, rx_len);
	LL_DMA_EnableDoubleBufferMode(DMA2, LL_DMA_STREAM_2);
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
	/*DMA_tx*/
	LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_7, (uint32_t)(&USART1->DR));
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);
	/*USART配置*/
	LL_USART_EnableDMAReq_RX(USART1);
	LL_USART_EnableDMAReq_TX(USART1);
	LL_USART_EnableIT_IDLE(USART1);
	LL_USART_ClearFlag_IDLE(USART1);
	LL_USART_Enable(USART1);
	
}

/**
  * @brief  串口6初始化
	* @param	rx_buffr0		接收缓存区0
	* @param	len					接收长度
	* @param	fun					中断回调函数
 **/
void bsp_usart6_Init(uint8_t* rx_buffr0, uint16_t rx_len, usart_call_back fun)
{
	usart_manage_obj[6].rx_buffer_size = rx_len;
	usart_manage_obj[6].rx_buffer0 = rx_buffr0;
	usart_manage_obj[6].call_back_f = fun;
	
	/*DMA配置*/
	/*DMA_rx*/
	LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_1, (uint32_t)(&USART6->DR));
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_1, (uint32_t)rx_buffr0);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, rx_len);
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
	/*DMA_tx*/
	LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_6, (uint32_t)(&USART6->DR));
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_6);
	/*USART配置*/
	LL_USART_EnableDMAReq_RX(USART6);
	LL_USART_EnableDMAReq_TX(USART6);
	LL_USART_EnableIT_IDLE(USART6);
	LL_USART_ClearFlag_IDLE(USART6);
	LL_USART_Enable(USART6);
	
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
		if(LL_DMA_GetCurrentTargetMem(DMA2, LL_DMA_STREAM_2) == 0)		
		{
			LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
			while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_2)){
			LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);}
			LL_DMA_ClearFlag_TC2(DMA2); LL_DMA_ClearFlag_HT2(DMA2);
			LL_DMA_SetCurrentTargetMem(DMA2, LL_DMA_STREAM_2, LL_DMA_CURRENTTARGETMEM1);
			this_time_rx_len = usart_manage_obj[1].rx_buffer_size - LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_2);
			LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
				
				if(usart_manage_obj[1].call_back_f != NULL)
				usart_manage_obj[1].call_back_f(usart_manage_obj[1].rx_buffer0, this_time_rx_len);
				
		}
		else
		{
			LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
			while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_2)){
			LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);}
			LL_DMA_ClearFlag_TC2(DMA2); LL_DMA_ClearFlag_HT2(DMA2);
			LL_DMA_SetCurrentTargetMem(DMA2, LL_DMA_STREAM_2, LL_DMA_CURRENTTARGETMEM0);
			this_time_rx_len = usart_manage_obj[1].rx_buffer_size - LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_2);
			LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
			
			if(usart_manage_obj[1].call_back_f != NULL)
				usart_manage_obj[1].call_back_f(usart_manage_obj[1].rx_buffer1, this_time_rx_len);
			
		}
	}
}

/*串口6中断服务函数*/
void usart6_IQR_Service(void)
{
	static uint16_t this_time_rx_len = 0;
	if(LL_USART_IsActiveFlag_RXNE(USART6) == SET)
	{
		LL_USART_ClearFlag_RXNE(USART6);
	}
	else if(LL_USART_IsActiveFlag_IDLE(USART6) == SET)
	{
		LL_USART_ClearFlag_IDLE(USART6);
		
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
		while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_1)){
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);}
		LL_DMA_ClearFlag_TC1(DMA2); LL_DMA_ClearFlag_HT1(DMA2);
		this_time_rx_len = usart_manage_obj[6].rx_buffer_size - LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_1);
		LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, usart_manage_obj[6].rx_buffer_size);
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
		
		if(usart_manage_obj[6].call_back_f != NULL)
		usart_manage_obj[6].call_back_f(usart_manage_obj[6].rx_buffer0, this_time_rx_len);
			
	}
}


/**
  * @brief  串口1DMA传输
 **/
void Usart1_DMA_Transmit(uint32_t data, uint16_t ndtr)
{
	while(LL_USART_IsActiveFlag_TC(USART1) == 0){}
	LL_USART_ClearFlag_TC(USART1);
	LL_DMA_ClearFlag_TC7(DMA2);
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);
	while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_7)){		//确保DMA可以被设置
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);}
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_7, data);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, ndtr);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);
}

/**
  * @brief  串口6DMA传输
 **/
void Usart6_DMA_Transmit(uint32_t data, uint16_t ndtr)
{
	while(LL_USART_IsActiveFlag_TC(USART6) == 0){}
	LL_USART_ClearFlag_TC(USART6);
	LL_DMA_ClearFlag_TC6(DMA2);
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_6);
	while(LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_6)){		//确保DMA可以被设置
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_6);}
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_6, data);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_6, ndtr);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_6);
}
