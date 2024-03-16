#include "bsp_key.h"

uint8_t key_state = 0;		//����״̬
uint8_t key_falling = 0;	//�����½��ر�־
uint8_t key_rising	= 0;	//���������ر�־

/**
  * @brief  ����ɨ��
	* @return ����״̬
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


