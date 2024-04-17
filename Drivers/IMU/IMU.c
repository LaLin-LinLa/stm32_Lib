#include "IMU.h"
#include "math.h"
#include "Smooth_Filter.h"
#include "kalman.h"
#include "pid.h"

PidTypeDef     IMU_Temp_PID;
SmoothFilter_t IMU_Temp_Filter;
extKalman_t 	 kalman;

//�������¶ȿ���PID
#define IMU_Temp_PID_Kp     400.0
#define IMU_Temp_PID_Ki     0.5
#define IMU_Temp_PID_Kd     0.5
#define IMU_Temp_Set        70.0    //������Ԥ���¶�

static volatile float twoKp = twoKpDef;                                           // 2 * proportional gain (Kp)
static volatile float twoKi = twoKiDef;  
static volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
static volatile float integralFBx = 0.0f, integralFBy = 0.0f, integralFBz = 0.0f; // integral error terms scaled by Ki

static void IMU_Init_Calibration(IMU_Data_t *imu_data);
static void IMU_InData_Solve(IMU_Data_t *imu_data);
static void mahony_ahrs_updateIMU(IMU_Data_t *imu_data);

/**
	* @brief  IMU��ʼ��
	* @retval None
	*/
void IMU_Init(IMU_Data_t *imu_data)
{
	#if _USE_MPU9250
	while(MPU9250_Init());
	#endif
	float pid[3] = {IMU_Temp_PID_Kp, IMU_Temp_PID_Ki, IMU_Temp_PID_Kd};
	IMU_Init_Calibration(imu_data);
	SmoothFilter_Init(&IMU_Temp_Filter, 8);
	PID_Init(&IMU_Temp_PID, PID_POSITION, pid, 1000, 0);
	KalmanCreate(&kalman,20,200);
}

/**
	* @brief  IMUѭ��
	* @retval None
	*/
void IMU_Loop(IMU_Data_t *imu_data)
{
	//������PID���µ���
	MPU9250_Get_Temp(&imu_data->temp);
	imu_data->temp = SmoothFilter_Calc(&IMU_Temp_Filter, imu_data->temp);
	MPU9250_Set_Heat_PWM((int)PID_Calc(&IMU_Temp_PID, imu_data->temp, IMU_Temp_Set));
	IMU_InData_Solve(imu_data);
	KalmanFilter(&kalman, imu_data->accel_x);
	KalmanFilter(&kalman, imu_data->accel_y);
	KalmanFilter(&kalman, imu_data->accel_z);
	KalmanFilter(&kalman, imu_data->gyro_x);
	KalmanFilter(&kalman, imu_data->gyro_y);
	KalmanFilter(&kalman, imu_data->gyro_z);
	
	mahony_ahrs_updateIMU(imu_data);
	
	
	//����
	imu_data->yaw = imu_data->yaw_new;
	//���
	if((imu_data->yaw_new - imu_data->yaw_last) >= 330) {
	imu_data->yaw_count--;
	}
	else if((imu_data->yaw_new - imu_data->yaw_last) <= -330) {
		imu_data->yaw_count++;
	}
	imu_data->yaw_total = imu_data->yaw_count*360 + imu_data->yaw_new;
	
}


/**
  * @brief  ���ٿ���
  * @param  ԭʼ����
  * @retval ������ĵ���
  */
static float invSqrt(float x) 
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;

  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));

  return y;
}

/**
  * @brief	��ȡ������ԭʼ����
  * @param  IMU���ݽṹ��
  * @retval void
  */
static void IMU_Get_Raw_Data(IMU_Data_t *imu_data) 
{
	IMU_Get_Gyro(&imu_data->gyro_x, &imu_data->gyro_y, &imu_data->gyro_z);
	IMU_Get_Accel(&imu_data->accel_x, &imu_data->accel_y, &imu_data->accel_z);
	
}

/**
  * @brief  ���ٶ�У׼��ʼ��
  * @param  IMU���ݽṹ��
  * @retval void
  */
static void IMU_Init_Calibration(IMU_Data_t *imu_data) {
	uint16_t i=0, cnt=0;
	for(i = 0;i < 2048 ;i++)
	{
		IMU_Get_Raw_Data(imu_data);
		if(i>300)
		{
      imu_data->gyro_x_offset += imu_data->gyro_x;
			imu_data->gyro_y_offset += imu_data->gyro_y;
			imu_data->gyro_z_offset += imu_data->gyro_z;
		  cnt++;
			IMU_Init_Delay_ms(2);
		}
	}
	imu_data->gyro_x_offset /= (float)cnt;
  imu_data->gyro_y_offset /= (float)cnt;
	imu_data->gyro_z_offset /= (float)cnt;
}


/**
  * @brief  ���������ݴ���
  * @param  IMU���ݽṹ��
  * @retval void
  */
static void IMU_InData_Solve(IMU_Data_t *imu_data)
{
	char i;
	float	gx_sum, gy_sum, gz_sum;
	float	ax_sum, ay_sum, az_sum;
	for(i = 0; i<50; i++)
	{
		IMU_Get_Raw_Data(imu_data);
		
		gx_sum += (imu_data->gyro_x-imu_data->gyro_x_offset);
		gy_sum += (imu_data->gyro_y-imu_data->gyro_y_offset);
		gz_sum += (imu_data->gyro_z-imu_data->gyro_z_offset);
		ax_sum += imu_data->accel_x;
		ay_sum += imu_data->accel_y;
		az_sum += imu_data->accel_z;
	}
	
	imu_data->gyro_x = gx_sum/50 *DEG2RAD;
	imu_data->gyro_y = gy_sum/50 *DEG2RAD;
	imu_data->gyro_z = gz_sum/50 *DEG2RAD;
	
	imu_data->accel_x = ax_sum/50 *ACCEL_GRAVITY;
	imu_data->accel_y = ay_sum/50 *ACCEL_GRAVITY;
	imu_data->accel_z = az_sum/50 *ACCEL_GRAVITY;	
	
}

/**
  * @brief  ������̬��(������ǿ��)
  * @param  IMU���ݽṹ��
  */
static void mahony_ahrs_updateIMU(IMU_Data_t *imu_data) 
{
	float recipNorm;
  float halfvx, halfvy, halfvz;
  float halfex, halfey, halfez;
	float qa, qb, qc;
	
	// �������ٶȼƲ�����Чʱ�ż��㷴��(������ٶȼƹ�һ���е�NaN)
  if (!((imu_data->accel_x == 0.0f) && (imu_data->accel_y == 0.0f) && (imu_data->accel_z == 0.0f)))
  {
		//��һ�����ٶȼƲ���
		recipNorm = invSqrt(imu_data->accel_x*imu_data->accel_x + imu_data->accel_y*imu_data->accel_y + imu_data->accel_z*imu_data->accel_z);
		imu_data->accel_x *= recipNorm;
		imu_data->accel_y *= recipNorm;
		imu_data->accel_z *= recipNorm;
		
		// ���������ķ���
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
			
		// ����ǹ��Ƶ���������Ͳ�������������Ĳ��֮��
		halfex = (imu_data->accel_y * halfvz - imu_data->accel_z * halfvy);
		halfey = (imu_data->accel_z * halfvx - imu_data->accel_x * halfvz);
		halfez = (imu_data->accel_x * halfvy - imu_data->accel_y * halfvx);

		//�����Ӧ�û��ַ������������
		if (twoKi > 0.0f)
		{
			integralFBx += twoKi * halfex * (1.0f / sampleFreq); 
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			imu_data->gyro_x += integralFBx; 
			imu_data->gyro_y += integralFBy;
			imu_data->gyro_z += integralFBz;
		}
		else
		{
			integralFBx = 0.0f; //��ֹ��������
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}
		//Ӧ�ñ�������
		imu_data->gyro_x += twoKp * halfex;
		imu_data->gyro_y += twoKp * halfey;
		imu_data->gyro_z += twoKp * halfez;
	}

	// ������Ԫ���ı仯��
  imu_data->gyro_x *= (0.5f * (1.0f / sampleFreq)); // pre-multiply common factors
  imu_data->gyro_y *= (0.5f * (1.0f / sampleFreq));
  imu_data->gyro_z *= (0.5f * (1.0f / sampleFreq));
  qa = q0;
  qb = q1;
  qc = q2;
  q0 += (-qb * imu_data->gyro_x - qc * imu_data->gyro_y - q3 * imu_data->gyro_z);
  q1 += (qa * imu_data->gyro_x + qc * imu_data->gyro_z - q3 * imu_data->gyro_y);
  q2 += (qa * imu_data->gyro_y - qb * imu_data->gyro_z + q3 * imu_data->gyro_x);
  q3 += (qa * imu_data->gyro_z + qb * imu_data->gyro_y - qc * imu_data->gyro_x);
	
	// ��������Ԫ��
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
  imu_data->roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3f; // roll     -pi----pi
  imu_data->pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3f;                                // pitch    -pi/2----pi/2
	imu_data->yaw_last = imu_data->yaw_new;
  imu_data->yaw_new = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3f;  // yaw      -pi----pi
	
}




