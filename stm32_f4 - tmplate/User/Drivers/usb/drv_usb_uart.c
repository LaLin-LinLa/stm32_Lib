#include "drv_usb_uart.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include <stdarg.h>

#define APP_TX_SIZE	256

/**
	*	@brief	USB初始化
 **/
void drv_USB_Init(void)
{
	USB_Status_Rest();
	MX_USB_DEVICE_Init();
}

/**
	*	@brief	上电重新枚举
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
	*	@brief	USB打印
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


