/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "move.h"
#include "pwm.h"
#include "auto_drive.h"
#include "ultra.h"
extern volatile uint8_t mode;
extern volatile uint8_t rxData;

extern volatile float leftDistance;
extern volatile float frontDistance;
extern volatile float rightDistance;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for AutoDriveTask */
osThreadId_t AutoDriveTaskHandle;
const osThreadAttr_t AutoDriveTask_attributes = {
  .name = "AutoDriveTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of AutoDriveTask */
  AutoDriveTaskHandle = osThreadNew(StartTask02, NULL, &AutoDriveTask_attributes);

  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(StartTask03, NULL, &ControlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  if(mode == 1)
	          {
	              leftDistance = HCSR04_Left();
	              osDelay(5);

	              frontDistance = HCSR04_Front();
	              osDelay(5);

	              rightDistance = HCSR04_Right();
	              osDelay(5);
	          }
	          else
	          {
	              osDelay(20);
	          }
     }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the AutoDriveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
	  if(mode == 1)
	         {
	             AutoDrive();
	         }
	  else {
		  moveStop();
	  }

    osDelay(10);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the ControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
	uint8_t lastSwitch = GPIO_PIN_SET;

	    for(;;)
	    {
	        uint8_t currentSwitch =
	            HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	        // 스위치 눌림 감지
	        if(lastSwitch == GPIO_PIN_SET &&
	           currentSwitch == GPIO_PIN_RESET)
	        {
	            osDelay(50);

	            if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
	            {
	                mode = !mode;

	                // 모드 변경 시 정지
	                PWM_SetSpeed(0, 0);
	            }

	            // 버튼 떼기를 기다림
	            while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
	            {
	                osDelay(1);
	            }
	        }

	        lastSwitch = currentSwitch;


	        // =========================
	        // MANUAL MODE
	        // =========================
	        if(mode == 0)
	        {
	            switch(rxData)
	            {
	                case 0x01:
	                    moveForward();
	                    PWM_SetSpeed(950, 950);
	                    break;

	                case 0x02:
	                    moveBackward();
	                    PWM_SetSpeed(950, 950);
	                    break;

	                case 0x03:
	                    moveLeft();
	                    PWM_SetSpeed(750, 750);
	                    break;

	                case 0x04:
	                    moveRight();
	                    PWM_SetSpeed(750, 750);
	                    break;

	                case 0x05:
	                    PWM_SetSpeed(0, 0);
	                    break;
	            }
	        }

	        osDelay(1);
	    }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

