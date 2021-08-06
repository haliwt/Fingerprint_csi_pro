#include "lamp.h"
#include "main.h"
#include "bletooth.h"

LAMP lamp_t;
uint8_t nowLightState;
void LAMP_InitValue(void)
{
    HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LAMP_POWER_Port,LAMP_POWER_Pin,GPIO_PIN_RESET);
	
}




void LAMP_POWER_ON(void)
{
    HAL_GPIO_WritePin(LAMP_POWER_Port,LAMP_POWER_Pin, GPIO_PIN_SET);
	nowLightState=NOW_LIGHT_IS_ON;
}
void LAMP_POWER_OFF(void)
{
	HAL_GPIO_WritePin(LAMP_POWER_Port,LAMP_POWER_Pin, GPIO_PIN_RESET);
	nowLightState=NOW_LIGHT_IS_OFF;
}
/**********************************************************************
	*
	*Function Name:void LAMP_WHICH_ONE_ON(void)
	*Function :
	*
	*
	*
**********************************************************************/
void LAMP_WHICH_ONE_ON(uint8_t lampnum)
{
	switch(lampnum){

      case 0:
			nowLightState=NOW_LIGHT_IS_OFF;

	  break;

	  case 1:

	        if(nowLightState != NOW_LIGHT_IS_ON){

	        
		  	    LAMP_POWER_OFF();
			
			  	HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_SET);

		  
				HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_RESET);
			 	LAMP_POWER_ON();
				nowLightState=NOW_LIGHT_IS_ON;
				lamp_t.lampNum =1;
	        }

      break;

	  case 2:
	  	    if(nowLightState != NOW_LIGHT_IS_ON){
		  	    LAMP_POWER_OFF();
				
	  	        HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_SET);

	  		    HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_RESET);
		
				HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_RESET);
		 		LAMP_POWER_ON();
				nowLightState=NOW_LIGHT_IS_ON;
				lamp_t.lampNum =2;
	  	    }

	  break;

	  case 3:
	  		if(nowLightState != NOW_LIGHT_IS_ON){
		  	   LAMP_POWER_OFF();
		  	   HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_SET);

		        HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_RESET);
			
				HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_RESET);
			    LAMP_POWER_ON();
				nowLightState=NOW_LIGHT_IS_ON;
				lamp_t.lampNum =3;
	  		}

	  break;

	  case 4:
	  	if(nowLightState != NOW_LIGHT_IS_ON){
		  	  LAMP_POWER_OFF();
		  	  HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_SET);

			   HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_RESET);
			   HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_RESET);
			   HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_RESET);
			 
			   HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_RESET);
			   HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_RESET);

		  
			   LAMP_POWER_ON();
			   nowLightState=NOW_LIGHT_IS_ON;
			   lamp_t.lampNum =4;
	  		}

	  break;

	  case 5:
	  	if(nowLightState != NOW_LIGHT_IS_ON){
		  	 LAMP_POWER_OFF();
		  	 HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_SET);

			  HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_RESET);
			
			  HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_RESET);
			   LAMP_POWER_ON();
			   nowLightState=NOW_LIGHT_IS_ON;
			   lamp_t.lampNum =5;
	  		}
	  break;

	  case 6:
	  	if(nowLightState != NOW_LIGHT_IS_ON){
	  		LAMP_POWER_OFF();
	  		HAL_GPIO_WritePin(LAMP_6_Port,LAMP_6_Pin,GPIO_PIN_SET);

		  	HAL_GPIO_WritePin(LAMP_1_Port,LAMP_1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LAMP_2_Port,LAMP_2_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LAMP_3_Port,LAMP_3_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LAMP_4_Port,LAMP_4_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LAMP_5_Port,LAMP_5_Pin,GPIO_PIN_RESET);
		
			LAMP_POWER_ON();
			nowLightState=NOW_LIGHT_IS_ON;
			lamp_t.lampNum =6;
	  		}

	  break;

	  default :
		nowLightState=NOW_LIGHT_IS_OFF;
		lamp_t.lampNum =0;

	  break;

	


	}


}






