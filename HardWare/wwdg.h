#ifndef __WWDG_H__
#define __WWDG_H__

//ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬static���εĺ�������д��ͷ�ļ��� \
	inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
	
#include "stm32f4xx.h"

void wwdg_init(uint32_t WWDG_Prescaler, uint8_t WindowValue);

#endif
