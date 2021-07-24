/*
 * uart.c
 *
 *  Created on: Sep 23, 2020
 *      Author: om
 */

#include "usart.h"
#include "motor.h"
#include "cmd_link.h"
#include "mainled.h"

//#define CMD_LINKER	huart2
#define BLE_USART	huart2




#define STATE_PREAMBLE1	0	// 'M'	fixed
#define STATE_PREAMBLE2	1	// 'X'	fixed
#define STATE_ADDR		2	// 'M' this board
#define STATE_CMD		3	// motor control command
#define STATE_SIZE		4	// length of command parameter
#define STATE_PARA		5	// parameter
#define STATE_CRC		6	// checksum

#define BUFFER_SIZE	32

#define MAX_CMD_PARA_SIZE	8

static uint8_t bleParaIndex=1;

static uint8_t currLight,currState; //WT.EDIT 


static uint8_t decodeFlag,bleDecodeFlag;


#define USART_RX_TIMEOUT	5000	// 2s
#define USART_TX_TIMEOUT	5000	// 1s


extern void posCtrl(uint8_t destPos);
extern void stopFinding(void);
extern void stopFinding_FastMotor(void);
static void initBtleModule(void); //bluletooth 



static uint8_t bleInputCmd[32];
static uint8_t bleCrcCheck;



volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run

static uint8_t nowLightState;
static uint8_t outputBuf[MAX_BUFFER_SIZE],bleOutputBuf[MAX_BUFFER_SIZE];
static uint8_t inputBuf[MAX_BUFFER_SIZE],bleBuf[MAX_BUFFER_SIZE];
static uint8_t transferSize,bleTransferSize;

static uint8_t bleTarget;
static uint8_t bleIndex;

static uint8_t swStr[3]={"+++"};
static uint8_t resetCmd[]={"AT+RESET"};
static uint8_t getAdvDataCmd[]={"AT+ADVDATA"};
static uint8_t advData[]={"AT+ADVDATA=03FF03FF"};
static uint8_t bleCmdSize;
static uint8_t state,bleState;


volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
volatile static uint8_t bleTransOngoingFlag;


static uint8_t state;
static uint8_t decodeFlag;
static uint8_t needReportFlag;
static uint8_t needReportFlag_debug;

static uint8_t returnCode;
static uint8_t transferSize;
static uint8_t outputBuf[BUFFER_SIZE];


volatile static uint8_t transOngoingFlag;


static uint8_t checkBleModuleAVDData(void);

//static void runCmd(void);

//static void reportState_debug(uint8_t stateCode);
static void trigParameterUpdateImmediate(void);
static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum);



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
static void bleRunCmd(void);
/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
static void initBtleModule(void)
{
	uint8_t tryTimes=3;

	while(tryTimes)
	{
		HAL_UART_Abort(&BLE_USART);
		HAL_Delay(50);
		HAL_UART_Transmit(&BLE_USART,swStr,3,USART_TX_TIMEOUT);
		HAL_Delay(500);
		if(checkBleModuleAVDData())	tryTimes=0;
		else tryTimes--;
	}
	HAL_UART_Abort(&BLE_USART);
}
/**************************************************************
	**
	*Function Name:static void bleRunCmd(void)
	*Function: 
	*Input Ref: 
	*Return Ref:
	*
***************************************************************/
static uint8_t checkBleModuleAVDData(void)
{
	HAL_StatusTypeDef ret;

	HAL_UART_Abort(&BLE_USART);
	HAL_UART_Transmit(&BLE_USART,getAdvDataCmd,10,USART_TX_TIMEOUT);
	ret=HAL_UART_Receive(&BLE_USART,inputBuf,24,USART_RX_TIMEOUT);
	if(ret==HAL_OK)
	{
		if(inputBuf[20]=='F' && inputBuf[21]=='F')
		{
			// switch to normal mode
			HAL_UART_Transmit(&BLE_USART,swStr,3,USART_TX_TIMEOUT);
		}
		else
		{
			HAL_UART_Transmit(&BLE_USART,advData,19,USART_TX_TIMEOUT);
			HAL_Delay(50);
			HAL_UART_Transmit(&BLE_USART,resetCmd,8,USART_TX_TIMEOUT);
			HAL_Delay(2000);
		}
		return 1;
	}
	return 0;
}


void cmdInit(void)
{
	state=STATE_PREAMBLE1;
	decodeFlag=0;
	needReportFlag=0;
	needReportFlag_debug=0;
	transOngoingFlag=0;
	bleDecodeFlag=0; //bluetooth flag
	currState= 0xff;
	currLight= 0xff;
	//HAL_UART_Abort(&CMD_LINKER);
	HAL_UART_Abort(&BLE_USART);
	//HAL_UART_Receive_IT(&CMD_LINKER,buf,1);
	//bluetooth
	initBtleModule();
	//HAL_UART_Receive_IT(&CMD_LINKER,buf,1);
	HAL_UART_Receive_IT(&BLE_USART,bleBuf,1);
}
/**************************************************************
	**
	*Function Name:void decode(void)
	*Function: 
	*Input Ref: 
	*Return Ref:
	*
***************************************************************/
void decode(void)
{

	if(bleDecodeFlag) //bluetooth to UART
	{
		bleDecodeFlag=0;
		bleRunCmd();
	}
}

void trigReportFlag(uint8_t stateCode)
{
	needReportFlag=1;
	returnCode=stateCode;
}

void trigReportFlag_Debug(uint8_t stateCode)
{
	needReportFlag_debug=1;
	returnCode=stateCode;
}

/**************************************************************
	**
	*Function Name:static void bleRunCmd(void)
	*Function: 
	*Input Ref: 
	*Return Ref:
	*
***************************************************************/
#if 0
static void runCmd(void)
{
	uint8_t cmdType=inputCmd[0];
	uint8_t paraSize=inputCmd[1];

	switch(cmdType)
	{
	case 'R':	// motor run command
		//speed=inputCmd[1];
		//dir=inputCmd[2];
		//startMotor(1,0);
		if(paraSize==1)
		{
			gInputSignal=inputCmd[2]-0x30;
			getCmdTime =1;

		}
		else if(paraSize==2)
		{
			gInputSignal=(inputCmd[2]-0x30)*10+(inputCmd[3]-0x30);
			getCmdTime =1;

		}
		else
		{
			return;	// error
		}
		if(gInputSignal>MAX_FILTER_NUM)	return;	// error

		 posCtrl(gInputSignal);
		   
		 break;
	case 'S':	// motor stop command
		stopFinding();
		break;
	default:
		break;
	}
}
#endif
/**************************************************************
	**
	*Function Name:static void bleRunCmd(void)
	*Function: 
	*Input Ref: 
	*Return Ref:
	*
***************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&BLE_USART) //蓝牙接收
	{
		switch(bleState)
		{
		case STATE_PREAMBLE1:
			if(bleBuf[0]=='M')
				bleState=STATE_PREAMBLE2;
			break;
		case STATE_PREAMBLE2:
			if(bleBuf[0]=='X')
			{
				bleState=STATE_ADDR;
			}
			else
				bleState=STATE_PREAMBLE1;
			break;
		case STATE_ADDR:
			if(bleBuf[0]==BOARD_ADDR_BT)
			{
				bleState=STATE_CMD;
			}
			else
				bleState=STATE_PREAMBLE1;
			break;
		case STATE_CMD:
			bleInputCmd[0]=bleBuf[0];
			bleCrcCheck = 0xAA ^ bleInputCmd[0];
			//decodeFlag=1;
			bleState=STATE_SIZE;
			break;
		case STATE_SIZE:
			bleCmdSize=bleBuf[0];
			if(bleCmdSize>MAX_CMD_PARA_SIZE)	// out of range
			{
				bleState=STATE_PREAMBLE1;
			}
			else if(bleCmdSize>0)
			{
				bleParaIndex=1;
				bleCrcCheck ^= bleCmdSize;
				bleState=STATE_PARA;
			}
			else	// no parameter
			{
				bleState=STATE_CRC;
			}
			break;
		case STATE_PARA:
			bleInputCmd[bleParaIndex]=bleBuf[0];
			bleCrcCheck ^= bleInputCmd[bleParaIndex];
			bleParaIndex++;
			bleCmdSize--;
			if(bleCmdSize==0) bleState=STATE_CRC;
			break;
		case STATE_CRC:
			if((bleCrcCheck ^ bleBuf[0])==0)
			{
				bleDecodeFlag=1;
			}
			bleState=STATE_PREAMBLE1;
			break;
		default:
			bleState=STATE_PREAMBLE1;
			bleDecodeFlag=0;
		}
		HAL_UART_Receive_IT(&BLE_USART,bleBuf,1);
	}
}



void reportState_debug(uint8_t stateCode)
{
	//uint8_t i,crc;

	//crc=0x55;
	outputBuf[0]=0x0A;
	
	outputBuf[1]=stateCode;	// change to ascii number
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=2;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		//HAL_UART_Transmit_IT(&CMD_LINKER,outputBuf,transferSize);
	}
}
/**************************************************************
	**
	*Function Name:static void bleRunCmd(void)
	*Function: 
	*Input Ref: 
	*Return Ref:
	*
***************************************************************/
static void bleRunCmd(void)
{
	static uint8_t stateIndex ,lightIndex;
	
	uint8_t cmdType=bleInputCmd[0];

	//static uint8_t keyBR_Counts=0;
	switch(cmdType)
	{
	case 'L':	// 0x4C,led control command
		bleTarget=bleInputCmd[1];
		bleIndex=bleInputCmd[2];
		switch(bleTarget)
		{
		case 0:
			if(bleIndex<MAX_LIGHT_NUMBER)
			{
				LedOnOff(bleIndex);
			}
			else if(bleIndex==MAX_LIGHT_NUMBER)
			{
				if(nowLightState==NOW_LIGHT_IS_ON)
				{
					 mainTurnOff_TheFirstLedA();
				}
				else
				{
					//setCurrentLightOn();
				}
				if(stateIndex !=currState || lightIndex!=currLight )//currUnion = 0xff,
				{
                        stateIndex = currState;
						lightIndex = currLight;
						notifyStatusToHost(lightIndex,stateIndex); //Smart Menu update parameter .
						//for Blue UART Transmit 
				}
				return;
			}
			break;
		case 1:
			//if(bleIndex<MAX_FILTER_NUMBER)	setEchoFilter(bleIndex);
			//else if(bleIndex==MAX_FILTER_NUMBER) brightnessAdj(BRIGHTNESS_ADJ_UP);
			//else brightnessAdj(BRIGHTNESS_ADJ_DOWN);
			break;
		case 2:
		case 3:
			if(bleTarget==3) bleIndex+=8;
			if(bleIndex<MAX_UNION_NUMBER)
			{
				//setEchoUnion(bleIndex);
			}
			break;
		default:
			break;
		}
		trigParameterUpdateImmediate();
		break;
	case 'G':	// 0x47,only get leds status
		if(stateIndex !=currState || lightIndex!=currLight )//currUnion = 0xff,
		{
                        stateIndex = currState;
						lightIndex = currLight;
						notifyStatusToHost(lightIndex,stateIndex); //Smart Menu update parameter .
						//for Blue UART Transmit 
		}
		break;
	default:
		break;
	}
}

/**********************************************************************************************************
	**
	*Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum)
	*Function : 
	*Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
	*Return Ref:NO
	*
*********************************************************************************************************/
static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum)
{
	uint8_t i,crc=0xAA;

	while(bleTransOngoingFlag);

	bleOutputBuf[0]=BOARD_ADDR_BT;  //HEX:42 
	bleOutputBuf[1]='L'; 	// leds status 'L' -HEX:4C
	bleOutputBuf[2]=lightNum; //the first group LED number on or off 
	bleOutputBuf[3]=filterNum; //filter of number 
	
    //bleOutputBuf[4]=0xff;
	//bleOutputBuf[5]=0;
	
	for(i=2;i<4;i++) crc ^= bleOutputBuf[i];
	bleOutputBuf[i]= crc;	// checksum
	bleTransferSize=i+1;

	if(HAL_UART_Transmit_IT(&BLE_USART,bleOutputBuf,bleTransferSize)==HAL_OK)
	{
		bleTransOngoingFlag=1;
	}
}

/********************************************************************************
	**
	*Function Name:void trigParameterUpdateImmediate(void)
	*Function :UART callback function  for UART interrupt for transmit data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void trigParameterUpdateImmediate(void)
{
	//checkParameterFlag=1;
	//counter_15m=(uint32_t)0L;
}

/********************************************************************************
	**
	*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for transmit data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart==&BLE_USART)
	{
		bleTransOngoingFlag=0;	// reset busy flag
	}
}

