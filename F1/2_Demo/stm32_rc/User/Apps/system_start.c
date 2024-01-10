#include "system_start.h"
#include "system.h"
#include "bsp_systick.h"
#include "bsp_spi.h"
#include "drv_24l01.h"

uint8_t Tx_buf[] = {"Hello!"};

/**
	* @brief  系统初始化
	* @retval None
	*/
void System_Init(void)
{	     
	bsp_systick_Init(SYSTICK_10US_TICK);
	bsp_spi_Init(SPI1);
	drv_24l01_Init();
	while(NRF24L01_Check())
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		delay_ms_sys(100);
	}
	
}

/**
	* @brief  系统任务循环
	* @retval None
	*/
void System_Loop(void)
{
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	NRF24L01_TX_Mode();
	while(1)
	{
//		memset(Tx_buf,0,sizeof(Tx_buf));
//		snprintf((char*)Tx_buf,20,"NO.1");
		if(NRF24L01_TxPacket(Tx_buf)==TX_OK)
		{
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}
	}
}


