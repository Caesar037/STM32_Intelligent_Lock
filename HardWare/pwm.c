#include "pwm.h"

//PF9---TIM14_CH1
void pwm_led_init(void)	//需要用到定时器的输出通道输出PWM脉冲信号
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//使能GPIO和定时器时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	//作引脚关联，关联到定时器输出通道1
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//复用模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//定时器定时时间为：T = (10000-1) * (8400-1) / 84000000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = (10000/100)-1;	//自动重载值，即设定PWM的频率（计数器从0累加到99最大值，重新归0，重新计算）（100HZ）
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//预分频器值
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//分频设置为1，即相当于不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//设置为向上计数
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//重复计数（只有高级定时器TIM1和TIM8才需要配置）
	//初始化定时器
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);

	//通道的参数配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //通道模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;	//指定要加载到捕获比较寄存器中的脉冲值。即设定比较寄存器的比较值，即设定占空比值（0%）
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;	//指定低电平为有效电平，即设定0%的低电平，即灯默认状态为关
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);	//初始化通道1

	//开启定时器计数（激活定时器，开始计数）
	TIM_Cmd(TIM14, ENABLE);
}

//PF8---TIM13_CH1
void pwm_beep_init(void)	//需要用到定时器的输出通道输出PWM脉冲信号
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//使能GPIO和定时器时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	
	//作引脚关联，关联到定时器输出通道1
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//复用模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//定时器定时时间为：(10000-1) * (8400-1) / 84000000 = 1Hz --> 1s
	TIM_TimeBaseInitStruct.TIM_Period = (10000/100)-1;	//自动重载值，即设定PWM的频率（计数器从0累加到99最大值，重新归0，重新计算）（100HZ）
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//预分频器值
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//分频设置为1，即相当于不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//设置为向上计数
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//重复计数（只有高级定时器TIM1和TIM8才需要配置）
	//初始化定时器
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseInitStruct);

	//通道的参数配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //通道模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ((10000/100)*(0)/100);	//指定要加载到捕获比较寄存器中的脉冲值。即设定比较寄存器的比较值，即设定占空比值（0%）
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//指定低电平为有效电平，即设定0%的低电平，即灯默认状态为关
	TIM_OC1Init(TIM13, &TIM_OCInitStruct);	//初始化通道1

	//开启定时器计数（激活定时器，开始计数）
	TIM_Cmd(TIM13, ENABLE);
}

static uint32_t tim13_cnt=0;

void pwm_beep_set(uint32_t freq, uint32_t duty)
{
	//定义结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	
	//关闭TIM13
	TIM_Cmd(TIM13, DISABLE);
	
    //定时器的基本配置，用于配置定时器的输出脉冲的频率为 freq Hz
    TIM_TimeBaseStruct.TIM_Period = (400000/freq)-1; //设置定时脉冲的频率
    TIM_TimeBaseStruct.TIM_Prescaler = 210-1;		 //第一次分频，简称为预分频
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    tim13_cnt = TIM_TimeBaseStruct.TIM_Period;
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStruct);
	
	TIM_SetCompare1(TIM13,(tim13_cnt+1) * duty/100); //调整占空比
	
	//使能TIM13
	TIM_Cmd(TIM13, ENABLE);	
}

//PC9---TIM3_CH4
void pwm_SG90_init(void)	//需要用到定时器的输出通道输出PWM脉冲信号
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//作引脚关联，关联到定时器输出通道1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//输出模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//定时器定时时间为：((10000/50)-1) * (8400-1) / 84000000 = 50Hz  --> 20ms 控制舵机需要的周期时间为20ms，即PWM的频率为50Hz
	TIM_TimeBaseInitStruct.TIM_Period = (10000/50)-1;	//自动重载值，即设定PWM的频率（计数器从0累加到200最大值，重新归0，重新计算）分母就是200
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//预分频器值
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//分频设置为1，即相当于不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//设置为向上计数
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//重复计数（只有高级定时器才需要配置）
	//初始化定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//通道的参数配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //通道模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;	//舵机转动角度：5：0°；	10：45°；	15：90°；	20：135°；	25：180° 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//指定高电平为有效电平，即设定0%的高电平，即灯默认状态为关
	TIM_OC4Init(TIM3, &TIM_OCInitStruct);	//初始化通道4
	
	//开启定时器计数（激活定时器，开始计数）
	TIM_Cmd(TIM3, ENABLE);
}

