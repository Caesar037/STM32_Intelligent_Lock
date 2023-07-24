#include "iic.h"

//ģ��IIC
void IIC_Sim_Init(void)
{
	//��������
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//������Ӧ�Ķ˿���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SDA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SCL, ENABLE);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = SIM_SDA_PIN;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(SIM_SDA_POR, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = SIM_SCL_PIN;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(SIM_SCL_POR, &GPIO_InitStruct);
	
	//�ֶ��������ŵ�Ĭ�ϵ�ƽ״̬���ߵ�ƽ����ʾΪ����״̬
	IIC_SDA_SET(1);
	IIC_SCL_SET(1);
	
}

//SDA���ģʽ
void IIC_Sim_SetSDAOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SDA, ENABLE);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = SIM_SDA_PIN;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(SIM_SDA_POR, &GPIO_InitStruct);
}

//SDA����ģʽ
void IIC_Sim_SetSDAInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SDA, ENABLE);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = SIM_SDA_PIN;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//��������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(SIM_SDA_POR, &GPIO_InitStruct);
}

//��ʼ�źţ�ͨ�����øú���ʵ����������һ����ʼ�źţ�
void IIC_Sim_Start(void)
{
	//ȷ��ΪSDA���
	IIC_Sim_SetSDAOutputMode();
	
	//ȷ��Ϊ����
	IIC_SCL_SET(1);
	IIC_SDA_SET(1);
	Delay_us(5);
	
	//��SDA������
	IIC_SDA_SET(0);
	Delay_us(2);
	
	//��SCL�����ͣ��Է����������SDA�ߵĵ�ƽ���Ա����ݵķ���
	IIC_SCL_SET(0);
	Delay_us(1);
}

//�����źţ�ͨ�����øú���ʵ����������һ�������źţ�
void IIC_Sim_Stop(void)
{
	//ȷ��ΪSDA���
	IIC_Sim_SetSDAOutputMode();
	
	//ȷ����������Ϊ�͵�ƽ���������������
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	Delay_us(2);
	
	//��SCL������
	IIC_SCL_SET(1);
	Delay_us(2);
	
	//��SDA�����ߣ���ȷ���ڷ��ͽ����ź�
	IIC_SDA_SET(1);
	Delay_us(5);
}

//��������(����/����/��ַ)
void IIC_Sim_SendByte(uint8_t data)
{
	uint8_t i = 0;
	
	//ȷ��ΪSDA���
	IIC_Sim_SetSDAOutputMode();
	
	//ȷ����������Ϊ�͵�ƽ���Ա�������ݵķ��ͣ�SCLȷ��SDA���л���ƽ��SDAȷ�����ڵ�״̬Ϊ�͵�ƽ��
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	
	//������ʵ�����ݣ���8bits���ͳ�ȥ��
	for(i=0; i<8; i++)
	{	
		//data & 1000 0000
		if(data & 0x80) //���λΪ1����if��������MSB��λ�����
		{
			IIC_SDA_SET(1);	
		}
		else
		{
			IIC_SDA_SET(0);
		}
		
		//���ݵ����ƣ�������һλ
		data <<= 1;	//����д����data << 1;
		Delay_us(2);
		
		//��SCL�ߵĵ�ƽ���ߣ���������SDA�ĵ�ƽ�����߶Է������ȶ��Ķ�ȡIIC��SDA�ߵĵ�ƽ��
		IIC_SCL_SET(1);
		Delay_us(2);
		
		//��SCL�����ͣ��Է����������SDA�ߵĵ�ƽ���Ա����ݵķ���
		IIC_SCL_SET(0);
		Delay_us(1);
	}
}
	
//�жϴӻ��Ƿ���Ӧ��
uint8_t IIC_Sim_WaitSlavAck(void)
{
	uint8_t ack = 0; //ack = 0000 0000
	
	//ȷ��ΪSDA����
	IIC_Sim_SetSDAInputMode();
	
	//������SCL�����ͣ��Է�������ӻ�����SDA�ߵĵ�ƽ���Ա�ACK�ķ���
	IIC_SCL_SET(0);
	Delay_us(2);
	
	//������SCL�����ߣ�������SDA�����ȶ���ȡSDA�ߵĵ�ƽ
	IIC_SCL_SET(1);
	Delay_us(2);

	//�ߵ�ƽ���һ���ж϶�ȡ����SDA���ŵ�ƽ״̬
	if(IIC_SDA_READ == 0)
	{
		ack = 0; //��ʾӦ��
	}
	else
	{
		ack = 1; //��ʾ��Ӧ��
		//IIC_Sim_Stop();
	}
	
	//��SCL�����ͣ��Է����������SDA�ߵĵ�ƽ���Ա����ݵķ���
	IIC_SCL_SET(0);
	Delay_us(1);
	
	return ack;
}

//������ȡ����
uint8_t IIC_Sim_ReadByte(void)
{
	uint8_t i = 0;
	uint8_t data = 0;
	
	//ȷ��ΪSDA����
	IIC_Sim_SetSDAInputMode();
	//��SCL�����ͣ���ʱSDA�����ߵ����ݿ��Խ����л�,��ʱ�ӻ�����SDA�����߷�������
	IIC_SCL_SET(0);
	
	for(i=0; i<8; i++)
	{
		//�Ա�ӻ�����SDA�ߵĵ�ƽ
		Delay_us(2);
		
		//��SCL�����ߣ�����SDA�ߵĵ�ƽ
		IIC_SCL_SET(1);
		Delay_us(2);
		
		//��ԭ�������ݽ������ƣ�����ԭʼ��λ
		data <<= 1;
		
		//��ȡSDA���ŵĵ�ƽ����¼�ߵ�ƽ��״̬����1�����8λ�Ŀռ���
		if(IIC_SDA_READ == 1)
		{
			data |= 0x01;	//0000 0001
		}
		
		//��SCL�����ͣ�����ӻ�����SDA�ߵĵ�ƽ���Ա������������ȡ
		IIC_SCL_SET(0);
		Delay_us(1);
	}

	return data; 
}

//������������Ӧ���ź�
void IIC_Sim_MasterAck(char ack)
{
	//ȷ��ΪSDA���
	IIC_Sim_SetSDAOutputMode();
	
	//ȷ����������Ϊ�͵�ƽ���Ա�������ݵķ��ͣ�SCLȷ��SDA���л���ƽ��SDAȷ�����ڵ�״̬Ϊ�͵�ƽ��
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	
	if(ack != 0)	//1����Ӧ��
	{
		IIC_SDA_SET(1);
	}
//	else	//��ʡ��//0��Ӧ�� SDA��Ϊ�͵�ƽ��ʾӦ��
//	{
//		IIC_SDA_SET(0);
//	}
	Delay_us(2);
	
	//��SCL�����ߣ�֪ͨ�ӻ���ȡACKӦ���ź�
	IIC_SCL_SET(1);
	Delay_us(2);
	
	//��SCL�����ͣ��Է����������SDA�ߵĵ�ƽ���Ա����ݵķ���
	IIC_SCL_SET(0);
	Delay_us(1);
}

