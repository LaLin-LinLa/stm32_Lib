#include "led.h"

void led_init(void)
{
	HAL_GPIO_WritePin(GPIOC, LD_ALL, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void led_ctrl(uint16_t LEDx,_Bool state)
{
	HAL_GPIO_WritePin(GPIOC, LEDx, (state == 0)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void led_toggle(uint16_t LEDx)
{
	HAL_GPIO_TogglePin(GPIOC, LEDx);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
