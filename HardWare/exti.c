#include "exti.h"

//�ⲿ�ж����ú������԰�������PA0\PE2\PE3\PE4Ϊ����
void exti_init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//GPIO�ṹ������ĳ�Ա��ֵ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStruct.GPIO_OType =	;	//����ľ��巽ʽ
	GPIO_Init(GPIOA, &GPIO_InitStruct);	//����GPIO_Init()���ݽṹ�������Ա��Ϣ������Ӧ��GPIOA
	
	//GPIO�ṹ������ĳ�Ա��ֵ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStruct.GPIO_OType =	;	//����ľ��巽ʽ
	GPIO_Init(GPIOE, &GPIO_InitStruct);	//����GPIO_Init()���ݽṹ�������Ա��Ϣ������Ӧ��GPIOE
	
	//����ӳ���ϵ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	
	//EXTI�ṹ������ĳ�Ա��ֵ
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//���ж�
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;	//��Ӧ�����źŰ󶨶�Ӧ�ж��ߣ�0~15������23��EXTI_Line
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;	//����ѡ��
	EXTI_Init(&EXTI_InitStruct);	//����EXTI_Init()���ݽṹ�������Ա��Ϣ������Ӧ���ⲿ�ж�
	
	
	//NVIC�ṹ������ĳ�Ա��ֵ
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI0_IRQn;	//�����ж�ԴΪEXTI0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	1;//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //�жϵȼ��е���Ӧ���ȼ�������
	//Ƕ�������жϿ����������ã������ж�Դ�������жϵȼ���
	NVIC_Init(&NVIC_InitStruct);
	
	//NVIC�ṹ������ĳ�Ա��ֵ
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI2_IRQn;	//�����ж�ԴΪEXTI2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	2;//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //�жϵȼ��е���Ӧ���ȼ�������
	//Ƕ�������жϿ����������ã������ж�Դ�������жϵȼ���
	NVIC_Init(&NVIC_InitStruct);
	
	//NVIC�ṹ������ĳ�Ա��ֵ
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI3_IRQn;	//�����ж�ԴΪEXTI3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	2;//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //�жϵȼ��е���Ӧ���ȼ�������
	//Ƕ�������жϿ����������ã������ж�Դ�������жϵȼ���
	NVIC_Init(&NVIC_InitStruct);
	
	//NVIC�ṹ������ĳ�Ա��ֵ
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI4_IRQn;	//�����ж�ԴΪEXTI4
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	2;//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //�жϵȼ��е���Ӧ���ȼ�������
	//Ƕ�������жϿ����������ã������ж�Դ�������жϵȼ���
	NVIC_Init(&NVIC_InitStruct);
	
}


//����ȫ�ֱ����ı�־λ
uint8_t EXTI0_flag = 0;
uint8_t EXTI2_flag = 0;
uint8_t EXTI3_flag = 0;
uint8_t EXTI4_flag = 0;

//�ⲿ�жϷ��������ú������ɸ������ơ��������������ɷ��ء����ɵ��á����ɴ��Ρ��������Σ�
void EXTI0_IRQHandler(void)
{
	//�����ж����ĸ��ж�Դ�����ˣ�״̬�����˸ı䣩
	if( SET == EXTI_GetITStatus( EXTI_Line0 ) )
	{
		//Ϊ�˷����´��ٴ��жϵ��ж�״̬�ĸı䣬��������жϵ�״̬
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		//����ص����飺��ơ��ı��־λ
		EXTI0_flag = !EXTI0_flag;
		//���°��������ⲿ�жϣ�CPU��Ӧ�жϣ������жϷ��������ѱ�־λ���ģ������������while�������ϵ��жϱ�־λ�ĸı䣬�����������鼴�ɣ�
	
	}
}

void EXTI2_IRQHandler(void)
{
	//�����ж����ĸ��ж�Դ�����ˣ�״̬�����˸ı䣩
	if( SET == EXTI_GetITStatus( EXTI_Line2 ) )
	{
		//Ϊ�˷����´��ٴ��жϵ��ж�״̬�ĸı䣬��������жϵ�״̬
		EXTI_ClearITPendingBit(EXTI_Line2);
		
		//����ص����飺��ơ��ı��־λ
		EXTI2_flag = !EXTI2_flag;
		//���°��������ⲿ�жϣ�CPU��Ӧ�жϣ������жϷ��������ѱ�־λ���ģ������������while�������ϵ��жϱ�־λ�ĸı䣬�����������鼴�ɣ�
	
	}
}

//�ⲿ�жϷ��������ú������ɸ������ơ��������������ɷ��ء����ɵ��á����ɴ��Ρ��������Σ�
void EXTI3_IRQHandler(void)
{
	//�����ж����ĸ��ж�Դ�����ˣ�״̬�����˸ı䣩
	if( SET == EXTI_GetITStatus( EXTI_Line3 ) )
	{
		//Ϊ�˷����´��ٴ��жϵ��ж�״̬�ĸı䣬��������жϵ�״̬
		EXTI_ClearITPendingBit(EXTI_Line3);
		
		//����ص����飺��ơ��ı��־λ
		EXTI3_flag = !EXTI3_flag;
		//���°��������ⲿ�жϣ�CPU��Ӧ�жϣ������жϷ��������ѱ�־λ���ģ������������while�������ϵ��жϱ�־λ�ĸı䣬�����������鼴�ɣ�
	
	}
}

//�ⲿ�жϷ��������ú������ɸ������ơ��������������ɷ��ء����ɵ��á����ɴ��Ρ��������Σ�
void EXTI4_IRQHandler(void)
{
	//�����ж����ĸ��ж�Դ�����ˣ�״̬�����˸ı䣩
	if( SET == EXTI_GetITStatus( EXTI_Line4 ) )
	{
		//Ϊ�˷����´��ٴ��жϵ��ж�״̬�ĸı䣬��������жϵ�״̬
		EXTI_ClearITPendingBit(EXTI_Line4);
		
		//����ص����飺��ơ��ı��־λ
		EXTI4_flag = !EXTI4_flag;
		//���°��������ⲿ�жϣ�CPU��Ӧ�жϣ������жϷ��������ѱ�־λ���ģ������������while�������ϵ��жϱ�־λ�ĸı䣬�����������鼴�ɣ�
	
	}
}
