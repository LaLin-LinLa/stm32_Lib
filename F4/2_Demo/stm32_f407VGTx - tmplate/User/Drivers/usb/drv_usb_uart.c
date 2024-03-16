/**
  ******************************************************************************
  * @file    drv_usb_uart.c/h
  * @brief   Code for USB driver in STM32 series MCU	
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
			-#����drv_USB_Init��ʼ��USB����
			-#ʹ��usb_printf��ӡ���ݣ��÷�ͬprintf������
			
    @attention
			-# ��ģ����Ҫ����usbd_cdc_if.c/h
			-# ��ģ����Ҫ����usb_device.c/h		
			-# ��ģ��δ��װ���ճ��򣬽��ղ�����usbd_cdc_if.c��CDC_Receive_FS������ʵ��				
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/

#include "drv_usb_uart.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include <stdarg.h>

#define APP_TX_SIZE	256

/**
	*	@brief	USB��ʼ��
 **/
void drv_USB_Init(void)
{
	USB_Status_Rest();
	MX_USB_DEVICE_Init();
}

/**
	*	@brief	�ϵ�����ö��
 **/
void USB_Status_Rest(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
}

/**
	*	@brief	USB��ӡ
 **/
void usb_printf(const char *format, ...)
{
	va_list args;
	uint32_t length;
	uint8_t buff[APP_TX_SIZE];
	
	va_start(args, format);
	length = vsnprintf((char *)buff, APP_TX_SIZE, (char *)format, args);
	va_end(args);
	CDC_Transmit_FS(buff, length);
}


