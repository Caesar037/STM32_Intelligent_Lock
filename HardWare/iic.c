#include "iic.h"

//模拟IIC
void IIC_Sim_Init(void)
{
	//引脚配置
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//开启对应的端口组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SDA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SCL, ENABLE);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = SIM_SDA_PIN;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//下拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(SIM_SDA_POR, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = SIM_SCL_PIN;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//下拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(SIM_SCL_POR, &GPIO_InitStruct);
	
	//手动设置引脚的默认电平状态：高电平，表示为空闲状态
	IIC_SDA_SET(1);
	IIC_SCL_SET(1);
	
}

//SDA输出模式
void IIC_Sim_SetSDAOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SDA, ENABLE);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = SIM_SDA_PIN;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//下拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(SIM_SDA_POR, &GPIO_InitStruct);
}

//SDA输入模式
void IIC_Sim_SetSDAInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SDA, ENABLE);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = SIM_SDA_PIN;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//输入模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉电阻
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(SIM_SDA_POR, &GPIO_InitStruct);
}

//起始信号（通过调用该函数实现主机发送一个起始信号）
void IIC_Sim_Start(void)
{
	//确保为SDA输出
	IIC_Sim_SetSDAOutputMode();
	
	//确保为空闲
	IIC_SCL_SET(1);
	IIC_SDA_SET(1);
	Delay_us(5);
	
	//对SDA线拉低
	IIC_SDA_SET(0);
	Delay_us(2);
	
	//对SCL线拉低，以方便后续更改SDA线的电平，以便数据的发送
	IIC_SCL_SET(0);
	Delay_us(1);
}

//结束信号（通过调用该函数实现主机发送一个结束信号）
void IIC_Sim_Stop(void)
{
	//确保为SDA输出
	IIC_Sim_SetSDAOutputMode();
	
	//确保两根引脚为低电平，方便后续的拉高
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	Delay_us(2);
	
	//对SCL线拉高
	IIC_SCL_SET(1);
	Delay_us(2);
	
	//对SDA线拉高，以确定在发送结束信号
	IIC_SDA_SET(1);
	Delay_us(5);
}

//发送数据(数据/命令/地址)
void IIC_Sim_SendByte(uint8_t data)
{
	uint8_t i = 0;
	
	//确保为SDA输出
	IIC_Sim_SetSDAOutputMode();
	
	//确保两根引脚为低电平（以便后续数据的发送，SCL确保SDA可切换电平，SDA确保现在的状态为低电平）
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	
	//发送真实的数据（以8bits发送出去）
	for(i=0; i<8; i++)
	{	
		//data & 1000 0000
		if(data & 0x80) //最高位为1，则if语句成立（MSB高位相出）
		{
			IIC_SDA_SET(1);	
		}
		else
		{
			IIC_SDA_SET(0);
		}
		
		//数据的左移，发送下一位
		data <<= 1;	//错误写法：data << 1;
		Delay_us(2);
		
		//把SCL线的电平拉高，“锁定”SDA的电平，告诉对方可以稳定的读取IIC的SDA线的电平了
		IIC_SCL_SET(1);
		Delay_us(2);
		
		//对SCL线拉低，以方便后续更改SDA线的电平，以便数据的发送
		IIC_SCL_SET(0);
		Delay_us(1);
	}
}
	
//判断从机是否有应答
uint8_t IIC_Sim_WaitSlavAck(void)
{
	uint8_t ack = 0; //ack = 0000 0000
	
	//确保为SDA输入
	IIC_Sim_SetSDAInputMode();
	
	//主机对SCL线拉低，以方便后续从机更改SDA线的电平，以便ACK的发送
	IIC_SCL_SET(0);
	Delay_us(2);
	
	//主机对SCL线拉高，以锁定SDA，再稳定读取SDA线的电平
	IIC_SCL_SET(1);
	Delay_us(2);

	//高电平最后一刻判断读取到的SDA引脚电平状态
	if(IIC_SDA_READ == 0)
	{
		ack = 0; //表示应答
	}
	else
	{
		ack = 1; //表示无应答
		//IIC_Sim_Stop();
	}
	
	//对SCL线拉低，以方便后续更改SDA线的电平，以便数据的发送
	IIC_SCL_SET(0);
	Delay_us(1);
	
	return ack;
}

//主机读取数据
uint8_t IIC_Sim_ReadByte(void)
{
	uint8_t i = 0;
	uint8_t data = 0;
	
	//确保为SDA输入
	IIC_Sim_SetSDAInputMode();
	//把SCL线拉低，此时SDA数据线的数据可以进行切换,此时从机利用SDA数据线发送数据
	IIC_SCL_SET(0);
	
	for(i=0; i<8; i++)
	{
		//以便从机更改SDA线的电平
		Delay_us(2);
		
		//把SCL线拉高，锁定SDA线的电平
		IIC_SCL_SET(1);
		Delay_us(2);
		
		//把原本的数据进行左移，保留原始的位
		data <<= 1;
		
		//读取SDA引脚的电平，记录高电平的状态，把1存放在8位的空间里
		if(IIC_SDA_READ == 1)
		{
			data |= 0x01;	//0000 0001
		}
		
		//把SCL线拉低，方便从机更改SDA线的电平，以便后续的主机读取
		IIC_SCL_SET(0);
		Delay_us(1);
	}

	return data; 
}

//主机主动发送应答信号
void IIC_Sim_MasterAck(char ack)
{
	//确保为SDA输出
	IIC_Sim_SetSDAOutputMode();
	
	//确保两根引脚为低电平（以便后续数据的发送，SCL确保SDA可切换电平，SDA确保现在的状态为低电平）
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	
	if(ack != 0)	//1：不应答
	{
		IIC_SDA_SET(1);
	}
//	else	//可省略//0：应答， SDA线为低电平表示应答
//	{
//		IIC_SDA_SET(0);
//	}
	Delay_us(2);
	
	//把SCL线拉高，通知从机读取ACK应答信号
	IIC_SCL_SET(1);
	Delay_us(2);
	
	//对SCL线拉低，以方便后续更改SDA线的电平，以便数据的发送
	IIC_SCL_SET(0);
	Delay_us(1);
}

