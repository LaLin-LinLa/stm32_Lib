#include "system_start.h"
#include "system.h"
#include "bsp_systick.h"

/**
	* @brief  ϵͳ��ʼ��
	* @retval None
	*/
void System_Init(void)
{	     
	bsp_systick_Init(100000);
	
}

/**
	* @brief  ϵͳ����ѭ��
	* @retval None
	*/
void System_Loop(void)
{

}

/**
	* @brief  ϵͳ�����ʼ��
	* @retval None
	*/
void RTOS_Init(void)
{
	
}

