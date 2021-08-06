/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
#define SENSOR_SWITCH_Pin          GPIO_PIN_11
#define SENSOR_SWITCH_Port         GPIOA

#define SENSOR_ORIGIN_Pin           GPIO_PIN_6
#define SENSOR_ORIGIN_Port          GPIOA

#define SENSOR_NUMBER_Pin           GPIO_PIN_7
#define SENSOR_NUMBER_Port          GPIOA

#define ADC_CHANNEL_NUMBER          2

typedef struct {

    uint8_t adcOrigin_value0;
    uint8_t adcOrigin_value1;
    uint8_t filterNumbers_value0;
    uint8_t filterNumbers_value1;
    uint8_t filterNumbers;
    uint8_t filterOriginPos;
    uint8_t filterFilterNumberPos;
    uint8_t sensorOrigin_flag;
    uint8_t sensorFilterNumbers_flag;
	uint8_t sensorOn_flag;

}ADCVALUE;

extern ADCVALUE adc_t;

extern uint8_t currSensorFlag;

void ADC_InitValue(void);

void SENSOR_OFF(void);


uint16_t Get_Adc_Average(uint32_t ch,uint8_t times,uint8_t rank);
void UART_TRANSMIT_TO_PC_DATA(void);
void SENSOR_ORIGIN_DetectPos(void);
void SENSOR_FilterNumbers_DetectPos(void);
void FilterNumbers_Calculate(void);
void SENSOR_AND_ADC_Start(uint8_t onoff);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
