#include "system_start.h"
#include "system.h"
#include "system_app.h"
#include "system_driver.h"


/**
	* @brief  ϵͳ��ʼ��
	* @retval None
	*/
void System_Init(void)
{
	System_Driver_Init();
	System_App_Init();
}

/**
	* @brief  ϵͳ�����ʼ��
	* @retval None
	*/
void FreeRTOS_Init(void)
{
	System_Driver_Loop();
	System_App_Loop();

}
