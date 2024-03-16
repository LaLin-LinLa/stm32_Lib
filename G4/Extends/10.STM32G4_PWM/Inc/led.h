#ifndef	__LED_H
#define	__LED_H

#include "main.h"

#define			LD1					GPIO_PIN_8   
#define			LD2					GPIO_PIN_9   
#define			LD3					GPIO_PIN_10  
#define			LD4					GPIO_PIN_11  
#define			LD5					GPIO_PIN_12  
#define			LD6					GPIO_PIN_13  
#define			LD7					GPIO_PIN_14  
#define			LD8					GPIO_PIN_15  
#define			LD_ALL			GPIO_PIN_All 

void led_init(void);
void led_ctrl(uint16_t LEDx,_Bool state);
void led_toggle(uint16_t LEDx);

#endif
