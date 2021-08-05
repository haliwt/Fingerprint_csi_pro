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
static uint16_t Get_Adc(uint32_t ch,uint8_t rank) ;
static void SENSOR_ON(void);
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
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
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
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
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

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 2);
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

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
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
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
   SENSOR_ON();
   HAL_ADC_Start(&hadc1); //WT.EDIT 
	
	   i++;
	   //if(i%2 ==0){
	         adcx=Get_Adc_Average(ADC_CHANNEL_7,5,ADC_REGULAR_RANK_1);//»ñÈ¡Í¨µÀ1µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù
	    //}
	   // else{
		
		   //  adcx=Get_Adc_Average(ADC_CHANNEL_6,5,ADC_REGULAR_RANK_1);//»ñÈ¡Í¨µÀ1µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù
		
	//	}
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
		   //ÏÔÊ¾µçÑ¹ÖµµÄÕûÊý²¿·Ö£¬3.1111µÄ»°£¬ÕâÀï¾ÍÊÇÏÔÊ¾3
	    // printf("vadc = %d",data);
	    HAL_UART_Transmit(&huart2, &data2, 1, 1);
		temp-=adcx;                           //°ÑÒÑ¾­ÏÔÊ¾µÄÕûÊý²¿·ÖÈ¥µô£¬ÁôÏÂÐ¡Êý²¿·Ö£¬±ÈÈç3.1111-3=0.1111
		temp*=100;  	   //Ð¡Êý²¿·Ö³ËÒÔ1000£¬ÀýÈç£º0.1111¾Í×ª»»Îª111.1£¬Ïàµ±ÓÚ±£ÁôÈýÎ»Ð¡Êý¡£
		 //ÏÔÊ¾Ð¡Êý²¿·Ö£¨Ç°Ãæ×ª»»ÎªÁËÕûÐÎÏÔÊ¾£©£¬ÕâÀïÏÔÊ¾µÄ¾ÍÊÇ111.
	     
	    data3 =(uint8_t)temp;
		  //printf("point= %ld\n",data);
	    HAL_UART_Transmit(&huart2, &data3, 1, 1);
		  HAL_Delay(250);	
	   


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
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
