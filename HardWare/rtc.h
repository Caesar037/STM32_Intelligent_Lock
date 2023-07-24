#ifndef __RTC_H__
#define __RTC_H__

//头文件作用：作函数的声明，外部变量的声明，宏&typedef的定义，结构体&枚举&共用体类型定义，static修饰的函数尽量写在头文件， \
	inline修饰的函数写在头文件，作其他头文件的包含；
	
#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>

//设定写入备份寄存器里的数据为：
#define BKP_DATA   0xFFFF
#define LSE_OR_LSI 1

void RTC_TimeShow(void);
void RTC_DataShow(void);

void rtc_init(void);
void RTC_AlarmConfig(void);
void RTC_WakeUpConfig(void);


#endif
