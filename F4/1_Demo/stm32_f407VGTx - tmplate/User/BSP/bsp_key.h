#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define 		NONE				0x00
#define			KEY0				0x01
#define			KEY1				0x02
#define			KEY_UP			0x04

#define			KEY_STATE_GET			(HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin) << 0 |	\
															 HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) << 1 | \
															(~HAL_GPIO_ReadPin(WK_UP_GPIO_Port,WK_UP_Pin)) << 2	)
 
extern uint8_t key_state;		//按键状态
extern uint8_t key_falling;	//按键下降沿标志
extern uint8_t key_rising;	//按键上升沿标志

void Key_Scan(void);

#ifdef __cplusplus
}
#endif
#endif

