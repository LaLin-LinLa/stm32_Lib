#include "key.h"

uint8_t key_falling = 0;
uint8_t key_state = 0;

void key_refresh(void)
{
	uint8_t key_temp = 0xF0;
	key_temp |= KEY_STATE_GET;
	key_temp ^= 0xFF;
	key_falling = key_temp & (key_state ^ key_temp);
	key_state = key_temp;
}
