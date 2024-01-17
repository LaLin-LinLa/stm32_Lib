#ifndef	__KEY_H
#define	__KEY_H

#include "main.h"

#define			B1				0x01
#define			B2				0x02
#define			B3				0x04
#define			B4				0x08

#define			KEY_STATE_GET			(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) << 0 |\
															HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) << 1 |\
															HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) << 2 |\
															HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) << 3)

extern uint8_t key_falling;
extern uint8_t key_state;

void key_refresh(void);

#endif	/*__KEY_H*/
