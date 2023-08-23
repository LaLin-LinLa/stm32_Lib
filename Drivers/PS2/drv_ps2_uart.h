#ifndef _DRV_PS2_UART_H_
#define _DRV_PS2_UART_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define PS2_LEN				32
#define PS2_VALUE_OFFSET		((uint16_t)127)

typedef enum 
{
	L_KEY = 0,
	R_KEY,
	R2,
	R1,
	RU,
	RR,
	RD,
	RL,
	ST,
	SE,
	LR,
	LD,
	LL,
	LU,
	L1,
	L2,
} KEY_NUM;

typedef struct
{
	int16_t ch[4];	//0: L×óÓÒ 128 ~ -128; 1: LÉÏÏÂ 
	char s[16];
	float voltage;
} PS2_usart_ctrl_t;

uint8_t PS2_Pack_Check(uint8_t *buf, uint16_t len);
void PS2_DatePrcess(volatile const uint8_t * buf);
extern const PS2_usart_ctrl_t *get_PS2_usart_control_point(void);

#ifdef __cplusplus
}
#endif
#endif

