#include "key.h"
#include "cmd_link.h"


uint8_t powerOn_flag;


/*****************************************************************************************************
	**
	*Function Name:uint8_t KEY_Scan(void)
	*Function : 
	*
	*
	*
******************************************************************************************************/
uint8_t KEY_Scan(void)
{
	static uint16_t k1=0;

	uint8_t value=0;
	if(HAL_GPIO_ReadPin(POWER_KEY_GPIO_Port, POWER_KEY_Pin)==0){

        if(k1<10000){
			k1++;
        }
		if(k1 >10000 && k1<20000){
			k1++;
        }

    }
	else{
          k1=0;
  
	}
    
    if(k1==9990){
	   	 value = 1;
	    k1=0;
	    return value;

    }
	if(k1==19990){
      value =2 ;
	  k1=0;
	  return value ;

	}
    return 0;
}

/************************************************************
	**
	*Function Name:void CheckState(uint8_t keyvalue)
	*Function : 
	*
	*
	*
************************************************************/
void CheckState(uint8_t keyvalue)
{
    switch(keyvalue){

	   case 0:
            powerOn_flag =0;
	   break;

	   case 1:
            powerOn_flag =1;
	   break;

	   case 2:
	   	   powerOn_flag = 2;

	   break;

	   default:

	   break;


    }

}

/************************************************************
	**
	*Function Name:void CheckRun(void)
	*Function : 
	*
	*
	*
************************************************************/
void CheckRun(void)
{
   switch(powerOn_flag){

     case nothing:

     break;


	 case power_on:
	 	    HAL_GPIO_WritePin(POWER_STATE_GPIO_Port,POWER_STATE_Pin, GPIO_PIN_SET);
			decode();
			HAL_GPIO_WritePin(LEDAPWM_GPIO_Port,LEDAPWM_Pin, GPIO_PIN_SET);
			//LedOn(2);
			HAL_GPIO_WritePin(LEDA1_GPIO_Port, LEDA1_Pin, GPIO_PIN_SET);

	 break;

	 case power_off:
	 	  HAL_GPIO_WritePin(POWER_STATE_GPIO_Port,POWER_STATE_Pin, GPIO_PIN_RESET);

	 break;
	 

   	}
   	



}

