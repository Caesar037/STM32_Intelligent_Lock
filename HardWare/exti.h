#ifndef __EXTI_H__
#define __EXTI_H__

/**
 *ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬
 *static���εĺ�������д��ͷ�ļ���inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
 */	
 
#include "stm32f4xx.h"

//�������ⲿ����
extern uint8_t EXTI0_flag;
extern uint8_t EXTI2_flag;
extern uint8_t EXTI3_flag;
extern uint8_t EXTI4_flag;

void exti_init(void);

#endif
