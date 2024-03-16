/**
  ******************************************************************************
  * @file    system_communication.c/h
  * @brief   ϵͳӦ�ò��ļ�
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

#include "mod_Game.h"
#include "mod_Gimbal.h"


/*����1*/
TaskHandle_t Task1_Handler;
void Task1_Task(void *p_arg);
#define TASK1_TIM	1


/**
	* @brief  ϵͳӦ�ò��ʼ��
	* @retval None
	*/
void System_App_Init(void)
{
	
}

/**
	* @brief  ϵͳӦ�ò�ѭ��
	* @retval None
	*/
void System_App_Loop(void)
{
	/*����1*/
	xTaskCreate(Task1_Task, "task1", Normal_Stack_Size, NULL, PriorityHigh, &Task1_Handler); 
}

/*=========================================================������=========================================================*/

/** 
	* @brief ����1 
 **/
void Task1_Task(void *p_arg)
{
	TickType_t xLastWakeTime_t = xTaskGetTickCount();
	while(1)
	{
		
		vTaskDelayUntil(&xLastWakeTime_t,TASK1_TIM);
	}
		
}

