/**
  ******************************************************************************
  * @file    drv_ps2_spi.c/h
  * @brief   ps2遥控器协议解析驱动代码(SPI模式)
  * @version 1.0
	@verbatim
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# 使用PS2_DateDatePrcess解包接收数据
  
      -# 使用get_ps2_control_point获取解包后的数据

    @attention
      本模块需要依赖bsp_spi.c/h，默认spi为spi1
			本模块需要依赖bsp_timer.c/h
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "drv_ps2_spi.h"
#include "bsp_timer.h"
#include "bsp_spi.h"

#define sp2_delay	delay_us_nos
#define DelayTIM	10

#define CS_H		HAL_GPIO_WritePin(SP2_CS_GPIO_Port,SP2_CS_Pin,GPIO_PIN_SET);
#define CS_L		HAL_GPIO_WritePin(SP2_CS_GPIO_Port,SP2_CS_Pin,GPIO_PIN_RESET);

static PS2_ctrl_t ps2_ctrl = {
	.ps2_cmd = 	 {0x01,0x42},
	.ps2_data =  {0},
	.XY = 			 {500, 500, 500, 500},		//摇杆模拟值(500归中)
	.MASK = 		 {0},						//按键值初始化
	.PS2_Mode =  No,
};


/**
  * @brief          通过指针获取PS2手柄数据
  * @retval         none
  */
const PS2_ctrl_t *get_ps2_control_point(void)
{
    return &ps2_ctrl;
}


/**
  * @brief      PS2手柄获取数据
  * @retval     none
  */
void PS2_DateGet(void)
{
	volatile uint8_t byte=0;
	CS_L  //拉低，开始通讯
	sp2_delay(DelayTIM);
	SPI_ReadWrite_8Byte(SPI1, ps2_ctrl.ps2_cmd[0]);									//发送0x01，启动通讯
	sp2_delay(DelayTIM);
	ps2_ctrl.ps2_data[1] = SPI_ReadWrite_8Byte(SPI1, ps2_ctrl.ps2_cmd[1]);		//发送0x42，请求接受数据
	sp2_delay(DelayTIM);
	if(ps2_ctrl.ps2_data[1] == 0x41)
	{
		ps2_ctrl.PS2_Mode = Green;
	}
	else if(ps2_ctrl.ps2_data[1] == 0x73)
	{
		ps2_ctrl.PS2_Mode = Red;
	}
	else
	{
		ps2_ctrl.PS2_Mode = No;
	}
	ps2_ctrl.ps2_data[2] = SPI_ReadWrite_8Byte(SPI1, 0x00);
	sp2_delay(DelayTIM);
	if(ps2_ctrl.ps2_data[2] == 0x5a)
	{
		for(byte = 3;byte <9;byte++){
			ps2_ctrl.ps2_data[byte] = SPI_ReadWrite_8Byte(SPI1, 0x00);
			sp2_delay(DelayTIM);
		}
	}
	CS_H  	 //拉高，准备下次通讯
}

/**
  * @brief      PS2手柄清除缓存数据
  * @retval     none
  */
void PS2_DateClear(void)
{
	uint8_t byte;
	for(byte=0;byte<9;byte++)
		ps2_ctrl.ps2_data[byte]=0x00;
}

/**
  * @brief      PS2手柄拨杆解析
  * @retval     none
  */
void PS2_DateAnolog(void)
{
	uint8_t i = 0;
	if(ps2_ctrl.PS2_Mode == Red)
	{
		for ( i = 5; i < 9; i++)
		{
			ps2_ctrl.ps2_data[i] = (uint16_t)ps2_ctrl.ps2_data[i];
			ps2_ctrl.XY[i-5] = (ps2_ctrl.ps2_data[i] * 1000)/255;
			if(ps2_ctrl.XY[i-5] < 503 && ps2_ctrl.XY[i-5] >497) ps2_ctrl.XY[i-5] = 500;
		}
	}
	else if(ps2_ctrl.PS2_Mode == Green)
	{
		for ( i = 0; i < 4; i++)
		{
			ps2_ctrl.XY[i] = 500;
		}
	}
}

/**
  * @brief      PS2手柄按键解析
  * @retval     none
  */
void PS2_DateKey(void)
{
	uint8_t loc = 1;
	uint8_t set = 0;
  for(loc = 8;loc > 0;loc--)  //位运算读取前八位
  {
		loc -= 1;
		ps2_ctrl.MASK[set] = (ps2_ctrl.ps2_data[3]&(1<<loc))>>loc;
		loc += 1;
		set++;
  }
	for(loc = 8;loc > 0;loc--)   //位运算读取后八位
  {
		loc -= 1;
		ps2_ctrl.MASK[set] = (ps2_ctrl.ps2_data[4]&(1<<loc))>>loc;
		loc += 1;
		set++;
  }
	for(set = 0;set < 16;set++)    //因为协议上按键按下为0，未按下为1，故需要对其进行反转
	{
		if(ps2_ctrl.MASK[set] == 1)  ps2_ctrl.MASK[set] = 0;
		else  ps2_ctrl.MASK[set] = 1;			 
	}
}


/**
  * @brief      PS2手柄数据解包
  * @retval     none
  */
void PS2_DateDatePrcess(void)
{
	PS2_DateClear();
	PS2_DateGet();
	PS2_DateAnolog();
	PS2_DateKey();
}



