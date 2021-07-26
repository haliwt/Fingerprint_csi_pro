#ifndef __KEY_H
#define __KEY_H
#include "gpio.h"

typedef enum{

   nothing,
   power_on,
   power_off

}pwstate;

extern uint8_t powerOn_flag;

uint8_t KEY_Scan(void);
void CheckState(uint8_t keyvalue);
//void CheckRun(void);




#endif 