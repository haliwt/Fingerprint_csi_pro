#ifndef __PWM_H
#define __PWM_H
#include "main.h"
#include "stm32g0xx_hal_tim.h"
#include "gpio.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM3_Init(void);

                        
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);



#endif 

