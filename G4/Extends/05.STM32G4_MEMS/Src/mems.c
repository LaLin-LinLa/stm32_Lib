/*
  程序说明: CT117E嵌入式竞赛板GPIO模拟I2C总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT117E嵌入式竞赛板
  日    期: 2011-8-9
*/

#include "mems.h"

/** I2C 总线接口 */
#define I2C_PORT GPIOA
#define SDA_Pin	GPIO_PIN_5
#define SCL_Pin GPIO_PIN_4

#define FAILURE 0
#define SUCCESS 1

//配置SDA信号线为输入模式
void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStructure); 
}

//配置SDA信号线为输出模式
void SDA_Output_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;		// 要配置为开漏输出
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStructure); 
}

//
void SDA_Output( uint16_t val )
{
	if ( val ) {
		HAL_GPIO_WritePin(I2C_PORT,SDA_Pin,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(I2C_PORT,SDA_Pin,GPIO_PIN_RESET);
	}
}

//
void SCL_Output( uint16_t val )
{
	if ( val ) {
		HAL_GPIO_WritePin(I2C_PORT,SCL_Pin,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(I2C_PORT,SCL_Pin,GPIO_PIN_RESET);
	}
}

//
uint8_t SDA_Input()
{
	return HAL_GPIO_ReadPin( I2C_PORT, SDA_Pin);
}

//延时程序
void delay1(unsigned int n)
{
	unsigned int i;
	for ( i=0;i<n;++i);
}

//I2C总线启动
void I2CStart(void)
{
	SDA_Output(1);delay1(500);
	SCL_Output(1);delay1(500);
	SDA_Output(0);delay1(500);
	SCL_Output(0);delay1(500);
}

//I2C总线停止
void I2CStop(void)
{
	SCL_Output(0); delay1(500);
	SDA_Output(0); delay1(500);
	SCL_Output(1); delay1(500);
	SDA_Output(1); delay1(500);

}

//等待应答
unsigned char I2CWaitAck(void)
{
	unsigned short cErrTime = 5;
	SDA_Input_Mode(); 
	delay1(500);
	SCL_Output(1);delay1(500);
	while(SDA_Input())
	{
		cErrTime--;
		delay1(500);
		if (0 == cErrTime)
		{
			SDA_Output_Mode();
			I2CStop();
			return FAILURE;
		}
	}
	SDA_Output_Mode();
	SCL_Output(0);delay1(500); 
	return SUCCESS;
}

//发送应答位
void I2CSendAck(void)
{
	SDA_Output(0);delay1(500);
	delay1(500);
	SCL_Output(1); delay1(500);
	SCL_Output(0); delay1(500);

}

//
void I2CSendNotAck(void)
{
	SDA_Output(1);
	delay1(500);
	SCL_Output(1); delay1(500);
	SCL_Output(0); delay1(500);

}

//通过I2C总线发送一个字节数据
void I2CSendByte(unsigned char cSendByte)
{
	unsigned char  i = 8;
	while (i--)
	{
		SCL_Output(0);delay1(500); 
		SDA_Output(cSendByte & 0x80); delay1(500);
		cSendByte += cSendByte;
		delay1(500); 
		SCL_Output(1);delay1(500); 
	}
	SCL_Output(0);delay1(500); 
}

//从I2C总线接收一个字节数据
unsigned char I2CReceiveByte(void)
{
	unsigned char i = 8;
	unsigned char cR_Byte = 0;
	SDA_Input_Mode(); 
	while (i--)
	{
		cR_Byte += cR_Byte;
		SCL_Output(0);delay1(500); 
		delay1(500); 
		SCL_Output(1);delay1(500); 
		cR_Byte |=  SDA_Input(); 
	}
	SCL_Output(0);delay1(500); 
	SDA_Output_Mode();
	return cR_Byte;
}

//I2C总线初始化
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = SDA_Pin | SCL_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStructure); 

}

void i2c_write(uint8_t add,uint8_t dat)
{
	I2CStart();
	I2CSendByte(0x38);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte(dat);
	I2CWaitAck();
	I2CStop();
}

int8_t i2c_read(uint8_t add)
{
	int8_t ret;
	
	I2CStart();
	I2CSendByte(0x38);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0x39);
	I2CWaitAck();
	ret = I2CReceiveByte();
	I2CSendNotAck();
	I2CStop();
	
	return ret;
}
