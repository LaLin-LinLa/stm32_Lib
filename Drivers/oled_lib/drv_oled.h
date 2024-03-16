#ifndef __DRV_OLED_H
#define __DRV_OLED_H			  	 			  
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define ADDR	0x78
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define SCREEN_PAGE_NUM			   	(8)     //������ (����)
#define SCREEN_PAGEDATA_NUM   	(128)   //ÿ���е�����

#define OLED_IIC_PIN	DRV_I2C1

#define S	12
#define M	16
#define L	24

/*OLED_API*/  
void OLED_Display_On(void);																		//OLEDȫ��
void OLED_Display_Off(void);	   							   		    			//OLEDȫ��
void OLED_Init(void);																					//OLED��ʼ��
void OLED_Clear(void);																				//OLED����
void OLED_Refresh_Gram(void);																	//OLED��������
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);							//OLED�������
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);				//OLED��ʾ�ַ�
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);					//OLED��ʾ����
void OLED_ShowSignNum(uint8_t x,uint8_t y,int32_t num,uint8_t len,uint8_t size);			//OLED��ʾ����(������)
void OLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);	//OLED��ʾʮ�������� 
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);							//OLED��ʾ�ַ���

#ifdef __cplusplus
}
#endif
#endif  
	 



