#include "drv_motor.h"
#include "bsp_can.h"

extern CAN_HandleTypeDef 	hcan1;
extern CAN_HandleTypeDef 	hcan2;
static motor_measure_t	motor_driver[4] = {0};
static motor_measure_t	motor_steer[4] = {
			 {.calib = 5,},		//ѡȡ��n��(�ȶ�)����ֵ��ΪУ׼ֵ
			 {.calib = 5,},
			 {.calib = 5,},
			 {.calib = 5,},
};


/*��ȡrm�������*/
void get_motor_measure(motor_measure_t* motor, uint8_t* Data){
	
	motor->last_ecd		= motor->ecd;
	motor->ecd 				= (uint16_t)(Data[0]<<8|Data[1]);	//�Ƕ���Ϣ
	motor->speed_rpm	= (uint16_t)(Data[2]<<8|Data[3]);	//ת����Ϣ
	motor->torque			= (uint16_t)(Data[4]<<8|Data[5]);	//������Ϣ
	if((motor->ecd - motor->last_ecd) > 4096)
		motor->round_cnt--;
	else if((motor->ecd - motor->last_ecd) < -4096)
		motor->round_cnt++;
	if(motor->calib > 0)
	{
		if(motor->calib == 1)
		{
			motor->ecd_offset = motor->ecd;
			motor->round_cnt = 0;
			motor->total_ecd	=	0;
			motor->calib = 0;
		}
		else{
			motor->calib--;
		}
	}else if(!motor->calib)
	{
		motor->total_ecd	=	motor->round_cnt*8192 + motor->ecd - motor->ecd_offset;	//�ܻ�е�Ƕ�
	}
}

void Steer_motor_calib(uint8_t i, uint8_t num)
{
	motor_steer[i].calib = num;
}

/**
  * @brief          ת������Ϣ��ȡ����ѯ��ʽ��
  * @retval         none
  */
void Steer_motor_DatePrcess(void){
	while(!HAL_CAN_GetRxFifoFillLevel(&hcan2,CAN_FILTER_FIFO1)){}
		static CAN_RxBuffer CAN_RxBuffer;
		HAL_CAN_GetRxMessage(&hcan2,CAN_FILTER_FIFO1,&CAN_RxBuffer.header,CAN_RxBuffer.data);
		switch(CAN_RxBuffer.header.StdId)
		{
			case C610_1:
				get_motor_measure(&motor_steer[0],CAN_RxBuffer.data);
			break;
			case C610_2:
				get_motor_measure(&motor_steer[1],CAN_RxBuffer.data);
			break;
			case C610_3:
				get_motor_measure(&motor_steer[2],CAN_RxBuffer.data);
			break;
			case C610_4:
				get_motor_measure(&motor_steer[3],CAN_RxBuffer.data);
			break;
			default:
				break;
		}
}

/**
  * @brief          ���������Ϣ��ȡ����ѯ��ʽ��
  * @retval         none
  */
void Driver_motor_DatePrcess(void){
	while(!HAL_CAN_GetRxFifoFillLevel(&hcan1,CAN_FILTER_FIFO0)){}
	static CAN_RxBuffer CAN_RxBuffer;
	HAL_CAN_GetRxMessage(&hcan1,CAN_FILTER_FIFO0,&CAN_RxBuffer.header,CAN_RxBuffer.data);
	switch(CAN_RxBuffer.header.StdId)
	{
		case C620_1:
			get_motor_measure(&motor_driver[0],CAN_RxBuffer.data);
		break;
		case C620_2:
			get_motor_measure(&motor_driver[1],CAN_RxBuffer.data);
		break;
		case C620_3:
			get_motor_measure(&motor_driver[2],CAN_RxBuffer.data);
		break;
		case C620_4:
			get_motor_measure(&motor_driver[3],CAN_RxBuffer.data);
		break;
		default:
			break;
	}
}

/**
  * @brief          ת������Ϣ��ȡ���жϷ�ʽ��
  * @param		      ID: ��׼֡ID
	* @param		      Data: �����Ϣ
  * @retval         none
  */
void Steer_motor_DatePrcess_IT(uint32_t ID, uint8_t *Data){
	switch(ID)
	{
		case C610_1:
			get_motor_measure(&motor_steer[0],Data);
		break;
		case C610_2:
			get_motor_measure(&motor_steer[1],Data);
		break;
		case C610_3:
			get_motor_measure(&motor_steer[2],Data);
		break;
		case C610_4:
			get_motor_measure(&motor_steer[3],Data);
		break;
		default:
			break;
	}
}

/**
  * @brief          ���������Ϣ��ȡ���жϷ�ʽ��
  * @param		      ID: ��׼֡ID
	* @param		      Data: �����Ϣ
  * @retval         none
  */
void Driver_motor_DatePrcess_IT(uint32_t ID, uint8_t *Data){
	switch(ID)
	{
		case C620_1:
			get_motor_measure(&motor_driver[0],Data);
		break;
		case C620_2:
			get_motor_measure(&motor_driver[1],Data);
		break;
		case C620_3:
			get_motor_measure(&motor_driver[2],Data);
		break;
		case C620_4:
			get_motor_measure(&motor_driver[3],Data);
		break;
		default:
			break;
	}
}

/*==========��������������������==========*/
void CAN_CMD_DRIVER(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){
		static uint8_t Data[8] = {0};
		Data[0] = iq1 >> 8;
		Data[1] = iq1;
		Data[2] = iq2 >> 8;
		Data[3] = iq2;
		Data[4] = iq3 >> 8;
		Data[5] = iq3;
		Data[6] = iq4 >> 8;
		Data[7] = iq4;
	
		CANx_SendData(&hcan1, CAN_C620_ALL_ID, Data, 0x08, 3);
}

/*==========����ת�������������==========*/
void CAN_CMD_STEER(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){
		static uint8_t Data[8] = {0};
		Data[0] = iq1 >> 8;
		Data[1] = iq1;
		Data[2] = iq2 >> 8;
		Data[3] = iq2;
		Data[4] = iq3 >> 8;
		Data[5] = iq3;
		Data[6] = iq4 >> 8;
		Data[7] = iq4;
	
		CANx_SendData(&hcan2, CAN_C610_ALL_ID, Data, 0x08, 3);
}

//�����������������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Driver_Measure_Point(uint8_t i)
{
    return &motor_driver[(i & 0x03)];
}
//����ת����������ַ��ͨ��ָ�뷽ʽ��ȡԭʼ����
const motor_measure_t *get_Steer_Measure_Point(uint8_t i)
{
    return &motor_steer[(i & 0x03)];
}
