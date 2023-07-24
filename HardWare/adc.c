#include "adc.h"

//�������裺PF7---ADC3_IN5
void adc_init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//����ص�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;		//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;	//ģ��ģʽ
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	    //��������ת��ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//�Ĵ����������Ҷ���
	//ADC_InitStruct.ADC_ExternalTrigConv = ;	//�Ƿ������ⲿ����
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//�ⲿ�������ص�ѡ��
	ADC_InitStruct.ADC_NbrOfConversion = 1;	    //ָ��ת��ͨ������Ϊ1��
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;	//���÷ֱ���Ϊ12λ����󣩣�������ת�����ȣ�ת������Խ�ߣ���Ӧ����������ԽС��3.3V/4096 = 0.8mv
	ADC_Init(ADC3, &ADC_InitStruct);
	
	//����ͨ������
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;	    //���ö���ADCģʽ
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//����DMAģʽ��ֱ���ڴ���ʣ�
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;	//����Ƶ�õ�����Ƶ�ʣ�84MHz/2 = 42MHz��3~5��������Ƶ��֮�䣬��APB2�����PCLK2��Ƶ����������Ƶ���ֱܷ���Ӱ�죬�䲻����168MHz/12 = 14MHz��
	//ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//���������Ĳ���ʱ���������ڶ��ADC�������
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//ʹ������ADC3
	ADC_Cmd(ADC3, ENABLE);
}

//��ȡADCת�����
float get_adc_value(void)
{
	uint32_t analog = 0;
	
	//ָ��ͨ��������ADC����ͨ�������ԣ�����ͨ����˳�����У�������ռ��
	//��ת��ʱ�� = �������ڣ�3~5�� + 12������ --> 15/42MHz = 0.357us	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);	//ADC��ͨ����ת��˳�򣬲�������
	
	//����ADC3��ͨ��ת��
	ADC_SoftwareStartConv(ADC3);
	
	//������ѭ���ȴ�ת����ɣ������жϣ���ѡ���ã�
	//������
	
	//��ȡADC3��ת�����
	analog = ADC_GetConversionValue(ADC3);
	
	//�����ڶ��ε�����������ο���ѹ3.3V���Ա�
	return analog*3.3/4096;
	
}
