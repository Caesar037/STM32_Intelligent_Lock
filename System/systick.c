#include "systick.h"

static __IO uint32_t TimingDelay;
 
//����ϵͳ�δ�ʱ�� SysTick
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))
	{
		while (1);
	}
}

//��ȡ���ĳ���
//�� SysTick �жϺ��� SysTick_Handler()����
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

//�ж���ʱ����10usΪһ����λ
//Delay_us(1) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
void Systick_Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


//��������ʱ����Ĭ��us��
void Delay(uint32_t nTime)
{
	uint32_t ticks;
	uint32_t told,tnow,reload,tcnt=0;
	reload = SysTick->LOAD;			//��ȡ��װ�ؼĴ���ֵ
	ticks = nTime*(SystemCoreClock/1000000);	//�����ʱ�Ĵ�����168Hz����168�λ���1us
	told=SysTick->VAL;                   	//��ȡ��ǰ��ֵ�Ĵ���ֵ����ʼʱ��ֵ������ told=100

	while(1)
	{
		tnow=SysTick->VAL;			//��ȡ��ǰ��ֵ�Ĵ���ֵ

		if(tnow!=told)              	//��ǰֵ�����ڿ�ʼֵ˵�����ڼ���
		{         
			//systick��ʱ���ǵ���ʱ ���µݼ���  
			if(tnow<told)			//��ǰֵС�ڿ�ʼ��ֵ��˵��δ�Ƶ�0������tnow=80
				tcnt+=told-tnow;		//����ֵ=��ʼֵ-��ǰֵ

			else					//��ǰֵ���ڿ�ʼ��ֵ��˵���ѼƵ�0�����¼���
				tcnt+=reload-tnow+told; //����ֵ=��װ��ֵ-��ǰֵ+��ʼֵ

			told=tnow;                	//���¿�ʼֵ
			if(tcnt>=ticks)break;     	//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�
		} 
	}     
}

////��ʱ΢��  21MHZ
//void  Delay_us(uint16_t nus)
//{
//	SysTick->CTRL 	= 0; 					// �رն�ʱ��
//	SysTick->LOAD 	= 21*nus -1; 			// ���ü�����ֵ
//	SysTick->VAL 	= 0; 					// ��յ�ǰ����ֵ
//	SysTick->CTRL 	= 1; 					// ������ʱ��
//	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ�ʱ�䵽��
//	SysTick->CTRL	 = 0; 					// �رն�ʱ��
//}

////��ʱ����  21HZ
//void  Delay_ms(uint16_t nms)
//{
//	SysTick->CTRL 	= 0; 					// �رն�ʱ��
//	SysTick->LOAD 	= 21*nms*1000 -1; 		// ���ü�����ֵ
//	SysTick->VAL 	= 0; 					// ��յ�ǰ����ֵ
//	SysTick->CTRL 	= 1; 					// ������ʱ��
//	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ�ʱ�䵽��
//	SysTick->CTRL	 = 0; 					// �رն�ʱ��
//}
