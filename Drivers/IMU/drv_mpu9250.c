#include "drv_mpu9250.h"
#include "bsp_timer.h"
#include "bsp_spi.h"

#define PWM_TIMx								TIM3
#define DELAY_TIM								10
#define MPU_delay_ms(n)         delay_ms_nos(n)

/**
  * @brief  ������
  * @param  �����Ǳ��
  *         1��������1
  *         2��������2
  * @param  �Ĵ�����ַ
  * @param  ����������
  * @param  ��ȡ������
  * @retval void
  */
void MPU9250_Read_Len(int cs_n, uint8_t reg,int len, unsigned char *buf)
{
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_RESET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_RESET);
	
	SPI_ReadWrite_8Byte(SPI1, reg | 0x80);
	while(len--) {
		*buf++ = SPI_ReadWrite_8Byte(SPI1, 0xFF);
		reg++;
	}
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_SET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  ����д
  * @param  �����Ǳ��
  *         1��������1
  *         2��������2
  * @param  �Ĵ�����ַ
  * @param  ����д����
  * @param  д�������
  * @retval void
  */
void MPU9250_Write_Len(int cs_n, unsigned char reg,int len,unsigned char *buf)
{
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_RESET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_RESET);
	
	SPI_ReadWrite_8Byte(SPI1, reg);
	for(char i = 0; i < len; i++, reg++) {
		SPI_ReadWrite_8Byte(SPI1, buf[i]);
	}
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_SET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  ��ȡ1�ֽ�
  * @param  �����Ǳ��
  *         1��������1
  *         2��������2
  * @param  �Ĵ�����ַ
  * @retval ��ȡ������
  */
uint8_t MPU9250_Read_Byte(int cs_n, unsigned char reg)
{
	uint8_t data;
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_RESET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_RESET);
	
	SPI_ReadWrite_8Byte(SPI1, reg | 0x80);
	data = SPI_ReadWrite_8Byte(SPI1, 0xFF);
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_SET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_SET);
	
	return data;
}

/**
  * @brief  д��1�ֽ�
  * @param  �����Ǳ��
  *         1��������1
  *         2��������2
  * @param  �Ĵ�����ַ
  * @param  д�������
  * @retval void
  */
void MPU9250_Write_Byte(int cs_n, unsigned char reg,unsigned char data)
{
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_RESET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_RESET);
	
	SPI_ReadWrite_8Byte(SPI1, reg);
	SPI_ReadWrite_8Byte(SPI1, data);
	
	if(cs_n == 1)
		HAL_GPIO_WritePin(MPU1_CS_GPIO_Port, MPU1_CS_Pin, GPIO_PIN_SET);
	else if (cs_n == 2)
		HAL_GPIO_WritePin(MPU2_CS_GPIO_Port, MPU2_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  ��ȡ�������¶�
  * @param  ��ȡ�����¶�
  * @retval void
  */
void MPU9250_Get_Temp(float *temp) {
	unsigned char buf[2];
	short raw1, raw2;
	
	MPU9250_Read_Len(1, MPU_TEMP_OUTH_REG, 2, buf);
	raw1 = ((short)(buf[0])<<8)|buf[1];
	MPU9250_Read_Len(2, MPU_TEMP_OUTH_REG, 2, buf);
	raw2 = ((short)(buf[0])<<8)|buf[1];
	*temp = (((double)raw1 / 333.87 + 21) + ((double)raw2 / 333.87 + 21)) / 2;
}

/**
  * @brief  ��ȡ�����Ǽ��ٶ�
  * @param  ��ȡ����x����ٶ�
  * @param  ��ȡ����y����ٶ�
  * @param  ��ȡ����z����ٶ�
  * @retval void
  */
void MPU9250_Get_Accel(short *ax, short *ay, short *az) {
	unsigned char buf[6];
	short raw1[3], raw2[3];
	
	MPU9250_Read_Len(1, MPU_ACCEL_XOUTH_REG, 6, buf);
	raw1[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw1[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw1[2] = ((unsigned short)buf[4]<<8)|buf[5];
	MPU9250_Read_Len(2, MPU_ACCEL_XOUTH_REG, 6, buf);
	raw2[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw2[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw2[2] = ((unsigned short)buf[4]<<8)|buf[5];
	
	*ax = (raw1[0] + raw2[1]) / 2;
	*ay = (raw1[1] - raw2[0]) / 2;
	*az = (raw1[2] + raw2[2]) / 2;
}

/**
  * @brief  ��ȡ��λת����������Ǽ��ٶȣ���λΪg
  * @param  ��ȡ����x����ٶ�
  * @param  ��ȡ����y����ٶ�
  * @param  ��ȡ����z����ٶ�
  * @retval void
  */
void MPU9250_Get_Accel_transform(float *ax, float *ay, float *az) {
	short rax, ray, raz;
	
	MPU9250_Get_Accel(&rax, &ray, &raz);
	//2g����
//	*ax = (float)rax / 16384;
//	*ay = (float)ray / 16384;
//	*az = (float)raz / 16384;
	//8g����
	*ax = (float)rax / 4096;
	*ay = (float)ray / 4096;
	*az = (float)raz / 4096;
}

/**
  * @brief  ��ȡ�����ǽǼ��ٶ�
  * @param  ��ȡ����x��Ǽ��ٶ�
  * @param  ��ȡ����y��Ǽ��ٶ�
  * @param  ��ȡ����z��Ǽ��ٶ�
  * @retval void
  */
void MPU9250_Get_Gyro(short *gx, short *gy, short *gz) {
	unsigned char buf[6];
	short raw1[3], raw2[3];
	
	MPU9250_Read_Len(1, MPU_GYRO_XOUTH_REG, 6, buf);
	raw1[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw1[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw1[2] = ((unsigned short)buf[4]<<8)|buf[5];
	MPU9250_Read_Len(2, MPU_GYRO_XOUTH_REG, 6, buf);
	raw2[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw2[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw2[2] = ((unsigned short)buf[4]<<8)|buf[5];
	
	*gx = (raw1[0] + raw2[1]) / 2;
	*gy = (raw1[1] - raw2[0]) / 2;
	*gz = (raw1[2] + raw2[2]) / 2;
}

/**
  * @brief  ��ȡ��λת����������ǽǼ��ٶȣ���λΪ��/��
  * @param  ��ȡ����x��Ǽ��ٶ�
  * @param  ��ȡ����y��Ǽ��ٶ�
  * @param  ��ȡ����z��Ǽ��ٶ�
  * @retval void
  */
void MPU9250_Get_Gyro_transform(float *gx, float *gy, float *gz) {
	short rgx, rgy, rgz;
	
	MPU9250_Get_Gyro(&rgx, &rgy, &rgz);
	//2000dps����
	*gx = ((float)rgx * 0.060976f);
	*gy = ((float)rgy * 0.060976f);
	*gz = ((float)rgz * 0.060976f);
}

/**
  * @brief  ��ȡ����������
  * @param  ��ȡ����x�����������
  * @param  ��ȡ����y�����������
  * @param  ��ȡ����z�����������
  * @retval void
  */
void MPU9250_Get_Mag(short *mx, short *my, short *mz) {
	
}

/**
  * @brief  ���ü���PWMռ�ձ�
  * @param  PWMռ�ձȣ���ΧΪ0~1000
  * @retval void
  */
void MPU9250_Set_Heat_PWM(int duty) {
	//ע���޷�������PID��������򳬶�ʱ������ֵ
	if(duty > 1000)
		duty = 1000;
	if(duty < 0)
		duty = 0;
	PWM_TIMx->CCR2 = duty;
}

/**
  * @brief  ������1�Ĵ�����ʼ��
  * @retval 1����ʼ��ʧ��
  *         0����ʼ���ɹ�
  */
int MPU9250_1_Init(void) {
	unsigned char get_ID = 0;
	
	get_ID = MPU9250_Read_Byte(1, MPU_DEVICE_ID_REG);
	MPU_delay_ms(DELAY_TIM);
	if(get_ID==0x70)
	{
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x80);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x00);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_SIGPATH_RST_REG,  0x07);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x03);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_CFG_REG,          0x00);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_SAMPLE_RATE_REG,  0x00);  //1K������
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_CFG_REG,          0x00);  //�ڲ���ͨ�˲�Ƶ��
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_GYRO_CFG_REG,     3<<3);  //���������̣�0~3��Ӧ250��500��1k��2k��dps��
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_ACCEL_CFG_REG,    2<<3);  //���ٶ����̣�0~3��Ӧ2g��4g��8g��16g
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_INT_EN_REG,       0x01);  //�ж�
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_INTBP_CFG_REG,    0x01);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, 0x27, 0x0D);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, 0x67, 0x0D);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, 0x27, 0x0D);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x01);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(DELAY_TIM);
		return 0;
	}
	else
		return 1;
}

/**
  * @brief  ������2�Ĵ�����ʼ��
  * @retval 1����ʼ��ʧ��
  *         0����ʼ���ɹ�
  */
int MPU9250_2_Init(void) {
	unsigned char get_ID = 0;
	
	get_ID = MPU9250_Read_Byte(2, MPU_DEVICE_ID_REG);
	MPU_delay_ms(DELAY_TIM);
	if(get_ID==0x70)
	{
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x80);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x00);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_SIGPATH_RST_REG,  0x07);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x03);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_CFG_REG,          0x00);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_SAMPLE_RATE_REG,  0x00);  //1K������
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_CFG_REG,          0x00);  //�ڲ���ͨ�˲�Ƶ��
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_GYRO_CFG_REG,     3<<3);  //���������̣�0~3��Ӧ250��500��1k��2k��dps��
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_ACCEL_CFG_REG,    2<<3);  //���ٶ����̣�0~3��Ӧ2g��4g��8g��16g
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_INT_EN_REG,       0x01);  //�ж�
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_INTBP_CFG_REG,    0x01);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, 0x27, 0x0D);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, 0x26, 0x0D);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, 0x27, 0x0D);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x01);
		MPU_delay_ms(DELAY_TIM);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(DELAY_TIM);
		return 0;
	}
	else
		return 1;
}
/**
  * @brief  �����ǳ�ʼ��
  * @retval 1����ʼ��ʧ��
  *         0����ʼ���ɹ�
  * @mark   ���������ǵĳ�ʼ������Ӧ��һ��
  */
int MPU9250_Init(void) {
		if(MPU9250_1_Init())
			return 1;
		if(MPU9250_2_Init())
			return 1;
	LL_TIM_EnableAllOutputs(PWM_TIMx);
	LL_TIM_EnableCounter(PWM_TIMx);
	LL_TIM_CC_EnableChannel(PWM_TIMx, LL_TIM_CHANNEL_CH3);
	LL_TIM_OC_SetCompareCH3(PWM_TIMx, 0);
	return 0;
}




