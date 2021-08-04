#ifndef __RUN_H
#define __RUN_H
#include "main.h"
#include "key.h"


typedef struct {

     uint8_t Lampitem;
     uint8_t power_state;
	 uint8_t runMode;


}RUN;

extern RUN run_t;

void CheckMode(uint8_t keyvalue);
void CheckRun(void);




#endif 


