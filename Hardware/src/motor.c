#include "motor.h"
#include "tim.h"



void MOTOR_MicroStep_FUN(uint8_t micro)
{
    switch(micro){

        case 0://FUU
             HAL_GPIO_WritePin(MOTOR_M0_Port,MOTOR_M0_Pin,GPIO_PIN_RESET);
             HAL_GPIO_WritePin(MOTOR_M1_Port,MOTOR_M1_Pin,GPIO_PIN_RESET);
        break;

        case 1://1/4
            HAL_GPIO_WritePin(MOTOR_M0_Port,MOTOR_M0_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_M1_Port,MOTOR_M1_Pin,GPIO_PIN_RESET);
        break;

        case 2:

        break;


    }

}
/*************************************************************************
 * 
 * Function Name:void MOTOR_Run(void)
 * Function :motor run for DRV8846 IC
 * 
 * 
 ************************************************************************/
void MOTOR_Run(void)
{
   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
   MOTOR_MicroStep_FUN(0);
   MOTOR_DIR_CCW();
   MOTOR_DRV8846_SLEEP_OFF();
   MOTOR_DRV8846_ENABLE();
   
   

 
   }



