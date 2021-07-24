/*
 * position.c
 *
 *  Created on: Feb 5, 2021
 *      Author: om
 */


#include "adc.h"
#include "motor.h"
#include "cmd_link.h"

#define HOME_POS_FILTER_NUM		0
#define LAST_POS_FILTER_NUM		(MAX_FILTER_NUM-1)
#define ERROR_POS_FILTER_NUM	0xff

#define MOTION_STATE_PRECHECK	0
#define MOTION_STATE_TOHOME		1
#define MOTION_STATE_FIND		2

#define SENSOR_ENABLE	GPIO_PIN_RESET
#define SENSOR_DISABLE	GPIO_PIN_SET

#define AWD_SENSE_LEVEL_HIGH	1
#define AWD_SENSE_LEVEL_LOW		0

#define FILTER_SENSOR_IN_POS	AWD_SENSE_LEVEL_LOW
#define FILTER_SENSOR_OUT_POS	AWD_SENSE_LEVEL_HIGH


static uint8_t motionState;
static uint8_t filterNum,scanIndex;
static uint8_t currSenseLevel;
static uint8_t filterFindingFlag;
static uint8_t rotateDir;
volatile static uint8_t powerOnFlag;

void adcStartConvert(ADC_HandleTypeDef *hadc);
void stopFinding(void);
void stopFinding_FastMotor(void);

static void adcStopConvert(ADC_HandleTypeDef *hadc);
static void changeAWD(ADC_HandleTypeDef *hadc,uint8_t senseLevel);
static uint8_t determindDir(uint8_t destPos);
static void sensorCtrl(uint8_t en);
static void sensorCtrl_EndPos(uint8_t en);
static void sensorCtrl_StartPos(uint8_t en);


static uint16_t getRotateDelay(void);

extern void trigReportFlag(uint8_t stateCode);
extern void trigReportFlag_Debug(uint8_t stateCode);

void posDetectInit(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
	powerOnFlag=1;
	scanIndex=ERROR_POS_FILTER_NUM;
}

void posCtrl(uint8_t destPos)
{
    static uint8_t currpos=0xff;
	HAL_ADC_Stop(&hadc1);
	motionState=MOTION_STATE_PRECHECK;
	sensorCtrl(SENSOR_ENABLE);
//	HAL_Delay(500);
	rotateDir=determindDir(gInputSignal);
	filterNum=gInputSignal;//destPos;
	reportState_debug(gInputSignal);
	if(gInputSignal != currpos){
          currpos = filterNum;
	}
	else if(gInputSignal == currpos){
          currpos = gInputSignal;
	}
	HAL_Delay(100);//HAL_Delay(1000);
	if(gInputSignal == currpos){
		currpos = gInputSignal;
	 
	__HAL_ADC_DISABLE_IT(&hadc1,ADC_IT_AWD1|ADC_IT_AWD2|ADC_IT_AWD3);
	__HAL_ADC_CLEAR_FLAG(&hadc1,ADC_FLAG_AWD1|ADC_FLAG_AWD2|ADC_FLAG_AWD3);
	if(powerOnFlag)
	{
		//powerOnFlag=0;
		__HAL_ADC_ENABLE_IT(&hadc1,ADC_IT_AWD3);
		//__HAL_ADC_DISABLE_IT(&hadc1,ADC_IT_AWD1|ADC_IT_AWD2);
	}
	else
	{
		__HAL_ADC_ENABLE_IT(&hadc1,ADC_IT_AWD2|ADC_IT_AWD3);
		//__HAL_ADC_DISABLE_IT(&hadc1,ADC_IT_AWD1);
	}
	
	currSenseLevel=FILTER_SENSOR_OUT_POS;
	changeAWD(&hadc1,currSenseLevel);
	filterFindingFlag=1;
	startMotor(1,rotateDir);
	HAL_ADC_Start(&hadc1);
	}
}


void adcStartConvert(ADC_HandleTypeDef *hadc)
{
	if(filterFindingFlag)
		SET_BIT(hadc->Instance->CR,ADC_CR_ADSTART);
}

/********************************************************/
void stopFinding(void)
{
	filterFindingFlag=0;
	stopMotor(getRotateDelay());
	HAL_ADC_Stop(&hadc1);
	sensorCtrl(SENSOR_DISABLE);
}
/***************************************************************/
void stopFinding_FastMotor(void)
{
	filterFindingFlag=0;
	stopMotor(getRotateDelay());
	HAL_ADC_Stop(&hadc1);
	sensorCtrl(SENSOR_DISABLE);



}

/*****************************************************************/
static void adcStopConvert(ADC_HandleTypeDef *hadc)
{
	SET_BIT(hadc->Instance->CR,ADC_CR_ADSTP); // 1<<4 ->1 0000,ADSTP->adc stop conversion
	while((hadc->Instance->CR & ADC_CR_ADSTP) != 0UL);
}

static void changeAWD(ADC_HandleTypeDef *hadc,uint8_t senseLevel)
{
	switch(senseLevel)
	{
	case FILTER_SENSOR_IN_POS:
		hadc->Instance->TR2 = (4095<<ADC_TR1_HT1_BITOFFSET_POS)|1000;
		break;
	case FILTER_SENSOR_OUT_POS:
		hadc->Instance->TR2 = (3225<<ADC_TR1_HT1_BITOFFSET_POS)|0;
		break;
	}
}

static uint8_t determindDir(uint8_t pos)
{
	if(scanIndex>gInputSignal) 
	{
		if(scanIndex-gInputSignal<5) return RUN_CCW;
	}
	else
	{
		if(gInputSignal-scanIndex>5) return RUN_CCW;
	}
	return RUN_CW;
	
	
}

static void sensorCtrl(uint8_t en)
{
	HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, en);
	HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, en);
}
/*****************************************************
	*
	*Function Name:
	*Function: PA4 input ---> output PA7
	*
	*
*****************************************************/
static void sensorCtrl_StartPos(uint8_t en)
{
	HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, en);

}

/*************************************************************************
	*
	*Function Name:static void sensorCtrl_EndPos(uint8_t en)
	*Function :PA5 input --> OUT PUT PA6
	*
*************************************************************************/
static void sensorCtrl_EndPos(uint8_t en)
{
	HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, en);
}
/*******************************************************/

static uint16_t getRotateDelay(void)
{
	switch(rotateDir)
	{
	case RUN_CW:
		//return 1040;
		break;
	case RUN_CCW:
		//return 1040;	//50ms
		break;
	default:
		break;
	}
	return 0;
}
/******************************************************************************************
	*
	*Function Name:
	*Function :ADC 中断回调函数
	*
	*
	*
*******************************************************************************************/
//void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
//{
//	if(motionState==MOTION_STATE_PRECHECK)
//	{
//		adcStopConvert(hadc);
//		__HAL_ADC_DISABLE_IT(hadc,ADC_IT_AWD1);
//		__HAL_ADC_CLEAR_FLAG(hadc,ADC_FLAG_AWD3);
//		adcStartConvert(hadc);
//
//		motionState=MOTION_STATE_TOHOME;
//		startMotor(1,rotateDir);
//	}
//}

void HAL_ADCEx_LevelOutOfWindow2Callback(ADC_HandleTypeDef *hadc)	// filter index
{

	static uint8_t k;
   //if(motionState==MOTION_STATE_FIND)
	{
       k++;
		if(currSenseLevel==FILTER_SENSOR_IN_POS) //上限 ：高电平 不遮挡光
		{
			if(rotateDir==RUN_CW)
			{
				scanIndex++;
				if(scanIndex>=MAX_FILTER_NUM) scanIndex=0;
				
			
			}
			else
			{
				if(scanIndex==0) scanIndex=MAX_FILTER_NUM-1;
				else
				{
					scanIndex--;

				}
			}
			
			if(scanIndex==gInputSignal && k>1)
			{
                 k=0;
				 trigReportFlag_Debug('filterNum');
                 stopFinding();
				 trigReportFlag('1');
               }
			else
			{
				adcStopConvert(hadc);
				currSenseLevel=FILTER_SENSOR_OUT_POS;//下限，遮光
				changeAWD(hadc,currSenseLevel);
#ifndef USE_INHIBIT_DELAY
				adcStartConvert(hadc);
#else
				inhibitDetect();
#endif	// USE_INHIBIT_DELAY
			}
		}
		else
		{
			//adcStopConvert(hadc);
			currSenseLevel=FILTER_SENSOR_IN_POS;
			changeAWD(hadc,currSenseLevel);
			//adcStartConvert(hadc);
		}
	}
}

void HAL_ADCEx_LevelOutOfWindow3Callback(ADC_HandleTypeDef *hadc)	// home position
{
	switch(motionState)
	{
	case MOTION_STATE_PRECHECK:
	case MOTION_STATE_TOHOME:
	case MOTION_STATE_FIND:
//		if(rotateDir==RUN_CW)		scanIndex=HOME_POS_FILTER_NUM;
//		else 						scanIndex=MAX_FILTER_NUM;
		scanIndex=HOME_POS_FILTER_NUM;
		powerOnFlag=0;
		if(gInputSignal==HOME_POS_FILTER_NUM)
		{
           
			stopFinding();
			trigReportFlag('1');
		}
		else
		{
			adcStopConvert(hadc);
//			__HAL_ADC_DISABLE_IT(hadc,ADC_IT_AWD3);
			currSenseLevel=FILTER_SENSOR_OUT_POS;
			changeAWD(hadc,currSenseLevel);
			__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD2);
			__HAL_ADC_ENABLE_IT(&hadc1,ADC_IT_AWD2);
//			if(motionState==MOTION_STATE_PRECHECK)
//			{
//				__HAL_ADC_DISABLE_IT(hadc,ADC_IT_AWD1);
//				startMotor(1,rotateDir);
//			}
			motionState=MOTION_STATE_FIND;
#ifndef USE_INHIBIT_DELAY
			adcStartConvert(hadc);
#else
			inhibitDetect();
#endif	// USE_INHIBIT_DELAY
		}
		break;
	default:
		break;
	}
}

