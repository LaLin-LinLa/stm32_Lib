#ifndef _USER_LIB_H_
#define _USER_LIB_H_
#include <stdint.h>
#include <stddef.h>

float float_map(float a, float amin, float amax, float bmin, float bmax);
int int_map(int a, int amin, int amax, int bmin, int bmax);
int int_map_shoot_pwm(int a, int amin, int amax, int value, int MAX, int MIN);
extern float float_deadline(float Value, float minValue, float maxValue);
extern float float_limit(float raw, float limit_min, float limit_max);
extern int16_t int16_deadline(int16_t Value, int16_t minValue, int16_t maxValue);
extern int16_t int16_limit(int16_t raw, int16_t limit_min, int16_t limit_max);
extern int16_t int16_buffer(int16_t Value, int16_t buffer);
#endif

