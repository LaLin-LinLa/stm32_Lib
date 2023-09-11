#ifndef _BSP_I2C_SOFT_H
#define _BSP_I2C_SOFT_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

/** 
	* @brief  根据硬件电路的要求，用户应将I2C的通信线路设置得尽可能小.用户可以减少这个时间，以减少读取数据的时间 
	*/
#define IIC_Delay_Time 20	
	
typedef struct 
{
	GPIO_TypeDef * IIC_SCL_GPIO_PORT;
	GPIO_TypeDef * IIC_SDA_GPIO_PORT;
	uint32_t IIC_SCL_PIN;
	uint32_t IIC_SDA_PIN;
	uint32_t IIC_SCL_PIN_NUM;
	uint32_t IIC_SDA_PIN_NUM;	
}IIC_PIN_Typedef;

extern IIC_PIN_Typedef DRV_I2C1;
extern IIC_PIN_Typedef DRV_I2C2;
extern IIC_PIN_Typedef DRV_I2C3;

void IIC_Delay(uint8_t m);
void bsp_I2C_soft_Init(IIC_PIN_Typedef *iic_pin, GPIO_TypeDef *gpio_scl, GPIO_TypeDef *gpio_sda, uint32_t scl_pinx, uint32_t sda_pinx, uint32_t scl_pin_num, uint32_t sda_pin_num);                                          
void IIC_Start(IIC_PIN_Typedef *iic_pin);				                  
void IIC_Stop(IIC_PIN_Typedef *iic_pin);	  			                  
void IIC_Send_Byte(IIC_PIN_Typedef *iic_pin,unsigned char txd);			   
unsigned char IIC_Read_Byte(IIC_PIN_Typedef *iic_pin,unsigned char ack); 
unsigned char IIC_Wait_Ack(IIC_PIN_Typedef *iic_pin); 				    
void IIC_Ack(IIC_PIN_Typedef *iic_pin);					                  
void IIC_NAck(IIC_PIN_Typedef *iic_pin);				                 
unsigned char IIC_Device_Write_Byte(IIC_PIN_Typedef *iic_pin, unsigned char addr, unsigned char reg, unsigned char data);
unsigned char IIC_Device_Read_Byte(IIC_PIN_Typedef *iic_pin,unsigned char addr,unsigned char reg);
unsigned char IIC_Device_Write_Len(IIC_PIN_Typedef *iic_pin, unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf);
unsigned char IIC_Device_Read_Len(IIC_PIN_Typedef *iic_pin, unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf);

#ifdef __cplusplus
}
#endif
#endif

