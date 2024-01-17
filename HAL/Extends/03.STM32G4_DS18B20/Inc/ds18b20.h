#ifndef __DS18B20_H
#define __DS18B20_H

#include "main.h"

#define OW_DIR_OUT() 	mode_output1()
#define OW_DIR_IN() 	mode_input1()
#define OW_OUT_LOW() 	(HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET))
#define OW_GET_IN()  	(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6))

#define OW_SKIP_ROM 		0xCC
#define DS18B20_CONVERT 	0x44
#define DS18B20_READ 		0xBE


void ds18b20_init_x(void);
float read_temperature(void);

#endif

