#include "pwm.h"

//PF9---TIM14_CH1
void pwm_led_init(void)	//��Ҫ�õ���ʱ�������ͨ�����PWM�����ź�
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//ʹ��GPIO�Ͷ�ʱ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	//�����Ź�������������ʱ�����ͨ��1
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//��ʱ����ʱʱ��Ϊ��T = (10000-1) * (8400-1) / 84000000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = (10000/100)-1;	//�Զ�����ֵ�����趨PWM��Ƶ�ʣ���������0�ۼӵ�99���ֵ�����¹�0�����¼��㣩��100HZ��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//Ԥ��Ƶ��ֵ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//��Ƶ����Ϊ1�����൱�ڲ���Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//����Ϊ���ϼ���
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//�ظ�������ֻ�и߼���ʱ��TIM1��TIM8����Ҫ���ã�
	//��ʼ����ʱ��
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);

	//ͨ���Ĳ�������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //ͨ��ģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;	//ָ��Ҫ���ص�����ȽϼĴ����е�����ֵ�����趨�ȽϼĴ����ıȽ�ֵ�����趨ռ�ձ�ֵ��0%��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;	//ָ���͵�ƽΪ��Ч��ƽ�����趨0%�ĵ͵�ƽ������Ĭ��״̬Ϊ��
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);	//��ʼ��ͨ��1

	//������ʱ�����������ʱ������ʼ������
	TIM_Cmd(TIM14, ENABLE);
}

//PF8---TIM13_CH1
void pwm_beep_init(void)	//��Ҫ�õ���ʱ�������ͨ�����PWM�����ź�
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//ʹ��GPIO�Ͷ�ʱ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	
	//�����Ź�������������ʱ�����ͨ��1
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//��ʱ����ʱʱ��Ϊ��(10000-1) * (8400-1) / 84000000 = 1Hz --> 1s
	TIM_TimeBaseInitStruct.TIM_Period = (10000/100)-1;	//�Զ�����ֵ�����趨PWM��Ƶ�ʣ���������0�ۼӵ�99���ֵ�����¹�0�����¼��㣩��100HZ��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//Ԥ��Ƶ��ֵ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//��Ƶ����Ϊ1�����൱�ڲ���Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//����Ϊ���ϼ���
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//�ظ�������ֻ�и߼���ʱ��TIM1��TIM8����Ҫ���ã�
	//��ʼ����ʱ��
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseInitStruct);

	//ͨ���Ĳ�������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //ͨ��ģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ((10000/100)*(0)/100);	//ָ��Ҫ���ص�����ȽϼĴ����е�����ֵ�����趨�ȽϼĴ����ıȽ�ֵ�����趨ռ�ձ�ֵ��0%��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//ָ���͵�ƽΪ��Ч��ƽ�����趨0%�ĵ͵�ƽ������Ĭ��״̬Ϊ��
	TIM_OC1Init(TIM13, &TIM_OCInitStruct);	//��ʼ��ͨ��1

	//������ʱ�����������ʱ������ʼ������
	TIM_Cmd(TIM13, ENABLE);
}

static uint32_t tim13_cnt=0;

void pwm_beep_set(uint32_t freq, uint32_t duty)
{
	//����ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	
	//�ر�TIM13
	TIM_Cmd(TIM13, DISABLE);
	
    //��ʱ���Ļ������ã��������ö�ʱ������������Ƶ��Ϊ freq Hz
    TIM_TimeBaseStruct.TIM_Period = (400000/freq)-1; //���ö�ʱ�����Ƶ��
    TIM_TimeBaseStruct.TIM_Prescaler = 210-1;		 //��һ�η�Ƶ�����ΪԤ��Ƶ
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    tim13_cnt = TIM_TimeBaseStruct.TIM_Period;
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStruct);
	
	TIM_SetCompare1(TIM13,(tim13_cnt+1) * duty/100); //����ռ�ձ�
	
	//ʹ��TIM13
	TIM_Cmd(TIM13, ENABLE);	
}

//PC9---TIM3_CH4
void pwm_SG90_init(void)	//��Ҫ�õ���ʱ�������ͨ�����PWM�����ź�
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//�����Ź�������������ʱ�����ͨ��1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//���ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//��ʱ����ʱʱ��Ϊ��((10000/50)-1) * (8400-1) / 84000000 = 50Hz  --> 20ms ���ƶ����Ҫ������ʱ��Ϊ20ms����PWM��Ƶ��Ϊ50Hz
	TIM_TimeBaseInitStruct.TIM_Period = (10000/50)-1;	//�Զ�����ֵ�����趨PWM��Ƶ�ʣ���������0�ۼӵ�200���ֵ�����¹�0�����¼��㣩��ĸ����200
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;	//Ԥ��Ƶ��ֵ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//��Ƶ����Ϊ1�����൱�ڲ���Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//����Ϊ���ϼ���
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 	//�ظ�������ֻ�и߼���ʱ������Ҫ���ã�
	//��ʼ����ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//ͨ���Ĳ�������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //ͨ��ģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;	//���ת���Ƕȣ�5��0�㣻	10��45�㣻	15��90�㣻	20��135�㣻	25��180�� 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//ָ���ߵ�ƽΪ��Ч��ƽ�����趨0%�ĸߵ�ƽ������Ĭ��״̬Ϊ��
	TIM_OC4Init(TIM3, &TIM_OCInitStruct);	//��ʼ��ͨ��4
	
	//������ʱ�����������ʱ������ʼ������
	TIM_Cmd(TIM3, ENABLE);
}

