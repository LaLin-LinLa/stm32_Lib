/**
  ******************************************************************************
  * @file    drv_24L01.c/h
  * @brief   Code for NRF24L01 driver in STM32 series MCU	
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
			-#����drv_24l01_Init��ʼ������ģ��
			-#ʹ��NRF24L01_Check���ģ���Ƿ�����
			-#ʹ��NRF24L01_TX_Mode���÷���ģʽ
			-#ʹ��NRF24L01_TxPacket����һ������
			-#ʹ��NRF24L01_RX_Mode���ý���ģʽ
			-#ʹ��NRF24L01_RxPacket����һ������
			
    @attention
			-# ��ģ����Ҫ����bsp_spi.c/h
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "drv_24l01.h"
#include "bsp_spi.h"

#define NRF_SPIx		SPI1
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���յ�ַ


/**
  * @brief  ��ʼ��nrf24l01
 **/	
void drv_24l01_Init(void)
{
	NRF_CE(0);
	NRF_CS(1);
}

/**
  * @brief  д�Ĵ���
	* @param	reg:	 �Ĵ���
	* @param	value: д���ֵ
 **/
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
  NRF_CS(0);
  status =SPI_ReadWrite_8Byte(NRF_SPIx, reg);//���ͼĴ����� 
  SPI_ReadWrite_8Byte(NRF_SPIx, value);      //д��Ĵ�����ֵ
	NRF_CS(1);  								//��ѡ��nrf
  return(status);       			//����״ֵ̬
}

/**
  * @brief  ���Ĵ���
	* @param	reg:	 �Ĵ���
 **/
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
	NRF_CS(0);		
  SPI_ReadWrite_8Byte(NRF_SPIx, reg);   //���ͼĴ�����
  reg_val=SPI_ReadWrite_8Byte(NRF_SPIx, 0XFF);//��ȡ�Ĵ�������
  NRF_CS(1);		    
  return(reg_val);           //����״ֵ̬
}	

/**
  * @brief  ��ָ��λ�ö���ָ�����ȵ�����
	* @param	reg:	�Ĵ���
	* @param	pBuf:	����ָ��
	* @param	len:	���ݳ���
	* @return �˴ζ�����״̬�Ĵ���ֵ
 **/
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
  uint8_t status,u8_ctr;	       
  NRF_CS(0);          
  status=SPI_ReadWrite_8Byte(NRF_SPIx, reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
  for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWrite_8Byte(NRF_SPIx, 0XFF);//��������
  NRF_CS(1);
  return status;        //���ض�����״ֵ̬
}

/**
  * @brief  ��ָ��λ��дָ�����ȵ�����
	* @param	reg:	�Ĵ���
	* @param	pBuf:	����ָ��
	* @param	len:	���ݳ���
	* @return �˴ζ�����״̬�Ĵ���ֵ
 **/
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
  uint8_t status,u8_ctr;	    
 	NRF_CS(0);          
  status = SPI_ReadWrite_8Byte(NRF_SPIx, reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWrite_8Byte(NRF_SPIx, *pBuf++); //д������	 
  NRF_CS(1);       
  return status;          //���ض�����״ֵ̬
}	


/**
  * @brief  ���24L01�Ƿ����
	* @return 0���ɹ�;1��ʧ��
 **/
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	SPI_SetSpeed(NRF_SPIx, LL_SPI_BAUDRATEPRESCALER_DIV64);//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	

/**
  * @brief  ����NRF24L01����һ������
	* @param	txbuf:	�����������׵�ַ
	* @return �������״��
 **/
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	SPI_SetSpeed(NRF_SPIx, LL_SPI_BAUDRATEPRESCALER_DIV64);//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ� 
	while(NRF_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

/**
  * @brief  ����NRF24L01����һ������
	* @param	rxbuf:	�������ݻ���
	* @return �������״��
 **/
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	SPI_SetSpeed(NRF_SPIx, LL_SPI_BAUDRATEPRESCALER_DIV64); //spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    
	
/**
  * @brief  �ú�����ʼ��NRF24L01��RXģʽ
	* @note		����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
	*					��CE��ߺ�,������RXģʽ,�����Խ���������
 **/
void NRF24L01_RX_Mode(void)
{
  NRF_CE(0); 
	//SPI_SetSpeed(NRF_SPIx, LL_SPI_BAUDRATEPRESCALER_DIV8);//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��  
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  NRF_CE(1); //CEΪ��,�������ģʽ 
}						 
 
/**
  * @brief  �ú�����ʼ��NRF24L01��TXģʽ
	* @note		����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
	*					PWR_UP,CRCʹ�ܣ���CE��ߺ�,������RXģʽ,�����Խ��������ˣ�CEΪ�ߴ���10us,���������͡�
 **/
void NRF24L01_TX_Mode(void)
{														 
	NRF_CE(0);	
	//SPI_SetSpeed(NRF_SPIx, LL_SPI_BAUDRATEPRESCALER_DIV8);//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��  
  NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_CE(1);//CEΪ��,10us����������
}
