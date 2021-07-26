/*
 * user_main.c
 *
 *  Created on: Sep 22, 2020
 *      Author: om
 */

#include "motor.h"
#include "cmd_link.h"
#include "pwm.h"
#include "mainled.h"

extern void posCtrl(uint8_t destPos);

void user_main(void)
{
	//HAL_Delay(3000);
	//MX_TIM3_Init(); //led pwm
	initMotor();
	cmdInit();
	HAL_Delay(500);
	HAL_GPIO_WritePin(POWER_STATE_GPIO_Port,POWER_STATE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin, GPIO_PIN_SET);
	while(1)
	{

		decode();
		HAL_GPIO_WritePin(LEDAPWM_GPIO_Port,LEDAPWM_Pin, GPIO_PIN_SET);
		//LedOn(2);
		HAL_GPIO_WritePin(LEDA1_GPIO_Port, LEDA1_Pin, GPIO_PIN_SET);

	}
}
