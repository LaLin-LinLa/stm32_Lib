#include "bsp_uart.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;


#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       	//����״̬���	
uint32_t USART_RX_CNT=0;			//���յ��ֽ��� 
uint8_t USART_RX_BUF[USART_REC_LEN] __attribute__ ((at(0X20001000)));//���ջ���,���USART_REC_LEN���ֽ�,��ʼ��ַΪ0X20001000.

void bsp_uart1_init(void)
{
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);		//���������ж�
}

void uart1_Rx_handler(void)
{
	uint8_t Res;
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		 HAL_UART_Receive(&huart1,&Res,1,1000);
		if(USART_RX_CNT<USART_REC_LEN)
		{
			USART_RX_BUF[USART_RX_CNT]=Res;
			USART_RX_CNT++;			 									     
		}
	}
}
