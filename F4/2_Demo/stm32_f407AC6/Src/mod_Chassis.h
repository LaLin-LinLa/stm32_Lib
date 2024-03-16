#ifndef __MOD_CHASSIS_H
#define __MOD_CHASSIS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "drv_motor.h"
#include "drv_dr16.h"
#include "math_my_pid.h"

#define	STEER_PID_POLARITY	1

/*==========���̿������ݽṹ��==========*/
typedef struct
{
	const DR16_ctrl_t *chassis_rc_ctrl;		//dr16ң��������ָ��
	Driver_Motor_t	drive_motor[4];				//�����ֵ����Ϣ
	Steer_Motor_t		steer_motor[4];				//ת���ֵ����Ϣ
	myPidTypeDef_t	drive_speed_pid[4];		//�������ٶȻ�pid
	myPidTypeDef_t	steer_speed_pid[4];		//ת�����ٶȻ�pid
	myPidTypeDef_t	steer_angle_pid[4];		//ת���ֽǶȻ�pid
	
	float	Vx, Vy, Vz;						//����ǰ���ٶ�Vy, ƽ���ٶ�Vx, �����ٶ�Vz
	int   driver_dir[4];				//�����ַ���
	float set_driver_speed[4];
	float	set_steer_ecd[4];
	float	set_steer_angle[4];
} Chassis_Control_t;

void Chassis_Init(Chassis_Control_t *chassis_init);
void Chassis_Loop(Chassis_Control_t *chassis_loop);

#ifdef __cplusplus
}
#endif
#endif
