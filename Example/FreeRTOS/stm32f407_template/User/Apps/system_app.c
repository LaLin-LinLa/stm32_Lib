/**
  ******************************************************************************
  * @file    system_communication.c/h
  * @brief   系统应用层文件
  * @version 1.0
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have any 
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  ******************************************************************************
*/	
#include "system_app.h"
#include "system_os.h"
#include "system.h"
#include "system_datapool.h"

#include "mod_OLED.h"

/*Test任务*/
TaskHandle_t Test_Task_Handler;
void Test_Task(void *p_arg);
#define Test_TIM	1

/**
	* @brief  系统应用层初始化
	* @retval None
	*/
void System_App_Init(void)
{
	mod_OLED_Init();
}

/**
	* @brief  系统应用层循环
	* @retval None
	*/
void System_App_Loop(void)
{
	/*Test任务*/
	xTaskCreate(Test_Task, "test_task", Small_Stack_Size, NULL, PriorityNormal, &Test_Task_Handler); 
}


/** 
	* @brief Test任务 
 **/
void Test_Task(void *p_arg)
{
	TickType_t xLastWakeTime_t = xTaskGetTickCount();
	while(1)
	{
		mod_OLED_Loop();
		vTaskDelayUntil(&xLastWakeTime_t,Test_TIM);
	}
}
