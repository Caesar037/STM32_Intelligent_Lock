#include "w25q128.h"

//w25q128����16MB��-256���飨1��64KB��-16��������1����4KB��-16��ҳ��1ҳ256�ֽڣ�

//
void W25Q128_Init(void)
{
	SPI_Sim_Init();
}

//��ȡID
uint16_t W25Q128_ReadID(void)
{
	uint16_t ID = 0;
	
	//����CSƬѡ�ź�
	W25Q128_CS = 0;
	
	//����ָ��(0x90)
	SPI_SendByte(0x90);

	//���͵�ַ
	SPI_SendByte(0x00);
	SPI_SendByte(0x00);
	SPI_SendByte(0x00);
	
	ID = SPI_SendByte(0xFF)<<8;//���ճ���ID���ڸ�8λ
	ID |= SPI_SendByte(0xFF);  //�����豸ID���ڵ�8λ
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
	
	return ID;
}

//��ȡ��������
void  W25Q128_ReadData(uint32_t Address, uint8_t *rdatabuf, uint32_t datalen)
{
	
	//����CSƬѡ�ź�
	W25Q128_CS = 0;

	//����ָ��(0x03)
	SPI_SendByte(0x03);

	//����24λ��ַ
	SPI_SendByte((Address & 0xFF0000)>>16);
	SPI_SendByte((Address & 0xFF00)>>8);
	SPI_SendByte((Address & 0xFF));
	
	//��������
	while(datalen--)
	{
		*rdatabuf ++= SPI_SendByte(0xFF);
	}
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
}

//ʹ��д��
void  W25Q128_WriteEnable(void)
{
	//����CSƬѡ�ź�
	W25Q128_CS = 0;

	//����ָ��(0x06)
	SPI_SendByte(0x06);
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
}

//��ֹд��
void  W25Q128_WriteDisable(void)
{
	//����CSƬѡ�ź�
	W25Q128_CS = 0;

	//����ָ��(0x04)
	SPI_SendByte(0x04);
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
}

//��ȡ״̬�Ĵ���1�����ݣ�1=���ڹ�����0=����״̬��
uint8_t W25Q128_ReadStatusRegister1(void)
{
	uint8_t Status = 0;
	//����CSƬѡ�ź�
	W25Q128_CS = 0;

	//����ָ��(0x05)
	SPI_SendByte(0x05);
	
	Status = SPI_SendByte(0xFF);
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
	
	return  Status;
}

//��������  Address--��ʼ��ַ
void W25Q128_SectorErase(uint32_t Address)
{
	//Ҫ��дʹ��
	W25Q128_WriteEnable();
	Delay_ms(1);
	
	//����CSƬѡ�ź�
	W25Q128_CS = 0;
	
	//����ָ��(0x20)
	SPI_SendByte(0x20);
	
	//����24λ��ַ
	SPI_SendByte((Address & 0xFF0000)>>16);
	SPI_SendByte((Address & 0xFF00)>>8);
	SPI_SendByte((Address & 0xFF));
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
	
	// xxxx xxxx & 0000 0001 �ж���͵�BUSYλ
	while(W25Q128_ReadStatusRegister1() &0x01); //�ж��Ƿ�æ��1=���ڹ�����0=����״̬��
	
	W25Q128_WriteDisable();//��ֹд��
}

//д����������
void  W25Q128_PageProgram(uint32_t Address, uint8_t *rdatabuf, uint32_t datalen)
{
	//Ҫ��дʹ��
	W25Q128_WriteEnable();
	Delay_ms(1);
	
	//����CSƬѡ�ź�
	W25Q128_CS = 0;

	//����ָ��(0x02)
	SPI_SendByte(0x02);

	//����24λ��ַ����λ���У�MSB��
	SPI_SendByte((Address & 0xFF0000)>>16);
	SPI_SendByte((Address & 0xFF00)>>8);
	SPI_SendByte((Address & 0xFF));
	
	//��������
	while(datalen--)
	{
		SPI_SendByte(*rdatabuf++); //д���ݲ��ý���
	}
	
	//����CSƬѡ�ź�
	W25Q128_CS = 1;
	
	while(W25Q128_ReadStatusRegister1() &0x01); //�ж��Ƿ�æ��1=���ڹ�����0=����״̬��
	
	W25Q128_WriteDisable();//��ֹд��
}

