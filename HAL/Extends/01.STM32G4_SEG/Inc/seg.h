#ifndef __SEG_H
#define __SEG_H

#include "main.h"

#define SEG_H		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET)
#define SEG_L		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)

#define SCK_H		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)
#define SCK_L		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET)

#define RCLK_H		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)
#define RCLK_L		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)


void seg_set(uint8_t seg1,uint8_t seg2,uint8_t seg3);

#endif /*__SEG_H*/
