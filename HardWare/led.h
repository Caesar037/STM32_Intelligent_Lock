#ifndef __LED_H__
#define __LED_H__

/**
 *头文件作用：作函数的声明，外部变量的声明，宏&typedef的定义，结构体&枚举&共用体类型定义，
 *static修饰的函数尽量写在头文件，inline修饰的函数写在头文件，作其他头文件的包含；
 */	
#include "stm32f4xx.h"

#define LED0(x) (x)?(GPIO_ResetBits(GPIOF, GPIO_Pin_9)):(GPIO_SetBits(GPIOF, GPIO_Pin_9))
#define LED1(x) (x)?(GPIO_ResetBits(GPIOF, GPIO_Pin_10)):(GPIO_SetBits(GPIOF, GPIO_Pin_10))
#define LED2(x) (x)?(GPIO_ResetBits(GPIOE, GPIO_Pin_13)):(GPIO_SetBits(GPIOE, GPIO_Pin_13))
#define LED3(x) (x)?(GPIO_ResetBits(GPIOE, GPIO_Pin_14)):(GPIO_SetBits(GPIOE, GPIO_Pin_14))

void led_init(void);
	

#endif
