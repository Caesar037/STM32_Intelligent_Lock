//////////////////////////////////////////////////////////////////////////////////	 
//  ��������  : MFRC522ģ����ʾ����(STM32F407ZET6ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//				PD14	SDA(NSS)	D0
//				PD0	 	SCK			D2
//				PE7		MOSI		D4
//				PE9 	MISO		D6
//				PE13	GND			D10
//				PE15 	RST			D12
//				PD9		VCC			D14
//              ----------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////
#include "mfrc522.h"


//��ͨ���������ߵ͵�ƽ����䵱VCC��GND
void MFRC522_Power_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��������PE13---GND��PD9---VCC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			//���ź�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		//����������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOD, GPIO_Pin_9);
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
}

//SPI3��ʼ��
void MFRC522_SPI_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	/*����Ƭѡ����Ϊ���ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_14; //NSS��SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_15; //MISO��RST
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	/*����Ƭѡ����Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	
	//ģʽ3��ʱ���������߿��е�ʱ���Ǹߵ�ƽ	
    GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_14); //���߿���
    GPIO_SetBits(GPIOE,GPIO_Pin_7|GPIO_Pin_15); //���߿���
}



//������������MFRC522��ĳһ�Ĵ���дһ���ֽ�����
//���������addr--�Ĵ�����ַ��val--Ҫд���ֵ
void MFRC522_Write2Register(u8 addr, u8 val) 
{
	unsigned char i, ucAddr;

    MF522_NSS(0);
    MF522_SCK(0);
    
    // ��ַ����һλ����ΪLSB��Ҫ���� ��RFUλ(Reserved for Future Use)
    // &0x7E �ǰ�bit1~bit6 �ĵ�ַ(address)д��
    // |0x80 ��Ϊ��ʹ���λΪ1   1(Read) 0(Write) ��ʹ�� '��'
    ucAddr = ((addr<<1)&0x7E); 
    
    for(i=8;i>0;i--)
    {
        if( (ucAddr&0x80)==0x80 )
        {
           MF522_MOSI(1);
        }
        else
           MF522_MOSI(0);
    
        //��SCK�ߵ�ƽ�����ڼ䷢������
        MF522_SCK(1);
        ucAddr <<= 1;
        MF522_SCK(0);
        Delay_us(10);        
    }
    
    //���ͼĴ�������
    for(i=8;i>0;i--)
    {
        if( (val&0x80)==0x80 )
        {
           MF522_MOSI(1);
        }
        else
           MF522_MOSI(0);
    
        //��SCK�ߵ�ƽ�����ڼ䷢������
        MF522_SCK(1);
        val <<= 1;
        MF522_SCK(0);
        Delay_us(10);  
    }
    
    MF522_NSS(1);
    MF522_SCK(1);
}

//������������MFRC522��ĳһ�Ĵ�����һ���ֽ�����
//���������addr--�Ĵ�����ַ
//�� �� ֵ�����ض�ȡ����һ���ֽ����� 
u8 MFRC522_ReadFromRegister(u8 addr) 
{
    unsigned char i, ucAddr;
    unsigned char read_data = 0;
    
    MF522_NSS(0);
    MF522_SCK(0);
    
    // ��ַ����һλ����ΪLSB��Ҫ���� ��RFUλ(Reserved for Future Use)
    // &0x7E �ǰ�bit1~bit6 �ĵ�ַ(address)д��
    // |0x80 ��Ϊ��ʹ���λΪ1   1(Read) 0(Write) ��ʹ�� '��'
    ucAddr = ((addr<<1)&0x7E)|0x80;
    
    for(i=8;i>0;i--)
    {
        if( (ucAddr&0x80)==0x80 )
        {
           MF522_MOSI(1);
        }
        else
           MF522_MOSI(0);
    
        //��SCK�ߵ�ƽ�����ڼ䷢������
        MF522_SCK(1);
        ucAddr <<= 1;
        MF522_SCK(0);
        Delay_us(10);   
    }
    
    //��ȡ�Ĵ�������
    for(i=8;i>0;i--)
    {
       
        //��SCK�ߵ�ƽ�����ڼ䷢������
        MF522_SCK(1);
        read_data <<=1;       
        read_data |= MF522_MISO_READ;    
        MF522_SCK(0);
        Delay_us(10);      
    }
    
    MF522_NSS(1);
    MF522_SCK(1);
    
    return read_data;
}


//������������ֻ���ܶ�дλ��Ч
//������������RC522�Ĵ���λ
//���������addr--�Ĵ�����ַ;mask--��λֵ
void SetBitMask(u8 addr, u8 mask)   
{     
	u8 tmp=0;
   
	tmp=MFRC522_ReadFromRegister(addr);     
	MFRC522_Write2Register(addr,tmp|mask);  // set bit mask 
}

//������������RC522�Ĵ���λ
//���������addr--�Ĵ�����ַ;mask--��λֵ
void ClearBitMask(u8 addr, u8 mask)   
{     
	u8 tmp=0;

    tmp=MFRC522_ReadFromRegister(addr);     
	MFRC522_Write2Register(addr,tmp&(~mask));  // set bit mask 
}

//������������λMFRC522
void MFRC522_Reset(void) 
{ 
	//�⸴λ���Բ���
	MF522_RST(1);
	Delay_us(10);
	MF522_RST(0);
	Delay_us(10);
	MF522_RST(1);
	Delay_us(10); 
	//�ڸ�λ   
	MFRC522_Write2Register(CommandReg, PCD_RESETPHASE); 
}

//��������,ÿ��������ر����߷���֮��Ӧ������1ms�ļ��
void MFRC522_AntennaOn(void) 
{  
	u8 temp;
	//   
	temp=MFRC522_ReadFromRegister(TxControlReg);  
	if ((temp&0x03)==0)  
	{   
		SetBitMask(TxControlReg,0x03);  
	}
}

//�ر�����,ÿ��������ر����߷���֮��Ӧ������1ms�ļ��
void MFRC522_AntennaOff(void) 
{  
	ClearBitMask(TxControlReg,0x03);
}

//MFRC522ģ���ʼ��
void MFRC522_Initializtion(void) 
{
    //��ģ��SPI���Ž��г�ʼ��
	MFRC522_SPI_Init();  
    
    //��λMFRC522
	MFRC522_Reset();    
    
	//Timer: TPrescaler*TreloadVal/6.78MHz = 0xD3E*0x32/6.78=25ms     
	MFRC522_Write2Register(TModeReg,0x8D);				        //TAuto=1Ϊ�Զ�����ģʽ����ͨ��Э��Ӱ�򡣵�4λΪԤ��Ƶֵ�ĸ�4λ
	MFRC522_Write2Register(TPrescalerReg,0x3E); 	            //Ԥ��Ƶֵ�ĵ�8λ     
	MFRC522_Write2Register(TReloadRegL,0x32);		            //�������ĵ�8λ                
	MFRC522_Write2Register(TReloadRegH,0x00);		            //�������ĸ�8λ       
	MFRC522_Write2Register(TxAutoReg,0x40); 			        //100%ASK     
	MFRC522_Write2Register(ModeReg,0x3D); 				        //CRC��ʼֵ0x6363
	MFRC522_Write2Register(CommandReg,0x00);			        //����MFRC522     
	MFRC522_AntennaOn();          							    //������ 
}


//����������RC522��ISO14443��ͨѶ
//���������command--MF522������
//					sendData--ͨ��RC522���͵���Ƭ������
//					sendLen--���͵����ݳ���
//					BackData--���յ��Ŀ�Ƭ��������
//					BackLen--�������ݵ�λ����
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_ToCard(u8 command, u8 *sendData, u8 sendLen, u8 *backData, u16 *backLen) 
{
	u8  status=MI_ERR;
	u8  irqEn=0x00;
	u8  waitIRq=0x00;
	u8  lastBits;
	u8  n;
	u16 i;
	//������Ԥ���жϲ���
	switch (command)     
	{         
		case PCD_AUTHENT:  		//��֤����   
			irqEn 	= 0x12;			//    
			waitIRq = 0x10;			//    
			break;
		case PCD_TRANSCEIVE: 	//����FIFO������      
			irqEn 	= 0x77;			//    
			waitIRq = 0x30;			//    
			break;      
		default:    
			break;     
	}

	MFRC522_Write2Register(ComIEnReg, irqEn|0x80);		//�����ж�����     
	ClearBitMask(ComIrqReg, 0x80);  				//��������ж�����λ               	
	SetBitMask(FIFOLevelReg, 0x80);  				//FlushBuffer=1, FIFO��ʼ��
	MFRC522_Write2Register(CommandReg, PCD_IDLE); 		//ʹMFRC522����   
    
	//��FIFO��д������     
	for (i=0; i<sendLen; i++)
    {
        MFRC522_Write2Register(FIFODataReg, sendData[i]);
    }
    
	//ִ������
	MFRC522_Write2Register(CommandReg, command);
	//���߷�������     
	if (command == PCD_TRANSCEIVE)					//����ǿ�Ƭͨ�����MFRC522��ʼ�����߷�������      
		SetBitMask(BitFramingReg, 0x80);  		//StartSend=1,transmission of data starts      
	//�ȴ������������     
	i = 10000; //i����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms     
	do      
	{        
		n = MFRC522_ReadFromRegister(ComIrqReg);
		//irq_regdata=n;	//test         
		i--;
		//wait_count=i;		//test		     
	}while ((i!=0) && !(n&0x01) && !(n&waitIRq));	//��������˳�n=0x64
	//ֹͣ����
	ClearBitMask(BitFramingReg, 0x80);   		//StartSend=0
	//�����25ms�ڶ�����
	if (i != 0)     
	{            
		if(!(MFRC522_ReadFromRegister(ErrorReg) & 0x1B)) //BufferOvfl Collerr CRCErr ProtecolErr         
		{            
			if (n & irqEn & 0x01)			//                  
				status = MI_NOTAGERR;		//
			//
			if (command == PCD_TRANSCEIVE)             
			{                 
				n = MFRC522_ReadFromRegister(FIFOLevelReg);		//n=0x02                
				lastBits = MFRC522_ReadFromRegister(ControlReg) & 0x07;	//lastBits=0               
				if (lastBits!=0)                         
					*backLen = (n-1)*8 + lastBits; 
				else
					*backLen = n*8;									//backLen=0x10=16
				//
				if (n == 0)                         
				 	n = 1;                        
				if (n > MAX_LEN)         
				 	n = MAX_LEN;
				//
				for (i=0; i<n; i++)                 
					backData[i] = MFRC522_ReadFromRegister(FIFODataReg); 
			}
			//
			status = MI_OK;		
		}
		else
			status = MI_ERR;
	}	
	//
	MFRC522_Write2Register(ControlReg,0x80);				//timer stops     
	MFRC522_Write2Register(CommandReg, PCD_IDLE);	//
	//
	return status;
}



//����������Ѱ������ȡ�����ͺ�
//���������reqMode--Ѱ����ʽ
//					TagType--���ؿ�Ƭ����
//					0x4400 = Mifare_UltraLight
//					0x0400 = Mifare_One(S50)
//					0x0200 = Mifare_One(S70)
//					0x0800 = Mifare_Pro(X)
//					0x4403 = Mifare_DESFire
//�� �� ֵ���ɹ�����MI_OK	
u8 MFRC522_Request(u8 reqMode, u8 *TagType)
{  
	u8  status;    
	u16 backBits;   //���յ�������λ��
    
    ClearBitMask(Status2Reg,0x08);
	MFRC522_Write2Register(BitFramingReg, 0x07);  //TxLastBists = BitFramingReg[2..0]   
	TagType[0] = reqMode;  
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits); 

	if ((status != MI_OK) || (backBits != 0x10))  
	{       
		status = MI_ERR;
	}
   
	return status; 

}

//��������������ͻ��⪡��ȡѡ�п�Ƭ�Ŀ����к�
//���������serNum--����4�ֽڿ����к�,��5�ֽ�ΪУ���ֽ�
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Anticoll(u8 *serNum) 
{     
	u8  status;     
	u8  i;     
	u8  serNumCheck=0;     
	u16 unLen;
	//           
	ClearBitMask(Status2Reg, 0x08);  			//TempSensclear     
	ClearBitMask(CollReg,0x80);   				//ValuesAfterColl  
	MFRC522_Write2Register(BitFramingReg, 0x00);  	//TxLastBists = BitFramingReg[2..0]
	serNum[0] = PICC_ANTICOLL1;     
	serNum[1] = 0x20;     
	status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
	//      
	if (status == MI_OK)
	{   
		//У�鿨���к�   
		for(i=0;i<4;i++)   
			serNumCheck^=serNum[i];
		//
		if(serNumCheck!=serNum[i])        
			status=MI_ERR;
	}
	SetBitMask(CollReg,0x80);  //ValuesAfterColl=1
	//      
	return status;
}

//������������MF522����CRC
//���������pIndata--Ҫ����CRC�����ݪ�len--���ݳ��Ȫ�pOutData--�����CRC���
void CalulateCRC(u8 *pIndata, u8 len, u8 *pOutData) 
{     
	u16 i;
	u8  n;
	//      
	ClearBitMask(DivIrqReg, 0x04);   			//CRCIrq = 0     
	SetBitMask(FIFOLevelReg, 0x80);   		//��FIFOָ��     
	MFRC522_Write2Register(CommandReg, PCD_IDLE);   
	//��FIFO��д������      
	for (i=0; i<len; i++)
		MFRC522_Write2Register(FIFODataReg, *(pIndata+i));
	//��ʼRCR����
	MFRC522_Write2Register(CommandReg, PCD_CALCCRC);
	//�ȴ�CRC�������     
	i = 1000;     
	do      
	{         
		n = MFRC522_ReadFromRegister(DivIrqReg);         
		i--;     
	}while ((i!=0) && !(n&0x04));   //CRCIrq = 1
	//��ȡCRC������     
	pOutData[0] = MFRC522_ReadFromRegister(CRCResultRegL);     
	pOutData[1] = MFRC522_ReadFromRegister(CRCResultRegH);
	MFRC522_Write2Register(CommandReg, PCD_IDLE);
}

//����������ѡ������ȡ���洢������
//���������serNum--���뿨���к�
//�� �� ֵ���ɹ����ؿ�����
u8 MFRC522_SelectTag(u8 *serNum) 
{     
	u8  i;     
	u8  status;     
	u8  size;     
	u16 recvBits;     
	u8  buffer[9];
	//     
	buffer[0] = PICC_ANTICOLL1;	//��ײ��1     
	buffer[1] = 0x70;
	buffer[6] = 0x00;						     
	for (i=0; i<4; i++)					
	{
		buffer[i+2] = *(serNum+i);	//buffer[2]-buffer[5]Ϊ�����к�
		buffer[6]  ^=	*(serNum+i);	//��У����
	}
	//
	CalulateCRC(buffer, 7, &buffer[7]);	//buffer[7]-buffer[8]ΪRCRУ����
	ClearBitMask(Status2Reg,0x08);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
	//
	if ((status == MI_OK) && (recvBits == 0x18))    
		size = buffer[0];     
	else    
		size = 0;
	//	     
	return size; 
}


//������������֤��Ƭ����
//���������authMode--������֤ģʽ
//					0x60 = ��֤A��Կ
//					0x61 = ��֤B��Կ
//					BlockAddr--���ַ
//					Sectorkey--��������
//					serNum--��Ƭ���кŪ�4�ֽ�
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Auth(u8 authMode, u8 BlockAddr, u8 *Sectorkey, u8 *serNum) 
{     
	u8  status;     
	u16 recvBits;     
	u8  i;  
	u8  buff[12];    
	//��֤ģʽ+���ַ+��������+�����к�     
	buff[0] = authMode;		//��֤ģʽ     
	buff[1] = BlockAddr;	//���ַ     
	for (i=0; i<6; i++)
		buff[i+2] = *(Sectorkey+i);	//��������
	//
	for (i=0; i<4; i++)
		buff[i+8] = *(serNum+i);		//�����к�
	//
	status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
	//      
	if ((status != MI_OK) || (!(MFRC522_ReadFromRegister(Status2Reg) & 0x08)))
		status = MI_ERR;
	//
	return status;
}

//������������������
//���������blockAddr--���ַ;recvData--�����Ŀ�����
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Read(u8 blockAddr, u8 *recvData) 
{     
	u8  status;     
	u16 unLen;
	//      
	recvData[0] = PICC_READ;     
	recvData[1] = blockAddr;     
	CalulateCRC(recvData,2, &recvData[2]);     
	status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
	//
	if ((status != MI_OK) || (unLen != 0x90))
		status = MI_ERR;
	//
	return status;
}

//����������д������
//���������blockAddr--���ַ;writeData--���д16�ֽ�����
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Write(u8 blockAddr, u8 *writeData) 
{     
	u8  status;     
	u16 recvBits;     
	u8  i;  
	u8  buff[18];
	//           
	buff[0] = PICC_WRITE;
	buff[1] = blockAddr;
	CalulateCRC(buff, 2, &buff[2]);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
	//
	if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
		status = MI_ERR;
	//
	if (status == MI_OK)     
	{         
		for (i=0; i<16; i++)  //��FIFOд16Byte����                     
			buff[i] = *(writeData+i);
		//                     
		CalulateCRC(buff, 16, &buff[16]);         
		status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);           
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))               
			status = MI_ERR;         
	}          
	return status;
}

//�������������Ƭ��������״̬
void MFRC522_Halt(void) 
{    
	u16 unLen;     
	u8  buff[4];
	//       
	buff[0] = PICC_HALT;     
	buff[1] = 0;     
	CalulateCRC(buff, 2, &buff[2]);       
	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}
