#include "system_start.h"
#include "system.h"
#include "bsp_timer.h"
#include "bsp_systick.h"
#include "bsp_i2c_soft.h"
#include "drv_oled.h"

#include "u8g2.h"

u8g2_t u8g2; 

/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{
	bsp_systick_Init(SYSTICK_10US_TICK);
	bsp_Timer_Init(1);
//	bsp_I2C_soft_Init(&DRV_I2C1, I2C1_SCL_S_GPIO_Port, I2C1_SDA_S_GPIO_Port, I2C1_SCL_S_Pin, I2C1_SDA_S_Pin, 6, 7);	//模拟I2C1初始化配置
//	OLED_Init();
//	OLED_Clear();
//	OLED_ShowString(20,24, "OLED TEST",16);  
//	OLED_Refresh_Gram();
	u8g2_Init(&u8g2);
}

/**
	* @brief  系统任务初始化
	* @retval None
	*/
void System_Loop(void)
{
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
	u8g2_FirstPage(&u8g2);
	do {
			u8g2_SetFontMode(&u8g2, 1);
			u8g2_SetFontDirection(&u8g2, 0);
			u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
			u8g2_DrawStr(&u8g2, 10, 20, "U");
			u8g2_SetFontDirection(&u8g2, 1);
			u8g2_SetFont(&u8g2, u8g2_font_inb30_mn);
			u8g2_DrawStr(&u8g2, 31, 8, "8");
			u8g2_SetFontDirection(&u8g2, 0);
			u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
			u8g2_DrawStr(&u8g2, 61, 30, "g");
			u8g2_DrawStr(&u8g2, 77, 30, "\xb2");
			u8g2_DrawHLine(&u8g2, 12, 35, 47);
			u8g2_DrawHLine(&u8g2, 13, 36, 47);
			u8g2_DrawVLine(&u8g2, 55, 32, 12);
			u8g2_DrawVLine(&u8g2, 56, 33, 12);
			u8g2_SetFont(&u8g2, u8g2_font_4x6_tr);
			u8g2_DrawStr(&u8g2, 11, 54, "github.com/olikraus/u8g2");
	} while (u8g2_NextPage(&u8g2));
	delay_ms_sys(200);
}
