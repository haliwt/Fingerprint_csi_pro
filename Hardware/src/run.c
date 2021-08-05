#include "run.h"
#include "lamp.h"


RUN run_t;
/************************************************************************
	**
	*Function Name:void CheckMode(uint8_t keyvalue)
	*Function : 
	*Input Ref:input key pressed value
	*Retrun Ref:
	*
***************************************************************************/
void CheckMode(uint8_t keyvalue)
{
    switch(keyvalue){

        case 0:
           if(!keyvalue)
              return;
        break;
            
        case 0x11:
			  run_t.runMode =0x11;
             
        break;

        case 0x10: //power off
           
             run_t.runMode = 0x10;
        break;


        default:

        break;
       }
}

/************************************************************************
	**
	*Function Name:void CheckMode(uint8_t keyvalue)
	*Function : 
	*Input Ref:input key pressed value
	*Retrun Ref:
	*
***************************************************************************/
void CheckRun(void)
{
    switch(run_t.runMode){

      case 0 :
         if(!run_t.runMode)
             return ;
      break;

      case 0x10:
	  	    LAMP_POWER_OFF();
            POWER_OFF();
			
      break;

      case 0x11:
           POWER_ON();
		   LAMP_WHICH_ONE_ON(6);
		   

      break;
      
      default :

      break;



    }



}
