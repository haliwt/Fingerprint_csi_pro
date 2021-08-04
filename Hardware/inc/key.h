#ifndef __KEY_H
#define __KEY_H
#include "main.h"

#define POWER_KEY_Pin   GPIO_PIN_8
#define POWER_KEY_Port  GPIOA 

#define POWER_STATE_Pin     GPIO_PIN_6 //high
#define POWER_STATE_Port    GPIOC

uint8_t KEY_Scan(void);

void POWER_ON(void);
void POWER_OFF(void);




#endif 



