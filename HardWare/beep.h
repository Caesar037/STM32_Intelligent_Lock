#ifndef __LED_H__
#define __LED_H__

/**
 *ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬
 *static���εĺ�������д��ͷ�ļ���inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
 */	
#include "stm32f4xx.h"

#define BEEP(x) (x)?(GPIO_ResetBits(GPIOF, GPIO_Pin_8)):(GPIO_SetBits(GPIOF, GPIO_Pin_8))

void beep_init(void);


#endif
