#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;

/*LED*/
#define LED0(a)	if (a==1)	\
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); \
					else if  (a==0)	\
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); \
					else if(a==2) \
				HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)
#define LED1(a)	if (a==1)	\
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET); \
					else if  (a==0)		\
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); \
					else if(a==2) \
				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)

/*delay*/
void bsp_systick_init(uint8_t SYSCLK);
void delay_ms(__IO uint16_t nms);
void delay_us(__IO uint32_t nus);			

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 
					
#ifdef __cplusplus
}
#endif
#endif

