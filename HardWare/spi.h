#ifndef __SPI_H__
#define __SPI_H__

//ͷ�ļ����ã����������������ⲿ��������������&typedef�Ķ��壬�ṹ��&ö��&���������Ͷ��壬static���εĺ�������д��ͷ�ļ��� \
	inline���εĺ���д��ͷ�ļ���������ͷ�ļ��İ�����
	
#include "stm32f4xx.h"


#define Master_Or_Slave 1

//CS---PB14
//SCK---PB3
//MISO---PB4
//MOSI---PB5
#define SPI_CS_GPIO_CLK   RCC_AHB1Periph_GPIOB
#define SPI_SCK_GPIO_CLK  RCC_AHB1Periph_GPIOB
#define SPI_MOSI_GPIO_CLK RCC_AHB1Periph_GPIOB
#define SPI_MISO_GPIO_CLK RCC_AHB1Periph_GPIOB

#define SPI_CS_GPIO_PORT GPIOB
#define SPI_SCK_GPIO_PORT GPIOB
#define SPI_MOSI_GPIO_PORT GPIOB
#define SPI_MISO_GPIO_PORT GPIOB

#define SPI_CS_PIN GPIO_Pin_14
#define SPI_SCK_PIN GPIO_Pin_3
#define SPI_MISO_PIN GPIO_Pin_4
#define SPI_MOSI_PIN GPIO_Pin_5

#define SPI_SCK_SOURCE GPIO_PinSource3
#define SPI_MISO_SOURCE GPIO_PinSource4
#define SPI_MOSI_SOURCE GPIO_PinSource5

#define SPI_AF GPIO_AF_SPI1

#define SPIX SPI1

#define SPI_CS_SET(x) (x)?(GPIO_SetBits(SPI_CS_GPIO_PORT, SPI_CS_PIN)):(GPIO_ResetBits(SPI_CS_GPIO_PORT, SPI_CS_PIN))



void SPI_Sim_Init(void);
uint8_t SPI_SendByte(uint8_t byte);


#endif
