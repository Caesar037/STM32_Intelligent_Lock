#include "keyboard.h"

void key_board_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    //ʹ�ܶ˿�A��Ӳ��ʱ�ӣ����ǶԶ˿�A����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    //ʹ�ܶ˿�C��Ӳ��ʱ�ӣ����ǶԶ˿�C����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    //ʹ�ܶ˿�D��Ӳ��ʱ�ӣ����ǶԶ˿�D����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    //ʹ�ܶ˿�E��Ӳ��ʱ�ӣ����ǶԶ˿�E����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    //ʹ�ܶ˿�F��Ӳ��ʱ�ӣ����ǶԶ˿�F����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//ʹ�ܶ˿�G��Ӳ��ʱ�ӣ����ǶԶ˿�G����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    
    //PD6 PD7 PC6 PC8      //PC11 PE5 PA6 PG9
	//ʹ��GPIO_Init����������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;       //�������ģʽ,Ĭ�ϵ�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7; //ָ����6 7������
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;     //���٣����ǹ��������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;  //���������������ʹ���������裩
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //D��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_8; //ָ����6 8������
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //C��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;            //ָ����8������
    GPIO_Init(GPIOF,&GPIO_InitStructure);              //F��
    
	/* �����������ģʽ����ʹ���ڲ��������裬�������յ�ʱ��Ϊ�̶��ĸߵ�ƽ */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	   //����
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;         //����ģʽ
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;            //ָ����6������
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //A��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;            //ָ����5������
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //E��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;            //ָ����6������
    GPIO_Init(GPIOG,&GPIO_InitStructure);              //G��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;           //ָ����11������
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //C��
    
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
