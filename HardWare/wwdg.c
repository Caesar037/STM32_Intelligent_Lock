#include "wwdg.h"

//�趨���ڿ��Ź��ĳ�ʱʱ��(8��ƵΪ��)����42MHZ/4096��/8 = 1281HZ->780us�����ʱʱ��Ϊ����127-63��*780us��
//��127�ݼ���80-->����47��--->����1�λ���780us---36.66ms(��ʱ����û���Ӧ��ι����
//��127�ݼ���63-->����64��--->����1�λ���780us--->49.92ms(����ֵ�ݼ���0x3F(63)���ѵ�ʱ�䣩
void wwdg_init(uint32_t WWDG_Prescaler, uint8_t WindowValue)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//����APB1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	//����Ԥ��Ƶֵ
	WWDG_SetPrescaler(WWDG_Prescaler);

	//���ô�������ֵ������ֵ80*780=62.4ms������ֵ63*780=49.14ms��
	WWDG_SetWindowValue(WindowValue);
	
	//�������ڿ��Ź�:�趨�������Ŀ�ʼ����ֵΪ127��0X7F�����ֵ����ʼֵ��127*780=99.06ms��
	WWDG_Enable(127);
	
	//����NVIC���ж�
	//NVIC�ṹ������ĳ�Ա��ֵ
	NVIC_InitStruct.NVIC_IRQChannel	= WWDG_IRQn;	//�����ж�ԴΪWWDG
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	0;	//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	//�жϵȼ��е���Ӧ���ȼ�������
	//Ƕ�������жϿ����������ã������ж�Դ�������жϵȼ���
	NVIC_Init(&NVIC_InitStruct);
	
	//�����־λ����ʼ����״̬δ֪��
	WWDG_ClearFlag();
	
	//�������ڿ��Ź��ж�
	WWDG_EnableIT();
	
	//�����ڳ�ʼ��ι��
	//WWDG_SetCounter(127);
}


//���ڿ��Ź����жϷ���������ι������
void WWDG_IRQHandler(void)
{
	if( SET == WWDG_GetFlagStatus())
	{
		WWDG_ClearFlag();
		
		//����صĲ���
		WWDG_SetCounter(127);	//ι��������������ʼֵ��
	}
}
