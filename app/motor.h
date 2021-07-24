/*
 * motor.h
 *
 *  Created on: Sep 22, 2020
 *      Author: om
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "tim.h"

#define RUN_CW	0		// rotate directory
#define RUN_CCW	1

#define ACCELERATE	3	 // motor states
#define SLEWING 	2
#define DECELERATE 	1
#define DONE 		0

#define MAX_TABLE_INDEX		0xff
#define INIT_TZERO			65

#define MICRO_STEPS_FULL	0
#define MICRO_STEPS_HALF	1
#define MICRO_STEPS_4		2
#define MICRO_STEPS_8		3
#define MICRO_STEPS_16		4
#define MICRO_STEPS_32		5

#define ENABLE_DRIVER	0
#define DISABLE_DRIVER	1

#define MAX_FILTER_NUM		10

#define DEFAULT_INHIBIT_COUNT	99//1800// 2000//3500	// 0.35s
#define USE_INHIBIT_DELAY //WT.EDIT 20210612

void initMotor(void);
void startMotor(uint8_t speed,uint8_t dir);
void stopMotor(uint16_t preDelay);
void inhibitDetect(void);

#endif /* MOTOR_H_ */
