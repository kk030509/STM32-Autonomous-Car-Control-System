#include "ultra.h"

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    while(__HAL_TIM_GET_COUNTER(&htim2) < us);
}

float HCSR04_Right(void)
{
    uint32_t time = 0;

    HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_RESET);
    delay_us(2);

    HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG1_GPIO_Port, TRIG1_Pin, GPIO_PIN_RESET);

    while(HAL_GPIO_ReadPin(ECHO1_GPIO_Port, ECHO1_Pin) == GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > 30000)
            return -1;
    }

    __HAL_TIM_SET_COUNTER(&htim2,0);

    while(HAL_GPIO_ReadPin(ECHO1_GPIO_Port, ECHO1_Pin) == GPIO_PIN_SET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > 30000)
            return -1;
    }

    time = __HAL_TIM_GET_COUNTER(&htim2);

    return (float)time / 58.0f;
}
float HCSR04_Front(void){

    uint32_t time = 0;

    HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_RESET);
    delay_us(2);

    HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, GPIO_PIN_RESET);

    while(HAL_GPIO_ReadPin(ECHO2_GPIO_Port, ECHO2_Pin) == GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > 30000) // 30ms
            return -1;
    }

    __HAL_TIM_SET_COUNTER(&htim2,0);

    while(HAL_GPIO_ReadPin(ECHO2_GPIO_Port, ECHO2_Pin) == GPIO_PIN_SET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > 30000)
            return -1;
    }

    time = __HAL_TIM_GET_COUNTER(&htim2);

    return (float)time / 58.0f;
}
float HCSR04_Left(void){

    uint32_t time = 0;

    HAL_GPIO_WritePin(TRIG3_GPIO_Port, TRIG3_Pin, GPIO_PIN_RESET);
    delay_us(2);

    HAL_GPIO_WritePin(TRIG3_GPIO_Port, TRIG3_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG3_GPIO_Port, TRIG3_Pin, GPIO_PIN_RESET);

    while(HAL_GPIO_ReadPin(ECHO3_GPIO_Port, ECHO3_Pin) == GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > 30000)
            return -1;
    }

    __HAL_TIM_SET_COUNTER(&htim2,0);

    while(HAL_GPIO_ReadPin(ECHO3_GPIO_Port, ECHO3_Pin) == GPIO_PIN_SET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim2) > 30000)
            return -1;
    }

    time = __HAL_TIM_GET_COUNTER(&htim2);

    return (float)time / 58.0f;
}
