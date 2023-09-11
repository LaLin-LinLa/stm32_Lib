#ifndef __DRV_OLED_H
#define __DRV_OLED_H			  	 			  
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define ADDR	0x78
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define SCREEN_PAGE_NUM			   	(8)     //总行数 (大行)
#define SCREEN_PAGEDATA_NUM   	(128)   //每大行的列数

#define OLED_IIC_PIN	DRV_I2C1

#define S	12
#define M	16
#define L	24

/*OLED_API*/  
void OLED_Display_On(void);																		//OLED全亮
void OLED_Display_Off(void);	   							   		    			//OLED全灭
void OLED_Init(void);																					//OLED初始化
void OLED_Clear(void);																				//OLED清屏
void OLED_Refresh_Gram(void);																	//OLED更新数据
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);							//OLED填充区域
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);				//OLED显示字符
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);					//OLED显示数字
void OLED_ShowSignNum(uint8_t x,uint8_t y,int32_t num,uint8_t len,uint8_t size);			//OLED显示数字(带符号)
void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);	//OLED显示十六进制数 
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);							//OLED显示字符串

#ifdef __cplusplus
}
#endif
#endif  
	 



