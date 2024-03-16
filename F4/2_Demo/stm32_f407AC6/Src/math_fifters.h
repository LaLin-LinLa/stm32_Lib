#ifndef  __MATH_FIFTERS_H
#define  __MATH_FIFTERS_H
#ifdef  __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

	/**
 * A simple low pass filter.
 *
 * @param value
 * The filtered value.
 *
 * @param sample
 * Next sample.
 *
 * @param filter_constant
 * Filter constant. Range 0.0 to 1.0, where 1.0 gives the unfiltered value.
 */
#define UTILS_LP_FAST(value, sample, filter_constant)    (value -= (filter_constant) * ((value) - (sample)))		
	
typedef struct LP_Filter_s{
	bool flag[2];	//���ݱ仯����: 1��ʾ����;	0��ʾ��С
	float k;			//�˲�ϵ��
	float k_init;	//��ʼ�˲�ϵ��(Ĭ����̬ϵ��)
	float k_gain;	//�˲�ϵ������
	uint8_t cnt;	//�˲�������
	float threshold_data;		//��������������ֵ
	uint8_t threshold_cnt;	//�˲���������ֵ
} LP_Filter_t;

float LP_Filter(LP_Filter_t* filter, float data_new, float data_last);

#ifdef  __cplusplus
}
#endif
#endif 

