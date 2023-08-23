#ifndef __DRV_DR16_H
#define __DRV_DR16_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define RC_FRAME_LENGTH	18u
#define RC_CH_VALUE_MIN         ((uint16_t)364)
#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)
#define RC_CH_VALUE_MAX         ((uint16_t)1684)
#define RC_SW_UP                ((uint16_t)1)
#define RC_SW_MID               ((uint16_t)3)
#define RC_SW_DOWN              ((uint16_t)2)
#define switch_is_down(s)       (s == RC_SW_DOWN)
#define switch_is_mid(s)        (s == RC_SW_MID)
#define switch_is_up(s)         (s == RC_SW_UP)
#define ModeChannel_R 		0
#define ModeChannel_L 		1		
#define RC_DEADLINE	            		20	
typedef __packed struct
{
	/*Ò£¿ØÆ÷Êý¾Ý°ü*/
	__packed struct
	{
					int16_t ch[5];
					char s[2];
	} rc;
} DR16_ctrl_t;

void dr16_DatePrcess(volatile const uint8_t * sbus_buf);
extern const DR16_ctrl_t *get_dr16_control_point(void);

#ifdef __cplusplus
}
#endif
#endif
