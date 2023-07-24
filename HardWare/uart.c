#include "uart.h"

//����ṹ�����
static	GPIO_InitTypeDef GPIO_InitStruct;	
static	USART_InitTypeDef USART_InitStruct;
static	NVIC_InitTypeDef NVIC_InitStruct;

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  ����ָ�����ȵ��ֽ� **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx, *p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* ����һ���ֽ����ݵ�USART */
		Usart_SendByte(pUSARTx, array[i]);	
	}
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k = 0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	USART_ClearFlag(pUSARTx, USART_FLAG_TC);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USARTx_Print, (uint8_t) ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(USARTx_Print, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USARTx_Print, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USARTx_Print);
}

//����1 ����Ϊ��PA9(TXD,��������)  PA10(RXD,��������)
void UART1_Config(uint32_t BaudRate)
{	
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//��������1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//�������ŵĸ��ù��ܣ���GPIO�봮��ͨ�Ž���������ϵ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//����ģʽ
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//�������ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//ʹ�����º��������ݽṹ������ĳ�Ա��Ϣ������GPIO�˿�������
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//�Խṹ�������Ա��ֵ
	USART_InitStruct.USART_BaudRate = BaudRate;//�����ʣ������ݴ����ٶ�
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//�ֳ������������ݵ�����λ���ȣ��������У��λ���ֳ��趨Ϊ9bit
	USART_InitStruct.USART_Parity = USART_Parity_No;	//У��λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//ֹͣλ
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//����Ӳ������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	//���ýṹ�������Ա��Ϣ�Դ���1������(��ʼ��)
	USART_Init(USART1, &USART_InitStruct);
	
	//NVIC�ṹ������ĳ�Ա��ֵ
	NVIC_InitStruct.NVIC_IRQChannel	= USART1_IRQn;	//�����ж�ԴΪUSART1
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =	1;	//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	//�жϵȼ��е���Ӧ���ȼ�������
	//Ƕ�������жϿ����������ã������ж�Դ�������жϵȼ���
	NVIC_Init(&NVIC_InitStruct);
	
	//�������ڵĽ����жϣ���Ϊ��Ҫ��ʱ�����ⲿ���������͸���оƬ�����ݣ����Կ������ǽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);	//��������
}

//����2 ����Ϊ��PA2(TXD,��������)  PA3(RXD,��������)
void UART2_Config(uint32_t BaudRate)
{
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//��������1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//�������ŵĸ��ù��ܣ���GPIO�봮��ͨ�Ž���������ϵ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//����ģʽ
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//�������ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	USART_InitStruct.USART_BaudRate = BaudRate;	//�����ʣ������ݴ����ٶ�
	USART_InitStruct.USART_WordLength	= USART_WordLength_8b;	//8λ�ֳ�
	USART_InitStruct.USART_StopBits	= USART_StopBits_1;	//1λֹͣλ
	USART_InitStruct.USART_Parity	= USART_Parity_No;	//�޼���
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;	//�жϵȼ��е���Ӧ���ȼ�������
	NVIC_Init(&NVIC_InitStruct);	//����NVIC
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
	
	USART_Cmd(USART2, ENABLE);	//��������
}

//����3 ����Ϊ��PB10(TXD,��������)  PB11(RXD,��������)
void UART3_Config(uint32_t BaudRate)
{
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//��������1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//�������ŵĸ��ù��ܣ���GPIO�봮��ͨ�Ž���������ϵ��
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	//����ģʽ
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	//�������ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	USART_InitStruct.USART_BaudRate = BaudRate;	//�����ʣ������ݴ����ٶ�
	USART_InitStruct.USART_WordLength	= USART_WordLength_8b;	//8λ�ֳ�
	USART_InitStruct.USART_StopBits	= USART_StopBits_1;	//1λֹͣλ
	USART_InitStruct.USART_Parity	= USART_Parity_No;	//�޼���
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//�жϵȼ��е���ռ���ȼ�������(���������ú��жϷ���Ϊ��2��������ռ�ȼ���Ϊ0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;	//�жϵȼ��е���Ӧ���ȼ�������
	NVIC_Init(&NVIC_InitStruct);	//����NVIC
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
	
	USART_Cmd(USART3, ENABLE);	//��������
}

volatile uint8_t Change_Password = 0;
volatile uint8_t Change_Rfid = 0;
volatile uint8_t Add_Finger = 0;
volatile uint8_t Clean_Finger = 0;
volatile uint8_t Add_Face = 0;
volatile uint8_t Clean_Face = 0;
volatile uint8_t Lock_Flag = 0;

//TXE	---	�������ݼĴ���
//RXNE	---	��ȡ���ݼĴ���
//TC	---	������ɱ�־λ
volatile uint32_t 	Rx1Counter	= 0;
volatile uint32_t 	Rx1TMP		= 0;
volatile uint32_t 	Rx1Data		= 0;
volatile uint32_t	Rx1End 		= 0;
volatile uint8_t 	Rx1Buffer[64] = {0};
volatile uint8_t 	Tx1Buffer[64] = {0};

//����1���жϷ�����
void USART1_IRQHandler(void)
{
	//����һ���жϣ�CPUִ��һ���жϷ�����������8��λ���ݣ��������100���ֽڣ�������жϷ�����100��
	if( SET == USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		//����ص����飺��������
		Rx1Data = USART_ReceiveData(USART1);	//��ȡ1�ֽ����ݣ�8bit == 1byte��
		
		if(Rx1Counter < sizeof(Rx1Buffer))
		{
			Rx1Buffer[Rx1Counter++] = Rx1Data;
		}
//		else	//Խ���⣬�����ַ��������������ڹ̶�����ָ���ѡ����δ����TIM7���ʱʹ�ã�
//		{
//			Rx1Counter = 0;
//			Rx1End = 1;
//		}
		//��ձ�־λ��������Ӧ�µ��ж�����
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
	//�������ݡ�ת����������
	//USART_SendData(USARTx_WiFi, Rx1Data);

	//��оƬ���յ����������·������������ߣ����ԣ�
	//USART_SendData(USARTx_Print, Rx1Data);
	
	//�ж������Ƿ������ȫ�����ж�����ռ�������һ�������Ƿ�Ϊ������־λ����\n��,���������Զ��壻
	if( Rx1Buffer[Rx1Counter-1] == '\n')
	{
		//strstr���������� Rx1Buffer �е�һ�γ��� \n �ַ�����λ�ã����δ�ҵ��򷵻� null��
		if( strstr((char *)Rx1Buffer, "A\n") != NULL )		//���յ�����ʵ����Ϊ��\n��
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
		
		//���ô�����ݵ�λ��
		Rx1Counter = 0;
		//���������ʷ����
		memset((char *)Rx1Buffer, 0, sizeof(Rx1Buffer));
	}
	

}

//����2��2��ʱ������������¼���־���շ�����
volatile uint32_t 	Rx2Counter	= 0;
volatile uint32_t 	Rx2TMP		= 0;
volatile uint32_t 	Rx2Data		= 0;
volatile uint32_t	Rx2End 		= 0;
volatile uint8_t 	Rx2Buffer[64] = {0};
volatile uint8_t 	Tx2Buffer[64] = {0};


//����2���жϷ�����
void USART2_IRQHandler(void)
{
	//����һ���жϣ�CPUִ��һ���жϷ�����������8��λ���ݣ��������100���ֽڣ�������жϷ�����100��
	if( SET == USART_GetITStatus(USART2, USART_IT_RXNE) )
	{
		//����ص����飺��������
		Rx2Data = USART_ReceiveData(USART2);	//��ȡ1�ֽ����ݣ�8bit == 1byte��
		
		if(Rx2Counter < sizeof(Rx2Buffer))
		{
			Rx2Buffer[Rx2Counter++] = Rx2Data;
		}
//		else	//Խ���⣬�����ַ��������������ڹ̶�����ָ���ѡ����δ����TIM7���ʱʹ�ã�
//		{
//			Rx2Counter = 0;
//			Rx2End = 1;
//		}
		
		//��ձ�־λ��������Ӧ�µ��ж�����
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
	//�������ݡ����������Ƿ�Ϊ��Ҫ�����ݡ�A��
//	if(Rx2Data == 'A')
//	{
//		//��������
//		
//	}

	//�ѽ��յ������ݷ��͸���ӡ����
	//USART_SendData(USARTx_Print, Rx2Data);
}

volatile uint32_t 	Rx3Counter	= 0;
volatile uint32_t 	Rx3TMP		= 0;
volatile uint32_t 	Rx3Data		= 0;
volatile uint32_t	Rx3End 		= 0;
volatile uint8_t 	Rx3Buffer[64] = {0};
volatile uint8_t 	Tx3Buffer[64] = {0};

//����3���жϷ�����
void USART3_IRQHandler(void)
{
	if( SET == USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		//����ص����飺��������
		Rx3Data = USART_ReceiveData(USART3);	//��ȡ1�ֽ����ݣ�8bit == 1byte��
		
		if(Rx3Counter < sizeof(Rx3Buffer))
		{
			Rx3Buffer[Rx3Counter++] = Rx3Data;
		}
//		else	//Խ���⣬�����ַ��������������ڹ̶�����ָ���ѡ����δ����TIM7���ʱʹ�ã�
//		{
//			Rx3Counter = 0;
//			Rx3End = 1;
//		}
		
		//��ձ�־λ��������Ӧ�µ��ж�����
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	
	//�������ݡ���������
#if WIFI_AP
	//WiFi_APͨ��ģʽ
	//�ж������Ƿ������ȫ�����ж�����ռ�������һ�������Ƿ�Ϊ������־λ����\n��,���������Զ��壻
	if( Rx3Buffer[Rx3Counter-1] == '\n')
	{
		//strstr���������� Rx2Buffer �е�һ�γ��� LED1\r\n �ַ�����λ�ã����δ�ҵ��򷵻� null��
		if( strstr((char *)Rx3Buffer, "LED1\r\n") != NULL )		//���յ�����ʵ����Ϊ��LED1\r\n��
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13);	//��ת��״̬
		}
		if( strstr((char *)Rx3Buffer, "LED2\r\n") != NULL)
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_14);	//��ת��״̬
		}
		
		//���ô�����ݵ�λ��
		Rx3Counter = 0;
		//���������ʷ����
		memset((char *)Rx3Buffer, 0, sizeof(Rx3Buffer));
	}
#endif

#if WIFI_STA
	//WiFi_STAͨ��ģʽ

#endif

	//�ѽ��յ������ݷ��͸���ӡ����
	//USART_SendData(USARTx_Print, Rx3Data);
	
}
