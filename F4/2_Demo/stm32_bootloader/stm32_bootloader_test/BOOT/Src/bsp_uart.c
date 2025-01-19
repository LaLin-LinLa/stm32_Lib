#include "bsp_uart.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       	//接收状态标记	
uint32_t USART_RX_CNT=0;			//接收的字节数 
uint8_t USART_RX_BUF[USART_REC_LEN] __attribute__ ((at(0X20001000)));//接收缓冲,最大USART_REC_LEN个字节,起始地址为0X20001000.

void bsp_uart1_init(void)
{
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);		//开启接收中断
}

void uart1_Rx_handler(void)
{
	uint8_t Res;
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		 HAL_UART_Receive(&huart1,&Res,1,1000);
		if(USART_RX_CNT<USART_REC_LEN)
		{
			USART_RX_BUF[USART_RX_CNT]=Res;
			USART_RX_CNT++;			 									     
		}
	}
}
