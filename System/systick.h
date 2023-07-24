#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f4xx.h"

void SysTick_Init(void);
void TimingDelay_Decrement(void);
void Systick_Delay_us(__IO uint32_t nTime);	//中断延时程序（默认us）

void Delay(uint32_t nus);	//计数器延时程序（默认us）
//void Delay_us(uint16_t nus);
//void Delay_ms(uint16_t nms);

#define Systick_Delay_ms(x)	Systick_Delay_us(1000*x)
#define Delay_us(x)	Delay(x)
#define Delay_ms(x)	Delay(1000*x)

#endif
