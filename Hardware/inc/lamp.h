#ifndef __LAMP_H
#define __LAMP_H
#include "main.h"

#define LAMP_1_Pin      GPIO_PIN_0  //0->on 1->off
#define LAMP_1_Port     GPIOB 

#define LAMP_2_Pin      GPIO_PIN_4 
#define LAMP_2_Port     GPIOA 

#define LAMP_3_Pin      GPIO_PIN_5 
#define LAMP_3_Port     GPIOA

#define LAMP_4_Pin      GPIO_PIN_1 
#define LAMP_4_Port     GPIOA 

#define LAMP_5_Pin      GPIO_PIN_0
#define LAMP_5_Port     GPIOA 

#define LAMP_6_Pin      GPIO_PIN_15 
#define LAMP_6_Port     GPIOC 

#define LAMP_ADD_Pin      GPIO_PIN_14
#define LAMP_ADD_Port     GPIOC  

#define LAMP_POWER_Pin      GPIO_PIN_1 //Lamp suupport power EN
#define LAMP_POWER_Port     GPIOB



void LAMP_InitValue(void);

void LAMP_POWER_ON(void);
void LAMP_POWER_OFF(void);

void LAMP_WHICH_ONE_ON(uint8_t lamp);




#endif
