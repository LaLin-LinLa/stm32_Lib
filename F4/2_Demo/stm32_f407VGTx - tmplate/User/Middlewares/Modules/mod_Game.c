#include "mod_Game.h"
#include "drv_LCD.h"
#include "drv_usb_uart.h"

Game_Control_t	game = {0};

uint16_t x = 120;
uint16_t y = 160;


/**
	* @brief  游戏机任务初始化
	* @retval None
	*/
void mod_Game_Init(void)
{
	LCD_Clear(WHITE);
	game.game_rc_ctrl = get_PS2_usart_control_point();
	//LCD_DrawRectangle(10,10,230,310);
	//LCD_Fill(x+10, y+10, x-10, y-10, WHITE);
}

/**
	* @brief  游戏机任务循环
	* @retval None
	*/
void mod_Game_Loop(void)
{
	usb_printf("SsSSssss\r\n");
	LCD_ShowString(30,40,210,24,24,"PS2");	
	LCD_ShowString(30,70,210,24,24,"CH[0]: 	");
	LCD_ShowNum(110,70, game.game_rc_ctrl->ch[0], 3,24);
	LCD_ShowString(30,100,210,24,24,"CH[1]: 	");
	LCD_ShowNum(110,100, game.game_rc_ctrl->ch[1], 3,24);
	LCD_ShowString(30,130,210,24,24,"CH[2]: 	");
	LCD_ShowNum(110,130, game.game_rc_ctrl->ch[2], 3,24);
	LCD_ShowString(30,160,210,24,24,"CH[3]:   ");
	LCD_ShowNum(110,160, game.game_rc_ctrl->ch[3], 3,24);
	LCD_ShowString(30,190,210,24,24,"voltage: ");
	LCD_ShowNum(110,190, game.game_rc_ctrl->voltage, 3,24);
	
}
