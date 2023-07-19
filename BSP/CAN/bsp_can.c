/**
  ******************************************************************************
  * @file    bsp_can.c/h
  * @brief   Code for CAN driver in STM32 series MCU, supported packaged:
	*          - STM32Cube_FW_F4_V1.27.1.
	*          - STM32Cube_FW_F4_V1.24.0.
  *          - STM32Cube_FW_F1_V1.8.0.
  *          - STM32Cube_FW_H7_V1.5.0.		 
	@verbatim  V1.2
	===================================================================      
									##### How to use this driver #####
	=================================================================== 
    @note
      -# 调用bsp_can_Init，初始化CAN,设置CAN接收处理函数的指针。
			-# 如需接收can中的消息，需调用CAN_Filter_Mask_Config配置滤波器（需注意配置格式）
      -# 使用CANx_SendData发送标准帧数据
      -# 使用CANx_SendExtData发送扩展帧数据
    @attention
      -# 本模块只能保存一条来自于同一个FIFO的消息(详细见下方HAL库FIFOx中断的实现),请注意及时读走消息。
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
#include "bsp_can.h"

static void (*pCAN1_RxCpltCallback)(CAN_RxBuffer *);
static void (*pCAN2_RxCpltCallback)(CAN_RxBuffer *);

/**
* @brief  CAN初始化
* @param  hcan: CANx created by CubeMX.
* @return None.
*/
uint8_t bsp_can_Init(CAN_HandleTypeDef* hcan, void (*pFunc)(CAN_RxBuffer*))
{
	assert_param(hcan != NULL);
	
  HAL_CAN_Start(hcan);
	__HAL_CAN_DISABLE_IT(hcan, CAN_IT_ERROR);
  __HAL_CAN_ENABLE_IT(hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
  __HAL_CAN_ENABLE_IT(hcan,CAN_IT_RX_FIFO1_MSG_PENDING);
  
	if(hcan->Instance == CAN1)
	{
		pCAN1_RxCpltCallback = pFunc;
		return SUCCESS;
	}
	else if(hcan->Instance == CAN2)
	{
		pCAN2_RxCpltCallback = pFunc;
		return SUCCESS;
	}
	else
		return ERROR;
}


/**
 * @brief 配置CAN的过滤器
 * @param object_para [|编号|FIFOx|ID类型|帧类型|ID|]
 * @param Id          [ID]
 * @param MaskId      [屏蔽位(0x3ff,0x1FFFFFFF)]
 * @brief             [筛选器:|编号|FIFOx|ID类型|帧类型|ID|屏蔽位(0x3ff,0x1FFFFFFF)|] \n
 *
 */
void CAN_Filter_Mask_Config(CAN_HandleTypeDef * hcan, uint8_t object_para,uint32_t Id,uint32_t MaskId)
{
	CAN_FilterTypeDef  CAN_FilterInitStructure;

	assert_param(hcan != NULL);

	if( (object_para&0x02))	//筛选器ID>=10
	{
    CAN_FilterInitStructure.FilterIdHigh         = Id<<3<<16;                       /* 掩码后ID的高16bit */
    CAN_FilterInitStructure.FilterIdLow          = Id<<3| ((object_para&0x03)<<1);  /* 掩码后ID的低16bit */
    CAN_FilterInitStructure.FilterMaskIdHigh     = MaskId<<3<<16;                   /* ID掩码值高16bit */
    CAN_FilterInitStructure.FilterMaskIdLow      = MaskId<<3| ((object_para&0x03)<<1);;   /* ID掩码值低16bit */
	}
	else
	{
    CAN_FilterInitStructure.FilterIdHigh         = Id<<5;                           /* 掩码后ID的高16bit */
    CAN_FilterInitStructure.FilterIdLow          = ((object_para&0x03)<<1);         /* 掩码后ID的低16bit */
    CAN_FilterInitStructure.FilterMaskIdHigh     = MaskId<<5;                       /* ID掩码值高16bit */
    CAN_FilterInitStructure.FilterMaskIdLow      = ((object_para&0x03)<<1);;        /* ID掩码值低16bit */
	}
    CAN_FilterInitStructure.FilterBank           = object_para>>3;                  /* 滤波器序号，0-13，共14个滤波器 */
    CAN_FilterInitStructure.FilterFIFOAssignment = (object_para>>2)&0x01;           /* 滤波器绑定FIFO 0 */
    CAN_FilterInitStructure.FilterActivation     = ENABLE;                          /* 使能滤波器 */
    CAN_FilterInitStructure.FilterMode         = CAN_FILTERMODE_IDMASK;             /* 滤波器模式，设置ID掩码模式 */
    CAN_FilterInitStructure.FilterScale        = CAN_FILTERSCALE_32BIT;             /* 32位滤波 */
    CAN_FilterInitStructure.SlaveStartFilterBank = 14;                              /* Select the start bank filter for slave CAN*/
    HAL_CAN_ConfigFilter(hcan, &CAN_FilterInitStructure);
}


/**
* @brief  CAn发送标准帧
* @param  hcan  :CAN bus used to send.
* @param  ID    :ID of frame.
* @param  *pData:Data to send.
* @param  Len   :Length of data.
* @return CAN_SUCCESS:  Operation success.
* @return CAN_LINE_BUSY:CAN line busy.
*/
uint8_t CANx_SendData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t Len)
{
	static CAN_TxHeaderTypeDef   Tx_Header;
  uint32_t  used_mailbox;
	/* Check the parameters */
	assert_param(hcan != NULL);
	
	Tx_Header.StdId=ID;
	Tx_Header.ExtId=0;
	Tx_Header.IDE=0;
	Tx_Header.RTR=0;
	Tx_Header.DLC=Len;
	
	if(HAL_CAN_AddTxMessage(hcan,&Tx_Header,pData,&used_mailbox)!= HAL_OK)
	{
			return CAN_LINE_BUSY;
	}
	else{}
		
	return CAN_SUCCESS;
}


/**
* @brief  CAN发送扩展帧
* @param  hcan  :CAN bus used to send.
* @param  ID    :ID of frame.
* @param  *pData:Data to send.
* @param  Len   :Length of data.
* @return CAN_SUCCESS:  Operation success.
* @return CAN_LINE_BUSY:CAN line busy.
*/
uint8_t CANx_SendExtData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t Len)
{
	static CAN_TxHeaderTypeDef   Tx_Header;
  uint32_t  used_mailbox;
	/* Check the parameters */
	assert_param(hcan != NULL);
	
	Tx_Header.RTR=0;
	Tx_Header.DLC=Len;
	Tx_Header.StdId=0;
	Tx_Header.ExtId=ID;
	Tx_Header.IDE=CAN_ID_EXT;

	if(HAL_CAN_AddTxMessage(hcan,&Tx_Header,pData,&used_mailbox)!= HAL_OK)
	{
			return CAN_LINE_BUSY;
	}
	else{}
		
	return CAN_SUCCESS;
}


/*HAL库FIFO0中断*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/*!< CAN receive buffer */
	static CAN_RxBuffer CAN_RxBuffer;

	if(hcan->Instance ==CAN1)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&CAN_RxBuffer.header,CAN_RxBuffer.data);
		pCAN1_RxCpltCallback(&CAN_RxBuffer);
	}
	else if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&CAN_RxBuffer.header,CAN_RxBuffer.data);
			pCAN2_RxCpltCallback(&CAN_RxBuffer);
	}
}
/*HAL库FIFO1中断*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	static CAN_RxBuffer CAN_RxBuffer;

	if(hcan->Instance ==CAN1)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&CAN_RxBuffer.header,CAN_RxBuffer.data);
		pCAN1_RxCpltCallback(&CAN_RxBuffer);
	}
	else if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO1,&CAN_RxBuffer.header,CAN_RxBuffer.data);
		pCAN2_RxCpltCallback(&CAN_RxBuffer);
	}
}
