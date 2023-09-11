#ifndef _MOD_GIMBAL_H_
#define _MOD_GIMBAL_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "system.h"
#include "drv_ps2_uart.h"

/*==========云台控制数据结构体==========*/
typedef struct
{
	const PS2_usart_ctrl_t *gimabl_rc_ctrl;
	
} Gimabl_Control_t;

void mod_Gimbal_Init(void);
void mod_Gimbal_Loop(void);

#ifdef __cplusplus
}
#endif
#endif

