/**
  ******************************************************************************
  * @file    drv_W25Qx.c/h
  * @brief   Code for W25Qx driver in STM32 series MCU	
	*					 (����оƬ)W25Q16:
	*					 �洢�ռ䣺2M Byte (256 * 8192)
	*					 ҳ����		8192 (2^13)
	*					 ÿҳ��С��256 Byte (2^8)
	*					 Memory(2M Byte) 
	*					 -> Block*32(/64K Byte) 
	*					 	-> Sector*16(/4K Byte) 
	*					 		-> Page*16(/256 Byte)
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
			-# ����drv_W25Qx_Init��ʼ��W25Qx
			-# ʹ��W25QXX_Read��ȡ
			-# ʹ��W25QXX_Writeд��
    @attention
			-# ��д���ɳ���оƬ�����ڴ��С
			-# ��ģ����Ҫ����bsp_spi.c/h
			-# ��ģ����Ҫ����bsp_timer.c/h
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "drv_W25Qx.h"
#include "bsp_spi.h"
#include "bsp_timer.h"

#define W25Qx_Debug		0
#define W25Qx_SPIx		SPI1														//SPIѡ��
#define SPIx_speed		LL_SPI_BAUDRATEPRESCALER_DIV2		//SPI����
uint16_t W25QXX_TYPE=W25Q16;	//Ĭ����W25Q16


/**
  * @brief  ��ʼ��W25Qx
 **/
void drv_W25Qx_Init(void)
{
	uint8_t temp;
	W25QXX_TYPE=W25QXX_ReadID();	        //��ȡFLASH ID.
	if(W25QXX_TYPE == NM25Q256)
	{
		temp = W25QXX_ReadSR(3);				//��ȡ״̬�Ĵ���3���жϵ�ַģʽ
		if((temp&0X01)==0)			        //�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
		{
			W25Q_CE(0); 			        //ѡ��
			SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_Enable4ByteAddr);//���ͽ���4�ֽڵ�ַģʽָ��   
			W25Q_CE(1);       		        //ȡ��Ƭѡ   
		}
	}
	W25Q_CE(1);
}

/**
  * @brief  ��ȡоƬID
	* @return 0XEF13:	оƬ�ͺ�ΪW25Q80
	*					0XEF14: оƬ�ͺ�ΪW25Q16
	*					0XEF15: оƬ�ͺ�ΪW25Q32
	*					0XEF16: оƬ�ͺ�ΪW25Q64
	*					0XEF17: оƬ�ͺ�ΪW25Q128
	*					0XEF18: оƬ�ͺ�ΪW25Q256
 **/
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25Q_CE(0);	
	SPI_SetSpeed(W25Qx_SPIx, SPIx_speed);
	SPI_ReadWrite_8Byte(W25Qx_SPIx, 0x90);//���Ͷ�ȡID����	    
	SPI_ReadWrite_8Byte(W25Qx_SPIx, 0x00);
	SPI_ReadWrite_8Byte(W25Qx_SPIx, 0x00); 
	SPI_ReadWrite_8Byte(W25Qx_SPIx, 0x00);		   
	Temp|=SPI_ReadWrite_16Byte(W25Qx_SPIx, 0xFF)<<8;
	Temp|=SPI_ReadWrite_16Byte(W25Qx_SPIx, 0xFF);
	W25Q_CE(1);			    
	return Temp;
}  

/**
  * @brief  ��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
	* @param  regno:״̬�Ĵ����ţ���:1~3
	* @note		------------------------------------------
	*					״̬�Ĵ���1��
	*					BIT |7  |6  |5  |4   |3   |2   |1   |0   |
	*							|SPR|RV	|TB	|BP2 |BP1 |BP0 |WEL |BUSY|
	*							SPR:	Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
	*							TB,		BP2,BP1,BP0:FLASH����д��������
	*							WEL:	дʹ������
	*							BUSY:	æ���λ(1,æ;0,����) Ĭ��:0x00
	*					------------------------------------------
	*					״̬�Ĵ���2��
	*					BIT |7  |6  |5  |4   |3   |2   |1  |0   |
	*							|SUS|CMP|LB3|LB2 |LB1 |(R) |QE |SRP1|
	*					------------------------------------------
	*					״̬�Ĵ���3��
	*					BIT |7   		 |6   |5   | 4 | 3 | 2 | 1 |0  |
	*							|HOLD/RST|DRV1|DRV0|(R)|(R)|WPS|ADP|ADS|
	*					------------------------------------------		
	* @return ״̬�Ĵ���ֵ
 **/
uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
	switch(regno)
	{
			case 1:
					command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
					break;
			case 2:
					command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
					break;
			case 3:
					command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
					break;
			default:
					command=W25X_ReadStatusReg1;    
					break;
	}    
	W25Q_CE(0);	     
	SPI_SetSpeed(W25Qx_SPIx, SPIx_speed);		  
	SPI_ReadWrite_8Byte(W25Qx_SPIx, command);            				//���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI_ReadWrite_8Byte(W25Qx_SPIx, 0Xff);          				//��ȡһ���ֽ�  
	W25Q_CE(1);	                            										//ȡ��Ƭѡ     
	return byte;   
} 

/**
	*	@brief дW25QXX״̬�Ĵ���
 **/
void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
    uint8_t command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }    
	W25Q_CE(0);
	SPI_SetSpeed(W25Qx_SPIx, SPIx_speed);
	SPI_ReadWrite_8Byte(W25Qx_SPIx, command);            //����дȡ״̬�Ĵ�������    
	SPI_ReadWrite_8Byte(W25Qx_SPIx, sr);                 //д��һ���ֽ�  
	W25Q_CE(1);    	      
}   

/**
	*	@brief W25QXXдʹ��
 **/
void W25QXX_Write_Enable(void)   
{
	W25Q_CE(0);      
	SPI_SetSpeed(W25Qx_SPIx, SPIx_speed);	
  SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_WriteEnable);   //����дʹ��  
	W25Q_CE(1);     	      
} 
 
/**
	*	@brief W25QXXд��ֹ
 **/
void W25QXX_Write_Disable(void)   
{  
	W25Q_CE(0);                             
	SPI_SetSpeed(W25Qx_SPIx, SPIx_speed);
  SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_WriteDisable);  //����д��ָֹ��    
	W25Q_CE(1);                            //ȡ��Ƭѡ     	      
} 

/**
	*	@brief ��ȡSPI FLASH
	* @param pBuffer ���ݴ洢��
	* @param ReadAddr ��ʼ��ȡ�ĵ�ַ
	* @param NumByteToRead Ҫ��ȡ���ֽ���(���65535)
 **/
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	W25Q_CE(0);  
	SPI_SetSpeed(W25Qx_SPIx, SPIx_speed);
    SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_ReadData);      //���Ͷ�ȡ����  
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((ReadAddr)>>24));    
    }
    SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((ReadAddr)>>16));
    SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((ReadAddr)>>8));   
    SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI_ReadWrite_8Byte(W25Qx_SPIx, 0XFF);    //ѭ������  
    }
	W25Q_CE(1);   				    	      
}  

/**
	*	@brief ��һҳ(0~65535)��д������256���ֽڵ�����
	* @param pBuffer ���ݴ洢��
	* @param ReadAddr ��ʼ��ȡ�ĵ�ַ
	* @param NumByteToRead Ҫ��ȡ���ֽ���(���256)
 **/
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  W25QXX_Write_Enable();                  //SET WEL 
	W25Q_CE(0);                            //ʹ������   
	SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_PageProgram);   //����дҳ����   
	if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
	{
		SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((WriteAddr)>>24)); 
	}
	SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
	SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((WriteAddr)>>8));   
	SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)SPI_ReadWrite_8Byte(W25Qx_SPIx, pBuffer[i]);//ѭ��д��  
	W25Q_CE(1);                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
} 

/**
	*	@brief �޼���дSPI FLASH 
	* @param pBuffer ���ݴ洢��
	* @param WriteAddr:��ʼд��ĵ�ַ(24bit)
	* @param NumByteToWrite:Ҫд����ֽ���(���65535)
 **/
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 

/**
	*	@brief д��SPI FLASH
	*				 (�ú�������������!)
	* @param pBuffer ���ݴ洢��
	* @param WriteAddr:��ʼд��ĵ�ַ(24bit)
	* @param NumByteToWrite:Ҫд����ֽ���(���65535)
 **/
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
	#if W25Qx_Debug
		printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
	#endif
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}

/**
	*	@brief ����һ������
	* @param Dst_Addr ������ַ ����ʵ����������
	* @note	 ����һ������������ʱ��:150ms
 **/
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	#if W25Qx_Debug
 	printf("fe:%x\r\n",Dst_Addr);	//����falsh�������,������
	#endif
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25Q_CE(0);                             //ʹ������   
    SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_SectorErase);   //������������ָ�� 
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((Dst_Addr)>>24)); 
    }
    SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)((Dst_Addr)>>8));   
    SPI_ReadWrite_8Byte(W25Qx_SPIx, (uint8_t)Dst_Addr);  
		W25Q_CE(1); 														//ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				    //�ȴ��������
}  

/**
	*	@brief ��������оƬ
	* @note	 too fuck long...
 **/
void W25QXX_Erase_Chip(void)   
{                                   
	W25QXX_Write_Enable();                 //SET WEL 
	W25QXX_Wait_Busy();   
	W25Q_CE(0);                            //ʹ������   
	SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_ChipErase);    //����Ƭ��������  
	W25Q_CE(1);                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   

//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
} 
//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  W25Q_CE(0);                            //ʹ������   
  SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_PowerDown);  //���͵�������  
	W25Q_CE(1);                            //ȡ��Ƭѡ     	      
  delay_us_nos(3);                       //�ȴ�TPD  
}   
//����
void W25QXX_WAKEUP(void)   
{  
  W25Q_CE(0);                                //ʹ������   
  SPI_ReadWrite_8Byte(W25Qx_SPIx, W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
  W25Q_CE(1);                                //ȡ��Ƭѡ     	      
  delay_us_nos(3);                           //�ȴ�TRES1
}   

