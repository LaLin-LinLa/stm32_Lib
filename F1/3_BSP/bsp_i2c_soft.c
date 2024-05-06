/**
  ******************************************************************************
  * @file    bsp_i2c_soft.c/h
  * @brief   Code for softIIC driver in STM32 series MCU	 
	  @verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# 调用bsp_I2C_Init初始化I2C，注册相关引脚
			-# 调用IIC_Device_Write_Byte写入1字节数据
      -# 调用IIC_Device_Read_Byte读取1字节数据
      -# 调用IIC_Device_Write_Len写入多字节数据
      -# 调用IIC_Device_Read_Len读取多字节数据
    @attention
      -# 本模块仅支持HAL库
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "bsp_i2c_soft.h"

IIC_PIN_Typedef DRV_I2C1;
IIC_PIN_Typedef DRV_I2C2;
IIC_PIN_Typedef DRV_I2C3;

/* SDA输入配置 */
void xSDA_IN(IIC_PIN_Typedef *iic_pin)              		    
{	
	if(iic_pin->IIC_SDA_PIN_NUM <= 7)
	{iic_pin->IIC_SDA_GPIO_PORT->CRL &= ~((0x0f<<4*iic_pin->IIC_SDA_PIN_NUM)&0xffffffff);iic_pin->IIC_SDA_GPIO_PORT->CRL |= (uint32_t)8<<(iic_pin->IIC_SDA_PIN_NUM*4);}
	else
	{iic_pin->IIC_SDA_GPIO_PORT->CRH &= ~((0x0f<<4*(iic_pin->IIC_SDA_PIN_NUM-8))&0xffffffff);iic_pin->IIC_SDA_GPIO_PORT->CRH |= (uint32_t)8<<((iic_pin->IIC_SDA_PIN_NUM-8)*4);}
}

/* SDA输出配置 */
void xSDA_OUT(IIC_PIN_Typedef *iic_pin)             		    
{
	if(iic_pin->IIC_SDA_PIN_NUM <= 7)
	{iic_pin->IIC_SDA_GPIO_PORT->CRL &= ~((0x0f<<4*iic_pin->IIC_SDA_PIN_NUM)&0xffffffff);iic_pin->IIC_SDA_GPIO_PORT->CRL |= (uint32_t)3<<(iic_pin->IIC_SDA_PIN_NUM*4);}
	else
	{iic_pin->IIC_SDA_GPIO_PORT->CRH &= ~((0x0f<<4*(iic_pin->IIC_SDA_PIN_NUM-8))&0xffffffff);iic_pin->IIC_SDA_GPIO_PORT->CRH |= (uint32_t)3<<((iic_pin->IIC_SDA_PIN_NUM-8)*4);}
}

/* SCL输出电平 */
void xIIC_SCL(IIC_PIN_Typedef *iic_pin,const char x)        
{
	(x!=0)?HAL_GPIO_WritePin(iic_pin->IIC_SCL_GPIO_PORT,iic_pin->IIC_SCL_PIN,GPIO_PIN_SET)\
				:HAL_GPIO_WritePin(iic_pin->IIC_SCL_GPIO_PORT,iic_pin->IIC_SCL_PIN,GPIO_PIN_RESET);
}

/* SDA输出电平 */
void xIIC_SDA(IIC_PIN_Typedef *iic_pin,const char x)        
{
  (x!=0)?HAL_GPIO_WritePin(iic_pin->IIC_SDA_GPIO_PORT,iic_pin->IIC_SDA_PIN,GPIO_PIN_SET)\
				:HAL_GPIO_WritePin(iic_pin->IIC_SDA_GPIO_PORT,iic_pin->IIC_SDA_PIN,GPIO_PIN_RESET);	
}

/* SDA读 */
GPIO_PinState xREAD_SDA(IIC_PIN_Typedef *iic_pin)
{
    GPIO_PinState bitstatus;
    if((iic_pin->IIC_SDA_GPIO_PORT->IDR & iic_pin->IIC_SDA_PIN) != (uint32_t)GPIO_PIN_RESET)
        bitstatus = GPIO_PIN_SET;
    else
        bitstatus = GPIO_PIN_RESET;
    return bitstatus;	
}

/**
  * @brief  iic delay
  * @param  m: time for delay, 1 represent nearly 1us
  * @retval void
  */
void IIC_Delay(uint8_t m)
{
  uint8_t i = 0;    
  while(m--)
  {
    i = IIC_Delay_Time;
    while(i--);
  }
}

/**
  * @brief  iic initialization
  * @retval void
  */
void bsp_I2C_soft_Init(IIC_PIN_Typedef *iic_pin, GPIO_TypeDef *gpio_scl, GPIO_TypeDef *gpio_sda, uint32_t scl_pinx, uint32_t sda_pinx, uint32_t scl_pin_num, uint32_t sda_pin_num)
{ 
	iic_pin->IIC_SCL_GPIO_PORT = gpio_scl;
	iic_pin->IIC_SDA_GPIO_PORT = gpio_sda;
	iic_pin->IIC_SCL_PIN = scl_pinx;
	iic_pin->IIC_SDA_PIN = sda_pinx;
	iic_pin->IIC_SCL_PIN_NUM = scl_pin_num;
	iic_pin->IIC_SDA_PIN_NUM = sda_pin_num;
	
	/* SCL High */
	xIIC_SCL(iic_pin,1);

	/* SDA High */  
	xIIC_SDA(iic_pin,1);
}


/**
  * @brief  iic start signal
  * @param  hiic: iic handler
  * @retval void
  */
void IIC_Start(IIC_PIN_Typedef *iic_pin)
{  
	/* SDA Output */
	xSDA_OUT(iic_pin);   
	xIIC_SDA(iic_pin, 1);  
	xIIC_SCL(iic_pin, 1);
	IIC_Delay(1);
	
	/* START: when CLK is high,DATA change form high to low */
	xIIC_SDA(iic_pin, 0);  
	IIC_Delay(1);
	
	/* Clamp I2C bus, prepare to send or receive data */
	xIIC_SCL(iic_pin, 0);  
}

/**
  * @brief  iic stop signal
  * @param  hiic: iic handler
  * @retval void
  */
void IIC_Stop(IIC_PIN_Typedef *iic_pin)
{ 
	/* SDA Output */
	xSDA_OUT(iic_pin);   
	xIIC_SCL(iic_pin, 0);  
	
	/* STOP: when CLK is high DATA change form low to high */
	xIIC_SDA(iic_pin, 0);  
	IIC_Delay(1);
	xIIC_SCL(iic_pin, 1);   
	IIC_Delay(1);
	
	/* Send I2C bus stop signal */
	xIIC_SDA(iic_pin, 1);  
}

/**
  * @brief  iic wait ack signal
  * @param  hiic: iic handler
  * @retval 1,fail
  *         0,success
  */
unsigned char IIC_Wait_Ack(IIC_PIN_Typedef *iic_pin)
{
	unsigned char ucErrTime=0;
	
  /* SDA Input */
  xSDA_IN(iic_pin);   
  xIIC_SDA(iic_pin, 1);

  IIC_Delay(1);
  xIIC_SCL(iic_pin, 1);
  IIC_Delay(1);

  while(xREAD_SDA(iic_pin))
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      IIC_Stop(iic_pin);
      return 1;
    }
  }
  xIIC_SCL(iic_pin, 0);
  return 0;
}

/**
  * @brief  iic produce ack signal
  * @param  hiic: iic handler
  * @retval void
  */
void IIC_Ack(IIC_PIN_Typedef *iic_pin)
{
	xIIC_SCL(iic_pin, 0); 
	xSDA_OUT(iic_pin);   
	xIIC_SDA(iic_pin, 0);  
	IIC_Delay(1);
	xIIC_SCL(iic_pin, 1);  
	IIC_Delay(1);
	xIIC_SCL(iic_pin, 0);  
}

/**
  * @brief  iic produce no ack signal
  * @param  hiic: iic handler
  * @retval void
  */
void IIC_NAck(IIC_PIN_Typedef *iic_pin)
{  
	xIIC_SCL(iic_pin, 0); 
	xSDA_OUT(iic_pin);   
	xIIC_SDA(iic_pin, 1);  
	IIC_Delay(1);
	xIIC_SCL(iic_pin, 1);  
	IIC_Delay(1);
	xIIC_SCL(iic_pin, 0);  
}

/**
  * @brief  iic send a byte
  * @param  hiic: iic handler
  * @param  txd: the byte to be sent 
  * @retval void
  */
void IIC_Send_Byte(IIC_PIN_Typedef *iic_pin,unsigned char txd)
{
  unsigned char t;
	xSDA_OUT(iic_pin);  
	xIIC_SCL(iic_pin, 0);
	for(t=0;t<8;t++)
	{
		xIIC_SDA(iic_pin, (txd&0x80)>>7);
		txd<<=1;
		
		IIC_Delay(1);   
		xIIC_SCL(iic_pin, 1);
		IIC_Delay(1);
		xIIC_SCL(iic_pin, 0);
		IIC_Delay(1);
	}
}

/**
  * @brief  iic read a byte
  * @param  hiic: iic handler
  * @param  ack: 0,send no ack signal
  *              1,send ack signal
  * @retval void
  */
unsigned char IIC_Read_Byte(IIC_PIN_Typedef *iic_pin,unsigned char ack)
{
	unsigned char i,receive=0;
    
	/* SDA Input */
	xSDA_IN(iic_pin);
	
	for(i=0;i<8;i++ )
	{
		xIIC_SCL(iic_pin, 0);
		IIC_Delay(1);
		xIIC_SCL(iic_pin, 1);
		receive<<=1;
		if(xREAD_SDA(iic_pin))
			receive++;
		IIC_Delay(1);
	}
	if(!ack)
	{
		IIC_NAck(iic_pin);
	}
	else 
	{
		IIC_Ack(iic_pin);
	}
	return receive;	
}

/**
  * @brief  iic write a byte to the register of the device
  * @param  hiic: iic handler
  * @param  addr: the address of the device
  * @param  reg: the address of the register(offset address)
  * @param  data: the byte to be sent 
  * @retval 0,success
  *         1,fail
  */
unsigned char IIC_Device_Write_Byte(IIC_PIN_Typedef *iic_pin, unsigned char addr, unsigned char reg, unsigned char data)
{
  IIC_Start(iic_pin);
	
	IIC_Send_Byte(iic_pin, (addr<<1)|0);
	if(IIC_Wait_Ack(iic_pin))	    
	{
		IIC_Stop(iic_pin);
		return 1;
	}
  IIC_Send_Byte(iic_pin,reg);	
  IIC_Wait_Ack(iic_pin);		
	    
	IIC_Send_Byte(iic_pin,data);   
	if(IIC_Wait_Ack(iic_pin))	    
	{
		IIC_Stop(iic_pin);
		return 1;
	}
    IIC_Stop(iic_pin);
	return 0;
}

/**
  * @brief  iic read a byte of the register of the device
  * @param  hiic: iic handler
  * @param  addr: the address of the device
  * @param  reg: the address of the register(offset address)
  * @retval the data read        
  */
unsigned char IIC_Device_Read_Byte(IIC_PIN_Typedef *iic_pin,unsigned char addr,unsigned char reg)
{
	unsigned char res;
  IIC_Start(iic_pin);
	
	IIC_Send_Byte(iic_pin,(addr<<1)|0);
	IIC_Wait_Ack(iic_pin);		
  
  IIC_Send_Byte(iic_pin,reg);	
  IIC_Wait_Ack(iic_pin);		
  IIC_Start(iic_pin);
	
	IIC_Send_Byte(iic_pin,(addr<<1)|1);
   IIC_Wait_Ack(iic_pin);		
  
	res = IIC_Read_Byte(iic_pin,0);

  IIC_Stop(iic_pin);			
	return res;
}

/**
  * @brief  iic continuous write to the register of the device 
  * @param  hiic: iic handler
  * @param  addr: the address of the device
  * @param  reg: the address of the register(offset address)
  * @param  len: write length
  * @param  buf: data buffer
  * @retval 0,success
  *         1,fail
  */
unsigned char IIC_Device_Write_Len(IIC_PIN_Typedef *iic_pin, unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
	unsigned char i;
    IIC_Start(iic_pin);
	
	/* send the address of the device and the write command */
	IIC_Send_Byte(iic_pin,(addr<<1)|0);
	if(IIC_Wait_Ack(iic_pin))	
	{
		IIC_Stop(iic_pin);
		return 1;
	}
	
	/* send the address of the register */
    IIC_Send_Byte(iic_pin,reg);	
    IIC_Wait_Ack(iic_pin);		
	for(i=0;i<len;i++)
	{
		/* send the data to the register */
		IIC_Send_Byte(iic_pin,buf[i]);	
		if(IIC_Wait_Ack(iic_pin))		
		{
			IIC_Stop(iic_pin);
			return 1;
		}
	}
    IIC_Stop(iic_pin);
	return 0;
}

/**
  * @brief  iic continuous read of the register of the device
  * @param  hiic: iic handler
  * @param  addr: the address of the device
  * @param  reg: the address of the register(offset address)
  * @param  len: read length
  * @param  buf: data buffer
  * @retval 0,success
  *         1,fail
  */
unsigned char IIC_Device_Read_Len(IIC_PIN_Typedef *iic_pin, unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
 	IIC_Start(iic_pin);
	
	/* send the address of the device and the write command */
	IIC_Send_Byte(iic_pin,(addr<<1)|0);
	if(IIC_Wait_Ack(iic_pin))	
	{
		IIC_Stop(iic_pin);
		return 1;
	}
	
	/* send the address of the register */
  IIC_Send_Byte(iic_pin,reg);	
  IIC_Wait_Ack(iic_pin);		
  IIC_Start(iic_pin);
	
	/* send the address of the device and the read command */
	IIC_Send_Byte(iic_pin,(addr<<1)|1);
  IIC_Wait_Ack(iic_pin);		
	while(len)
	{
		/* end of the reading, send no ack signal */
		if(len==1)*buf=IIC_Read_Byte(iic_pin,0);
		else *buf=IIC_Read_Byte(iic_pin,1);		
		len--;
		buf++;
	}
	/* produce a stop signal */
  IIC_Stop(iic_pin);	
	return 0;
}

