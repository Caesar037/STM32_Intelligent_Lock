#include "dht11.h"

//定义全局变量
static DHT11_Data_TypeDef DHT11_Data;	//温湿度结构体

/*
 * 函数名：DHT11_GPIO_Config
 * 描述  ：配置DHT11的I/O端口
 * 输入  ：无
 * 输出  ：无
 */
void DHT11_Config(void)
{		
	//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启DHT11_PORT的外设时钟
	RCC_AHB1PeriphClockCmd(DHT11_CLK, ENABLE); 

	//选择要控制的DHT11_PORT引脚
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	//设置引脚速率为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	//调用库函数，初始化DHT11_PORT
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure); 

	//主机拉高
	DHT11_DATA_OUT(DHT11_HIGH);
}

/*
 * 函数名：DHT11_Mode_IPU
 * 描述  ：使DHT11-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//选择要控制的DHT11_PORT引脚
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	//设置引脚模式为浮空输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	//设置引脚速率为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	//调用库函数，初始化DHT11_PORT
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

/*
 * 函数名：DHT11_Mode_Out_PP
 * 描述  ：使DHT11-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//选择要控制的DHT11_PORT引脚
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	//设置引脚速率为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	//调用库函数，初始化DHT11_PORT
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * 从DHT11读取一个字节，MSB先行
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0; i<8; i++)    
	{	 
		//每bit以14us低电平标置开始，轮询直到从机发出的14us低电平结束
		while( DHT11_DATA_IN() == Bit_RESET );


		//DHT11以26~28us的高电平表示“0”，以70us高电平表示“1”，通过检测 x us后的电平即可区别这两个状，x 即下面的延时
		DHT11_DELAY_US(40);	//延时 x us这个延时需要大于数据0持续的时间即可

		if(DHT11_DATA_IN() == Bit_SET)	//x us后仍为高电平表示数据“1”
		{
			//等待数据1的高电平结束
			while(DHT11_DATA_IN() == Bit_SET);

			temp |= (uint8_t)(0x01<<(7-i));		//把第7-i位置1，MSB先行 
		}
//		else	//x us后为低电平表示数据“0”(可省略)
//		{			   
//			temp &= (uint8_t)~(0x01<<(7-i));	//把第7-i位置0，MSB先行
//		}
	}
	return temp;
}

/*
 * 一次完整的数据传输为40bit，高位先出
 * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 * 输入  ：DHT11_Data_TypeDef *DHT11_Data
 * 输出  ：	成功 0
 *			失败 1
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	uint16_t count;
	
	//输出模式
	DHT11_Mode_Out_PP();
	//主机拉低
	DHT11_DATA_OUT(DHT11_LOW);
	//延时18ms以上
	DHT11_DELAY_MS(25);

	//总线拉高 主机延时30us
	DHT11_DATA_OUT(DHT11_HIGH); 

	DHT11_DELAY_US(35);	//延时35us

	//主机设为输入 判断从机响应信号
	DHT11_Mode_IPU();

	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		count=0;
		//轮询直到从机发出 的80us 低电平 响应信号结束
		while(DHT11_DATA_IN()==Bit_RESET)
		{
			count++;
			if(count>1000)  return 1;
			DHT11_DELAY_US(10);
		}

		count=0;
		//轮询直到从机发出的 80us 高电平 标置信号结束
		while(DHT11_DATA_IN()==Bit_SET)
		{
			count++;
			if(count>1000)  return 1;
			DHT11_DELAY_US(10);
		}
		//开始接收数据
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();

		//读取结束，引脚改为输出模式
		DHT11_Mode_Out_PP();
		//主机拉高
		DHT11_DATA_OUT(DHT11_HIGH);

		//检查读取的数据是否正确
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 0;

	}
	return 1;

}

static uint8_t format[128]={0};	//格式化的字符串数组

void print_dht11_data(void)
{
	//调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息
	if( Read_DHT11( &DHT11_Data ) == SUCCESS)
	{
		
		//格式化字符串，存入字符串数组中
		snprintf((char *)format, sizeof(format), "湿度：%d.%d ％RH	温度：%d.%d℃\r\n",	\
        DHT11_Data.humi_int,	\
        DHT11_Data.humi_deci,	\
        DHT11_Data.temp_int,	\
        DHT11_Data.temp_deci);
		
		//默认串口1输出
		printf("Read DHT11: %s\r\n",format);
		//串口2（蓝牙）输出
		//Usart_SendString(USARTx_Bluetooth, (char *)format);
	}		
	else
	{
		
		//格式化字符串，存入字符串数组中
		snprintf((char *)format, sizeof(format), "ERROR!\r\n");
		
		//默认串口1输出
		printf("Read DHT11: %s\r\n",format);
		//串口2（蓝牙）输出
		//Usart_SendString(USARTx_Bluetooth, (char *)format);
	}
	//注意： DHT11一次通讯时间最大3ms，主机连续采样间隔建议不小于100ms
	DHT11_DELAY_MS(100);
}
