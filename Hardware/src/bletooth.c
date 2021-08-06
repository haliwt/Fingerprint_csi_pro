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

#define BOARD_ADDR		    	0x43	// 'C' control board
#define BOARD_ADDR_BT	    	0x42	// 'B' for BT link --blue transmit
#define BOARD_ADDR_FILTER    	0x46    // 'F' Filter

uint8_t inputBuf[MAX_BUFFER_SIZE];

static uint8_t bleInputCmd[32];
static uint8_t bleState;
static uint8_t bleCmdSize;
static uint8_t bleParaIndex;
static uint8_t bleCrcCheck;
static uint8_t currFilter,currLamp;

static uint8_t bleTarget;
static uint8_t bleIndex;

static uint8_t nowLightState;

static uint8_t bleTransferSize;

uint8_t bleBuf[MAX_BUFFER_SIZE];
uint8_t outputBuf[MAX_BUFFER_SIZE];
uint8_t bleOutputBuf[MAX_BUFFER_SIZE];
static uint8_t bleTransferSize;

volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
volatile static uint8_t bleTransOngoingFlag;

static uint8_t swStr[3]={"+++"};
static uint8_t resetCmd[]={"AT+RESET"};
static uint8_t getAdvDataCmd[]={"AT+ADVDATA"};
static uint8_t advData[]={"AT+ADVDATA=03FF03FF"};

uint32_t counter_15m;

static uint8_t checkParameterFlag;

static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum);
static void trigParameterUpdateImmediate(void);
static void initBtleModule(void);
static uint8_t checkBleModuleAVDData(void);
/****************************************************************************************************
    **
    *Function Name:void Decode(void)
    *Function: 
    *Input Ref: 
    *Return Ref:
    *
****************************************************************************************************/
void Bluetooth_Decode(void)
{
    if(run_t.bleDecodeFlag){
        run_t.bleDecodeFlag =0;
		 bleRunCmd();

    }

}
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

	uint8_t cmdType=bleInputCmd[0];
    switch(cmdType)
	{
	case 'L':	// ‘L’->0x4C,led control command
		bleTarget=bleInputCmd[1];
		bleIndex=bleInputCmd[2]; //blue 
		switch(bleTarget)
		{
			case 0:
			    if(bleIndex<MAX_LIGHT_NUMBER) //lamp 4 group
				{
                    if(currLamp != bleIndex){
						currLamp = bleIndex;
						LAMP_WHICH_ONE_ON(bleIndex);//setEchoLight(bleIndex);
                    }
				}
				else if(bleIndex >=MAX_LIGHT_NUMBER)
				{
					if(nowLightState==NOW_LIGHT_IS_ON)
					{
						LAMP_POWER_OFF();
					}
					else
					{
						LAMP_POWER_ON();
					}
					notifyStatusToHost(((nowLightState==NOW_LIGHT_IS_ON) ? lamp_t.lampNum  : 0xff ),adc_t.filterNumbers);
					return;
				}
				break;
		
			
			default:
				break;
			}
			trigParameterUpdateImmediate();
	break;

	case 'F': //filter -> 0x46
		bleTarget=bleInputCmd[1];
		bleIndex=bleInputCmd[2]; //blue  
		switch(bleTarget){

			case 0:
				if(bleIndex<MAX_FILTER_NUMBER){
					if(currFilter !=bleIndex){
					    currFilter= bleIndex;
						adc_t.sensorOn_flag =1;
					    SENSOR_AND_ADC_Start(adc_t.sensorOn_flag);
					    FilterNumbers_Calculate();	//setEchoFilter(bleIndex);
                    }
					else{ 
						adc_t.sensorOn_flag =0;
						SENSOR_AND_ADC_Start(adc_t.sensorOn_flag);
						adc_t.filterNumbers=bleIndex;
					}
					
				}

			break;
		}
	break;
	
	case 'G':	// 0x47,only get leds status
		notifyStatusToHost(((nowLightState==NOW_LIGHT_IS_ON) ? lamp_t.lampNum  : 0xff ),adc_t.filterNumbers);
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
static void initBtleModule(void)
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

/**********************************************************************************************************
    **
    *Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum,uint8_t unionNum)
    *Function : 
    *Input Ref:lightNum--LED filterNum -filter number
    *Return Ref:NO
    *
*********************************************************************************************************/
static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum)
{
	uint8_t i,crc=0xAA;

	while(bleTransOngoingFlag);

	bleOutputBuf[0]=BOARD_ADDR_BT;  //HEX:0x42 ->'B'
	bleOutputBuf[1]='L'; 	// leds status 'L' -HEX:0X4C
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
/**********************************************************************************************************
    **
    *Function Name:void BlueCmdInit(void)
    *Function : BlueTooth initialize reference parameter
    *Input Ref:NO
    *Return Ref:NO
    *
*********************************************************************************************************/
void BlueCmdInit(void)
{
    
	currFilter = 0xff;
	currLamp = 0xff;
	bleState=STATE_PREAMBLE1;
    transOngoingFlag=0; //UART run
	bleTransOngoingFlag=0;
	nowLightState=NOW_LIGHT_IS_OFF;

    HAL_UART_Abort(&huart2);
	initBtleModule();
    
    HAL_UART_Receive_IT(&huart2,bleBuf,1);
	
}
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback interrupt for receive BLUETOOTH DATA
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
			if(bleBuf[0]=='M') //‘M’ -> 0x4d
				bleState=STATE_PREAMBLE2;
			break;
		case STATE_PREAMBLE2:
			if(bleBuf[0]=='X') //'X' ->0x58
			{
				bleState=STATE_ADDR;
			}
			else
				bleState=STATE_PREAMBLE1;
			break;
		case STATE_ADDR:
			if(bleBuf[0]==BOARD_ADDR_BT) //0x42->'B'
			{
				bleState=STATE_CMD;
			}
			else
				bleState=STATE_PREAMBLE1;
			break;
		case STATE_CMD:
			bleInputCmd[0]=bleBuf[0]; //蓝牙发送命令字节——> 0x4c or 0x47 or 0x41 or 0x46
			bleCrcCheck = 0xAA ^ bleInputCmd[0];
			//decodeFlag=1;
			bleState=STATE_SIZE;
			break;
		case STATE_SIZE:
			bleCmdSize=bleBuf[0]; //蓝牙发送数据参数长度 命令字节0x4c ->长度是00～03
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
/********************************************************************************
	**
	*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt 
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart2)
	{
		bleTransOngoingFlag=0;	// reset busy flag
	}
}