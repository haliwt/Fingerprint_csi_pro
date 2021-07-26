/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define S1_Pin GPIO_PIN_4
#define S1_GPIO_Port 			GPIOA

#define S2_Pin 					GPIO_PIN_5
#define S2_GPIO_Port 			GPIOA

//sensor output  PA6
#define SOP1_Pin 				GPIO_PIN_6
#define SOP1_GPIO_Port 			GPIOA
//sensor OUTPUT PA7
#define SOP2_Pin 				GPIO_PIN_7
#define SOP2_GPIO_Port 			GPIOA

#define DEC1_Pin 				GPIO_PIN_0
#define DEC1_GPIO_Port 			GPIOB

#define SLEEP_Pin 				GPIO_PIN_1
#define SLEEP_GPIO_Port 		GPIOB
#define DEC0_Pin 				GPIO_PIN_8
#define DEC0_GPIO_Port 			GPIOA
#define TOQ1_Pin 				GPIO_PIN_6
#define TOQ1_GPIO_Port 			GPIOC
#define TOQ0_Pin 				GPIO_PIN_11
#define TOQ0_GPIO_Port 			GPIOA
#define ADEC_Pin 				GPIO_PIN_12
#define ADEC_GPIO_Port 			GPIOA
#define STEP_Pin 				GPIO_PIN_15
#define STEP_GPIO_Port 			GPIOA
#define DIR_Pin 				GPIO_PIN_3
#define DIR_GPIO_Port 			GPIOB
#define EN_Pin 					GPIO_PIN_4
#define EN_GPIO_Port 			GPIOB
#define TOFF_Pin 				GPIO_PIN_5
#define TOFF_GPIO_Port 			GPIOB
#define M1_Pin 					GPIO_PIN_6
#define M1_GPIO_Port 			GPIOB
#define M0_Pin 					GPIO_PIN_7
#define M0_GPIO_Port 			GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
