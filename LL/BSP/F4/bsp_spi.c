/**
  ******************************************************************************
  * @file    bsp_spi.c/h
  * @brief   Code for SPI driver in STM32 series MCU	 
	@verbatim  V1.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
			-# ʹ��bsp_spi_Init	����spi����
      -# ����SPI_ReadWrite_8Byte ��д8�ֽ�����
			-# ����SPI_ReadWrite_16Byte ��д16�ֽ�����
    @attention
      -# ��ģ���֧��LL��
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "bsp_spi.h"

/**
  * @brief  SPI��ʼ��
  */
void bsp_spi_Init(SPI_TypeDef* SPIx)
{
	LL_SPI_Enable(SPIx);
}

void SPI_Delay(uint16_t t)
{
	while(t--);
}

/**
	* @brief  SPI����(8byte)
  * @param  Ҫ���͵��ֽ�
  */
void SPI_Send_8Byte(SPI_TypeDef* SPIx, uint8_t TxData, uint16_t t)
{
	uint32_t timeout_cnt_num = 1000;
	uint32_t timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_TXE(SPIx)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			break;
		}
	}

	SPIx->DR = TxData;
	SPI_Delay(t);
}


/**
  * @brief  SPI��д(8byte)
  * @param  Ҫ���͵��ֽ�
  * @retval ��ȡ�����ֽ�
  */
uint8_t SPI_ReadWrite_8Byte(SPI_TypeDef* SPIx, uint8_t TxData)
{
	uint32_t timeout_cnt_num = 500;
	uint32_t timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_TXE(SPIx)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			break;
		}
	}

	SPIx->DR = TxData;

	timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_RXNE(SPIx)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			break;
		}
	}
	return SPIx->DR;

}

/**
  * @brief  SPI��д(16byte)
  * @param  Ҫ���͵��ֽ�
  * @retval ��ȡ�����ֽ�
  */
uint16_t SPI_ReadWrite_16Byte(SPI_TypeDef* SPIx, uint16_t TxData)
{
	uint32_t timeout_cnt_num = 500;
	uint32_t timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_TXE(SPIx)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			break;
		}
	}
	SPIx->DR = (uint16_t)TxData;

	timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_RXNE(SPIx)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			break;
		}
	}
	return LL_SPI_ReceiveData16(SPIx);;

}

/**
  * @brief  SPI�ٶ�����
	* @param  SPIx: SPIѡ��
	* @param  SPI_BaudRatePrescaler: SPI����
  */
void SPI_SetSpeed(SPI_TypeDef* SPIx, uint8_t Prescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(Prescaler));	//���Լ��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=Prescaler;	
	LL_SPI_Enable(SPIx);
} 

