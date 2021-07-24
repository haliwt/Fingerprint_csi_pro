/*
 * user_main.c
 *
 *  Created on: Sep 22, 2020
 *      Author: om
 */

#include "motor.h"
#include "cmd_link.h"

extern void posCtrl(uint8_t destPos);

void user_main(void)
{
	//HAL_Delay(3000);
	initMotor();
	cmdInit();
	HAL_Delay(500);
	while(1)
	{
		decode();

	}
}
