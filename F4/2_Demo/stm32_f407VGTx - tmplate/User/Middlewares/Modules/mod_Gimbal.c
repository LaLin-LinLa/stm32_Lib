#include "mod_Gimbal.h"
//#include "system_os.h"
#include "drv_LCD.h"
#include "user_lib.h"

Gimabl_Control_t	gimabl = {0};

/**
	* @brief  云台模块初始化
	* @retval None
	*/
void mod_Gimbal_Init(void)
{
	gimabl.gimabl_rc_ctrl = get_PS2_usart_control_point();
	
}
uint8_t Yaw;
uint8_t Pitch;

/**
	* @brief  云台模块循环
	* @retval None
	*/
void mod_Gimbal_Loop(void)
{	
	LCD_ShowString(30,70,210,24,24,"CH[0]: 	");
	LCD_ShowNum(110,70, gimabl.gimabl_rc_ctrl->ch[0], 3,24);
	LCD_ShowString(30,100,210,24,24,"CH[1]: 	");
	LCD_ShowNum(110,100, gimabl.gimabl_rc_ctrl->ch[1], 3,24);
	LCD_ShowString(30,130,210,24,24,"CH[2]: 	");
	LCD_ShowNum(110,130, gimabl.gimabl_rc_ctrl->ch[2], 3,24);
	LCD_ShowString(30,160,210,24,24,"CH[3]:   ");
	LCD_ShowNum(110,160, gimabl.gimabl_rc_ctrl->ch[3], 3,24);
	Yaw = int_map(gimabl.gimabl_rc_ctrl->ch[0], -128, 128, 0, 24);
	Pitch = int_map(gimabl.gimabl_rc_ctrl->ch[1], -128, 128, 0, 24);
	LL_TIM_OC_SetCompareCH1(TIM4, Yaw);
	LL_TIM_OC_SetCompareCH2(TIM4, Pitch);
	
}

