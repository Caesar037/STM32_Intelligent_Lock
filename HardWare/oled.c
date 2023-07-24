#include "oled.h"
#include "oledfont.h"		//�ֿ�ͷ�ļ�
#include "bmp.h"			//ͼƬͷ�ļ�

/* ����������ĺ궨���аѺ������ʱ�����滻��SysTick����ʱ���������������Ǹ��ͻ����Ǹ��� */
#define DELAY_US( nus)	Delay_us( nus)	//��ʱ10us
#define DELAY_MS( nms)	Delay_ms( nms)	//��ʱ10ms

/*******************************IICͨ�����ô���**********************************/

//����SDA����������������ģʽ��ͨ�����ν������Ź���ģʽ���޸�
void OLED_SDA_Mode(GPIOMode_TypeDef IOMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin		= SDA_PIN;					
	GPIO_InitStructure.GPIO_Mode	= IOMode;				//���ģʽ/����ģʽ
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;		  
	
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
}

//IIC Start
void IIC_Start()
{
	OLED_SDA_Mode(GPIO_Mode_OUT);						//����SDA����Ϊ���ģʽ
	
	//��֤SCL��SDAΪ�ߵ�ƽ���������������
	OLED_SCLK(1);
	OLED_SDAW(1);
	
	//��ʱ1us�Ժ������߱�ɵ͵�ƽ
	DELAY_US(1);
	OLED_SDAW(0);
	
	//��ʱ1us�Ժ�ʱ����Ҳ��ɵ͵�ƽ
	DELAY_US(1);
	OLED_SCLK(0);
	DELAY_US(1);
}

//IIC Stop
void IIC_Stop()
{
	//��֤SDA����Ϊ���ģʽ
	OLED_SDA_Mode(GPIO_Mode_OUT);
	
	//��֤SCL��SDAΪ�͵�ƽ���������������
	OLED_SCLK(0);
	OLED_SDAW(0);
	
	//ʱ����Ϊ�ߵ�ƽ��������Ϊ�͵�ƽ
	DELAY_US(1);
	OLED_SCLK(1);
	
	//��ʱ1us�������߱�ߵ�ƽ
	DELAY_US(1);
	OLED_SDAW(1);
	DELAY_US(1);
}

//IIC Wait
uint8_t IIC_Wait_Ack()
{
	uint8_t ack=0;//������־���͵�ƽ�з������ߵ�ƽ�޷���
	
	//��֤SDA����Ϊ����ģʽ---��������ȡSDA�ϵĵ�ƽ
	OLED_SDA_Mode(GPIO_Mode_IN);
	
	//��֤SCL�����ͣ��Է�������ӻ�����SDA�ߵĵ�ƽ
	OLED_SCLK(0);
	DELAY_US(1);
	
	//ʱ���߸ߵ�ƽ
	OLED_SCLK(1);
	DELAY_US(1);
	
	//��Ӧ��Ϊ�͵�ƽ����Ӧ��Ϊ�ߵ�ƽ
	if(OLED_SDAR)	//��Ӧ��---PBin(6)==1
	{
		ack=1;
		IIC_Stop();	//����ֹͣ�ź�
	}
	else			//��Ӧ��
		ack=0;
	
	//��Ӧ��Ϳ��Խ�����һ�β���
	OLED_SCLK(0);//ʱ�������͵�ƽ�����������޸���ֵ
	DELAY_US(1);
	return ack;
	
}

//IIC Write byte
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;

	//��֤SDA����Ϊ���ģʽ
	OLED_SDA_Mode(GPIO_Mode_OUT);
	
	//д�����ݣ�ʱ���߱����ǵ͵�ƽ
	OLED_SCLK(0);//ʱ�������ͣ�Ҳ�����������ߴ�����ֵ�޸�
	
	for(i=0;i<8;i++)		
	{
		//IIC_Byte������Ҫ���͵����ݣ���0�η���7λ�ģ���1�η���6λ��
		if(IIC_Byte&(1<<(7-i)))//������������ֵ1
			OLED_SDAW(1);	//������Ϊ�ߵ�ƽ
		else 				//������������ֵ0
			OLED_SDAW(0);	//������Ϊ�͵�ƽ
		
		//��ʱһ��ʱ�������ʱ���ߣ�ֻ��SCLΪ�ߵ�ƽ���ſ��Զ�ȡ����
		DELAY_US(1);
		OLED_SCLK(1);	//��ʱ�������ߣ���ʼ��ȡ�����ߵ���ֵ
		
		//��һ��д�����ݣ�������SCLΪ�͵�ƽ
		DELAY_US(1);	//��1usʱ���ȡ�����Ժ�
		OLED_SCLK(0);	//��ʱ�������ͣ��������ߴ�����ֵ�޸�
		DELAY_US(1);
	}
}

//IIC Write Command
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);    	//�ӻ���ַ��SA0=0��Ӳ����ַѡ��λ����R/W#=0����д����λ��
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);		//�����ֽڣ�Co=0���Ƿ��������ͣ�0����������D/C#=1������/����ѡ��λ��
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}

//IIC Write Data
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);		//�ӻ���ַ��SA0=0��Ӳ����ַѡ��λ����R/W#=0����д����λ��
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);		//�����ֽڣ�Co=0���Ƿ��������ͣ�0����������D/C#=1������/����ѡ��λ��
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}

//IIC Write Byte
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}
	else 
	{
		Write_IIC_Command(dat);
	}
}

/*******************************OLED��ʾ�����ô���**********************************/

//��������
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}

//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		

//����������ȫ��0��
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}
}

//����������ȫ��1��
void OLED_Fill(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	}
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';		//�õ�ƫ�ƺ��ֵ			
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);
	}
}

//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//��ʾ���֣�������ʾ16*16��С��
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}
}

//���ͼƬ
void OLED_Fill_Picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//����ҳ��ַ��0~1��
		OLED_WR_Byte(0x00,0);		//������ʾλ�á��е͵�ַ
		OLED_WR_Byte(0x10,0);		//������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}

//��ʾBMPͼƬ�����128��64������ʼ������(x,y)��x1Ϊ�������꣨0��127����y1Ϊ�������꣨0��7��
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

	if(y1%8==0) y=y1/8;  
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA);  	
		}
	}
}

//ˮƽ��������ҹ���
void OLED_Scroll_X(void)
{
	OLED_WR_Byte(0x2E,OLED_CMD);        //�رչ���
	OLED_WR_Byte(0x26,OLED_CMD);        //ˮƽ��������ҹ��� 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0x00,OLED_CMD);        //��ʼҳ 0
	OLED_WR_Byte(0x07,OLED_CMD);        //����ʱ����
	OLED_WR_Byte(0x07,OLED_CMD);        //��ֹҳ 7
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0xFF,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0x2F,OLED_CMD);        //��������
}

//ˮƽ��ֱ��ˮƽ����
void OLED_Scroll_Y(void)
{
	OLED_WR_Byte(0x2e,OLED_CMD);        //�رչ���
	OLED_WR_Byte(0x29,OLED_CMD);        //ˮƽ��ֱ��ˮƽ���� 29/2a
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0x00,OLED_CMD);        //��ʼҳ 0
	OLED_WR_Byte(0x07,OLED_CMD);        //����ʱ����
	OLED_WR_Byte(0x07,OLED_CMD);        //��ֹҳ 1
	OLED_WR_Byte(0x01,OLED_CMD);        //��ֱ����ƫ����
	OLED_WR_Byte(0x2F,OLED_CMD);        //��������
}

/*************************************************
  Func Name:	OLED_ON
  Description:  ��OLED�������л���
  Input:        ��
  Output:       ��
*************************************************/
void OLED_ON(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //���õ�ɱ�
	OLED_WR_Byte(0X14,OLED_CMD);  //������ɱ�
	OLED_WR_Byte(0XAF,OLED_CMD);  //OLED����
}

/*************************************************
  Func Name:	OLED_OFF
  Description:  ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  Input:        ��
  Output:       ��
*************************************************/
void OLED_OFF(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //���õ�ɱ�
	OLED_WR_Byte(0X10,OLED_CMD);  //�رյ�ɱ�
	OLED_WR_Byte(0XAE,OLED_CMD);  //OLED����
}

//��ʼ��OLED��ʾ��������IC��SSD1306				    
void OLED_Config(void)
{	
/*********************GPIO_Pin SET*************************/
	GPIO_InitTypeDef GPIO_InitStructure;;
	
	RCC_AHB1PeriphClockCmd(SCL_SDA_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		= SCL_PIN | SDA_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;//�����������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;//û��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
	GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	
	OLED_SCLK(1);
	OLED_SDAW(1);
	
/**********************************************************/
	DELAY_MS(200);

	OLED_WR_Byte(0xAE,OLED_CMD);//---display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//---set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//---set page address
	OLED_WR_Byte(0x81,OLED_CMD);//---contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//---128   
	OLED_WR_Byte(0xA1,OLED_CMD);//---set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//---normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//---set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//---1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//---Com scan direction
	
	OLED_WR_Byte(0xD3,OLED_CMD);//set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//---turn on oled panel
	
	OLED_Clear();				 //�����Ļ
}

//��ͨ���������ߵ͵�ƽ����䵱VCC��GND
void OLED_Power_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��������PD15---GND��PD1---VCC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_15;//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//����Ĭ��״̬��PD15---0��PD1---1
	GPIO_ResetBits(GPIOD, GPIO_Pin_15);
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
}

//��ʾ����ʾ���Ժ���
void OLED_Test(void)
{	
//	//��ʾ����  ������x����ʼ���꣬��y�У���n�����֣�һ������ռ��16*16�����ص�
//	OLED_ShowCHinese(0,0,0);

	OLED_Clear();
	OLED_ShowCHinese(32,2,58);						//��
	OLED_ShowCHinese(48,2,59);						//��
	OLED_ShowCHinese(64,2,60);						//��
	
//	//��ʾӢ���ַ���  ������x���꣬��y�У���ʾ�����ݣ�ÿ���ֵĴ�С����ѡ8��16��
//	OLED_ShowString(0, 0, (u8*)"led1:", 16);

	OLED_ShowString(80, 2, (u8*)"...", 16);			//...
	DELAY_MS(1000);

//	//��ʾBMPͼƬ  ��������ʼ������(x,y)��x1Ϊ��ȣ�0��127����y1Ϊ���ȣ�0��7��
//	OLED_DrawBMP(0,0,128,8,BMP1);
	
	OLED_Clear();
//	OLED_DrawBMP(0,0,128,8,BMP1);
	DELAY_MS(1000);
}

//oled̫���˶������Ժ�����ʹ��ǰ������
void OLED_Space(void)
{
	OLED_DrawBMP(0,0,128,8,BMP_SPACE1);
	DELAY_US(1000);
	OLED_DrawBMP(0,0,128,8,BMP_SPACE2);
	DELAY_US(1000);
	OLED_DrawBMP(0,0,128,8,BMP_SPACE3);
	DELAY_US(1000);
	OLED_DrawBMP(0,0,128,8,BMP_SPACE4);
	DELAY_US(1000);
	OLED_DrawBMP(0,0,128,8,BMP_SPACE5);
	DELAY_US(1000);
}

//oled����ͼ�꺯��
void OLED_Unlock(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,UNLOCK);
}

//oled����ͼ�꺯��
void OLED_Lock(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,LOCK);
}

//oled����ͼ�꺯��
void OLED_Face(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,FACE);
}

//oled����ͼ�꺯��
void OLED_Finger(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,FINGER);
}

//���˵�����
void Main_Menu(uint8_t mode)
{
	OLED_Clear();
	OLED_ShowString(16, 0, (u8*)"password", 16);
	OLED_ShowString(16, 2, (u8*)"rfid    ", 16);
	OLED_ShowString(16, 4, (u8*)"finger  ", 16);
	OLED_ShowString(16, 6, (u8*)"face    ", 16);
	
	if(mode == 'A')			OLED_ShowChar(0,0,'*',16);
	else if(mode == 'B')	OLED_ShowChar(0,2,'*',16);
	else if(mode == 'C')	OLED_ShowChar(0,4,'*',16);
	else if(mode == 'D')	OLED_ShowChar(0,6,'*',16);
}

uint8_t n = 0;

//������溯��
void Password_Menu(uint8_t num) //num 0-5��6λ��
{
	OLED_Clear();
	OLED_ShowString(16, 4, (u8*)"_ _ _ _ _ _", 16);
	for(n=0; n<num; n++)	OLED_ShowChar((n+1)*16, 4, '*', 16);
}
