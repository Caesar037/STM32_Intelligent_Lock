#ifndef __RTC_H__
#define __RTC_H__

//ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬static���εĺ�������д��ͷ�ļ��� \
	inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
	
#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>

//�趨д�뱸�ݼĴ����������Ϊ��
#define BKP_DATA   0xFFFF
#define LSE_OR_LSI 1

void RTC_TimeShow(void);
void RTC_DataShow(void);

void rtc_init(void);
void RTC_AlarmConfig(void);
void RTC_WakeUpConfig(void);


#endif
