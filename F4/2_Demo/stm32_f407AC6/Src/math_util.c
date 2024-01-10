#include "math_util.h"

float Ramp_Cale( float target, float target_last, float Inc)
{
	if(_ABS(target_last) - _ABS(target) > 0)Inc = Inc * 6;//����ʱ�Ŵ�6��
	if(_ABS(target_last - target) >= Inc)
	{
		if((target_last) > target) target -= Inc;
    if((target_last) < target) target += Inc;
	}
	return target;
}

//int16����
int16_t int16_buffer(int16_t Value, int16_t buffer)
{
  if(Value > 0) 
	{
		Value -= buffer;
		if(Value < 0) Value=0;
	}
	else if(Value < 0)
	{
		Value += buffer;
		if(Value > 0) Value=0;
	}
	return Value;
}
/**
  * @brief				��Χӳ��
  * @author				RW
  * @param a			ԭʼ��ֵ
  * @param amin		ԭʼ��Сֵ
  * @param amax		ԭʼ���ֵ
  * @param bmin		ӳ������Сֵ
  * @param bmax		ӳ�������ֵ
  * @retval				ӳ������ֵ
  */
float float_map(float a, float amin, float amax, float bmin, float bmax)
{
	float adel = amax - amin, bdel = bmax - bmin;
	if(a > amax)a = amax;
	if(a < amin)a = amin;
	return (float)(bdel * ((float)(a-amin) / adel))+bmin;
}
int int_map(int a, int amin, int amax, int bmin, int bmax)
{
	int adel = amax - amin, bdel = bmax - bmin;
	if(a > amax)a = amax;
	if(a < amin)a = amin;
	return (bdel * ((float)(a-amin) / adel))+bmin;
}
