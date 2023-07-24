#ifndef __DHT11_H
#define __DHT11_H

//����ͷ�ļ�
#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"

//�궨��
/* ����������ĺ궨���аѺ������ʱ�����滻��SysTick����ʱ���������������Ǹ��ͻ����Ǹ��� */
#define DHT11_DELAY_US( nus)	Delay_us( nus)
#define DHT11_DELAY_MS( nms)	Delay_ms( nms)

#define DHT11_HIGH				1
#define DHT11_LOW				0

//DHT11---PG9
#define DHT11_CLK				RCC_AHB1Periph_GPIOG
#define DHT11_PIN				GPIO_Pin_9             
#define DHT11_PORT				GPIOG


#define DHT11_DATA_IN()			GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

#define DHT11_DATA_OUT(x)		if(x)								\
								GPIO_SetBits(DHT11_PORT,DHT11_PIN);	\
								else								\
								GPIO_ResetBits(DHT11_PORT,DHT11_PIN);

//��������
typedef struct
{
	uint8_t  humi_int;			//ʪ�ȵ���������
	uint8_t  humi_deci;	 		//ʪ�ȵ�С������
	uint8_t  temp_int;	 		//�¶ȵ���������
	uint8_t  temp_deci;	 		//�¶ȵ�С������
	uint8_t  check_sum;	 		//У���
	
}DHT11_Data_TypeDef;


//��������
void DHT11_Config(void);
void print_dht11_data(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);

#endif
