#ifndef _MOD_GAME_H_
#define _MOD_GAME_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "system.h"
#include "drv_ps2_uart.h"

/*==========游戏机控制数据结构体==========*/
typedef struct
{
	const PS2_usart_ctrl_t *game_rc_ctrl;
	
} Game_Control_t;


void mod_Game_Init(void);
void mod_Game_Loop(void);

#ifdef __cplusplus
}
#endif
#endif

