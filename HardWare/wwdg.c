#include "wwdg.h"

//设定窗口看门狗的超时时间(8分频为例)：（42MHZ/4096）/8 = 1281HZ->780us（最大超时时间为：（127-63）*780us）
//从127递减到80-->计数47次--->计数1次花费780us---36.66ms(该时间段用户不应该喂狗）
//从127递减到63-->计数64次--->计数1次花费780us--->49.92ms(计数值递减到0x3F(63)花费的时间）
void wwdg_init(uint32_t WWDG_Prescaler, uint8_t WindowValue)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//开启APB1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	//设置预分频值
	WWDG_SetPrescaler(WWDG_Prescaler);

	//设置窗口上限值（上限值80*780=62.4ms，下限值63*780=49.14ms）
	WWDG_SetWindowValue(WindowValue);
	
	//开启窗口看门狗:设定计数器的开始计数值为127（0X7F）最大值（起始值：127*780=99.06ms）
	WWDG_Enable(127);
	
	//开启NVIC总中断
	//NVIC结构体变量的成员赋值
	NVIC_InitStruct.NVIC_IRQChannel	= WWDG_IRQn;	//设置中断源为WWDG
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	0;	//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	//中断等级中的响应优先级的设置
	//嵌套向量中断控制器的配置（设置中断源、设置中断等级）
	NVIC_Init(&NVIC_InitStruct);
	
	//清除标志位（初始化后状态未知）
	WWDG_ClearFlag();
	
	//开启窗口看门狗中断
	WWDG_EnableIT();
	
	//不得在初始化喂狗
	//WWDG_SetCounter(127);
}


//窗口看门狗的中断服务函数进行喂狗操作
void WWDG_IRQHandler(void)
{
	if( SET == WWDG_GetFlagStatus())
	{
		WWDG_ClearFlag();
		
		//做相关的操作
		WWDG_SetCounter(127);	//喂狗函数（重载起始值）
	}
}
