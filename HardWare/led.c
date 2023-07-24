#include "led.h"

//LED引脚为：PF9\PF10\PE13\PE14
void led_init(void)
{
	/************	配置引脚	*************/
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//开启对应的端口组时钟，总线选择见中文参考手册2.2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;				//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;				//高速率
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//手动设置引脚的默认电平状态：高电平；（因为低电平能够使得灯亮）
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}
