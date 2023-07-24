#include "spi.h"


//CS---PB14  
//SPI1_SCK---PB3
//SPI1_MISO---PB4
//SPI1_MOSI---PB5
//配置stm32芯片的SPI接口为主机模式，且工作模式为mod3 ，即极性为1， 相位为1（第二个边沿检测）
void SPI_Sim_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	//开启对应的端口组时钟
	RCC_AHB1PeriphClockCmd(SPI_CS_GPIO_CLK | SPI_SCK_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_MISO_GPIO_CLK, ENABLE);	////42MHZ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//84MHZ
	
	//将SPI引脚连接到复用端口
	GPIO_PinAFConfig(SPI_SCK_GPIO_PORT, SPI_SCK_SOURCE, SPI_AF);
	GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_SOURCE, SPI_AF);
	GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, SPI_MISO_SOURCE, SPI_AF);

	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = SPI_CS_PIN;			//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;		//复用模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//无上下拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//SPI_CS引脚配置（从机）
	GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStruct);
	
	//SPI_SCK引脚配置
	GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN;
	GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
	
	//SPI_MOSI引脚配置
	GPIO_InitStruct.GPIO_Pin =  SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
	
	//SPI_MISO引脚配置
	GPIO_InitStruct.GPIO_Pin =  SPI_MISO_PIN;
	GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

#if Master_Or_Slave
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = SPI_CS_PIN;			//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		//推挽模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;		//下拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//SPI_CS引脚配置（主机）
	GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStruct);
#endif	

	//片选引脚直接拉高，表示默认状态为不选中模块
	SPI_CS_SET(1);
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//全双工通信
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		//主机模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;	//数据大小8位
	//以下极性和相位决定了SPI接口的工作模式，该模式的设置由通信从器件决定（常用设置为模式0）
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;			//极性为低电平，即SCK引脚空闲状态下为低电平
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;		//第一个边沿检测，CPHA=0;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			//软件控制CS
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//分频16倍
	
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;	//数据格式高位先出
	SPI_InitStruct.SPI_CRCPolynomial = 7;	//检测位设置
	SPI_Init(SPIX, &SPI_InitStruct);
	
	//使能sFLASH_SPI
	SPI_Cmd(SPIX, ENABLE);
}

//SPI 接收与发送函数（收发一体）
uint8_t SPI_SendByte(uint8_t byte)
{
  //DR寄存器不为空时循环
  while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET);

  //通过SPIX外围设备发送字节
  SPI_I2S_SendData(SPIX, byte);

  //等待读取一个字节
  while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_RXNE) == RESET);

  //返回从SPI总线读取的字节
  return SPI_I2S_ReceiveData(SPIX);
}

