/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "motor.h"
static uint16_t Get_Adc(uint32_t ch,uint8_t rank) ;
static void SENSOR_ON(void);
ADCVALUE adc_t;
uint8_t currSensorFlag;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
   // HAL_ADC_Start(&hadc1);           
  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA6     ------> ADC1_IN6
    PA7     ------> ADC1_IN7
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA6     ------> ADC1_IN6
    PA7     ------> ADC1_IN7
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6|GPIO_PIN_7);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����?
static uint16_t Get_Adc(uint32_t ch,uint8_t rank)   
{
    ADC_ChannelConfTypeDef ADC1_Handler={0};
   // ADC_ChannelConfTypeDef sConfig = {0};
    ADC1_Handler.Channel=ch;                                   //ͨ��
    ADC1_Handler.Rank=rank;                                       //��1�����У�����1
    ADC1_Handler.SamplingTime= ADC_SAMPLETIME_79CYCLES_5;      //����ʱ��               
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_Handler);        //ͨ������
	
	
    HAL_ADC_Start(&hadc1);                               //����ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //��ѯת��
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//�������һ��ADC1�������ת�����
}
  //��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ���?
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times,uint8_t rank)
{
	uint32_t temp_val=0;
	uint8_t t;
  HAL_ADC_Start(&hadc1); //WT.EDIT 
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch,rank);
		HAL_Delay(5);
	}
	return temp_val/times;
} 

/**********************************************************
   * 
   * Function Name:void UART_TRANSMIT_TO_PC_DATA(void)
   * Function :ADC
   * 
   * 
   * 
***********************************************************/
void UART_TRANSMIT_TO_PC_DATA(void)
{
  
   static uint8_t i=0;
    uint8_t data,data1,data2,data3;
    uint16_t adcx,data0;
   	float temp;
   
	
	   i++;
	   if(i%2 ==0){
	         adcx=Get_Adc_Average(ADC_CHANNEL_7,5,ADC_REGULAR_RANK_1);//»ñÈ¡Í¨µÀ1µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù
	    }
	   else{
		
		     adcx=Get_Adc_Average(ADC_CHANNEL_6,5,ADC_REGULAR_RANK_1);//»ñÈ¡Í¨µÀ1µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù
		
		}
		    //ÏÔÊ¾ADCC²ÉÑùºóµÄÔ­Ê¼Öµ
	    data0 = adcx;
	    data= adcx>>8;
	    //data = (uint8_t)adcx;
	    data1 = data0 <<4;
	    HAL_UART_Transmit(&huart2, &data, 1, 1);
	    HAL_UART_Transmit(&huart2, &data1, 1, 1);
		  temp=(float)adcx*(3.3/4096);         //»ñÈ¡¼ÆËãºóµÄ´øÐ¡ÊýµÄÊµ¼ÊµçÑ¹Öµ£¬±ÈÈç3.1111
	    
	  
		  adcx=temp; 	   //¸³ÖµÕûÊý²¿·Ö¸øadcx±äÁ¿£¬ÒòÎªadcxÎªu16ÕûÐÎ
	    data2=(uint8_t)adcx;
      adc_t.adcOrigin_value1=data2;
		   //ÏÔÊ¾µçÑ¹ÖµµÄÕûÊý²¿·Ö£¬3.1111µÄ»°£¬ÕâÀï¾ÍÊÇÏÔÊ¾3
	    // printf("vadc = %d",data);
	    HAL_UART_Transmit(&huart2, &data2, 1, 1);
		temp-=adcx;                           //°ÑÒÑ¾­ÏÔÊ¾µÄÕûÊý²¿·ÖÈ¥µô£¬ÁôÏÂÐ¡Êý²¿·Ö£¬±ÈÈç3.1111-3=0.1111
		temp*=100;  	   //Ð¡Êý²¿·Ö³ËÒÔ1000£¬ÀýÈç£º0.1111¾Í×ª»»Îª111.1£¬Ïàµ±ÓÚ±£ÁôÈýÎ»Ð¡Êý¡£
		 //ÏÔÊ¾Ð¡Êý²¿·Ö£¨Ç°Ãæ×ª»»ÎªÁËÕûÐÎÏÔÊ¾£©£¬ÕâÀïÏÔÊ¾µÄ¾ÍÊÇ111.
	     
	    data3 =(uint8_t)temp;
      adc_t.adcOrigin_value0 = data3;
		  //printf("point= %ld\n",data);
	    HAL_UART_Transmit(&huart2, &data3, 1, 1);
		  HAL_Delay(250);	
}
/**********************************************************
   * 
   * Function Name:void SENSOR_ORIGIN_DetectPos(void)
   * Function :ADC
   * 
   * 
   * 
***********************************************************/
void SENSOR_ORIGIN_DetectPos(void)
{
    
    uint8_t data,data1,data2,data3;
    uint16_t adcx,data0;
	  float temp;
  
     adcx=Get_Adc_Average(ADC_CHANNEL_6,5,ADC_REGULAR_RANK_1);//»ñÈ¡Í¨µÀ1µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù
		
		
		    //ÏÔÊ¾ADCC²ÉÑùºóµÄÔ­Ê¼Öµ
	  //  data0 = adcx;
	  //  data= adcx>>8;
	    //data = (uint8_t)adcx;
	  //  data1 = data0 <<4;
	   // HAL_UART_Transmit(&huart2, &data, 1, 1);
	   // HAL_UART_Transmit(&huart2, &data1, 1, 1);
		  temp=(float)adcx*(3.3/4096);         //»ñÈ¡¼ÆËãºóµÄ´øÐ¡ÊýµÄÊµ¼ÊµçÑ¹Öµ£¬±ÈÈç3.1111
	    
	  
		  adcx=temp; 	   //¸³ÖµÕûÊý²¿·Ö¸øadcx±äÁ¿£¬ÒòÎªadcxÎªu16ÕûÐÎ
	    data2=(uint8_t)adcx;
      adc_t.adcOrigin_value1=data2;
		   //ÏÔÊ¾µçÑ¹ÖµµÄÕûÊý²¿·Ö£¬3.1111µÄ»°£¬ÕâÀï¾ÍÊÇÏÔÊ¾3
	    // printf("vadc = %d",data);
	    HAL_UART_Transmit(&huart2, &data2, 1, 1);
		temp-=adcx;                           //°ÑÒÑ¾­ÏÔÊ¾µÄÕûÊý²¿·ÖÈ¥µô£¬ÁôÏÂÐ¡Êý²¿·Ö£¬±ÈÈç3.1111-3=0.1111
		temp*=100;  	   //Ð¡Êý²¿·Ö³ËÒÔ1000£¬ÀýÈç£º0.1111¾Í×ª»»Îª111.1£¬Ïàµ±ÓÚ±£ÁôÈýÎ»Ð¡Êý¡£
		 //ÏÔÊ¾Ð¡Êý²¿·Ö£¨Ç°Ãæ×ª»»ÎªÁËÕûÐÎÏÔÊ¾£©£¬ÕâÀïÏÔÊ¾µÄ¾ÍÊÇ111.
	     
	    data3 =(uint8_t)temp;
      adc_t.adcOrigin_value0 = data3;
		  //printf("point= %ld\n",data);
	    HAL_UART_Transmit(&huart2, &data3, 1, 1);
		//  HAL_Delay(5);	
}
/**********************************************************
   * 
   * Function Name:void SENSOR_FilterNumbers_DetectPos(void)
   * Function :ADC
   * Input Ref:NO
   * Return Ref:NO
   * 
***********************************************************/
void SENSOR_FilterNumbers_DetectPos(void)
{
   static uint8_t i=0;
    uint8_t data,data1,data2,data3;
    uint16_t adcx,data0;
	  float temp;
   
	
	 
	  adcx=Get_Adc_Average(ADC_CHANNEL_7,5,ADC_REGULAR_RANK_1);//»ñÈ¡Í¨µÀ1µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù
	   
		//ÏÔÊ¾ADCC²ÉÑùºóµÄÔ­Ê¼Öµ
	  //  data0 = adcx;
	   // data= adcx>>8;
	    //data = (uint8_t)adcx;
	   // data1 = data0 <<4;
	   // HAL_UART_Transmit(&huart2, &data, 1, 1);
	   // HAL_UART_Transmit(&huart2, &data1, 1, 1);
		  temp=(float)adcx*(3.3/4096);         //»ñÈ¡¼ÆËãºóµÄ´øÐ¡ÊýµÄÊµ¼ÊµçÑ¹Öµ£¬±ÈÈç3.1111
	    
	  
		  adcx=temp; 	   //¸³ÖµÕûÊý²¿·Ö¸øadcx±äÁ¿£¬ÒòÎªadcxÎªu16ÕûÐÎ
	    data2=(uint8_t)adcx;
      adc_t.filterNumbers_value1=data2;
		   //ÏÔÊ¾µçÑ¹ÖµµÄÕûÊý²¿·Ö£¬3.1111µÄ»°£¬ÕâÀï¾ÍÊÇÏÔÊ¾3
	    // printf("vadc = %d",data);
	    HAL_UART_Transmit(&huart2, &data2, 1, 1);
		temp-=adcx;                           //°ÑÒÑ¾­ÏÔÊ¾µÄÕûÊý²¿·ÖÈ¥µô£¬ÁôÏÂÐ¡Êý²¿·Ö£¬±ÈÈç3.1111-3=0.1111
		temp*=100;  	   //Ð¡Êý²¿·Ö³ËÒÔ1000£¬ÀýÈç£º0.1111¾Í×ª»»Îª111.1£¬Ïàµ±ÓÚ±£ÁôÈýÎ»Ð¡Êý¡£
		 //ÏÔÊ¾Ð¡Êý²¿·Ö£¨Ç°Ãæ×ª»»ÎªÁËÕûÐÎÏÔÊ¾£©£¬ÕâÀïÏÔÊ¾µÄ¾ÍÊÇ111.
	     
	    data3 =(uint8_t)temp;
      adc_t.filterNumbers_value0 = data3;
      HAL_UART_Transmit(&huart2, &data3, 1, 1);
		  if(adc_t.sensorOrigin_flag==1){
      //    HAL_Delay(100);
         
          if(adc_t.filterNumbers_value0 < 0xc)
          {
            if(motorRunDir_flag==CCW){
                if(adc_t.sensorFilterNumbers_flag==8)
                   adc_t.sensorFilterNumbers_flag =1;
                else
                  adc_t.sensorFilterNumbers_flag ++;
            }
            else{
                
                if(adc_t.sensorFilterNumbers_flag==0){
                    adc_t.sensorFilterNumbers_flag =1; 
                    i=0;
                }
                else{
                    adc_t.sensorFilterNumbers_flag = 7 - i;
                    i++;
                }
                   
            }
          }
      }
	    HAL_UART_Transmit(&huart2, &adc_t.sensorFilterNumbers_flag, 1, 1);
		//  HAL_Delay(5);	
	   



}
/**********************************************************
   * 
   * Function Name:void SENSOR_ON(void)
   * Function :ADC
   * 
   * 
   * 
***********************************************************/
static void SENSOR_ON(void)
{
    HAL_GPIO_WritePin(SENSOR_SWITCH_Port,SENSOR_SWITCH_Pin,GPIO_PIN_SET);

}
void SENSOR_OFF(void)
{

  HAL_GPIO_WritePin(SENSOR_SWITCH_Port,SENSOR_SWITCH_Pin,GPIO_PIN_RESET);
}
/**********************************************************
   * 
   * Function Name:void Filter_Cilter_Calculate(void)
   * Function :ADC
   * 
   * 
   * 
***********************************************************/
void FilterNumbers_Calculate(void)
{
   switch(adc_t.filterNumbers){

     
     case 0:
            adc_t.sensorFilterNumbers_flag =0;

     break;
     
      case 1: //power on the first
            
            if(adc_t.filterNumbers==1){
               if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;
                  if(adc_t.adcOrigin_value0 < 0xc && adc_t.filterNumbers_value0 < 0xc){
                      MOTOR_Stop() ;
                      HAL_ADC_Stop(&hadc1); //WT.EDIT 
                       SENSOR_OFF();
                      adc_t.sensorOrigin_flag =1;
                      adc_t.sensorFilterNumbers_flag =1;
                  }

            }
          }

     break;

     case 2: //filter-number 2
            if(adc_t.sensorFilterNumbers_flag==2){
              if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;
                MOTOR_Stop() ;
                HAL_ADC_Stop(&hadc1); //WT.EDIT 
                
              }
            }  
     break;

     case 3: //filter-number 3
           if(adc_t.sensorFilterNumbers_flag==3){
             if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;
                 MOTOR_Stop() ;
                HAL_ADC_Stop(&hadc1); //WT.EDIT 
                
              }
           }
       break;

       case 4:
             if(adc_t.sensorFilterNumbers_flag==4){
               if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;
                 MOTOR_Stop() ;
                HAL_ADC_Stop(&hadc1); //WT.EDIT 
                
              }
             }
       break;

       case 5:
             if(adc_t.sensorFilterNumbers_flag==5){
                if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;

                 MOTOR_Stop() ;
                HAL_ADC_Stop(&hadc1); //WT.EDIT 
                
              }
             }
       break;

        case 6:
             if(adc_t.sensorFilterNumbers_flag==6){
               if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;
                 MOTOR_Stop() ;
                HAL_ADC_Stop(&hadc1); //WT.EDIT 
                
              }
             }
       break;

        case 7:
             if(adc_t.sensorFilterNumbers_flag==7){
               if(currSensorFlag !=adc_t.sensorFilterNumbers_flag){
                 currSensorFlag = adc_t.sensorFilterNumbers_flag;
                 MOTOR_Stop() ;
                HAL_ADC_Stop(&hadc1); //WT.EDIT 
                
              }
             }
       break;

}

}


void ADC_InitValue(void)
{
   motorRunDir_flag=CCW;
   adc_t.filterNumbers=1;
   currSensorFlag=0xff;
   SENSOR_ON();
   HAL_ADC_Start(&hadc1); //WT.EDIT 
    MOTOR_Run();

}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
