#include "systick.h"

static __IO uint32_t TimingDelay;
 
//启动系统滴答定时器 SysTick
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))
	{
		while (1);
	}
}

//获取节拍程序
//在 SysTick 中断函数 SysTick_Handler()调用
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

//中断延时程序，10us为一个单位
//Delay_us(1) 则实现的延时为 1 * 10us = 10us
void Systick_Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


//计数器延时程序（默认us）
void Delay(uint32_t nTime)
{
	uint32_t ticks;
	uint32_t told,tnow,reload,tcnt=0;
	reload = SysTick->LOAD;			//获取重装载寄存器值
	ticks = nTime*(SystemCoreClock/1000000);	//打算计时的次数，168Hz，即168次花费1us
	told=SysTick->VAL;                   	//获取当前数值寄存器值（开始时数值）假设 told=100

	while(1)
	{
		tnow=SysTick->VAL;			//获取当前数值寄存器值

		if(tnow!=told)              	//当前值不等于开始值说明已在计数
		{         
			//systick定时器是倒计时 向下递减的  
			if(tnow<told)			//当前值小于开始数值，说明未计到0，假设tnow=80
				tcnt+=told-tnow;		//计数值=开始值-当前值

			else					//当前值大于开始数值，说明已计到0并重新计数
				tcnt+=reload-tnow+told; //计数值=重装载值-当前值+开始值

			told=tnow;                	//更新开始值
			if(tcnt>=ticks)break;     	//时间超过/等于要延迟的时间,则退出
		} 
	}     
}

////延时微秒  21MHZ
//void  Delay_us(uint16_t nus)
//{
//	SysTick->CTRL 	= 0; 					// 关闭定时器
//	SysTick->LOAD 	= 21*nus -1; 			// 设置计数初值
//	SysTick->VAL 	= 0; 					// 清空当前计数值
//	SysTick->CTRL 	= 1; 					// 开启定时器
//	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
//	SysTick->CTRL	 = 0; 					// 关闭定时器
//}

////延时毫秒  21HZ
//void  Delay_ms(uint16_t nms)
//{
//	SysTick->CTRL 	= 0; 					// 关闭定时器
//	SysTick->LOAD 	= 21*nms*1000 -1; 		// 设置计数初值
//	SysTick->VAL 	= 0; 					// 清空当前计数值
//	SysTick->CTRL 	= 1; 					// 开启定时器
//	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
//	SysTick->CTRL	 = 0; 					// 关闭定时器
//}
