#ifndef __LED_H__
#define __LED_H__

/**
 *ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬
 *static���εĺ�������д��ͷ�ļ���inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
 */	
#include "stm32f4xx.h"

#define LED0(x) (x)?(GPIO_ResetBits(GPIOF, GPIO_Pin_9)):(GPIO_SetBits(GPIOF, GPIO_Pin_9))
#define LED1(x) (x)?(GPIO_ResetBits(GPIOF, GPIO_Pin_10)):(GPIO_SetBits(GPIOF, GPIO_Pin_10))
#define LED2(x) (x)?(GPIO_ResetBits(GPIOE, GPIO_Pin_13)):(GPIO_SetBits(GPIOE, GPIO_Pin_13))
#define LED3(x) (x)?(GPIO_ResetBits(GPIOE, GPIO_Pin_14)):(GPIO_SetBits(GPIOE, GPIO_Pin_14))

void led_init(void);
	

#endif
