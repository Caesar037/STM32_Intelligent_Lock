#ifndef __IIC_H__
#define __IIC_H__

//ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬static���εĺ�������д��ͷ�ļ��� \
	inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
	
#include "stm32f4xx.h"
#include "systick.h"


//SDA: PB9
//SCL: PB8
#define RCC_AHB1Periph_SDA RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SCL RCC_AHB1Periph_GPIOB

#define SIM_SDA_POR GPIOB
#define SIM_SCL_POR GPIOB

#define SIM_SDA_PIN GPIO_Pin_9
#define SIM_SCL_PIN GPIO_Pin_8


#define IIC_SDA_SET(x) (x)?(GPIO_SetBits(SIM_SDA_POR, SIM_SDA_PIN)):(GPIO_ResetBits(SIM_SDA_POR, SIM_SDA_PIN))
#define IIC_SCL_SET(x) (x)?(GPIO_SetBits(SIM_SCL_POR, SIM_SCL_PIN)):(GPIO_ResetBits(SIM_SCL_POR, SIM_SCL_PIN))


#define IIC_SDA_READ GPIO_ReadInputDataBit(SIM_SDA_POR, SIM_SDA_PIN)



void IIC_Sim_Init(void);
void IIC_Sim_Start(void);
void IIC_Sim_Stop(void);
void IIC_Sim_SendByte(uint8_t data);
uint8_t IIC_Sim_ReadByte(void);
uint8_t IIC_Sim_WaitSlavAck(void);
void IIC_Sim_MasterAck(char ack);

#endif
