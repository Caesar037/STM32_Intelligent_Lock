#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"

#define KEY0	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY1	GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define KEY2	GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define KEY3	GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)

void key_init(void);
int key_value(void);

	
#endif
