/*
 * cmd_link.h
 *
 *  Created on: Sep 23, 2020
 *      Author: om
 */

#ifndef CMD_LINK_H_
#define CMD_LINK_H_

#define MAX_BUFFER_SIZE				32
#define MAX_CMD_PARA_SIZE			8


//#define CMD_LINKER	 huart2
#define BLE_USART	 huart2

#define BOARD_ADDR		0x43	// 'C' control board
#define BOARD_ADDR_BT	0x42	// 'B' for BT link --blue transmit

#define MAX_BUFFER_SIZE				32
#define MAX_CMD_PARA_SIZE			8


#define CMD_SUCCESS	0
#define CMD_ERROR	1

#define MOTOR_CMD_RUN	0
#define MOTOR_CMD_STOP	1

#define MOTOR_DIR_CW	0
#define MOTOR_DIR_CCW	1

#define MOTOR_SPEED_NORMAL	0
#define MOTOR_SPEED_HIGH	1




#define NOW_LIGHT_IS_OFF	0
#define NOW_LIGHT_IS_ON		1

#define MOTION_CW	1
#define MOTION_CCW	0

#define MAX_FILTER_NUMBER	10
#define MAX_LIGHT_NUMBER	7//WT.EDIT 2021.07.24//10
#define MAX_UNION_NUMBER	21//20
uint8_t nowLightState;


void cmdInit(void);
void decode(void);

void reportState_debug(uint8_t stateCode);


uint16_t getSplitTime;
uint8_t getCmdTime;
uint8_t gInputSignal;
#endif /* CMD_LINK_H_ */
