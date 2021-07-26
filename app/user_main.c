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
#include "key.h"

extern void posCtrl(uint8_t destPos);

void user_main(void)
{
	//HAL_Delay(3000);
	uint8_t keyvalue;
	//MX_TIM3_Init(); //led pwm
	initMotor();
	cmdInit();
	HAL_Delay(500);

	while(1)
	{
   
		keyvalue = KEY_Scan();
		CheckState(keyvalue);
		CheckRun();

	}
}
