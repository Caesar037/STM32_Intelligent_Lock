#ifndef __SPI_H__
#define __SPI_H__

//头文件作用：作函数的声明，外部变量的声明，宏&typedef的定义，结构体&枚举&共用体类型定义，static修饰的函数尽量写在头文件， \
	inline修饰的函数写在头文件，作其他头文件的包含；
	
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
