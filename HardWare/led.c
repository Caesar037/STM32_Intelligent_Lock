#include "led.h"

//LED����Ϊ��PF9\PF10\PE13\PE14
void led_init(void)
{
	/************	��������	*************/
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//������Ӧ�Ķ˿���ʱ�ӣ�����ѡ������Ĳο��ֲ�2.2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;				//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;				//������
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//�ֶ��������ŵ�Ĭ�ϵ�ƽ״̬���ߵ�ƽ������Ϊ�͵�ƽ�ܹ�ʹ�õ�����
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}
