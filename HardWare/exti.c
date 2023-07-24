#include "exti.h"

//外部中断配置函数（以按键引脚PA0\PE2\PE3\PE4为例）
void exti_init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//GPIO结构体变量的成员赋值
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStruct.GPIO_OType =	;	//输出的具体方式
	GPIO_Init(GPIOA, &GPIO_InitStruct);	//利用GPIO_Init()根据结构体变量成员信息配置相应的GPIOA
	
	//GPIO结构体变量的成员赋值
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStruct.GPIO_OType =	;	//输出的具体方式
	GPIO_Init(GPIOE, &GPIO_InitStruct);	//利用GPIO_Init()根据结构体变量成员信息配置相应的GPIOE
	
	//建立映射关系
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	
	//EXTI结构体变量的成员赋值
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//软中断
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;	//对应的引脚号绑定对应中断线（0~15），共23个EXTI_Line
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;	//边沿选择
	EXTI_Init(&EXTI_InitStruct);	//利用EXTI_Init()根据结构体变量成员信息配置相应的外部中断
	
	
	//NVIC结构体变量的成员赋值
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI0_IRQn;	//设置中断源为EXTI0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	1;//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //中断等级中的响应优先级的设置
	//嵌套向量中断控制器的配置（设置中断源、设置中断等级）
	NVIC_Init(&NVIC_InitStruct);
	
	//NVIC结构体变量的成员赋值
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI2_IRQn;	//设置中断源为EXTI2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	2;//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //中断等级中的响应优先级的设置
	//嵌套向量中断控制器的配置（设置中断源、设置中断等级）
	NVIC_Init(&NVIC_InitStruct);
	
	//NVIC结构体变量的成员赋值
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI3_IRQn;	//设置中断源为EXTI3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	2;//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //中断等级中的响应优先级的设置
	//嵌套向量中断控制器的配置（设置中断源、设置中断等级）
	NVIC_Init(&NVIC_InitStruct);
	
	//NVIC结构体变量的成员赋值
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI4_IRQn;	//设置中断源为EXTI4
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	2;//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //中断等级中的响应优先级的设置
	//嵌套向量中断控制器的配置（设置中断源、设置中断等级）
	NVIC_Init(&NVIC_InitStruct);
	
}


//定义全局变量的标志位
uint8_t EXTI0_flag = 0;
uint8_t EXTI2_flag = 0;
uint8_t EXTI3_flag = 0;
uint8_t EXTI4_flag = 0;

//外部中断服务函数（该函数不可更改名称、不可声明、不可返回、不可调用、不可传参、不可修饰）
void EXTI0_IRQHandler(void)
{
	//具体判断是哪个中断源触发了（状态发生了改变）
	if( SET == EXTI_GetITStatus( EXTI_Line0 ) )
	{
		//为了方便下次再此判断到中断状态的改变，所以清空中断的状态
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		//做相关的事情：点灯、改变标志位
		EXTI0_flag = !EXTI0_flag;
		//按下按键触发外部中断，CPU响应中断，处理中断服务函数，把标志位更改，在主函数里的while（）不断的判断标志位的改变，再作其他事情即可；
	
	}
}

void EXTI2_IRQHandler(void)
{
	//具体判断是哪个中断源触发了（状态发生了改变）
	if( SET == EXTI_GetITStatus( EXTI_Line2 ) )
	{
		//为了方便下次再此判断到中断状态的改变，所以清空中断的状态
		EXTI_ClearITPendingBit(EXTI_Line2);
		
		//做相关的事情：点灯、改变标志位
		EXTI2_flag = !EXTI2_flag;
		//按下按键触发外部中断，CPU响应中断，处理中断服务函数，把标志位更改，在主函数里的while（）不断的判断标志位的改变，再作其他事情即可；
	
	}
}

//外部中断服务函数（该函数不可更改名称、不可声明、不可返回、不可调用、不可传参、不可修饰）
void EXTI3_IRQHandler(void)
{
	//具体判断是哪个中断源触发了（状态发生了改变）
	if( SET == EXTI_GetITStatus( EXTI_Line3 ) )
	{
		//为了方便下次再此判断到中断状态的改变，所以清空中断的状态
		EXTI_ClearITPendingBit(EXTI_Line3);
		
		//做相关的事情：点灯、改变标志位
		EXTI3_flag = !EXTI3_flag;
		//按下按键触发外部中断，CPU响应中断，处理中断服务函数，把标志位更改，在主函数里的while（）不断的判断标志位的改变，再作其他事情即可；
	
	}
}

//外部中断服务函数（该函数不可更改名称、不可声明、不可返回、不可调用、不可传参、不可修饰）
void EXTI4_IRQHandler(void)
{
	//具体判断是哪个中断源触发了（状态发生了改变）
	if( SET == EXTI_GetITStatus( EXTI_Line4 ) )
	{
		//为了方便下次再此判断到中断状态的改变，所以清空中断的状态
		EXTI_ClearITPendingBit(EXTI_Line4);
		
		//做相关的事情：点灯、改变标志位
		EXTI4_flag = !EXTI4_flag;
		//按下按键触发外部中断，CPU响应中断，处理中断服务函数，把标志位更改，在主函数里的while（）不断的判断标志位的改变，再作其他事情即可；
	
	}
}
