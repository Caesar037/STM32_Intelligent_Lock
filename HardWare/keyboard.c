#include "keyboard.h"

void key_board_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    //使能端口A的硬件时钟，就是对端口A供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    //使能端口C的硬件时钟，就是对端口C供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    //使能端口D的硬件时钟，就是对端口D供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    //使能端口E的硬件时钟，就是对端口E供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    //使能端口F的硬件时钟，就是对端口F供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//使能端口G的硬件时钟，就是对端口G供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    
    //PD6 PD7 PC6 PC8      //PC11 PE5 PA6 PG9
	//使用GPIO_Init来配置引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;       //推挽输出模式,默认的
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7; //指定第6 7根引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;     //高速，但是功耗是最高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;  //无需上下拉（亦可使能下拉电阻）
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //D口
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_8; //指定第6 8根引脚
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //C口
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;            //指定第8根引脚
    GPIO_Init(GPIOF,&GPIO_InitStructure);              //F口
    
	/* 矩阵键盘输入模式必须使能内部上拉电阻，引脚悬空的时候为固定的高电平 */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	   //上拉
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;         //输入模式
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;            //指定第6根引脚
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //A口
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;            //指定第5根引脚
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //E口
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;            //指定第6根引脚
    GPIO_Init(GPIOG,&GPIO_InitStructure);              //G口
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;           //指定第11根引脚
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //C口
    
    PFout(8)=0;
}

char get_key_board(void)
{
    //PD6 PD7 PC6 PC8      
    PDout(6) = 0;
    PDout(7) = 1;
    PCout(6) = 1;
    PCout(8) = 1;
    Delay_ms(2);
	
    //PC11 PE5 PA6 PG9
    if( PCin(11) == 0 ) return '1';
    else if( PEin(5) == 0 ) return '2';
    else if( PAin(6) == 0 ) return '3';
    else if( PGin(9) == 0 ) return 'A';
    
    PDout(6) = 1;
    PDout(7) = 0; 
    PCout(6) = 1; 
    PCout(8) = 1;
    Delay_ms(2);

    //PC11 PE5 PA6 PG9
    if( PCin(11) == 0 ) return '4'; 
    else if( PEin(5) == 0 ) return '5';
    else if( PAin(6) == 0 ) return '6'; 
    else if( PGin(9) == 0 ) return 'B';
    
    PDout(6) = 1;
    PDout(7) = 1; 
    PCout(6) = 0; 
    PCout(8) = 1;
    Delay_ms(2);
    //PC11 PE5 PA6 PG9
    if( PCin(11) == 0 ) return '7'; 
    else if( PEin(5) == 0 ) return '8'; 
    else if( PAin(6) == 0 ) return '9'; 
    else if( PGin(9) == 0 ) return 'C';
    
    PDout(6) = 1;
    PDout(7) = 1; 
    PCout(6) = 1; 
    PCout(8) = 0;
    Delay_ms(2);
    //PC11 PE5 PA6 PG9
    if( PCin(11) == 0 ) return '*'; 
    else if( PEin(5) == 0 ) return '0'; 
    else if( PAin(6) == 0 ) return '#'; 
    else if( PGin(9) == 0 ) return 'D';
    
    return 'N';
}
