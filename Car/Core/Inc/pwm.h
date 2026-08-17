/*
 * pwm.h
 *
 *  Created on: Jul 30, 2026
 *      Author: gaeun
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_
#include "main.h"
#include "tim.h"

void PWM_Init(void);
void PWM_SetLeft(uint16_t speed);
void PWM_SetRight(uint16_t speed);
void PWM_SetSpeed(uint16_t left, uint16_t right);


#endif /* INC_PWM_H_ */
