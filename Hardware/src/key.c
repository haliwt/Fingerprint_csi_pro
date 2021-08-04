#include "key.h"


/************************************************************************
	**
	*Function Name:SideButton_SubItem_Input(void)
	*Function : 
	*Input Ref:NO
	*Retrun Ref:return key input value 0--don't pressed 1-pressed
	*
***************************************************************************/
uint8_t KEY_Scan(void)
{
	static uint16_t k11=0;
	static uint8_t cnt=0;
	uint8_t value=0;
	if(HAL_GPIO_ReadPin(POWER_KEY_Port, POWER_KEY_Pin)==0){
         if(k11<10000){
			  k11++;
		 }
	}
	else{
          k11=0;
	}

   if(k11 == 9990){
	     cnt =cnt ^ 0x01;
		if(cnt==1){
			
			value = 0x11;  //long time power pressed ---power on
			
		}
		else 
			value = 0x10; //short time power pressed ---power off
		

    }
	
    return value;
}

/************************************************************************
	**
	*Function Name:void POWER_ON(void)
	*Function : 
	*Input Ref:NO
	*Retrun Ref:return key input value 0--don't pressed 1-pressed
	*
***************************************************************************/
void POWER_ON(void)
{
      HAL_GPIO_WritePin(POWER_STATE_Port,POWER_STATE_Pin,GPIO_PIN_SET);

}
void POWER_OFF(void)
{
      HAL_GPIO_WritePin(POWER_STATE_Port,POWER_STATE_Pin,GPIO_PIN_RESET);

}
