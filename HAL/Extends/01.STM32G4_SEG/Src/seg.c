#include "seg.h"

const uint8_t seg_buff[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void seg_set(uint8_t seg1,uint8_t seg2,uint8_t seg3);

void seg_set(uint8_t seg1,uint8_t seg2,uint8_t seg3)
{
	uint32_t seg = (seg_buff[seg3] << 16) | (seg_buff[seg2] << 8) | seg_buff[seg1];
	uint8_t i;
	for(i = 0;i < 24; i++)
	{
		if(seg & 0x800000)
			SEG_H;
		else
			SEG_L;
		SCK_H;
		SCK_L;
		seg <<= 1;
	}
	RCLK_H;
	RCLK_L;
}
