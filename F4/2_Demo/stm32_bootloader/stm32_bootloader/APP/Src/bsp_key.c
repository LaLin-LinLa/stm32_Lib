#include "bsp_key.h"

void key_delay(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
 }

/**
  * @brief  按键扫描
	* @return 按键状态
 **/
uint8_t Key_Scan(uint8_t mode)
{
	static uint8_t key_up=1;     //按键松开标志
	if(mode==1)key_up=1;    //支持连按
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
			key_delay(10);
			key_up=0;
			if(KEY0==0)       return KEY0_PRES;
			else if(KEY1==0)  return KEY1_PRES;
			else if(WK_UP==1) return WKUP_PRES;          
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1;
	return 0;   //无按键按下
}


