#ifndef __PWM_H__
#define __PWM_H__

//ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬static���εĺ�������д��ͷ�ļ��� \
	inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
	
#include "stm32f4xx.h"

#define TONE_1_M	523
#define TONE_2_M	587
#define TONE_3_M	659
#define TONE_4_M	698
#define TONE_5_M	784
#define TONE_6_M	880
#define TONE_7_M	988

#define angle(x) 5+20*((x)*1.00/180) //����Ƕȣ�ȡֵ��0-180
#define Rotation(x) TIM_SetCompare4(TIM3, angle(x)); //���ö�����ֵĽǶ�

void pwm_led_init(void);	//PF9---TIM14_CH1

void pwm_beep_init(void);	//PF8---TIM13_CH1
void pwm_beep_set(uint32_t freq, uint32_t duty); //�������������

void pwm_SG90_init(void);	//PC9---TIM3_CH4


#endif
