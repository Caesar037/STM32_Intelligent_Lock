#ifndef __OLED_H
#define __OLED_H

//包含头文件
#include "stm32f4xx.h"
#include <stdio.h>
#include "stdlib.h"
#include "systick.h"

/*
GND   D1-->PD15
VCC   D3-->PD1
SCL   D5-->PE8
SDA   D7-->PE10
*/

//宏定义
#define OLED_MODE 	0
#define SIZE 		8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define SCL_SDA_CLK		RCC_AHB1Periph_GPIOE

#define SCL_PORT    	GPIOE
#define SCL_PIN     	GPIO_Pin_8             

#define SDA_PORT		GPIOE
#define SDA_PIN     	GPIO_Pin_10

#define OLED_CMD  		0			//写命令
#define OLED_DATA 		1			//写数据

#define OLED_SCLK(x)	(x)?(GPIO_SetBits(SCL_PORT,SCL_PIN)):(GPIO_ResetBits(SCL_PORT,SCL_PIN))	//SCL IIC
#define OLED_SDAW(x)	(x)?(GPIO_SetBits(SDA_PORT,SDA_PIN)):(GPIO_ResetBits(SDA_PORT,SDA_PIN))	//SDA IIC-----输出模式使用
#define OLED_SDAR		GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)	//SDA IIC-----输入模式使用



//变量声明


//函数声明
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
uint8_t IIC_Wait_Ack(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);

void OLED_Display_On(void);
void OLED_Display_Off(void);

void OLED_Clear(void);
void OLED_Fill(void);  
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_Fill_Picture(unsigned char fill_Data);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void OLED_OFF(void);
void OLED_ON(void);
void OLED_Scroll_X(void);
void OLED_Scroll_Y(void);

void OLED_Config(void);
void OLED_Power_Init(void);

void OLED_Test(void);
void OLED_Space(void);

void OLED_Unlock(void);
void OLED_Lock(void);
void OLED_Face(void);
void OLED_Finger(void);
void Main_Menu(uint8_t mode);
void Password_Menu(uint8_t num);

#endif  
	 



