/*
 * move.h
 *
 *  Created on: Jul 30, 2026
 *      Author: gaeun
 */

#ifndef INC_MOVE_H_
#define INC_MOVE_H_
#include "main.h"

#define IN1_PORT GPIOB
#define IN1_PIN GPIO_PIN_8

#define IN2_PORT GPIOC
#define IN2_PIN GPIO_PIN_9

#define IN3_PORT GPIOC
#define IN3_PIN GPIO_PIN_8

#define IN4_PORT GPIOC
#define IN4_PIN GPIO_PIN_6

void moveForward(void);
void moveBackward(void);
void moveRight(void);
void moveLeft(void);
void moveStop(void);

#endif /* INC_MOVE_H_ */
