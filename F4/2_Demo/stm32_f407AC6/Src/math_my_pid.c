/**
  ******************************************************************************
  * @file    math_my_pid.c/h
  * @brief   pid���ƺ���
  * @version 1.0
  * @par Change Log��
  * 		Date				Version				Author					Description
	*			22.5.23			V1.0					Gsx							Creator
	*			23.11.11		V2.0					Gsx							Update
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/
#include "math_my_pid.h"
#include "math_util.h"
#include "math_fifters.h"

static SystemTick_Fun Get_SystemTick = NULL;
uint32_t last_time;
float dt = 1;		//����ʱ��

/**
 * @brief  ��ʱ��ע�ắ��(1Tick = 1us)
 * @retval 1: success
           0: error input param
 * @author
 */
uint8_t getMicroTick_regist(uint32_t (*getTick_fun)(void)){
  if(getTick_fun != NULL)
  {
    Get_SystemTick = getTick_fun;
    return 1;
  }
  else 
    return 0;
}
/**
 * @brief  ����ʱ���
 */
uint8_t UpdataTimeStamp(void){
	uint32_t now_time=0;
	if(Get_SystemTick!=NULL){
		if (last_time == 0){
      last_time = Get_SystemTick();
      return 1;
    }
    now_time = Get_SystemTick();
	
		/*Overflow*/
		if (now_time < last_time)
			dt = (float)(now_time + (0xFFFFFFFF - last_time));
		else
			dt = (float)(now_time - last_time);

		last_time = now_time;
		dt *= 0.000001f;
			
		return 0;
	}
	else{
    dt = 1;
    return 1;
  }
}

/**
  * @brief  ���ֵ����
	* @param  pid
 **/
void myPID_Clear(myPidTypeDef_t *pid) 
{
	if (pid == NULL)
    {
        return;
    }

    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->fdb[0]= pid->fdb[1] = pid->fdb[2] = 0.0f;
}
/**
  * @brief  PID��ʼ��
	* @param  pid
	* @param  mode    	PIDģʽ��λ��ʽ/����ʽ
  * @param  PID[6]		PID��������
	*					����״̬�£�����0~5�ֱ�Ϊ��Kp,Ki,Kd,max_out,max_iout,DeadZone
	*					��ɢ״̬�£�����0~5�ֱ�Ϊ��Kp,Kp/Ti,Kp*Td,max_out,max_iout,DeadZone
	*					Ki=Kp*T/Ti	(TΪ����ʱ�䣬 TiΪ����ʱ�䳣��)
	*					Kd=Kp*Td/T	(TΪ����ʱ�䣬 TdΪ΢��ʱ�䳣��)
	* @warning������������������ʱ�䣬����ע���ʱ����	
 **/
void Init_myPID(myPidTypeDef_t *pid, uint8_t mode, const float PID[6])
{	
	
	if (pid == NULL || PID == NULL)
	{
			return;
	}
	pid->D_of_Current = false;
	pid->I_of_Change	= false;
	pid->discretize		=	false;
	pid->mode = mode;
	
	pid->set = 0.0f;
	pid->Kp = PID[0];
	pid->Ki = PID[1];
	pid->Kd = PID[2];
	pid->max_out = PID[3]; 
	pid->max_iout = PID[4];
	pid->DeadZone	= PID[5];
	
	myPID_Clear(pid);
}

/**
  * @brief  PID����
	* @param  PID
  * @param  ref			����ֵ
	* @param  set  		����ֵ
	* @return out			
 **/
float myPID_Calc(myPidTypeDef_t *pid,float ref,float set){
	if (pid == NULL)
	{
			return 0.0f;
	}
	if(pid->discretize)		//�������������������ʱ�䣬����ʱ�䲢����Ƿ�ע���ʱ��
	{
		if (UpdataTimeStamp())		//�ж��Ƿ�ִ��ʱ��
		return 0;
	}
	pid->set = set;
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];
	pid->error[0] = set - ref;
	
	if(_ABS(pid->error[1]) < pid->DeadZone)
	{
		return (pid->out = 0.0f);
	}
	
	UTILS_LP_FAST(pid->error[0], pid->error[1], 0.05f);
	
	if(pid->mode == PID_MODE_POS){			//λ��ʽpid
		/*����Pout*/
		if(pid->Kp != 0)
			pid->Pout = pid->Kp * pid->error[0];
		else
			pid->Pout = 0.0f;
		
		/*����Iout*/
		if(pid->Ki != 0){
			if(pid->I_of_Change){		//�������ٻ���
				float factor = 0.0f;
				if(_ABS(pid->error[0])<=pid->I_B){
					factor = 1.0f;
				}
				else if(_ABS(pid->error[0]) <= (double)(pid->I_A)+pid->I_B){
					factor = (pid->I_A - _ABS(pid->error[1])+pid->I_B)/pid->I_A;
				}
				else if(_ABS(pid->error[0]) > (double)(pid->I_A)+pid->I_B){
					factor = 0.0f;
				}
				pid->Iout += pid->Ki*factor*pid->error[0] * dt;
			}
			else{
				pid->Iout += pid->Ki * pid->error[0] * dt;
			}
			_LimitMax(pid->Iout, pid->max_iout);
		}else
			pid->Iout = 0.0f;
		
		/*����Dout*/
		if(pid->Ki != 0){
			if(pid->D_of_Current){	//����΢������
				pid->fdb[2] = pid->fdb[1];
				pid->fdb[1] = pid->fdb[0];
				pid->fdb[0] = ref;
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->fdb[0] - pid->fdb[1])/dt;
				UTILS_LP_FAST(pid->Dbuf[0], pid->Dbuf[1], 0.05f);		//����ȫ΢��
				pid->Dout = pid->Kd * pid->Dbuf[0];
			}
			else{
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1])/dt;
				UTILS_LP_FAST(pid->Dbuf[0], pid->Dbuf[1], 0.05f);		//����ȫ΢��
        pid->Dout = pid->Kd * pid->Dbuf[0];
			}
		}else
			pid->Dout = 0.0f;
		
		pid->out = pid->Pout + pid->Iout + pid->Dout;
    _LimitMax(pid->out, pid->max_out);
	}
	else if(pid->mode == PID_MODE_DEL){	//����ʽpid
		/*����Pout*/
		if(pid->Kp != 0)
			pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
		else
			pid->Pout = 0.0f;
		
		/*����Iout*/
		if(pid->Ki != 0){
			if(pid->I_of_Change){		//�������ٻ���
				float factor = 0.0f;
				if(_ABS(pid->error[0])<=pid->I_B){
					factor = 1.0f;
				}
				else if(_ABS(pid->error[0]) <= (double)(pid->I_A)+pid->I_B){
					factor = (pid->I_A - _ABS(pid->error[1])+pid->I_B)/pid->I_A;
				}
				else if(_ABS(pid->error[0]) > (double)(pid->I_A)+pid->I_B){
					factor = 0.0f;
				}
				pid->Iout = pid->Ki*factor*pid->error[0] * dt;
			}
			else{							
				pid->Iout = pid->Ki * pid->error[0] * dt;
			}
			_LimitMax(pid->Iout, pid->max_iout);
		}else
			pid->Iout = 0.0f;
		
		/*����Dout*/
		if(pid->Ki != 0){
			if(pid->D_of_Current){	//����΢������
				pid->fdb[2] = pid->fdb[1];
				pid->fdb[1] = pid->fdb[0];
				pid->fdb[0] = ref;
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->fdb[0] - 2.0f * pid->fdb[1] + pid->fdb[2]);
				UTILS_LP_FAST(pid->Dbuf[0], pid->Dbuf[1], 0.05f);		//����ȫ΢��
				pid->Dout = pid->Kd * pid->Dbuf[0]/dt;
			}
			else{
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
				UTILS_LP_FAST(pid->Dbuf[0], pid->Dbuf[1], 0.05f);		//����ȫ΢��
        pid->Dout = pid->Kd * pid->Dbuf[0]/dt;
			}
		}else
			pid->Dout = 0.0f;
		
		pid->out = pid->Pout + pid->Iout + pid->Dout;
    _LimitMax(pid->out, pid->max_out);
	}
	return pid->out;
	
}
