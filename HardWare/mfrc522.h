#ifndef _MFRC522_H_
#define _MFRC522_H_

#include "stm32f4xx.h"
#include "systick.h"
#include "sys.h"

/*硬件连接连接
	PD14	SDA(NSS)	D0
	PD0	 	SCK			D2
	PE7		MOSI		D4
	PE9 	MISO		D6
	PE13	GND			D10
	PE15 	RST			D12
	PD9		VCC			D14
*/


//定义MFRC522的引脚操作
#define MF522_NSS(x)        (x) ? GPIO_SetBits(GPIOD,GPIO_Pin_14) :GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define MF522_SCK(x)        (x) ? GPIO_SetBits(GPIOD,GPIO_Pin_0) :GPIO_ResetBits(GPIOD,GPIO_Pin_0)
#define MF522_MOSI(x)       (x) ? GPIO_SetBits(GPIOE,GPIO_Pin_7) :GPIO_ResetBits(GPIOE,GPIO_Pin_7)
#define MF522_RST(x)        (x) ? GPIO_SetBits(GPIOE,GPIO_Pin_15):GPIO_ResetBits(GPIOE,GPIO_Pin_15)
#define MF522_MISO_READ     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)  


/////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
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
//和MF522通讯时返回的错误代码
/////////////////////////////////////////////////////////////////////
#define MI_OK                     0
#define MI_NOTAGERR               1
#define MI_ERR                    2
//
#define MAX_LEN					  18


/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
//MFRC522模块模拟电源
void MFRC522_Power_Init(void);

//MFRC522模块SPI总线初始化
void MFRC522_SPI_Init(void);

//功能描述向MFRC522的某一寄存器写一个字节数据
//输入参数addr--寄存器地址val--要写入的值
void MFRC522_Write2Register(u8 addr, u8 val) ;

//功能描述从MFRC522的某一寄存器读一个字节数据
//输入参数addr--寄存器地址
//返 回 值返回读取到的一个字节数据 
u8 MFRC522_ReadFromRegister(u8 addr) ;

//下面两个函数只对能读写位有效
//功能描述置RC522寄存器位
//输入参数addr--寄存器地址;mask--置位值
void SetBitMask(u8 addr, u8 mask)   ;

//功能描述清RC522寄存器位
//输入参数addr--寄存器地址;mask--清位值
void ClearBitMask(u8 addr, u8 mask)   ;

//功能描述复位MFRC522
void MFRC522_Reset(void) ;


//开启天线,每次启动或关闭天线发射之间应至少有1ms的间隔
void MFRC522_AntennaOn(void) ;

//关闭天线,每次启动或关闭天线发射之间应至少有1ms的间隔
void MFRC522_AntennaOff(void) ;

//MFRC522模块初始化
void MFRC522_Initializtion(void) ;

//功能描述RC522和ISO14443卡通讯
//输入参数command--MF522命令字
//					sendData--通过RC522发送到卡片的数据
//					sendLen--发送的数据长度
//					BackData--接收到的卡片返回数据
//					BackLen--返回数据的位长度
//返 回 值成功返回MI_O
u8 MFRC522_ToCard(u8 command, u8 *sendData, u8 sendLen, u8 *backData, u16 *backLen) ;


//功能描述寻卡读取卡类型号
//输入参数reqMode--寻卡方式
//					TagType--返回卡片类型
//					0x4400 = Mifare_UltraLight
//					0x0400 = Mifare_One(S50)
//					0x0200 = Mifare_One(S70)
//					0x0800 = Mifare_Pro(X)
//					0x4403 = Mifare_DESFire
//返 回 值成功返回MI_OK	
u8 MFRC522_Request(u8 reqMode, u8 *TagType);


//功能描述防冲突检测读取选中卡片的卡序列号
//输入参数serNum--返回4字节卡序列号,第5字节为校验字节
//返 回 值成功返回MI_OK
u8 MFRC522_Anticoll(u8 *serNum) ;

//功能描述用MF522计算CRC
//输入参数pIndata--要读数CRC的数据len--数据长度pOutData--计算的CRC结果
void CalulateCRC(u8 *pIndata, u8 len, u8 *pOutData) ;

//功能描述选卡读取卡存储器容量
//输入参数serNum--传入卡序列号
//返 回 值成功返回卡容量
u8 MFRC522_SelectTag(u8 *serNum) ;


//功能描述验证卡片密码
//输入参数authMode--密码验证模式
//					0x60 = 验证A密钥
//					0x61 = 验证B密钥
//					BlockAddr--块地址
//					Sectorkey--扇区密码
//					serNum--卡片序列号4字节
//返 回 值成功返回MI_OK
u8 MFRC522_Auth(u8 authMode, u8 BlockAddr, u8 *Sectorkey, u8 *serNum) ;

//功能描述读块数据
//输入参数blockAddr--块地址;recvData--读出的块数据
//返 回 值成功返回MI_OK
u8 MFRC522_Read(u8 blockAddr, u8 *recvData) ;


//功能描述写块数据
//输入参数blockAddr--块地址;writeData--向块写16字节数据
//返 回 值成功返回MI_OK
u8 MFRC522_Write(u8 blockAddr, u8 *writeData) ;

//功能描述命令卡片进入休眠状态
void MFRC522_Halt(void) ;

#endif
