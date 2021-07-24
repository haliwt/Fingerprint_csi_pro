/*
 * motor.c
 *
 *  Created on: Sep 22, 2020
 *      Author: om
 */

#include "motor.h"
#include "adc.h"
#include "cmd_link.h"

#define MOTOR_CTRL_TIMER		htim3
#define STEP_DELAY_TIMER		htim16
#define INHIBIT_DETECT_TIMER	htim17

extern void posDetectInit(void);
extern void adcStartConvert(ADC_HandleTypeDef *hadc);

//static void stopMotorTimer(TIM_HandleTypeDef* htim);
//static void poweronDelay(TIM_HandleTypeDef* htim);
static void initMotorTimer(TIM_HandleTypeDef* htim,uint16_t value);
static void setMicroStepsPartten(uint8_t partten);
static void setEnable(uint8_t enable);
static uint16_t MUL8x8(uint8_t a, uint8_t b);
static void turnOffPoweronDelay(TIM_HandleTypeDef *htim);

//
// stepper motor linear velocity profile
// 255 * [sqrt(n+1)-sqrt(n)]
// n = 0 to 255
//
const uint8_t StepTable[256]=
{
   0xFF, 0x69, 0x51, 0x44, 0x3C, 0x36, 0x32, 0x2E,
   0x2B, 0x29, 0x27, 0x25, 0x24, 0x22, 0x21, 0x20,
   0x1F, 0x1E, 0x1D, 0x1C, 0x1C, 0x1B, 0x1A, 0x1A,
   0x19, 0x19, 0x18, 0x18, 0x17, 0x17, 0x17, 0x16,
   0x16, 0x16, 0x15, 0x15, 0x15, 0x14, 0x14, 0x14,
   0x14, 0x13, 0x13, 0x13, 0x13, 0x12, 0x12, 0x12,
   0x12, 0x12, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
   0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E,
   0x0E, 0x0E, 0x0E, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
   0x0C, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
   0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
   0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A, 0x0A,
   0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
   0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
   0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
   0x0A, 0x0A, 0x0A, 0x09, 0x09, 0x09, 0x09, 0x09,
   0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
   0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
   0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
   0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
   0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x07, 0x07
};

static uint8_t TableIndex;		// index for stepper motor table
static uint8_t motorState;		// motor state variable
static uint16_t delayTime;		// the next step interval
static uint8_t  TZero;

static uint8_t poweronFlag;		// first time delay is 10s
static uint8_t needStopFlag;	// indicate motor need decelerate to stop
static uint16_t preDelayForStall;
void initMotor(void)
{
	volatile uint32_t ret=0;

	poweronFlag=1;

	ret=STEP_DELAY_TIMER.Instance->SR;
	if(ret) STEP_DELAY_TIMER.Instance->SR=0;
	HAL_GPIO_WritePin(SLEEP_GPIO_Port, SLEEP_Pin, GPIO_PIN_SET);	// disable sleep
	HAL_Delay(10);
	HAL_TIM_Base_Start_IT(&STEP_DELAY_TIMER);
	TZero=INIT_TZERO;
	motorState=DONE;
	setMicroStepsPartten(MICRO_STEPS_8);
	//setEnable(ENABLE_DRIVER);
	posDetectInit();
}

void startMotor(uint8_t speed,uint8_t dir)
{


	if(motorState!=DONE) return;	// already running
	if(poweronFlag)
	{
		
		 turnOffPoweronDelay(&STEP_DELAY_TIMER);
		poweronFlag=0;
	}
	setEnable(ENABLE_DRIVER);
	//HAL_Delay(2);
	if(speed) 	TZero=50;//22;//WT.EDIT //22;//40;//50;//->big motor		// high speed needed
	else		TZero=INIT_TZERO;	// normal speed
	TableIndex=1;
	//delayTime=2000;//MUL8x8(TZero,StepTable[0]);
	delayTime=2000;//MUL8x8(TZero,StepTable[0]);
	initMotorTimer(&MOTOR_CTRL_TIMER,delayTime);
	if(dir)
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, RUN_CCW);
	else
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, RUN_CW);

	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);	// reset step pin
	//HAL_GPIO_WritePin(GATE_GPIO_Port, GATE_Pin, GPIO_PIN_RESET);	// reset gate pin
	HAL_TIM_OC_Start_IT(&MOTOR_CTRL_TIMER,TIM_CHANNEL_1);
	needStopFlag=0;
	preDelayForStall=0;
	motorState=ACCELERATE;
}

void stopMotor(uint16_t preDelay)
{
	if(motorState!=DONE)
	{
		needStopFlag=1;
		preDelayForStall=preDelay;
	}
}


void inhibitDetect(void)	// in 0.1ms
{
	INHIBIT_DETECT_TIMER.Instance->CNT=0;
	INHIBIT_DETECT_TIMER.Instance->ARR=DEFAULT_INHIBIT_COUNT;
	INHIBIT_DETECT_TIMER.Instance->EGR=TIM_EGR_UG;		// reset counter and prescaler counter
	INHIBIT_DETECT_TIMER.Instance->SR=0;	// clear interrupt flag
	HAL_TIM_Base_Start_IT(&INHIBIT_DETECT_TIMER);
}

//static void stopMotorTimer(TIM_HandleTypeDef* htim)
//{
//	 __HAL_TIM_DISABLE(htim);
//}

//static void poweronDelay(TIM_HandleTypeDef* htim)
//{
//	htim->Instance->CR1 |= 0x04;	// set URS bit,only overflow can generate interrupt
//
//}


static void initMotorTimer(TIM_HandleTypeDef* htim,uint16_t value)
{
	 htim->Instance->CNT=0;
	 htim->Instance->EGR=TIM_EGR_UG;		// reset counter and prescaler counter
	 htim->Instance->CCR1=(uint32_t)value;
}

static void setMicroStepsPartten(uint8_t partten)
{
	switch(partten)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		//HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
		break;
	case 4:
	case 5:

	default:
		//HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
		break;
	}
}

static void setEnable(uint8_t enable)
{
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, enable);
}

static uint16_t MUL8x8(uint8_t a, uint8_t b)
{
   uint16_t ab;
   ab = (uint16_t) a * b;          // cast a to int to trick compiler
   return ab;                      // return int
}

static void turnOffPoweronDelay(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Stop_IT(htim);	// stop the timer
	htim->Instance->PSC=23;//71;//23;		// change to 1Mhz
	htim->Instance->ARR=100;//120; //WT.EDIT 80;		// now the delay time be set to 80us
	htim->Instance->EGR=TIM_EGR_UG;		// reset counter and prescaler counter
}


/****************************************************************************************
	*
	*
	*Function:0.65ms interrupt Tim3
	*
	*
****************************************************************************************/
//Output Compare callback in non-blocking mode.
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t tableValue;
	uint16_t offset;

	if(htim!=&MOTOR_CTRL_TIMER) return;

     
	   HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);	// set step pin to 1

	// start the delay timer(80us),so the step pin can be reset
	HAL_TIM_Base_Start_IT(&STEP_DELAY_TIMER);

	if(motorState!=DONE)
	{
		// get next step interval
		tableValue=StepTable[TableIndex];
		offset=MUL8x8(TZero,tableValue);
		delayTime+=offset;

		// write the interval to the timer's compare reg
		htim->Instance->CCR1=delayTime; //PWM duty = CCR1
	}

	switch(motorState)
	{

	case ACCELERATE:
		if(needStopFlag)
		{
			motorState=DECELERATE;
		}
		else
		{
			if(TableIndex<MAX_TABLE_INDEX)
			{
				TableIndex++;
			}
			else
			{
				motorState=SLEWING;
			}
		}
		break;
	case SLEWING:
		//delayTime+=StepTable[MAX_TABLE_INDEX];
		if(needStopFlag)
		{
			if(preDelayForStall) preDelayForStall--;
			else motorState=DECELERATE;
		}
		break;
	case DECELERATE:
		motorState=DONE;
//		if(TableIndex)  //WT.EDIT 2021.06.12
//		{
//			TableIndex--;
//		}
//		else
//		{
//			motorState=DONE;
//		}
		break;
	case DONE:
		HAL_TIM_OC_Stop_IT(htim,TIM_CHANNEL_1);
		setEnable(DISABLE_DRIVER);
		//HAL_GPIO_WritePin(GATE_GPIO_Port, GATE_Pin, GPIO_PIN_SET);	// set gate pin to 1
		break;
	default:
		break;
	}
}
/***************************************************************************************
	*
	*Function :Timer interrupt call back function
	*          Timer 3 is 0.065ms , Tim16 is 10s ,Timer17 is 500ms
	*
	*
	*
***************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&STEP_DELAY_TIMER)
	{

		
	
		if(poweronFlag)
		{	// 10s time is elapsed
			poweronFlag=0;
			turnOffPoweronDelay(htim); //WT.EDIT
		  
			//HAL_GPIO_WritePin(GATE_GPIO_Port, GATE_Pin, GPIO_PIN_SET);	// set gate pin to 1
		}
		else
		{
			// reset step pin and stop the timer
			HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
			HAL_TIM_Base_Stop_IT(htim);
			htim->Instance->CNT=0;
			htim->Instance->EGR=TIM_EGR_UG;		// reset counter and prescaler counter
			htim->Instance->SR=0;	// clear interrupt flag
		}
	}
	else if(htim==&INHIBIT_DETECT_TIMER) //50ms
	{
//		if(getCmdTime ==1){
//		     getSplitTime++;
//			 trigReportFlag(getSplitTime);
//		}
		HAL_TIM_Base_Stop_IT(htim);
		htim->Instance->CNT=0;
		htim->Instance->EGR=TIM_EGR_UG;		// reset counter and prescaler counter
		htim->Instance->SR=0;	// clear interrupt flag
		//LL_ADC_EnableIT_AWD2(hadc1.Instance);
		//LL_ADC_EnableIT_AWD3(hadc1.Instance);
		adcStartConvert(&hadc1);
	}
}

