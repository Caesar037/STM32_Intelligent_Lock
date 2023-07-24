#ifndef __UART_H__
#define __UART_H__

//头文件作用：作函数的声明，外部变量的声明，宏&typedef的定义，结构体&枚举&共用体类型定义，static修饰的函数尽量写在头文件， \
	inline修饰的函数写在头文件，作其他头文件的包含；

#include <stdio.h>
#include "string.h"
#include "stm32f4xx.h"

#define USARTx_Print		USART1
#define USARTx_Bluetooth	USART2
#define USARTx_WiFi			USART3

#define WIFI_AP		0
#define WIFI_STA	0


extern volatile uint8_t Change_Password;
extern volatile uint8_t Change_Rfid;
extern volatile uint8_t Add_Finger;
extern volatile uint8_t Clean_Finger;
extern volatile uint8_t Add_Face;
extern volatile uint8_t Clean_Face;
extern volatile uint8_t Lock_Flag;

extern volatile uint32_t 	Rx1Counter;
extern volatile uint32_t 	Rx1TMP;
extern volatile uint32_t 	Rx1Data;
extern volatile uint32_t	Rx1End;
extern volatile uint8_t 	Rx1Buffer[64];
extern volatile uint8_t 	Tx1Buffer[64];

extern volatile uint32_t 	Rx2Counter;
extern volatile uint32_t 	Rx2TMP;
extern volatile uint32_t 	Rx2Data;
extern volatile uint32_t	Rx2End;
extern volatile uint8_t 	Rx2Buffer[64];
extern volatile uint8_t 	Tx2Buffer[64];

extern volatile uint32_t 	Rx3Counter;
extern volatile uint32_t 	Rx3TMP;
extern volatile uint32_t 	Rx3Data;
extern volatile uint32_t	Rx3End;
extern volatile uint8_t 	Rx3Buffer[64];
extern volatile uint8_t 	Tx3Buffer[64];

void UART1_Config(uint32_t BaudRate);
void UART2_Config(uint32_t BaudRate);
void UART3_Config(uint32_t BaudRate);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif
