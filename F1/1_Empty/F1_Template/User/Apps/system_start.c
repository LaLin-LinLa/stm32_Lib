#include "system_start.h"
#include "system.h"
#include "bsp_systick.h"

/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{	     
	bsp_systick_Init(100000);
	
}

/**
	* @brief  系统任务循环
	* @retval None
	*/
void System_Loop(void)
{

}

/**
	* @brief  系统任务初始化
	* @retval None
	*/
void RTOS_Init(void)
{
	
}

