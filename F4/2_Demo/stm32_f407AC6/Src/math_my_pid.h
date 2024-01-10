#ifndef  __MATH_MY_PID_H
#define  __MATH_MY_PID_H
#ifdef  __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t (*SystemTick_Fun)(void);

enum PID_MODE
{
    PID_MODE_POS = 0,	
    PID_MODE_DEL,		
};

typedef struct myPidTypeDef_s
{
		uint8_t mode;
    //PID ������
    float Kp;
    float Ki;
    float Kd;
	
    float max_out;  //����޷�
    float max_iout; //��������޷�

		float set;
    float fdb[3];		//����ֵ

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[2];  //΢���� 0���� 1��ȥ
    float error[3]; //����� 0���� 1��ȥ 2��ǰ
		
		float DeadZone;	//��������Ϊ����
		
		bool D_of_Current;	//�Ƿ���΢������
		bool I_of_Change;		//�Ƿ������ٻ���
		bool discretize;		//�Ƿ���������������ʱ��
		float I_A;					//���ٻ���A
		float I_B;					//���ٻ���B
		
} myPidTypeDef_t;

void myPID_Clear(myPidTypeDef_t *pid);
uint8_t getMicroTick_regist(uint32_t (*getTick_fun)(void));
void Init_myPID(myPidTypeDef_t *pid, uint8_t mode, const float PID[6]);
float myPID_Calc(myPidTypeDef_t *pid,float ref,float set);

#ifdef  __cplusplus
}
#endif
#endif 

