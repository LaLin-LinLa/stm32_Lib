#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define USART_REC_LEN  			2000  	//定义最大接收字节数
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART_RX_STA;         		//接收状态标记	
extern uint32_t USART_RX_CNT;				//接收的字节数
extern void uart1_Rx_handler(void);
extern void bsp_uart1_init(void);

#ifdef __cplusplus
}
#endif
#endif
