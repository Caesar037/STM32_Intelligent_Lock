#include "oled.h"
#include "oledfont.h"		//字库头文件
#include "bmp.h"			//图片头文件

/* 可以在下面的宏定义中把后面的延时函数替换换SysTick的延时函数，就是想用那个就换成那个的 */
#define DELAY_US( nus)	Delay_us( nus)	//延时10us
#define DELAY_MS( nms)	Delay_ms( nms)	//延时10ms

/*******************************IIC通信配置代码**********************************/

//由于SDA存在输入和输出两种模式，通过传参进行引脚工作模式的修改
void OLED_SDA_Mode(GPIOMode_TypeDef IOMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin		= SDA_PIN;					
	GPIO_InitStructure.GPIO_Mode	= IOMode;				//输出模式/输入模式
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;		  
	
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
}

//IIC Start
void IIC_Start()
{
	OLED_SDA_Mode(GPIO_Mode_OUT);						//设置SDA引脚为输出模式
	
	//保证SCL和SDA为高电平，方便后续的拉低
	OLED_SCLK(1);
	OLED_SDAW(1);
	
	//延时1us以后，数据线变成低电平
	DELAY_US(1);
	OLED_SDAW(0);
	
	//延时1us以后，时钟线也变成低电平
	DELAY_US(1);
	OLED_SCLK(0);
	DELAY_US(1);
}

//IIC Stop
void IIC_Stop()
{
	//保证SDA引脚为输出模式
	OLED_SDA_Mode(GPIO_Mode_OUT);
	
	//保证SCL和SDA为低电平，方便后续的拉高
	OLED_SCLK(0);
	OLED_SDAW(0);
	
	//时钟线为高电平，数据线为低电平
	DELAY_US(1);
	OLED_SCLK(1);
	
	//延时1us，数据线变高电平
	DELAY_US(1);
	OLED_SDAW(1);
	DELAY_US(1);
}

//IIC Wait
uint8_t IIC_Wait_Ack()
{
	uint8_t ack=0;//反馈标志，低电平有反馈，高电平无反馈
	
	//保证SDA引脚为输入模式---让主机读取SDA上的电平
	OLED_SDA_Mode(GPIO_Mode_IN);
	
	//保证SCL线拉低，以方便后续从机更改SDA线的电平
	OLED_SCLK(0);
	DELAY_US(1);
	
	//时钟线高电平
	OLED_SCLK(1);
	DELAY_US(1);
	
	//有应答为低电平，无应答为高电平
	if(OLED_SDAR)	//无应答---PBin(6)==1
	{
		ack=1;
		IIC_Stop();	//发送停止信号
	}
	else			//有应答
		ack=0;
	
	//有应答就可以进行下一次操作
	OLED_SCLK(0);//时钟线拉低电平，让数据线修改数值
	DELAY_US(1);
	return ack;
	
}

//IIC Write byte
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;

	//保证SDA引脚为输出模式
	OLED_SDA_Mode(GPIO_Mode_OUT);
	
	//写入数据，时钟线必须是低电平
	OLED_SCLK(0);//时钟线拉低，也就是让数据线处理数值修改
	
	for(i=0;i<8;i++)		
	{
		//IIC_Byte就是需要发送的数据，第0次发送7位的，第1次发送6位的
		if(IIC_Byte&(1<<(7-i)))//传过来的是数值1
			OLED_SDAW(1);	//数据线为高电平
		else 				//传过来的是数值0
			OLED_SDAW(0);	//数据线为低电平
		
		//延时一段时间后拉高时钟线，只有SCL为高电平，才可以读取数据
		DELAY_US(1);
		OLED_SCLK(1);	//把时钟线拉高，开始读取数据线的数值
		
		//下一次写入数据，必须是SCL为低电平
		DELAY_US(1);	//花1us时间读取数据以后
		OLED_SCLK(0);	//把时钟线拉低，让数据线处理数值修改
		DELAY_US(1);
	}
}

//IIC Write Command
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);    	//从机地址，SA0=0（硬件地址选择位），R/W#=0（读写请求位）
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);		//控制字节，Co=0（是否连续发送，0可连续），D/C#=1（数据/命令选择位）
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}

//IIC Write Data
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);		//从机地址，SA0=0（硬件地址选择位），R/W#=0（读写请求位）
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);		//控制字节，Co=0（是否连续发送，0可连续），D/C#=1（数据/命令选择位）
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

/*******************************OLED显示屏配置代码**********************************/

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		

//清屏函数（全置0）
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}
}

//填屏函数（全置1）
void OLED_Fill(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	}
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';		//得到偏移后的值			
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

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
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

//显示一个字符号串
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

//显示汉字，限制显示16*16大小的
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

//填充图片
void OLED_Fill_Picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//设置页地址（0~1）
		OLED_WR_Byte(0x00,0);		//设置显示位置—列低地址
		OLED_WR_Byte(0x10,0);		//设置显示位置—列高地址
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}

//显示BMP图片，最大（128×64），起始点坐标(x,y)，x1为结束坐标（0～127），y1为结束坐标（0～7）
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

//水平向左或者右滚动
void OLED_Scroll_X(void)
{
	OLED_WR_Byte(0x2E,OLED_CMD);        //关闭滚动
	OLED_WR_Byte(0x26,OLED_CMD);        //水平向左或者右滚动 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);        //起始页 0
	OLED_WR_Byte(0x07,OLED_CMD);        //滚动时间间隔
	OLED_WR_Byte(0x07,OLED_CMD);        //终止页 7
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0xFF,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动
}

//水平垂直和水平滚动
void OLED_Scroll_Y(void)
{
	OLED_WR_Byte(0x2e,OLED_CMD);        //关闭滚动
	OLED_WR_Byte(0x29,OLED_CMD);        //水平垂直和水平滚动 29/2a
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);        //起始页 0
	OLED_WR_Byte(0x07,OLED_CMD);        //滚动时间间隔
	OLED_WR_Byte(0x07,OLED_CMD);        //终止页 1
	OLED_WR_Byte(0x01,OLED_CMD);        //垂直滚动偏移量
	OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动
}

/*************************************************
  Func Name:	OLED_ON
  Description:  将OLED从休眠中唤醒
  Input:        无
  Output:       无
*************************************************/
void OLED_ON(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //设置电荷泵
	OLED_WR_Byte(0X14,OLED_CMD);  //开启电荷泵
	OLED_WR_Byte(0XAF,OLED_CMD);  //OLED唤醒
}

/*************************************************
  Func Name:	OLED_OFF
  Description:  让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  Input:        无
  Output:       无
*************************************************/
void OLED_OFF(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //设置电荷泵
	OLED_WR_Byte(0X10,OLED_CMD);  //关闭电荷泵
	OLED_WR_Byte(0XAE,OLED_CMD);  //OLED休眠
}

//初始化OLED显示屏的驱动IC：SSD1306				    
void OLED_Config(void)
{	
/*********************GPIO_Pin SET*************************/
	GPIO_InitTypeDef GPIO_InitStructure;;
	
	RCC_AHB1PeriphClockCmd(SCL_SDA_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		= SCL_PIN | SDA_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;//推挽输出类型
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;//没有上下拉
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
	
	OLED_Clear();				 //清除屏幕
}

//用通用引脚作高低电平输出充当VCC和GND
void OLED_Power_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置引脚PD15---GND、PD1---VCC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_15;//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//工作模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//具体的模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//电阻上下拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//设置默认状态：PD15---0、PD1---1
	GPIO_ResetBits(GPIOD, GPIO_Pin_15);
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
}

//显示屏显示测试函数
void OLED_Test(void)
{	
//	//显示汉字  参数：x轴起始坐标，第y行，第n个汉字，一个汉字占用16*16个像素点
//	OLED_ShowCHinese(0,0,0);

	OLED_Clear();
	OLED_ShowCHinese(32,2,58);						//开
	OLED_ShowCHinese(48,2,59);						//机
	OLED_ShowCHinese(64,2,60);						//中
	
//	//显示英文字符串  参数：x坐标，第y行，显示的数据，每个字的大小（可选8、16）
//	OLED_ShowString(0, 0, (u8*)"led1:", 16);

	OLED_ShowString(80, 2, (u8*)"...", 16);			//...
	DELAY_MS(1000);

//	//显示BMP图片  参数：起始点坐标(x,y)，x1为宽度（0～127），y1为长度（0～7）
//	OLED_DrawBMP(0,0,128,8,BMP1);
	
	OLED_Clear();
//	OLED_DrawBMP(0,0,128,8,BMP1);
	DELAY_MS(1000);
}

//oled太空人动画测试函数（使用前清屏）
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

//oled开锁图标函数
void OLED_Unlock(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,UNLOCK);
}

//oled关锁图标函数
void OLED_Lock(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,LOCK);
}

//oled关锁图标函数
void OLED_Face(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,FACE);
}

//oled关锁图标函数
void OLED_Finger(void)
{
	OLED_Clear();
	OLED_DrawBMP(32,0,96,8,FINGER);
}

//主菜单函数
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

//密码界面函数
void Password_Menu(uint8_t num) //num 0-5（6位）
{
	OLED_Clear();
	OLED_ShowString(16, 4, (u8*)"_ _ _ _ _ _", 16);
	for(n=0; n<num; n++)	OLED_ShowChar((n+1)*16, 4, '*', 16);
}
