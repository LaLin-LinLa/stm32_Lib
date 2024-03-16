#include "bsp_key.h"

uint8_t key_state = 0;		//按键状态
uint8_t key_falling = 0;	//按键下降沿标志
uint8_t key_rising	= 0;	//按键上升沿标志

/**
  * @brief  按键扫描
	* @return 按键状态
 **/
void Key_Scan(void)
{
	uint8_t key_temp = 0xF8;
	key_temp |= KEY_STATE_GET;
	key_temp ^= 0xFF;
	key_rising = (!key_temp) * (key_temp ^ key_state);
	key_falling = key_temp & (key_state ^ key_temp);
	key_state = key_temp;

}


