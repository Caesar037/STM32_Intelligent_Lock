#include "w25q128.h"

//w25q128（共16MB）-256个块（1块64KB）-16个扇区（1扇区4KB）-16个页（1页256字节）

//
void W25Q128_Init(void)
{
	SPI_Sim_Init();
}

//读取ID
uint16_t W25Q128_ReadID(void)
{
	uint16_t ID = 0;
	
	//拉低CS片选信号
	W25Q128_CS = 0;
	
	//发送指令(0x90)
	SPI_SendByte(0x90);

	//发送地址
	SPI_SendByte(0x00);
	SPI_SendByte(0x00);
	SPI_SendByte(0x00);
	
	ID = SPI_SendByte(0xFF)<<8;//接收厂商ID放在高8位
	ID |= SPI_SendByte(0xFF);  //接收设备ID放在低8位
	
	//拉高CS片选信号
	W25Q128_CS = 1;
	
	return ID;
}

//读取扇区数据
void  W25Q128_ReadData(uint32_t Address, uint8_t *rdatabuf, uint32_t datalen)
{
	
	//拉低CS片选信号
	W25Q128_CS = 0;

	//发送指令(0x03)
	SPI_SendByte(0x03);

	//发送24位地址
	SPI_SendByte((Address & 0xFF0000)>>16);
	SPI_SendByte((Address & 0xFF00)>>8);
	SPI_SendByte((Address & 0xFF));
	
	//接收数据
	while(datalen--)
	{
		*rdatabuf ++= SPI_SendByte(0xFF);
	}
	
	//拉高CS片选信号
	W25Q128_CS = 1;
}

//使能写入
void  W25Q128_WriteEnable(void)
{
	//拉低CS片选信号
	W25Q128_CS = 0;

	//发送指令(0x06)
	SPI_SendByte(0x06);
	
	//拉高CS片选信号
	W25Q128_CS = 1;
}

//禁止写入
void  W25Q128_WriteDisable(void)
{
	//拉低CS片选信号
	W25Q128_CS = 0;

	//发送指令(0x04)
	SPI_SendByte(0x04);
	
	//拉高CS片选信号
	W25Q128_CS = 1;
}

//读取状态寄存器1的内容（1=正在工作，0=空闲状态）
uint8_t W25Q128_ReadStatusRegister1(void)
{
	uint8_t Status = 0;
	//拉低CS片选信号
	W25Q128_CS = 0;

	//发送指令(0x05)
	SPI_SendByte(0x05);
	
	Status = SPI_SendByte(0xFF);
	
	//拉高CS片选信号
	W25Q128_CS = 1;
	
	return  Status;
}

//擦除扇区  Address--起始地址
void W25Q128_SectorErase(uint32_t Address)
{
	//要先写使能
	W25Q128_WriteEnable();
	Delay_ms(1);
	
	//拉低CS片选信号
	W25Q128_CS = 0;
	
	//发送指令(0x20)
	SPI_SendByte(0x20);
	
	//发送24位地址
	SPI_SendByte((Address & 0xFF0000)>>16);
	SPI_SendByte((Address & 0xFF00)>>8);
	SPI_SendByte((Address & 0xFF));
	
	//拉高CS片选信号
	W25Q128_CS = 1;
	
	// xxxx xxxx & 0000 0001 判断最低的BUSY位
	while(W25Q128_ReadStatusRegister1() &0x01); //判断是否繁忙（1=正在工作，0=空闲状态）
	
	W25Q128_WriteDisable();//禁止写入
}

//写入扇区数据
void  W25Q128_PageProgram(uint32_t Address, uint8_t *rdatabuf, uint32_t datalen)
{
	//要先写使能
	W25Q128_WriteEnable();
	Delay_ms(1);
	
	//拉低CS片选信号
	W25Q128_CS = 0;

	//发送指令(0x02)
	SPI_SendByte(0x02);

	//发送24位地址，高位先行（MSB）
	SPI_SendByte((Address & 0xFF0000)>>16);
	SPI_SendByte((Address & 0xFF00)>>8);
	SPI_SendByte((Address & 0xFF));
	
	//接收数据
	while(datalen--)
	{
		SPI_SendByte(*rdatabuf++); //写数据不用接收
	}
	
	//拉高CS片选信号
	W25Q128_CS = 1;
	
	while(W25Q128_ReadStatusRegister1() &0x01); //判断是否繁忙（1=正在工作，0=空闲状态）
	
	W25Q128_WriteDisable();//禁止写入
}

