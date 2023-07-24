#ifndef __W25Q128_H
#define __W25Q128_H

#include "stm32f4xx.h"
#include "systick.h"
#include "sys.h"
#include "spi.h"

#define W25Q128_CS  PBout(14)


void W25Q128_Init(void);

void  W25Q128_WriteEnable(void);
void  W25Q128_WriteDisable(void);

uint8_t W25Q128_ReadStatusRegister1(void);

uint16_t W25Q128_ReadID(void);
void W25Q128_ReadData(uint32_t Address, uint8_t *rdatabuf, uint32_t datalen);
void W25Q128_PageProgram(uint32_t Address, uint8_t *rdatabuf, uint32_t datalen);
void W25Q128_SectorErase(uint32_t Address);

#endif
