#include "sr04.h"

void sr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//打开端口B的硬件时钟（默认硬件时钟为关闭状态），说白了就是对端口B供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//打开端口E的硬件时钟（默认硬件时钟为关闭状态），说白了就是对端口E供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		
	//配置端口B的6引脚，就是PB6
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;			//指定6号引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;		//输出模式tx输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	//推挽 Push Pull
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;	//不用上下拉电阻
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;	//高速(速度越高，功耗越高，也更容易产生电磁干扰)
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//配置端口E的6引脚，就是PE6
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;			//指定6号引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;		//输入模式rx接收
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;	//不用上下拉电阻
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;	//高速(速度越高，功耗越高，也更容易产生电磁干扰)
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	//看时序图，初始电平为低电平
	PBout(6)=0;
}

int32_t sr04_get_distance(void)
{
	int32_t t=0;

	//输出高电平请求信号10us
	PBout(6)=1;
	Delay_us(10);
	PBout(6)=0;
	
	//等待高电平的出现
	while(PEin(6)==0);

	//测量高电平的时间
	while(PEin(6))
	{
		Delay_us(9); //传播9us，距离为3mm
		
		t++;
	}

	return (3*t/2);
}
