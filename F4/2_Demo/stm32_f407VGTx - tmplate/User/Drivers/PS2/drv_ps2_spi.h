#ifndef _DRV_PS2_SPI_H_
#define _DRV_PS2_SPI_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define PSS_Lx 2                //右摇杆X轴数据
#define PSS_Ly 3
#define PSS_Rx 0
#define PSS_Ry 1

/*所有按键状态中对应数值(针对MASK)*/
#define PSB_Left        0
#define PSB_Down        1
#define PSB_Right       2
#define PSB_Up          3
#define PSB_Start       4
#define PSB_Select      7
#define PSB_Square      8
#define PSB_Cross       9
#define PSB_Circle      10
#define PSB_Triangle    11
#define PSB_R1          12
#define PSB_L1          13
#define PSB_R2          14
#define PSB_L2          15

typedef struct
{
  uint8_t ps2_cmd[2];
  uint8_t ps2_data[9];
	uint16_t XY[4];
  uint8_t  MASK[16];
	enum{
		No,
		Green,
		Red,
	} PS2_Mode;

} PS2_ctrl_t;

extern const PS2_ctrl_t *get_ps2_control_point(void);
void PS2_DateDatePrcess(void);

#ifdef __cplusplus
}
#endif
#endif
