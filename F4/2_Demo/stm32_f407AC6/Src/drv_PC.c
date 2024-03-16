/**
  ******************************************************************************
  * @file    drv_PC.c/h
  * @brief   ��λ�������շ�ͨ�ô������
  * @version 2.0
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# ��ģ����ڴ��ڿ����ж�DMA˫����������ݣ��û���ʵ��Ӧ��ʱӦ���ݸ���Э�飬
         ����Ӧ�жϻص�������������У����ٵ���PC_DatePrcess��ԭ�����ݽ��
      -# ����get_miniPC_point��ȡ����������
      -# ����PC_DateSendͨ������DMA���Ͱ�����
      
    @attention
      ��ģ����Ҫ����bsp_usart.c/h��Ĭ�ϴ���Ϊ����1
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "drv_PC.h"
#include "bsp_usart.h"

static PC_rx_Union_t	PC_rx_union = {0};
/**
  * @brief          ͨ��ָ���ȡ��λ����������
  * @retval         none
  */
const PC_rx_Pack_t *get_PC_point(void)
{
    return &PC_rx_union.rxpack;
}

/**
  * @brief          ��λ��Э�����
  * @param[in]      buf: ԭ������ָ��
  * @retval         none
  */
void PC_DatePrcess(volatile const uint8_t * buf)
{
	/*���ݽ�����û����ݸ���Э����*/
	PC_rx_union.PcDataArray[0] = buf[0];
	PC_rx_union.PcDataArray[1] = buf[1];
	PC_rx_union.PcDataArray[2] = buf[2];
	PC_rx_union.PcDataArray[3] = buf[3];
}
/**
	* @brief          ������λ��
	* @param[in]      miniPC_tx���������ݹ�����
  * @retval         none
  */
void PC_DateSend(PC_tx_Union_t *PC_tx)
{
  /*���ݷ��ͣ��û����ݸ���Э�鷢��*/
	//JustFloat VofaЭ��
	PC_tx->txpack.tail[0] = 0x00;
	PC_tx->txpack.tail[1] = 0x00;
	PC_tx->txpack.tail[2] = 0x80;
	PC_tx->txpack.tail[3] = 0x7f;
	
	Usart1_DMA_Transmit((uint32_t)&PC_tx->PcDataArray, sizeof(PC_tx_Pack_t));
}
