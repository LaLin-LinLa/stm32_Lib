/**
  ******************************************************************************
  * @file    bsp_usart.c/h
  * @brief   Code for UART driver in STM32 series MCU	 
	@verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
			-#�Դ���1Ϊ����
				-# ����bsp_usart1_Init��ʼ������1��ز���(���ջ�����0��1���������ݳ��ȣ��жϻص�����)
				-# ��д��Ӧ���жϻص����������������
				-# ����Usart1_DMA_Transmit��DMA��ʽ��������
    @attention
			-# �û����������ѡ�����ô���ʹ��
      -# ��ģ���֧��LL��
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

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}

/**
  * @brief  ����1��ʼ��
	* @param	rx_buffr0		���ջ�����0
	* @param	len					���ճ���
	* @param	fun					�жϻص�����
 **/
void bsp_usart1_Init(uint8_t* rx_buffr0, uint16_t rx_len, usart_call_back fun)
{
	usart_manage_obj[1].rx_buffer_size = rx_len;
	usart_manage_obj[1].rx_buffer0 = rx_buffr0;
	usart_manage_obj[1].call_back_f = fun;
	
	/*DMA����*/
	/*DMA_rx*/
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)(&USART1->DR));
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)rx_buffr0);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, rx_len);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
	/*DMA_tx*/
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)(&USART1->DR));
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	/*USART����*/
	LL_USART_EnableDMAReq_RX(USART1);
	LL_USART_EnableDMAReq_TX(USART1);
	LL_USART_EnableIT_IDLE(USART1);
	LL_USART_ClearFlag_IDLE(USART1);
	LL_USART_Enable(USART1);
}

/*����1�жϷ�����*/
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
  * @brief  ����1DMA����
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
