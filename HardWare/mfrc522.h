#ifndef _MFRC522_H_
#define _MFRC522_H_

#include "stm32f4xx.h"
#include "systick.h"
#include "sys.h"

/*Ӳ����������
	PD14	SDA(NSS)	D0
	PD0	 	SCK			D2
	PE7		MOSI		D4
	PE9 	MISO		D6
	PE13	GND			D10
	PE15 	RST			D12
	PD9		VCC			D14
*/


//����MFRC522�����Ų���
#define MF522_NSS(x)        (x) ? GPIO_SetBits(GPIOD,GPIO_Pin_14) :GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define MF522_SCK(x)        (x) ? GPIO_SetBits(GPIOD,GPIO_Pin_0) :GPIO_ResetBits(GPIOD,GPIO_Pin_0)
#define MF522_MOSI(x)       (x) ? GPIO_SetBits(GPIOE,GPIO_Pin_7) :GPIO_ResetBits(GPIOE,GPIO_Pin_7)
#define MF522_RST(x)        (x) ? GPIO_SetBits(GPIOE,GPIO_Pin_15):GPIO_ResetBits(GPIOE,GPIO_Pin_15)
#define MF522_MISO_READ     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)  


/////////////////////////////////////////////////////////////////////
//MF522������
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //ȡ����ǰ����
#define PCD_AUTHENT           0x0E               //��֤��Կ
#define PCD_RECEIVE           0x08               //��������
#define PCD_TRANSMIT          0x04               //��������
#define PCD_TRANSCEIVE        0x0C               //���Ͳ���������
#define PCD_RESETPHASE        0x0F               //��λ
#define PCD_CALCCRC           0x03               //CRC����

/////////////////////////////////////////////////////////////////////
//Mifare_One��Ƭ������
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //Ѱ��������δ��������״̬
#define PICC_REQALL           0x52               //Ѱ��������ȫ����
#define PICC_ANTICOLL1        0x93               //����ײ
#define PICC_ANTICOLL2        0x95               //����ײ
#define PICC_AUTHENT1A        0x60               //��֤A��Կ
#define PICC_AUTHENT1B        0x61               //��֤B��Կ
#define PICC_READ             0x30               //����
#define PICC_WRITE            0xA0               //д��
#define PICC_DECREMENT        0xC0               //�ۿ�
#define PICC_INCREMENT        0xC1               //��ֵ
#define PICC_RESTORE          0xC2               //�������ݵ�������
#define PICC_TRANSFER         0xB0               //���滺����������
#define PICC_HALT             0x50               //����

/////////////////////////////////////////////////////////////////////
//MF522 FIFO���ȶ���
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522�Ĵ�������
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     RFU1C             		0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegH         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		  0x3F

/////////////////////////////////////////////////////////////////////
//��MF522ͨѶʱ���صĴ������
/////////////////////////////////////////////////////////////////////
#define MI_OK                     0
#define MI_NOTAGERR               1
#define MI_ERR                    2
//
#define MAX_LEN					  18


/////////////////////////////////////////////////////////////////////
//����ԭ��
/////////////////////////////////////////////////////////////////////
//MFRC522ģ��ģ���Դ
void MFRC522_Power_Init(void);

//MFRC522ģ��SPI���߳�ʼ��
void MFRC522_SPI_Init(void);

//������������MFRC522��ĳһ�Ĵ���дһ���ֽ�����
//���������addr--�Ĵ�����ַ��val--Ҫд���ֵ
void MFRC522_Write2Register(u8 addr, u8 val) ;

//������������MFRC522��ĳһ�Ĵ�����һ���ֽ�����
//���������addr--�Ĵ�����ַ
//�� �� ֵ�����ض�ȡ����һ���ֽ����� 
u8 MFRC522_ReadFromRegister(u8 addr) ;

//������������ֻ���ܶ�дλ��Ч
//������������RC522�Ĵ���λ
//���������addr--�Ĵ�����ַ;mask--��λֵ
void SetBitMask(u8 addr, u8 mask)   ;

//������������RC522�Ĵ���λ
//���������addr--�Ĵ�����ַ;mask--��λֵ
void ClearBitMask(u8 addr, u8 mask)   ;

//������������λMFRC522
void MFRC522_Reset(void) ;


//��������,ÿ��������ر����߷���֮��Ӧ������1ms�ļ��
void MFRC522_AntennaOn(void) ;

//�ر�����,ÿ��������ر����߷���֮��Ӧ������1ms�ļ��
void MFRC522_AntennaOff(void) ;

//MFRC522ģ���ʼ��
void MFRC522_Initializtion(void) ;

//����������RC522��ISO14443��ͨѶ
//���������command--MF522������
//					sendData--ͨ��RC522���͵���Ƭ������
//					sendLen--���͵����ݳ���
//					BackData--���յ��Ŀ�Ƭ��������
//					BackLen--�������ݵ�λ����
//�� �� ֵ���ɹ�����MI_O
u8 MFRC522_ToCard(u8 command, u8 *sendData, u8 sendLen, u8 *backData, u16 *backLen) ;


//����������Ѱ������ȡ�����ͺ�
//���������reqMode--Ѱ����ʽ
//					TagType--���ؿ�Ƭ����
//					0x4400 = Mifare_UltraLight
//					0x0400 = Mifare_One(S50)
//					0x0200 = Mifare_One(S70)
//					0x0800 = Mifare_Pro(X)
//					0x4403 = Mifare_DESFire
//�� �� ֵ���ɹ�����MI_OK	
u8 MFRC522_Request(u8 reqMode, u8 *TagType);


//��������������ͻ��⪡��ȡѡ�п�Ƭ�Ŀ����к�
//���������serNum--����4�ֽڿ����к�,��5�ֽ�ΪУ���ֽ�
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Anticoll(u8 *serNum) ;

//������������MF522����CRC
//���������pIndata--Ҫ����CRC�����ݪ�len--���ݳ��Ȫ�pOutData--�����CRC���
void CalulateCRC(u8 *pIndata, u8 len, u8 *pOutData) ;

//����������ѡ������ȡ���洢������
//���������serNum--���뿨���к�
//�� �� ֵ���ɹ����ؿ�����
u8 MFRC522_SelectTag(u8 *serNum) ;


//������������֤��Ƭ����
//���������authMode--������֤ģʽ
//					0x60 = ��֤A��Կ
//					0x61 = ��֤B��Կ
//					BlockAddr--���ַ
//					Sectorkey--��������
//					serNum--��Ƭ���кŪ�4�ֽ�
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Auth(u8 authMode, u8 BlockAddr, u8 *Sectorkey, u8 *serNum) ;

//������������������
//���������blockAddr--���ַ;recvData--�����Ŀ�����
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Read(u8 blockAddr, u8 *recvData) ;


//����������д������
//���������blockAddr--���ַ;writeData--���д16�ֽ�����
//�� �� ֵ���ɹ�����MI_OK
u8 MFRC522_Write(u8 blockAddr, u8 *writeData) ;

//�������������Ƭ��������״̬
void MFRC522_Halt(void) ;

#endif
