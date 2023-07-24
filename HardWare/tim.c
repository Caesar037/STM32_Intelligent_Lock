#include "tim.h"

//���û�����ʱ��7��ֻ�����ϼ��������趨��ʱ��7��ʱ1ms
void tim7_init(void)
{
	//����ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//��ʱ�ӣ�����ѡ������Ĳο��ֲ�2.2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	//�Խṹ�������Ա����ֵ���������Զ�����ֵ �� Ԥ��Ƶ��ֵ����ֵ���������趨��ʱ���Ķ�ʱʱ�䣬ʱ�䵽�������ж��źţ�
	TIM_TimeBaseInitStruct.TIM_Period = (10000/1000)-1;	//�Զ�����ֵ (10000)*(8400)/84000000=1s(������1ms��Ӧһ���ж�)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//Ԥ��Ƶ��ֵ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//��Ƶ����Ϊ1�����൱�ڲ���Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//����Ϊ���ϼ���
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//�ظ�������ֻ�и߼���ʱ������Ҫ���ã�
	
	//��ʼ����ʱ��
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	//����NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȵȼ�(����Ϊ��2���ȼ�ȡֵ��Χ��0~3��)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;	//��Ӧ���ȵȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//������Ӧ�Ķ�ʱ���ж�
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); 
	
	//������ʱ�����������ʱ������ʼ������
	TIM_Cmd(TIM7, ENABLE);
}


//��д�жϷ���������ʱ����ʱΪ1ms��
void TIM7_IRQHandler(void)
{
	//�ж��Ƕ�ʱ�����ĸ��ж�״̬���͸ı�
	if(SET == TIM_GetITStatus(TIM7, TIM_IT_Update) )
	{
		/*********** ʵ���Զ��幦�� ***********/
		
		IWDG_ReloadCounter();	//ι�����������ڶ�ʱ����������������ܹ���ʱι����ι���������������Ź���
		
		//����Ƿ�������һλ���ݣ�����1��⣬���ڱ䳤ָ���⣩
		if(Rx1TMP != Rx1Counter)
		{
			Rx1TMP = Rx1Counter;
		}
		else if(Rx1TMP && (Rx1TMP == Rx1Counter))
		{
			Rx1End = 1;	//�����¼���־λ��1������1��
			Rx1TMP = 0;
		}
		
		
		//����Ƿ�������һλ���ݣ�����2��⣬���ڱ䳤ָ���⣩
		if(Rx2TMP != Rx2Counter)
		{
			Rx2TMP = Rx2Counter;
		}
		else if(Rx2TMP && (Rx2TMP == Rx2Counter))
		{
			Rx2End = 1;	//�����¼���־λ��1������2��
			Rx2TMP = 0;
		}
		

		//����Ƿ�������һλ���ݣ�����3��⣬���ڱ䳤ָ���⣩
		if(Rx3TMP != Rx3Counter)
		{
			Rx3TMP = Rx3Counter;
		}
		else if(Rx3TMP && (Rx3TMP == Rx3Counter))
		{
			Rx3End = 1;	//�����¼���־λ��1������3��
			Rx3TMP = 0;
		}
		
		//����ж�״̬
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);		
	}
}
