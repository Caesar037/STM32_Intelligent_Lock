#include "spi.h"


//CS---PB14  
//SPI1_SCK---PB3
//SPI1_MISO---PB4
//SPI1_MOSI---PB5
//����stm32оƬ��SPI�ӿ�Ϊ����ģʽ���ҹ���ģʽΪmod3 ��������Ϊ1�� ��λΪ1���ڶ������ؼ�⣩
void SPI_Sim_Init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	//������Ӧ�Ķ˿���ʱ��
	RCC_AHB1PeriphClockCmd(SPI_CS_GPIO_CLK | SPI_SCK_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_MISO_GPIO_CLK, ENABLE);	////42MHZ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//84MHZ
	
	//��SPI�������ӵ����ö˿�
	GPIO_PinAFConfig(SPI_SCK_GPIO_PORT, SPI_SCK_SOURCE, SPI_AF);
	GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_SOURCE, SPI_AF);
	GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, SPI_MISO_SOURCE, SPI_AF);

	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = SPI_CS_PIN;			//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//SPI_CS�������ã��ӻ���
	GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStruct);
	
	//SPI_SCK��������
	GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN;
	GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
	
	//SPI_MOSI��������
	GPIO_InitStruct.GPIO_Pin =  SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
	
	//SPI_MISO��������
	GPIO_InitStruct.GPIO_Pin =  SPI_MISO_PIN;
	GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

#if Master_Or_Slave
	//�Խṹ�������Ա����ֵ�������Ա�ʹ�����º������� �ṹ������ĳ�Ա �����ŵĹ���ģʽ��������
	GPIO_InitStruct.GPIO_Pin = SPI_CS_PIN;			//���ź�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		//����ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;		//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//������
	//SPI_CS�������ã�������
	GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStruct);
#endif	

	//Ƭѡ����ֱ�����ߣ���ʾĬ��״̬Ϊ��ѡ��ģ��
	SPI_CS_SET(1);
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//ȫ˫��ͨ��
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		//����ģʽ
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;	//���ݴ�С8λ
	//���¼��Ժ���λ������SPI�ӿڵĹ���ģʽ����ģʽ��������ͨ�Ŵ�������������������Ϊģʽ0��
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;			//����Ϊ�͵�ƽ����SCK���ſ���״̬��Ϊ�͵�ƽ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;		//��һ�����ؼ�⣬CPHA=0;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			//�������CS
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//��Ƶ16��
	
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;	//���ݸ�ʽ��λ�ȳ�
	SPI_InitStruct.SPI_CRCPolynomial = 7;	//���λ����
	SPI_Init(SPIX, &SPI_InitStruct);
	
	//ʹ��sFLASH_SPI
	SPI_Cmd(SPIX, ENABLE);
}

//SPI �����뷢�ͺ������շ�һ�壩
uint8_t SPI_SendByte(uint8_t byte)
{
  //DR�Ĵ�����Ϊ��ʱѭ��
  while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET);

  //ͨ��SPIX��Χ�豸�����ֽ�
  SPI_I2S_SendData(SPIX, byte);

  //�ȴ���ȡһ���ֽ�
  while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_RXNE) == RESET);

  //���ش�SPI���߶�ȡ���ֽ�
  return SPI_I2S_ReceiveData(SPIX);
}

