#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

extern TIM_HandleTypeDef htim2;

#define TRIG1_GPIO_Port GPIOB
#define TRIG1_Pin       GPIO_PIN_13

#define ECHO1_GPIO_Port GPIOB
#define ECHO1_Pin       GPIO_PIN_14


#define TRIG2_GPIO_Port GPIOB
#define TRIG2_Pin       GPIO_PIN_1

#define ECHO2_GPIO_Port GPIOB
#define ECHO2_Pin       GPIO_PIN_15


#define TRIG3_GPIO_Port GPIOB
#define TRIG3_Pin       GPIO_PIN_4

#define ECHO3_GPIO_Port GPIOB
#define ECHO3_Pin       GPIO_PIN_5

void delay_us(uint16_t us);

float HCSR04_Left(void);
float HCSR04_Front(void);
float HCSR04_Right(void);;

#endif
