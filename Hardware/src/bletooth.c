#include "bletooth.h"
#include "usart.h"
#include "run.h"
#include "adc.h"


#define STATE_PREAMBLE1	0	// 'M'	fixed
#define STATE_PREAMBLE2	1	// 'X'	fixed
#define STATE_ADDR		2	// 'M' motor board , 'L' light board
#define STATE_CMD		3	// control command
#define STATE_SIZE		4	// length of command parameter
#define STATE_PARA		5	// parameter
#define STATE_CRC		6	// checksum
#define MAX_CMD_PARA_SIZE			8
#define USART_RX_TIMEOUT	5000	// 2s
#define USART_TX_TIMEOUT	5000	// 1s

#define BOARD_ADDR		0x43	// 'C' control board
#define BOARD_ADDR_BT	0x42	// 'B' for BT link --blue transmit

uint8_t inputBuf[MAX_BUFFER_SIZE];

static uint8_t inputCmd[32],bleInputCmd[32];
static uint8_t bleState;
static uint8_t bleCmdSize;
static uint8_t bleParaIndex;
static uint8_t bleCrcCheck;


static uint8_t bleTarget;
static uint8_t bleIndex;

static uint8_t nowLightState;
static uint8_t currLight,currFilter; //WT.EDIT 
static uint8_t bleTransferSize;

uint8_t bleBuf[MAX_BUFFER_SIZE];
uint8_t outputBuf[MAX_BUFFER_SIZE];
uint8_t bleOutputBuf[MAX_BUFFER_SIZE];


volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
volatile static uint8_t bleTransOngoingFlag;

static uint8_t swStr[3]={"+++"};
static uint8_t resetCmd[]={"AT+RESET"};
static uint8_t getAdvDataCmd[]={"AT+ADVDATA"};
static uint8_t advData[]={"AT+ADVDATA=03FF03FF"};

static uint32_t counter_15m;
static uint8_t checkParameterFlag;

static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum);
static void trigParameterUpdateImmediate(void);
static uint8_t checkBleModuleAVDData(void);
/****************************************************************************************************
    **
    *Function Name:static void bleRunCmd(void)
    *Function: 
    *Input Ref: 
    *Return Ref:
    *
****************************************************************************************************/
 void bleRunCmd(void)
{
//	uint8_t transfeSize=0;
//	uint8_t ledGroup,ledIndex;
	uint8_t cmdType=bleInputCmd[0];

	//static uint8_t keyBR_Counts=0;
	switch(cmdType)
	{
	case 'L':	// 0x4C,led control command
		bleTarget=bleInputCmd[1];
		bleIndex=bleInputCmd[2]; //blue 
		switch(bleTarget)
		{
		case 0:
			if(bleIndex<MAX_LIGHT_NUMBER) //lamp 4 group
			{
				LAMP_WHICH_ONE_ON(bleIndex);//setEchoLight(bleIndex);
			}
			else if(bleIndex==MAX_LIGHT_NUMBER)
			{
				if(nowLightState==NOW_LIGHT_IS_ON)
				{
					LAMP_POWER_OFF();
				}
				else
				{
					LAMP_POWER_ON();
				}
				notifyStatusToHost(((nowLightState==NOW_LIGHT_IS_ON) ? currLight : 0xff ),currFilter);
				return;
			}
			break;
		case 1: //filter 7 pcs 
			if(bleIndex<MAX_FILTER_NUMBER){
				 adc_t.filterNumbers=bleIndex;
				FilterNumbers_Calculate();	//setEchoFilter(bleIndex);
			}
			//else if(bleIndex==MAX_FILTER_NUMBER) brightnessAdj(BRIGHTNESS_ADJ_UP);
			//else brightnessAdj(BRIGHTNESS_ADJ_DOWN);
			break;
		case 2:
		
		default:
			break;
		}
		trigParameterUpdateImmediate();
		break;
	case 'G':	// 0x47,only get leds status
		notifyStatusToHost(((nowLightState==NOW_LIGHT_IS_ON) ? currLight : 0xff ),currFilter);
		break;
	default:
		break;
	}
}
/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void initBtleModule(void)
{
	uint8_t tryTimes=3;

	while(tryTimes)
	{
		HAL_UART_Abort(&huart2);
		HAL_Delay(50);
		HAL_UART_Transmit(&huart2,swStr,3,USART_TX_TIMEOUT);
		HAL_Delay(500);
		if(checkBleModuleAVDData())	tryTimes=0;
		else tryTimes--;
	}
	HAL_UART_Abort(&huart2);
}

static uint8_t checkBleModuleAVDData(void)
{
	HAL_StatusTypeDef ret;

	HAL_UART_Abort(&huart2);
	HAL_UART_Transmit(&huart2,getAdvDataCmd,10,USART_TX_TIMEOUT);
	ret=HAL_UART_Receive(&huart2,inputBuf,24,USART_RX_TIMEOUT);
	if(ret==HAL_OK)
	{
		if(inputBuf[20]=='F' && inputBuf[21]=='F')
		{
			// switch to normal mode
			HAL_UART_Transmit(&huart2,swStr,3,USART_TX_TIMEOUT);
		}
		else
		{
			HAL_UART_Transmit(&huart2,advData,19,USART_TX_TIMEOUT);
			HAL_Delay(50);
			HAL_UART_Transmit(&huart2,resetCmd,8,USART_TX_TIMEOUT);
			HAL_Delay(2000);
		}
		return 1;
	}
	return 0;
}

/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart==&huart2) //蓝牙接收
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
				run_t.bleDecodeFlag=1;
			}
			bleState=STATE_PREAMBLE1;
			break;
		default:
			bleState=STATE_PREAMBLE1;
			run_t.bleDecodeFlag=0;
		}
		HAL_UART_Receive_IT(&huart2,bleBuf,1);
	}
}
/**********************************************************************************************************
    **
    *Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum,uint8_t unionNum)
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
	
	for(i=2;i<4;i++) crc ^= bleOutputBuf[i];
	bleOutputBuf[i]= crc;	// checksum
	bleTransferSize=i+1;

	if(HAL_UART_Transmit_IT(&huart2,bleOutputBuf,bleTransferSize)==HAL_OK)
	{
		bleTransOngoingFlag=1;
	}
}
/**********************************************************************************************************
    **
    *Function Name:void trigParameterUpdateImmediate(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
static void trigParameterUpdateImmediate(void)
{
	checkParameterFlag=1;
	counter_15m=(uint32_t)0L;
}