#include "beep.h"

//BEEP����Ϊ��PF8
void beep_init(void)
{
	/************	��������	*************/
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//������Ӧ�Ķ˿���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;						//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;					//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;				//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//�ֶ��������ŵ�Ĭ�ϵ�ƽ״̬���͵�ƽ��
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}
