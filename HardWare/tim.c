#include "tim.h"

//配置基本定时器7（只能向上计数），设定定时器7定时1ms
void tim7_init(void)
{
	//定义结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//打开时钟，总线选择见中文参考手册2.2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	//对结构体变量成员作赋值操作（对自动重载值 和 预分频器值作赋值操作，即设定定时器的定时时间，时间到，产生中断信号）
	TIM_TimeBaseInitStruct.TIM_Period = (10000/1000)-1;	//自动重载值 (10000)*(8400)/84000000=1s(这里是1ms响应一次中断)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//预分频器值
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//分频设置为1，即相当于不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//设置为向上计数
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//重复计数（只有高级定时器才需要配置）
	
	//初始化定时器
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	//配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先等级(分组为组2：等级取值范围（0~3）)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;	//响应优先等级
	NVIC_Init(&NVIC_InitStruct);
	
	//开启相应的定时器中断
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); 
	
	//开启定时器计数（激活定时器，开始计数）
	TIM_Cmd(TIM7, ENABLE);
}


//编写中断服务函数（定时器定时为1ms）
void TIM7_IRQHandler(void)
{
	//判断是定时器的哪个中断状态发送改变
	if(SET == TIM_GetITStatus(TIM7, TIM_IT_Update) )
	{
		/*********** 实现自定义功能 ***********/
		
		IWDG_ReloadCounter();	//喂狗操作尽量在定时器服务函数里操作，能够按时喂狗。喂狗函数（独立看门狗）
		
		//检测是否接收最后一位数据（串口1检测，用于变长指令检测）
		if(Rx1TMP != Rx1Counter)
		{
			Rx1TMP = Rx1Counter;
		}
		else if(Rx1TMP && (Rx1TMP == Rx1Counter))
		{
			Rx1End = 1;	//结束事件标志位置1（串口1）
			Rx1TMP = 0;
		}
		
		
		//检测是否接收最后一位数据（串口2检测，用于变长指令检测）
		if(Rx2TMP != Rx2Counter)
		{
			Rx2TMP = Rx2Counter;
		}
		else if(Rx2TMP && (Rx2TMP == Rx2Counter))
		{
			Rx2End = 1;	//结束事件标志位置1（串口2）
			Rx2TMP = 0;
		}
		

		//检测是否接收最后一位数据（串口3检测，用于变长指令检测）
		if(Rx3TMP != Rx3Counter)
		{
			Rx3TMP = Rx3Counter;
		}
		else if(Rx3TMP && (Rx3TMP == Rx3Counter))
		{
			Rx3End = 1;	//结束事件标志位置1（串口3）
			Rx3TMP = 0;
		}
		
		//清除中断状态
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);		
	}
}
