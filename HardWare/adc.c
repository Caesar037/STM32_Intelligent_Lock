#include "adc.h"

//光敏电阻：PF7---ADC3_IN5
void adc_init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//打开相关的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;		//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;	//模拟模式
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	    //开启连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//寄存器中数据右对齐
	//ADC_InitStruct.ADC_ExternalTrigConv = ;	//是否启用外部触发
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//外部触发边沿的选择
	ADC_InitStruct.ADC_NbrOfConversion = 1;	    //指定转换通道数量为1；
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;	//设置分辨率为12位（最大），即设置转换精度，转换精度越高，相应的量化误差就越小；3.3V/4096 = 0.8mv
	ADC_Init(ADC3, &ADC_InitStruct);
	
	//常规通道配置
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;	    //设置独立ADC模式
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//禁用DMA模式（直接内存访问）
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;	//两分频得到采样频率，84MHz/2 = 42MHz（3~5倍的输入频率之间，由APB2外设的PCLK2分频产生，输入频率受分辨率影响，其不超过168MHz/12 = 14MHz）
	//ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//两个采样的采样时间间隔，用于多个ADC采样间隔
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//使能外设ADC3
	ADC_Cmd(ADC3, ENABLE);
}

//获取ADC转换结果
float get_adc_value(void)
{
	uint32_t analog = 0;
	
	//指定通道，配置ADC常规通道组特性（常规通道按顺序运行，不会抢占）
	//总转换时间 = 采样周期（3~5） + 12个周期 --> 15/42MHz = 0.357us	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);	//ADC，通道，转换顺序，采样周期
	
	//开启ADC3的通道转换
	ADC_SoftwareStartConv(ADC3);
	
	//主函数循环等待转换完成，或开启中断（可选配置）
	//？？？
	
	//获取ADC3的转换结果
	analog = ADC_GetConversionValue(ADC3);
	
	//再作第二次的量化处理，与参考电压3.3V作对比
	return analog*3.3/4096;
	
}
