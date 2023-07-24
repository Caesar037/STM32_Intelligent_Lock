#ifndef __AT24C02_H__
#define __AT24C02_H__

//头文件
#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"
#include "iic.h"

//宏定义

/*
 * 功  能：按页存储数据（1页8字节）
 * 参  数：
 *		addr：0x00--0xF
 *		pBuf：
 */
#define AT24C02_WRITE_PAGE(addr, pBuf) AT24C02_WriteData(addr, pBuf, 8)
#define AT24C02_READ_PAGE(addr, pBuf) AT24C02_ReadData(addr, pBuf, 8)

//函数声明

/*
 * 功  能：AT24C02实现页写 
 * 参  数：
 *		无
 * 返回值：
 *		无
 */
void AT24C02_Init(void);


/*
 * 功  能：AT24C02实现页写 
 * 参  数：
 *		addr	--->将数据写入 AT24C02 的 addr 这个地址中
 * 		pBuf	--->保存要写入的数据的首地址
 * 		length	--->你要写入的字节数
 * 返回值：
 *		失败	--->-1
 * 		成功	--->成功写入的字节数
 */
uint8_t AT24C02_WriteData(uint8_t addr , uint8_t *pBuf , uint8_t length);


/*
 * 功  能：AT24C02实现随机读
 *		addr	--->将从 AT24C02 读取数据的的地址
 * 		pBuf	--->读取到的数据所要存放的内存区域首地址
 * 		length	--->要读取的字节数
 * 返回值：
 *		失败	---> -1
 * 		成功	--->  0
 */
uint8_t AT24C02_ReadData(uint8_t addr , uint8_t *pBuf , uint8_t length);


#endif
