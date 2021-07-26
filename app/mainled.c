#include "mainled.h"
#include "usart.h"
#include "pwm.h"
#include "gpio.h"
#include "cmd_link.h"

mainled_t mainled;

static uint8_t hasLedOn;
//static uint8_t level_a;
//static uint8_t level_b;


static void setLevel_PWMA(uint8_t level);



/*************************************************************************
 	*
	*Function Name: void LedOnOff(void)
	*Function : Turn On LEDAxx LED
	*Input Ref: receive UART of data
	*Output Ref:No
	*
******************************************************************************/
void LedOn(uint8_t ledNum)
{

	mainled.pwmDutyCycle_ch22=90;
  
	 switch(ledNum){

	           case 0:
                    nowLightState=NOW_LIGHT_IS_OFF;
	           break;
	   	
			   case 1://PB0-[1]-----oled menu "UV365" {1}
				mainled.led_by_a = 1;
				mainled.led_by_b = 0;
				mainTurnOff_TheFirstLedA();
				HAL_Delay(20);
                //turn on LEDA4
                HAL_GPIO_WritePin(LEDA1_GPIO_Port, LEDA1_Pin, GPIO_PIN_SET);
			    nowLightState=NOW_LIGHT_IS_ON;
				
			    setLevel_PWMA(mainled.pwmDutyCycle_ch22);
			    //  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2) ;
                break;

            case 2: //PA4-[2]-----oled menu "Violet" {2}
				mainled.led_by_a = 1;
				mainled.led_by_b = 0;
				mainTurnOff_TheFirstLedA();
				HAL_Delay(20);
                //turn on LEDA5
                HAL_GPIO_WritePin(LEDA2_GPIO_Port, LEDA2_Pin, GPIO_PIN_SET);      //1.the first turn on LED
				 setLevel_PWMA(mainled.pwmDutyCycle_ch22);
			    // HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2) ;  //2.the second turn on Enable
			    nowLightState=NOW_LIGHT_IS_ON;
                break;

            case 3://PA5---[3]---"white" oled {0}  //
				mainled.led_by_a = 1;
				mainled.led_by_b = 0;
			
				mainTurnOff_TheFirstLedA();
				HAL_Delay(20);
                //turn on LEDA6
                HAL_GPIO_WritePin(LEDA3_GPIO_Port, LEDA3_Pin, GPIO_PIN_SET);
				nowLightState=NOW_LIGHT_IS_ON;
				setLevel_PWMA(mainled.pwmDutyCycle_ch22);
				//HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2) ; //2.the second turn on Enable
			    
                break;

            case 4: //PA1-[4]----oled menu "Green"--{6}
				mainled.led_by_a = 1;
				mainled.led_by_b = 0;
				mainTurnOff_TheFirstLedA();
				HAL_Delay(20);
                //turn on LEDA7
                HAL_GPIO_WritePin(LEDA4_GPIO_Port, LEDA4_Pin, GPIO_PIN_SET);
				nowLightState=NOW_LIGHT_IS_ON;
				setLevel_PWMA(mainled.pwmDutyCycle_ch22);
				//HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2) ;//2.the second turn on Enable
			    
                break;

            case 5://PA0-[5]----oled "Cyan"--{5}
				mainled.led_by_a = 1;
				mainled.led_by_b = 0;
				mainTurnOff_TheFirstLedA();
				HAL_Delay(20);
                //turn on LEDA8
                HAL_GPIO_WritePin(LEDA5_GPIO_Port, LEDA5_Pin, GPIO_PIN_SET);
				nowLightState=NOW_LIGHT_IS_ON;
				setLevel_PWMA(mainled.pwmDutyCycle_ch22);
				//HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2) ;  //2.TIM2_CH2
			    
            break;
			
		   //LEDB -The second group 
		    case 6://PC14 --[6]-----olde "640"-{9}
				mainled.led_by_b = 1;
				mainled.led_by_a = 0;
				mainTurnOff_TheFirstLedA();
				HAL_Delay(20);
				//1.turn on LEDB1 =1
                HAL_GPIO_WritePin(LEDA6_GPIO_Port, LEDA6_Pin, GPIO_PIN_SET);
				nowLightState=NOW_LIGHT_IS_ON;
                setLevel_PWMA(mainled.pwmDutyCycle_ch22);

                break;

		   default :
                nowLightState=NOW_LIGHT_IS_OFF;

		   break;

           
	   
           
	   }
	  
  }


/*************************************************************************
 	*
	*Function Name:void TurnOff_TheFirstLedA(void)
	*Function : 
	*Input Ref: NO
	*Output Ref:No
	*
******************************************************************************/
void mainTurnOff_TheFirstLedA(void)
{
     //turn off LEDA LED
	   HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4) ; //HAL_GPIO_WritePin(LEDAPWM_GPIO_Port, LEDAPWM_Pin, GPIO_PIN_RESET);//TIM2_CH2
	   HAL_Delay(20);
	   HAL_GPIO_WritePin(GPIOA,  LEDA2_Pin | LEDA3_Pin | LEDA4_Pin | LEDA5_Pin , GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOB, LEDA1_Pin , GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOC, LEDA6_Pin , GPIO_PIN_RESET);
	   mainled.pwmDutyCycle_ch22=LEVEL_DEFAULT; //WT.EDIT 2021.07.10
	   nowLightState=NOW_LIGHT_IS_OFF;
	   MX_TIM3_Init();//WT.EDIT 2021.07.10

}

/*************************************************************************
 	*
	*Function Name:
	*Function : 
	*Input Ref: NO
	*Output Ref:No
	*
******************************************************************************/
//static uint8_t BCC_CHECK(void)
//{
//   uint8_t i;
//	 
//	 uint8_t tembyte =0xAA ^ aRxBuffer[2];
//	
//    for (i = 3; i <6; i++) {
//        tembyte = tembyte ^ aRxBuffer[i];
//    }
//    return tembyte;

//}

/*************************************************************************
 	*
	*Function Name:
	*Function : 
	*Input Ref: NO
	*Output Ref:No
	*
******************************************************************************/

static void setLevel_PWMA(uint8_t levelval)
{
   // mainled.pwmDutyCycle_ch22 = levelval;
    if(mainled.pwmDutyCycle_ch22 <LEVEL_MIN) mainled.pwmDutyCycle_ch22=LEVEL_ZERO;;
	if(mainled.pwmDutyCycle_ch22 > LEVEL_PWM_MAX)mainled.pwmDutyCycle_ch22 = LEVEL_PWM_MAX;
	
	MX_TIM3_Init();
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4) ;  //2.the second turn on Enable time3 ch_4

}
/*************************************************************************
 	*
	*Function Name:void changeBrightness(uint8_t dir)
	*Function : 
	*Input Ref: NO
	*Output Ref:No
	*
******************************************************************************/
void changeBrightness(uint8_t dir)
{
	if(hasLedOn)
	{
		if(mainled.led_by_a == 1){
			if(dir=='1')	// adj +
			{
				//level_a+=LEVEL_STEP;
				mainled.pwmDutyCycle_ch22 +=LEVEL_STEP;
				//if(level_a>LEVEL_PWM_MAX ) level_a=LEVEL_PWM_MAX ;
				if(mainled.pwmDutyCycle_ch22 >LEVEL_PWM_MAX)mainled.pwmDutyCycle_ch22 =LEVEL_PWM_MAX;
			}
			else	// adj -
			{
				if(mainled.pwmDutyCycle_ch22 < LEVEL_MIN)mainled.pwmDutyCycle_ch22=LEVEL_ZERO;//if(level_a<LEVEL_MIN)	level_a=LEVEL_MIN;
				else if(mainled.pwmDutyCycle_ch22< LEVEL_STEP){
					mainled.pwmDutyCycle_ch22=LEVEL_ZERO;
				}
				else 	mainled.pwmDutyCycle_ch22-=LEVEL_STEP;
			}
			setLevel_PWMA(mainled.pwmDutyCycle_ch22);
		}
		if(mainled.led_by_b == 1){
			if(dir=='1')	// adj +
			{
				mainled.pwmDutyCycle_ch12 += LEVEL_STEP;//level_b+=LEVEL_STEP;
				//if(level_b>LEVEL_PWM_MAX ) level_b=LEVEL_PWM_MAX ;
				if(mainled.pwmDutyCycle_ch12 > LEVEL_PWM_MAX)mainled.pwmDutyCycle_ch12 =LEVEL_PWM_MAX;
			}
			else	// adj -
			{
				if(mainled.pwmDutyCycle_ch12 <LEVEL_MIN)	mainled.pwmDutyCycle_ch12 =LEVEL_ZERO;
				else if(mainled.pwmDutyCycle_ch12  < LEVEL_STEP) mainled.pwmDutyCycle_ch12 =LEVEL_ZERO;
				else 	
					mainled.pwmDutyCycle_ch12 -=LEVEL_STEP;
			}
			setLevel_PWMA(mainled.pwmDutyCycle_ch22);
		}
	}
}


