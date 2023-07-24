#include "at24c02.h"

//AT24C02һ����32ҳ��ÿҳһ����8���ֽڣ�����AT24C02���ڴ�ռ�һ����256�ֽڣ���256�ֽ�һ����2048bit��Ҳ����AT24C02��2Kָ����bit����Ŀ

/*
 * ��  �ܣ�AT24C02��ʼ��  
 * ��  ������
 * ����ֵ����
 */
void AT24C02_Init(void)
{
	IIC_Sim_Init();
}

/*
 * ��  �ܣ�AT24C02ʵ��ҳд 
 * ��  ����
 *		addr	---> ������д�� AT24C02 �� addr �����ַ��
 * 		pBuf	---> ����Ҫд������ݵ��׵�ַ
 * 		length	---> ��Ҫд����ֽ���
 * ����ֵ��
 *		ʧ��	---> 1
 * 		�ɹ�	---> �ɹ�д����ֽ���
 */
uint8_t AT24C02_WriteData(uint8_t addr, uint8_t *pBuf, uint8_t length)
{
	uint8_t count = 0;			//ͳ��д����ֽ���
	uint8_t tempAddr = addr;	//��ʱ�洢��ַ
	uint8_t i;
	
	for(i=0; i<length; i++)
	{
		//����һ�ν��� ���� ����ҳ�߽��ַ�����º���ʼ(8�ֽ�1ҳ����32ҳ)
		if( (i == 0) || (tempAddr%8 == 0) )
		{
			//STM32����ֹͣ�ź� �ر���һ�ε���ʼ�ź�
			IIC_Sim_Stop();
			Delay_ms(10);
			
			//1��STM32������ʼ�ź�
			IIC_Sim_Start();
			
			//2��STM32����������ַѡ����Ӧ�豸����˵�������Ҫִ��д����
			IIC_Sim_SendByte(0xA0);
			if( IIC_Sim_WaitSlavAck() )  //���ʽ����˵�����յ���Ӧ���ź�
			{
				IIC_Sim_Stop();
				printf("����������ַʧ��(0xA0)...\r\n");
				return 1;
			}
			//3��STM32�����ֵ�ַ˵�������Ҫ������д������ֵ�ַ
			IIC_Sim_SendByte(tempAddr);
			if( IIC_Sim_WaitSlavAck() )  //���ʽ����˵�����յ���Ӧ���ź�
			{
				IIC_Sim_Stop();
				printf("�����ֵ�ַʧ��(%#hhx)...\r\n", addr);
				return 1;
			}
		}
		
		//4��STM32���� ���� �� AT24C02
		IIC_Sim_SendByte(pBuf[count]);
		if( IIC_Sim_WaitSlavAck() )  //���ʽ����˵�����յ���Ӧ���ź�
		{
			IIC_Sim_Stop();
			printf("��������ʧ��...\r\n");
			return 1;
		}else{
			count++;		//ͳ��д����ֽ���
			tempAddr++;   		//����ַ����ƫ��
		}
	}
	
	//5��STM32����ֹͣ�ź�
	IIC_Sim_Stop();
	//printf("�����������...\r\n");
	
	return count;
}


/*
 * ��  �ܣ�AT24C02ʵ�����+˳���
 *		addr	---> ���� AT24C02 ��ȡ���ݵĵĵ�ַ
 * 		pBuf	---> ��ȡ����������Ҫ��ŵ��ڴ������׵�ַ
 * 		length	---> Ҫ��ȡ���ֽ���
 * ����ֵ��
 *		ʧ��	---> 1
 * 		�ɹ�	---> �ɹ���ȡ���ֽ���
 */
uint8_t AT24C02_ReadData(uint8_t addr ,uint8_t *pBuf ,uint8_t length)
{
    uint8_t count = 0; //ͳ��д����ֽ���
    uint8_t *p =pBuf;
    
    //STM32������ʼ�ź�
    IIC_Sim_Start();
    
    //�豸Ѱַ������д����
    IIC_Sim_SendByte(0xA0);
    
    //�ȴ��Է�Ӧ��
    if(IIC_Sim_WaitSlavAck())   //�� ack = 1 ˵�����յ���Ӧ���ź�
    {
        IIC_Sim_Stop();
        printf("����������ַʧ��(0xA0)...\r\n");
        return  1;        
    }
    
    //���߶Է����ĸ���ַ��ȡ����
    IIC_Sim_SendByte(addr);
    
    //�ȴ��Է�Ӧ��
    if(IIC_Sim_WaitSlavAck())   //�� ack = 1 ˵�����յ���Ӧ���ź�
    {
        IIC_Sim_Stop();
        printf("�����ֵ�ַʧ��(%#hhx)...\r\n", addr);
        return  1;        
    }    
    
    //STM32�ٴη�����ʼ�ź�
    IIC_Sim_Start();
    
    //�豸Ѱַ�����ж�����
    IIC_Sim_SendByte(0xA1);
    
    //�ȴ��Է�Ӧ��
    if(IIC_Sim_WaitSlavAck())   //�� ack = 1 ˵�����յ���Ӧ���ź�
    {
        IIC_Sim_Stop();
        printf("����������ַʧ��(0xA1)...\r\n");
        return  1;        
    }    
    
    //��ʼ��ȡ���ݲ���
    length = length-1;   //Ԥ�����һ��
    
    while(length--)
    {
        //��ȡ�ӻ�������������
        *p++ = IIC_Sim_ReadByte();
        count++;
        //STM32��ӻ�������Ӧ���ź�
        IIC_Sim_MasterAck(0);
    
    }
    
    //��ȡ���һ������
    *p = IIC_Sim_ReadByte();
    count++;
    //STM32��ӻ�������Ӧ���źţ�Ҳ����˵���Ѿ���ȡ���ˣ��㲻���ٷ�������
    IIC_Sim_MasterAck(1);
    
    //ֹͣ�ź�
    IIC_Sim_Stop();
	//printf("�����������...\r\n");
    
    return count;
    
}

