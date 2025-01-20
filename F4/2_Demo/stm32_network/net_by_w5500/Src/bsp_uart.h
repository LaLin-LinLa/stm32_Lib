#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define USART_REC_LEN  			2000  	//�����������ֽ���
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         		//����״̬���	
extern uint32_t USART_RX_CNT;				//���յ��ֽ���
extern void uart1_Rx_handler(void);
extern void bsp_uart1_init(void);

#ifdef __cplusplus
}
#endif
#endif
