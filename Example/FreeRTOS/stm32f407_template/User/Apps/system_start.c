#include "system_start.h"
#include "system.h"
#include "system_app.h"
#include "system_driver.h"


/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{
	System_Driver_Init();
	System_App_Init();
}

/**
	* @brief  系统任务初始化
	* @retval None
	*/
void RTOS_Init(void)
{
	System_Driver_Loop();
	System_App_Loop();
}


