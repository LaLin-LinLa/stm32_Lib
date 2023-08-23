#ifndef _DRV_USB_UART_H_
#define _DRV_USB_UART_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

void drv_USB_Init(void);
void USB_Status_Rest(void);
void usb_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif

