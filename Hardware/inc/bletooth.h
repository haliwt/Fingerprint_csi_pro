#ifndef __BLETOOTH_H
#define __BLETOOTH_H
#include "main.h"
#include "usart.h"
#include "lamp.h"

#define MAX_FILTER_NUMBER	7 // 7pcs filter
#define MAX_LIGHT_NUMBER	4 //4 group WT.EDIT 

#define NOW_LIGHT_IS_OFF	0
#define NOW_LIGHT_IS_ON		1
#define MAX_BUFFER_SIZE				32

extern uint8_t nowLightState;


extern uint8_t outputBuf[MAX_BUFFER_SIZE];
extern uint8_t bleOutputBuf[MAX_BUFFER_SIZE];
extern uint8_t bleBuf[MAX_BUFFER_SIZE];
extern uint8_t inputBuf[MAX_BUFFER_SIZE];

extern uint32_t counter_15m;



void bleRunCmd(void);

void BlueCmdInit(void);

void Bluetooth_Decode(void);



#endif 

