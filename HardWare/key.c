#include "key.h"

//按键引脚为：PA0\PE2\PE3\PE4
void key_init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//开启相应的端口时钟，总线选择见中文参考手册2.2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//对结构体变量进行赋值操作
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//利用芯片检测按键引脚的输入的高低电平状态
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//输入模式下，设置为上拉电阻时，引脚在没有外部输入时，引脚的电平默认为高电平；
	//GPIO_InitStruct.GPIO_OType =	;			//输出的具体方式
	//GPIO_InitStruct.GPIO_Speed = 	;			//输出模式下的高低电平切换速率
	
	//调用函数根据结构体变量（数据包）对端口组进行配置
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//其他三个按键PE2\PE3\PE4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//错略延时
void delay(int times)
{
	int i=0, j=0;

	for(i=0; i<times; i++)
		for(j=0; j<1000; j++);
}

//检测具体哪个按键按下
int key_value(void)
{
	//通过判断芯片与按键连接的引脚是否变为低电平来确定按键是否按下
	//判断 读取到的引脚的电平状态是否为低电平，如果为低电平则表示按键按下了一次
	//if( Bit_RESET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) )	//第一次判断按键按下
	//if( Bit_RESET == KEY0 )
	//if( 0 == KEY0 )
	if( !KEY0 || !KEY1 || !KEY2 || !KEY3)
	{
		//延时一定的时间
		delay(5);
		
//		//第二次判断按键按下
//		if( !KEY0 ) GPIO_ToggleBits(GPIOF, GPIO_Pin_9);	//翻转LED0灯的高低电平
//		if( !KEY1 ) GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
//		if( !KEY2 ) GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
//		if( !KEY3 ) GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
		
		if( !KEY0 ) return 0;
		if( !KEY1 ) return 1;
		if( !KEY2 ) return 2;
		if( !KEY3 ) return 3;
		
		while( !KEY0 || !KEY1 || !KEY2 || !KEY3);	//第一次判断按键松开
		delay(5);//延时一定的时间
		while( !KEY0 || !KEY1 || !KEY2 || !KEY3);	//第二次判断按键松开
	}
	
	return -1;
}
