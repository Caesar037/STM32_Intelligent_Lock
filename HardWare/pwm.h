#ifndef __PWM_H__
#define __PWM_H__

//头文件作用：作函数的声明，外部变量的声明，宏&typedef的定义，结构体&枚举&共用体类型定义，static修饰的函数尽量写在头文件， \
	inline修饰的函数写在头文件，作其他头文件的包含；
	
#include "stm32f4xx.h"

#define TONE_1_M	523
#define TONE_2_M	587
#define TONE_3_M	659
#define TONE_4_M	698
#define TONE_5_M	784
#define TONE_6_M	880
#define TONE_7_M	988

#define angle(x) 5+20*((x)*1.00/180) //舵机角度，取值：0-180
#define Rotation(x) TIM_SetCompare4(TIM3, angle(x)); //设置舵机保持的角度

void pwm_led_init(void);	//PF9---TIM14_CH1

void pwm_beep_init(void);	//PF8---TIM13_CH1
void pwm_beep_set(uint32_t freq, uint32_t duty); //调整音调和响度

void pwm_SG90_init(void);	//PC9---TIM3_CH4


#endif
