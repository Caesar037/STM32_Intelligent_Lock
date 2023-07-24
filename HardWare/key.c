#include "key.h"

//��������Ϊ��PA0\PE2\PE3\PE4
void key_init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//������Ӧ�Ķ˿�ʱ�ӣ�����ѡ������Ĳο��ֲ�2.2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//�Խṹ��������и�ֵ����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;	//����оƬ��ⰴ�����ŵ�����ĸߵ͵�ƽ״̬
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//����ģʽ�£�����Ϊ��������ʱ��������û���ⲿ����ʱ�����ŵĵ�ƽĬ��Ϊ�ߵ�ƽ��
	//GPIO_InitStruct.GPIO_OType =	;			//����ľ��巽ʽ
	//GPIO_InitStruct.GPIO_Speed = 	;			//���ģʽ�µĸߵ͵�ƽ�л�����
	
	//���ú������ݽṹ����������ݰ����Զ˿����������
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//������������PE2\PE3\PE4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//������ʱ
void delay(int times)
{
	int i=0, j=0;

	for(i=0; i<times; i++)
		for(j=0; j<1000; j++);
}

//�������ĸ���������
int key_value(void)
{
	//ͨ���ж�оƬ�밴�����ӵ������Ƿ��Ϊ�͵�ƽ��ȷ�������Ƿ���
	//�ж� ��ȡ�������ŵĵ�ƽ״̬�Ƿ�Ϊ�͵�ƽ�����Ϊ�͵�ƽ���ʾ����������һ��
	//if( Bit_RESET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) )	//��һ���жϰ�������
	//if( Bit_RESET == KEY0 )
	//if( 0 == KEY0 )
	if( !KEY0 || !KEY1 || !KEY2 || !KEY3)
	{
		//��ʱһ����ʱ��
		delay(5);
		
//		//�ڶ����жϰ�������
//		if( !KEY0 ) GPIO_ToggleBits(GPIOF, GPIO_Pin_9);	//��תLED0�Ƶĸߵ͵�ƽ
//		if( !KEY1 ) GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
//		if( !KEY2 ) GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
//		if( !KEY3 ) GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
		
		if( !KEY0 ) return 0;
		if( !KEY1 ) return 1;
		if( !KEY2 ) return 2;
		if( !KEY3 ) return 3;
		
		while( !KEY0 || !KEY1 || !KEY2 || !KEY3);	//��һ���жϰ����ɿ�
		delay(5);//��ʱһ����ʱ��
		while( !KEY0 || !KEY1 || !KEY2 || !KEY3);	//�ڶ����жϰ����ɿ�
	}
	
	return -1;
}
