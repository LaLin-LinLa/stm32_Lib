#include "mod_Chassis.h"
#include "math_util.h"
#include "math_fifters.h"
#include "tool_vofa.h"
#include "math.h"

#define	MAX_SPEED	25000		//������ٶ�
#define	MAX_W			8000		//�����ٶ�

//��Ҫ�����趨����ֵ�����ٳ�������ʱ����
const float SINCOS45_f32 = 0.70710678f;
const	float	Total_ANGLE	= (8192.0f*36.0f*4.0f);
const	float	ANGLE_VAL		= 180.0f/_PI;
const	float	ECD_VAL		= Total_ANGLE/360.0f;

//�ٶȻ�pid����
float PID_DRIVER_SPEED[4][6]={7.0f, 0.1f, 0.0f, 16384.0f, 800.0f, 1.0f,			//0
														  7.0f, 0.1f, 0.0f, 16384.0f, 800.0f, 1.0f,			//1
														  7.0f, 0.1f, 0.0f, 16384.0f, 800.0f,	1.0f,			//2
														  7.0f, 0.1f, 0.0f, 16384.0f, 800.0f, 1.0f};		//3
float PID_STEER_SPEED[4][6]={9.0f, 0.0f, 5.0f, 10000.0f, 800.0f, 5.0f,			//0
														 9.0f, 0.0f, 5.0f, 10000.0f, 800.0f, 5.0f,			//1
														 9.0f, 0.0f, 5.0f, 10000.0f, 800.0f, 5.0f,			//2
														 9.0f, 0.0f, 5.0f, 10000.0f, 800.0f, 5.0f};		  //3
float PID_STEER_ANGLE[4][6]={0.4f, 0.0f, 1.5f, Total_ANGLE, 800.0f, 5.0f,				//0
														 0.4f, 0.0f, 1.5f, Total_ANGLE, 800.0f,	5.0f,				//1
														 0.4f, 0.0f, 1.5f, Total_ANGLE, 800.0f,	5.0f,				//2
														 0.4f, 0.0f, 1.5f, Total_ANGLE, 800.0f, 5.0f};		  //3

static void Chassis_Motor_Control( Chassis_Control_t *motor);														 
static void Driver_steer_Cale(Chassis_Control_t *chassis);
static void Chassis_Control_Input(Chassis_Control_t *chassis);
														 
/*========================================================API����========================================================*/
/** 
	* @brief ���̳�ʼ������
 **/
void Chassis_Init(Chassis_Control_t *chassis_init)
{
	chassis_init->chassis_rc_ctrl = get_dr16_control_point();
	for(char i=0;i<4;i++)
	{
		Init_myPID(&chassis_init->drive_speed_pid[i], PID_MODE_POS, PID_DRIVER_SPEED[i]);
		Init_myPID(&chassis_init->steer_speed_pid[i], PID_MODE_POS, PID_STEER_SPEED[i]);
		Init_myPID(&chassis_init->steer_angle_pid[i], PID_MODE_POS, PID_STEER_ANGLE[i]);
		
		chassis_init->drive_speed_pid[i].discretize		=	true;		//�����ٶȻ�������������ʱ��
		
		
		chassis_init->steer_speed_pid[i].D_of_Current = true;		//�����ٶȻ�΢������
		chassis_init->steer_angle_pid[i].D_of_Current = true;		//�����ǶȻ�΢������
		chassis_init->steer_angle_pid[i].discretize		=	true;		//�����ǶȻ�������������ʱ��
		chassis_init->steer_speed_pid[i].discretize		= true;		//�����ٶȻ�������������ʱ��
		
		chassis_init->driver_dir[i] = 1;
		chassis_init->drive_motor[i].Ramp_Inc = 1000;
		
		chassis_init->drive_motor[i].motor_measure = get_Driver_Measure_Point(i);
		chassis_init->steer_motor[i].motor_measure = get_Steer_Measure_Point(i);
	}
	
}

/** 
	* @brief ���̿��ƺ���
 **/
//float Data[3] = {0.0f};

void Chassis_Loop(Chassis_Control_t *chassis_loop)
{
	Chassis_Control_Input(chassis_loop);
	Driver_steer_Cale(chassis_loop);
	Chassis_Motor_Control(chassis_loop);
//	Data[0] = chassis_loop->set_steer_ecd[0];
//	Data[1] = chassis_loop->steer_motor[0].motor_measure->total_ecd;
//	JustFloat_Send(Data, 2, USART1);
}
/*========================================================���ܺ���========================================================*/
//1  4
//2  3
/** 
	* @brief �����ٶȷֽ�(����) 
	* @param V_limit				���ٶ�����
	* @param W_limit				���ٶ�����
 **/
static void Driver_steer_Cale(Chassis_Control_t *chassis)
{
	if(!chassis->Vx && !chassis->Vy && !chassis->Vz)
	{
		for(char i=0;i<4;i++){
			chassis->set_steer_ecd[i] = chassis->set_steer_angle[i] = chassis->set_driver_speed[i] = 0.0f;
			chassis->driver_dir[i] = 1;
		}
		return;
	}
	
	chassis->set_steer_angle[0]		 = ANGLE_VAL*atan2f((chassis->Vy-chassis->Vz*SINCOS45_f32),(chassis->Vx-chassis->Vz*SINCOS45_f32));
	chassis->set_steer_angle[1]		 = ANGLE_VAL*atan2f((chassis->Vy-chassis->Vz*SINCOS45_f32),(chassis->Vx+chassis->Vz*SINCOS45_f32));
	chassis->set_steer_angle[2]		 = ANGLE_VAL*atan2f((chassis->Vy+chassis->Vz*SINCOS45_f32),(chassis->Vx+chassis->Vz*SINCOS45_f32));
	chassis->set_steer_angle[3]		 = ANGLE_VAL*atan2f((chassis->Vy+chassis->Vz*SINCOS45_f32),(chassis->Vx-chassis->Vz*SINCOS45_f32));
	
	for(char i=0;i<4;i++)
	{
		if(chassis->set_steer_angle[i] >= 0)
		{
			chassis->driver_dir[i] = 1;
			chassis->set_steer_angle[i] -= 90.0f;
			chassis->set_steer_ecd[i] = chassis->set_steer_angle[i] * ECD_VAL;
		}
		else if(chassis->set_steer_angle[i] < 0)
		{
			chassis->driver_dir[i] = -1;
			chassis->set_steer_angle[i] += 90.0f;
			chassis->set_steer_ecd[i] = chassis->set_steer_angle[i] * ECD_VAL;
		}
	}
		
	
	chassis->set_driver_speed[3] = -chassis->driver_dir[0]* __sqrtf(powf((chassis->Vy - chassis->Vz*SINCOS45_f32), 2)
																		 +powf((chassis->Vx - chassis->Vz*SINCOS45_f32), 2));
	chassis->set_driver_speed[0] = -chassis->driver_dir[1]*__sqrtf(powf((chassis->Vy - chassis->Vz*SINCOS45_f32), 2)
																		 +powf((chassis->Vx + chassis->Vz*SINCOS45_f32), 2));
	chassis->set_driver_speed[1] = chassis->driver_dir[2]*__sqrtf(powf((chassis->Vy + chassis->Vz*SINCOS45_f32), 2)
																		 +powf((chassis->Vx + chassis->Vz*SINCOS45_f32), 2));
	chassis->set_driver_speed[2] = chassis->driver_dir[3]*__sqrtf(powf((chassis->Vy + chassis->Vz*SINCOS45_f32), 2)
																		 +powf((chassis->Vx - chassis->Vz*SINCOS45_f32), 2));
}

/** 
	* @brief ���̿�������
 **/
static void Chassis_Control_Input(Chassis_Control_t *chassis)
{
	chassis->Vy = int_map(int16_buffer(chassis->chassis_rc_ctrl->rc.ch[3], 30), -630, 630, -MAX_SPEED, MAX_SPEED);
	chassis->Vx = int_map(int16_buffer(chassis->chassis_rc_ctrl->rc.ch[2], 30), -630, 630, -MAX_SPEED, MAX_SPEED);
	chassis->Vz = -int_map(int16_buffer(chassis->chassis_rc_ctrl->rc.ch[0], 30), -630, 630, -MAX_W, MAX_W);
}

/** 
	* @brief ���̵������
 **/
static void Chassis_Motor_Control( Chassis_Control_t *chassis)
{
	//pid����
	for(char i=0;i<4;i++)
	{	
		//������
		chassis->set_driver_speed[i] = Ramp_Cale(chassis->set_driver_speed[i], chassis->drive_speed_pid[i].set, chassis->drive_motor[i].Ramp_Inc);
		myPID_Calc(&chassis->drive_speed_pid[i], chassis->drive_motor[i].motor_measure->speed_rpm, chassis->set_driver_speed[i]);
		chassis->drive_motor[i].set_current = chassis->drive_speed_pid[i].out;
		//ת����
		myPID_Calc(&chassis->steer_angle_pid[i], chassis->steer_motor[i].motor_measure->total_ecd, chassis->set_steer_ecd[i]);
		chassis->steer_angle_pid[i].out *= STEER_PID_POLARITY;
		myPID_Calc(&chassis->steer_speed_pid[i], chassis->steer_motor[i].motor_measure->speed_rpm, chassis->steer_angle_pid[i].out);
		chassis->steer_motor[i].set_current = chassis->steer_speed_pid[i].out;
	}
}

