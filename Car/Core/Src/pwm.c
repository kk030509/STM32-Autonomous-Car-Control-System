#include "pwm.h"
//PA6 -> 채널1
//PA7 -> 채널2
void PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}
void PWM_SetLeft(uint16_t speed)
{
	 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
}
void PWM_SetRight(uint16_t speed)
{
	 __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
}
void PWM_SetSpeed(uint16_t left, uint16_t right)
{
	  PWM_SetLeft(left);
	  PWM_SetRight(right);
}

