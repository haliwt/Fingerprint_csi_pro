/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private defines -----------------------------------------------------------*/
//the first group LED 

 /* Private defines -----------------------------------------------------------*/
 //the first group LED
 #define LEDACUT_Pin 				 GPIO_PIN_0   //PA0
 #define LEDACUT_GPIO_Port 		     GPIOA

 #define LEDAPWM_Pin 				GPIO_PIN_1   //PB1 -TIM3_CH4
 #define LEDAPWM_GPIO_Port 			GPIOB

 #define LEDA1_Pin 					GPIO_PIN_0  //PB0
 #define LEDA1_GPIO_Port 		    GPIOB

 #define LEDA2_Pin 					GPIO_PIN_4    //PA4
 #define LEDA2_GPIO_Port 			GPIOA

 #define LEDA3_Pin 					GPIO_PIN_5   //PA5
 #define LEDA3_GPIO_Port 			GPIOA

 #define LEDA4_Pin 					GPIO_PIN_1  //PA1
 #define LEDA4_GPIO_Port 			GPIOA

 #define LEDA5_Pin 					GPIO_PIN_0   //PA0
 #define LEDA5_GPIO_Port 			GPIOA

 #define LEDA6_Pin 					GPIO_PIN_14   //PC14
 #define LEDA6_GPIO_Port 			GPIOC

 /*****************POWER-KEY *********************/

#define POWER_KEY_Pin                  GPIO_PIN_8
#define POWER_KEY_GPIO_Port            GPIOA

#define POWER_STATE_Pin                GPIO_PIN_6
#define POWER_STATE_GPIO_Port          GPIOC

 /* USER CODE BEGIN Private defines */



/* USER CODE BEGIN Private defines */

void mainMX_GPIO_Init(void);

void MX_GPIO_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
