#include "dht11.h"

//����ȫ�ֱ���
static DHT11_Data_TypeDef DHT11_Data;	//��ʪ�Ƚṹ��

/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11��I/O�˿�
 * ����  ����
 * ���  ����
 */
void DHT11_Config(void)
{		
	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;

	//����DHT11_PORT������ʱ��
	RCC_AHB1PeriphClockCmd(DHT11_CLK, ENABLE); 

	//ѡ��Ҫ���Ƶ�DHT11_PORT����
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	//���ÿ⺯������ʼ��DHT11_PORT
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure); 

	//��������
	DHT11_DATA_OUT(DHT11_HIGH);
}

/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//ѡ��Ҫ���Ƶ�DHT11_PORT����
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	//��������ģʽΪ��������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	//���ÿ⺯������ʼ��DHT11_PORT
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//ѡ��Ҫ���Ƶ�DHT11_PORT����
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	//���ÿ⺯������ʼ��DHT11_PORT
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * ��DHT11��ȡһ���ֽڣ�MSB����
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0; i<8; i++)    
	{	 
		//ÿbit��14us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ�������14us�͵�ƽ����
		while( DHT11_DATA_IN() == Bit_RESET );


		//DHT11��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����ͨ����� x us��ĵ�ƽ��������������״��x ���������ʱ
		DHT11_DELAY_US(40);	//��ʱ x us�����ʱ��Ҫ��������0������ʱ�伴��

		if(DHT11_DATA_IN() == Bit_SET)	//x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1��
		{
			//�ȴ�����1�ĸߵ�ƽ����
			while(DHT11_DATA_IN() == Bit_SET);

			temp |= (uint8_t)(0x01<<(7-i));		//�ѵ�7-iλ��1��MSB���� 
		}
//		else	//x us��Ϊ�͵�ƽ��ʾ���ݡ�0��(��ʡ��)
//		{			   
//			temp &= (uint8_t)~(0x01<<(7-i));	//�ѵ�7-iλ��0��MSB����
//		}
	}
	return temp;
}

/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 * ����  ��DHT11_Data_TypeDef *DHT11_Data
 * ���  ��	�ɹ� 0
 *			ʧ�� 1
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	uint16_t count;
	
	//���ģʽ
	DHT11_Mode_Out_PP();
	//��������
	DHT11_DATA_OUT(DHT11_LOW);
	//��ʱ18ms����
	DHT11_DELAY_MS(25);

	//�������� ������ʱ30us
	DHT11_DATA_OUT(DHT11_HIGH); 

	DHT11_DELAY_US(35);	//��ʱ35us

	//������Ϊ���� �жϴӻ���Ӧ�ź�
	DHT11_Mode_IPU();

	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		count=0;
		//��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���
		while(DHT11_DATA_IN()==Bit_RESET)
		{
			count++;
			if(count>1000)  return 1;
			DHT11_DELAY_US(10);
		}

		count=0;
		//��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���
		while(DHT11_DATA_IN()==Bit_SET)
		{
			count++;
			if(count>1000)  return 1;
			DHT11_DELAY_US(10);
		}
		//��ʼ��������
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();

		//��ȡ���������Ÿ�Ϊ���ģʽ
		DHT11_Mode_Out_PP();
		//��������
		DHT11_DATA_OUT(DHT11_HIGH);

		//����ȡ�������Ƿ���ȷ
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 0;

	}
	return 1;

}

static uint8_t format[128]={0};	//��ʽ�����ַ�������

void print_dht11_data(void)
{
	//����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ
	if( Read_DHT11( &DHT11_Data ) == SUCCESS)
	{
		
		//��ʽ���ַ����������ַ���������
		snprintf((char *)format, sizeof(format), "ʪ�ȣ�%d.%d ��RH	�¶ȣ�%d.%d��\r\n",	\
        DHT11_Data.humi_int,	\
        DHT11_Data.humi_deci,	\
        DHT11_Data.temp_int,	\
        DHT11_Data.temp_deci);
		
		//Ĭ�ϴ���1���
		printf("Read DHT11: %s\r\n",format);
		//����2�����������
		//Usart_SendString(USARTx_Bluetooth, (char *)format);
	}		
	else
	{
		
		//��ʽ���ַ����������ַ���������
		snprintf((char *)format, sizeof(format), "ERROR!\r\n");
		
		//Ĭ�ϴ���1���
		printf("Read DHT11: %s\r\n",format);
		//����2�����������
		//Usart_SendString(USARTx_Bluetooth, (char *)format);
	}
	//ע�⣺ DHT11һ��ͨѶʱ�����3ms��������������������鲻С��100ms
	DHT11_DELAY_MS(100);
}
