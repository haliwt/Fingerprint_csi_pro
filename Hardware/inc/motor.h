#ifndef __MOTOR_H
#define __MOTOR_H
#include "main.h"

#define MOTOR_EN_Pin        GPIO_PIN_4
#define MOTOR_EN_Port       GPIOB

#define MOTOR_DIR_Pin       GPIO_PIN_15
#define MOTOR_DIR_Port      GPIOA

#define MOTOR_STEP_Pin      GPIO_PIN_3
#define MOTOR_STEP_Port     GPIOB

#define MOTOR_M0_Pin        GPIO_PIN_7
#define MOTOR_M0_Port       GPIOB

#define MOTOR_M1_Pin        GPIO_PIN_6
#define MOTOR_M1_Port       GPIOB

//#define MOTOR_I0_Pin        GPIO_PIN_14
//#define MOTOR_I0_Port       GPIOA

//#define MOTOR_I1_Pin        GPIO_PIN_13
//#define MOTOR_I1_Port       GPIOA

#define MOTOR_SLEEP_Pin        GPIO_PIN_12
#define MOTOR_SLEEP_Port       GPIOA

#define CCW          1
#define CW           0



#define  MOTOR_DRV8846_DISABLE()        (HAL_GPIO_WritePin(GPIOB,MOTOR_EN_Pin,GPIO_PIN_SET))
#define  MOTOR_DRV8846_ENABLE()         (HAL_GPIO_WritePin(GPIOB,MOTOR_EN_Pin,GPIO_PIN_RESET))

#define MOTOR_DRV8846_SLEEP_OFF()       (HAL_GPIO_WritePin(MOTOR_SLEEP_Port,MOTOR_SLEEP_Pin,GPIO_PIN_SET))
#define MOTOR_DRV8846_SLEEP_ON()        (HAL_GPIO_WritePin(MOTOR_SLEEP_Port,MOTOR_SLEEP_Pin,GPIO_PIN_RESET))

#define MOTOR_DIR_CCW()                 (HAL_GPIO_WritePin(MOTOR_DIR_Port,MOTOR_DIR_Pin,GPIO_PIN_SET))
#define MOTOR_DIR_CW()                  (HAL_GPIO_WritePin(MOTOR_DIR_Port,MOTOR_DIR_Pin,GPIO_PIN_RESET))

extern uint8_t motorRunDir_flag;
void MOTOR_MicroStep_FUN(uint8_t micro);

void MOTOR_Run(void);
void MOTOR_Stop(void);



#endif 

