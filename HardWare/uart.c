#include "uart.h"

//定义结构体变量
static	GPIO_InitTypeDef GPIO_InitStruct;	
static	USART_InitTypeDef USART_InitStruct;
static	NVIC_InitTypeDef NVIC_InitStruct;

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx, ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx, *p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* 发送一个字节数据到USART */
		Usart_SendByte(pUSARTx, array[i]);	
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k = 0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	USART_ClearFlag(pUSARTx, USART_FLAG_TC);
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USARTx_Print, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USARTx_Print, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USARTx_Print, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USARTx_Print);
}

//串口1 引脚为：PA9(TXD,发送引脚)  PA10(RXD,接收引脚)
void UART1_Config(uint32_t BaudRate)
{	
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//开启串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//设置引脚的复用功能，即GPIO与串口通信建立关联关系；
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//对结构体变量成员作赋值操作，以便使用以下函数根据 结构体变量的成员 对引脚的工作模式进行配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//复用模式
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽输出模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	//使用以下函数，根据结构体变量的成员信息，配置GPIO端口组引脚
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//对结构体变量成员赋值
	USART_InitStruct.USART_BaudRate = BaudRate;//波特率，即数据传输速度
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//字长，即发送数据的数据位长度，如果开启校验位则字长设定为9bit
	USART_InitStruct.USART_Parity = USART_Parity_No;	//校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//停止位
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无需硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	//利用结构体变量成员信息对串口1作配置(初始化)
	USART_Init(USART1, &USART_InitStruct);
	
	//NVIC结构体变量的成员赋值
	NVIC_InitStruct.NVIC_IRQChannel	= USART1_IRQn;	//设置中断源为USART1
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	1;	//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	//中断等级中的响应优先级的设置
	//嵌套向量中断控制器的配置（设置中断源、设置中断等级）
	NVIC_Init(&NVIC_InitStruct);
	
	//开启串口的接收中断：因为需要及时处理外部传感器发送给到芯片的数据，所以开启的是接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);	//开启串口
}

//串口2 引脚为：PA2(TXD,发送引脚)  PA3(RXD,接收引脚)
void UART2_Config(uint32_t BaudRate)
{
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//开启串口1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//设置引脚的复用功能，即GPIO与串口通信建立关联关系；
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//复用模式
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽输出模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	USART_InitStruct.USART_BaudRate = BaudRate;	//波特率，即数据传输速度
	USART_InitStruct.USART_WordLength	= USART_WordLength_8b;	//8位字长
	USART_InitStruct.USART_StopBits	= USART_StopBits_1;	//1位停止位
	USART_InitStruct.USART_Parity	= USART_Parity_No;	//无检验
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;	//中断等级中的响应优先级的设置
	NVIC_Init(&NVIC_InitStruct);	//配置NVIC
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//开启串口接收中断
	
	USART_Cmd(USART2, ENABLE);	//开启串口
}

//串口3 引脚为：PB10(TXD,发送引脚)  PB11(RXD,接收引脚)
void UART3_Config(uint32_t BaudRate)
{
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//开启串口1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//设置引脚的复用功能，即GPIO与串口通信建立关联关系；
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//引脚号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//复用模式
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽输出模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉电阻
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//高速率
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	USART_InitStruct.USART_BaudRate = BaudRate;	//波特率，即数据传输速度
	USART_InitStruct.USART_WordLength	= USART_WordLength_8b;	//8位字长
	USART_InitStruct.USART_StopBits	= USART_StopBits_1;	//1位停止位
	USART_InitStruct.USART_Parity	= USART_Parity_No;	//无检验
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//中断等级中的抢占优先级的设置(由于已设置好中断分组为组2，所以抢占等级可为0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;	//中断等级中的响应优先级的设置
	NVIC_Init(&NVIC_InitStruct);	//配置NVIC
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开启串口接收中断
	
	USART_Cmd(USART3, ENABLE);	//开启串口
}

volatile uint8_t Change_Password = 0;
volatile uint8_t Change_Rfid = 0;
volatile uint8_t Add_Finger = 0;
volatile uint8_t Clean_Finger = 0;
volatile uint8_t Add_Face = 0;
volatile uint8_t Clean_Face = 0;
volatile uint8_t Lock_Flag = 0;

//TXE	---	发送数据寄存器
//RXNE	---	读取数据寄存器
//TC	---	发送完成标志位
volatile uint32_t 	Rx1Counter	= 0;
volatile uint32_t 	Rx1TMP		= 0;
volatile uint32_t 	Rx1Data		= 0;
volatile uint32_t	Rx1End 		= 0;
volatile uint8_t 	Rx1Buffer[64] = {0};
volatile uint8_t 	Tx1Buffer[64] = {0};

//串口1的中断服务函数
void USART1_IRQHandler(void)
{
	//产生一次中断，CPU执行一次中断服务函数，接收8个位数据；如果发送100个字节，则进入中断服务函数100次
	if( SET == USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		//做相关的事情：接收数据
		Rx1Data = USART_ReceiveData(USART1);	//读取1字节数据（8bit == 1byte）
		
		if(Rx1Counter < sizeof(Rx1Buffer))
		{
			Rx1Buffer[Rx1Counter++] = Rx1Data;
		}
//		else	//越界检测，接收字符数组已满，用于固定长度指令（可选，在未开启TIM7检测时使用）
//		{
//			Rx1Counter = 0;
//			Rx1End = 1;
//		}
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
	//分析数据、转发处理数据
	//USART_SendData(USARTx_WiFi, Rx1Data);

	//把芯片接收到的数据重新发出给到发送者（回显）
	//USART_SendData(USARTx_Print, Rx1Data);
	
	//判断数据是否接收完全，即判断数组空间里的最后一个数据是否为结束标志位：‘\n’,结束符可自定义；
	if( Rx1Buffer[Rx1Counter-1] == '\n')
	{
		//strstr函数返回在 Rx1Buffer 中第一次出现 \n 字符串的位置，如果未找到则返回 null。
		if( strstr((char *)Rx1Buffer, "A\n") != NULL )		//接收到的真实数据为“\n”
		{
			Change_Password = 1;
		}
		if( strstr((char *)Rx1Buffer, "B\n") != NULL)
		{
			Change_Rfid = 1;
		}
		if( strstr((char *)Rx1Buffer, "C1\n") != NULL)
		{
			Add_Finger = 1;
		}
		if( strstr((char *)Rx1Buffer, "C2\n") != NULL)
		{
			Clean_Finger = 1;
		}
		if( strstr((char *)Rx1Buffer, "D1\n") != NULL)
		{
			Add_Face = 1;
		}
		if( strstr((char *)Rx1Buffer, "D2\n") != NULL)
		{
			Clean_Face = 1;
		}
		if( strstr((char *)Rx1Buffer, "UNLOCK\n") != NULL)
		{
			 Lock_Flag = 1;
		}
		
		//重置存放数据的位置
		Rx1Counter = 0;
		//清空数组历史数据
		memset((char *)Rx1Buffer, 0, sizeof(Rx1Buffer));
	}
	

}

//串口2：2个时间计数，结束事件标志，收发数组
volatile uint32_t 	Rx2Counter	= 0;
volatile uint32_t 	Rx2TMP		= 0;
volatile uint32_t 	Rx2Data		= 0;
volatile uint32_t	Rx2End 		= 0;
volatile uint8_t 	Rx2Buffer[64] = {0};
volatile uint8_t 	Tx2Buffer[64] = {0};


//串口2的中断服务函数
void USART2_IRQHandler(void)
{
	//产生一次中断，CPU执行一次中断服务函数，接收8个位数据；如果发送100个字节，则进入中断服务函数100次
	if( SET == USART_GetITStatus(USART2, USART_IT_RXNE) )
	{
		//做相关的事情：接收数据
		Rx2Data = USART_ReceiveData(USART2);	//读取1字节数据（8bit == 1byte）
		
		if(Rx2Counter < sizeof(Rx2Buffer))
		{
			Rx2Buffer[Rx2Counter++] = Rx2Data;
		}
//		else	//越界检测，接收字符数组已满，用于固定长度指令（可选，在未开启TIM7检测时使用）
//		{
//			Rx2Counter = 0;
//			Rx2End = 1;
//		}
		
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
	//分析数据、处理数据是否为想要的数据“A”
//	if(Rx2Data == 'A')
//	{
//		//处理数据
//		
//	}

	//把接收到的数据发送给打印串口
	//USART_SendData(USARTx_Print, Rx2Data);
}

volatile uint32_t 	Rx3Counter	= 0;
volatile uint32_t 	Rx3TMP		= 0;
volatile uint32_t 	Rx3Data		= 0;
volatile uint32_t	Rx3End 		= 0;
volatile uint8_t 	Rx3Buffer[64] = {0};
volatile uint8_t 	Tx3Buffer[64] = {0};

//串口3的中断服务函数
void USART3_IRQHandler(void)
{
	if( SET == USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		//做相关的事情：接收数据
		Rx3Data = USART_ReceiveData(USART3);	//读取1字节数据（8bit == 1byte）
		
		if(Rx3Counter < sizeof(Rx3Buffer))
		{
			Rx3Buffer[Rx3Counter++] = Rx3Data;
		}
//		else	//越界检测，接收字符数组已满，用于固定长度指令（可选，在未开启TIM7检测时使用）
//		{
//			Rx3Counter = 0;
//			Rx3End = 1;
//		}
		
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	
	//分析数据、处理数据
#if WIFI_AP
	//WiFi_AP通信模式
	//判断数据是否接收完全，即判断数组空间里的最后一个数据是否为结束标志位：‘\n’,结束符可自定义；
	if( Rx3Buffer[Rx3Counter-1] == '\n')
	{
		//strstr函数返回在 Rx2Buffer 中第一次出现 LED1\r\n 字符串的位置，如果未找到则返回 null。
		if( strstr((char *)Rx3Buffer, "LED1\r\n") != NULL )		//接收到的真实数据为“LED1\r\n”
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13);	//翻转灯状态
		}
		if( strstr((char *)Rx3Buffer, "LED2\r\n") != NULL)
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_14);	//翻转灯状态
		}
		
		//重置存放数据的位置
		Rx3Counter = 0;
		//清空数组历史数据
		memset((char *)Rx3Buffer, 0, sizeof(Rx3Buffer));
	}
#endif

#if WIFI_STA
	//WiFi_STA通信模式

#endif

	//把接收到的数据发送给打印串口
	//USART_SendData(USARTx_Print, Rx3Data);
	
}
