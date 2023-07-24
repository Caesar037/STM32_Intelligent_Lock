#include "beep.h"

//BEEP引脚为：PF8
void beep_init(void)
{
	/************	配置引脚	*************/
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//开启对应的端口组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;						//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;					//下拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;				//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//手动设置引脚的默认电平状态：低电平；
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}
