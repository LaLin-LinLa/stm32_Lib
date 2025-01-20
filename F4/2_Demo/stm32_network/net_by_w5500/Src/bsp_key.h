#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)  //KEY0°´¼üPE4
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)  //KEY1°´¼üPE3
#define WK_UP       HAL_GPIO_ReadPin(WK_UP_GPIO_Port,WK_UP_Pin)  //WKUP°´¼üPA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

uint8_t Key_Scan(uint8_t mode);

#ifdef __cplusplus
}
#endif
#endif

