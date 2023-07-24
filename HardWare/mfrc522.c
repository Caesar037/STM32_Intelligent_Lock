//////////////////////////////////////////////////////////////////////////////////	 
//  功能描述  : MFRC522模块演示例程(STM32F407ZET6系列)
//              说明: 
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


//用通用引脚作高低电平输出充当VCC和GND
void MFRC522_Power_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置引脚PE13---GND、PD9---VCC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//工作模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//具体的模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//电阻上下拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			//引脚号
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		//电阻上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOD, GPIO_Pin_9);
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
}

//SPI3初始化
void MFRC522_SPI_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	/*配置片选引脚为输出模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_14; //NSS，SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_15; //MISO，RST
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	/*配置片选引脚为输入模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	
	//模式3，时钟线在总线空闲的时候是高电平	
    GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_14); //总线空闲
    GPIO_SetBits(GPIOE,GPIO_Pin_7|GPIO_Pin_15); //总线空闲
}



//功能描述向MFRC522的某一寄存器写一个字节数据
//输入参数addr--寄存器地址val--要写入的值
void MFRC522_Write2Register(u8 addr, u8 val) 
{
	unsigned char i, ucAddr;

    MF522_NSS(0);
    MF522_SCK(0);
    
    // 地址左移一位是因为LSB是要保留 即RFU位(Reserved for Future Use)
    // &0x7E 是把bit1~bit6 的地址(address)写入
    // |0x80 是为了使最高位为1   1(Read) 0(Write) 即使能 '读'
    ucAddr = ((addr<<1)&0x7E); 
    
    for(i=8;i>0;i--)
    {
        if( (ucAddr&0x80)==0x80 )
        {
           MF522_MOSI(1);
        }
        else
           MF522_MOSI(0);
    
        //在SCK高电平保持期间发送数据
        MF522_SCK(1);
        ucAddr <<= 1;
        MF522_SCK(0);
        Delay_us(10);        
    }
    
    //发送寄存器数据
    for(i=8;i>0;i--)
    {
        if( (val&0x80)==0x80 )
        {
           MF522_MOSI(1);
        }
        else
           MF522_MOSI(0);
    
        //在SCK高电平保持期间发送数据
        MF522_SCK(1);
        val <<= 1;
        MF522_SCK(0);
        Delay_us(10);  
    }
    
    MF522_NSS(1);
    MF522_SCK(1);
}

//功能描述从MFRC522的某一寄存器读一个字节数据
//输入参数addr--寄存器地址
//返 回 值返回读取到的一个字节数据 
u8 MFRC522_ReadFromRegister(u8 addr) 
{
    unsigned char i, ucAddr;
    unsigned char read_data = 0;
    
    MF522_NSS(0);
    MF522_SCK(0);
    
    // 地址左移一位是因为LSB是要保留 即RFU位(Reserved for Future Use)
    // &0x7E 是把bit1~bit6 的地址(address)写入
    // |0x80 是为了使最高位为1   1(Read) 0(Write) 即使能 '读'
    ucAddr = ((addr<<1)&0x7E)|0x80;
    
    for(i=8;i>0;i--)
    {
        if( (ucAddr&0x80)==0x80 )
        {
           MF522_MOSI(1);
        }
        else
           MF522_MOSI(0);
    
        //在SCK高电平保持期间发送数据
        MF522_SCK(1);
        ucAddr <<= 1;
        MF522_SCK(0);
        Delay_us(10);   
    }
    
    //读取寄存器数据
    for(i=8;i>0;i--)
    {
       
        //在SCK高电平保持期间发送数据
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


//下面两个函数只对能读写位有效
//功能描述置RC522寄存器位
//输入参数addr--寄存器地址;mask--置位值
void SetBitMask(u8 addr, u8 mask)   
{     
	u8 tmp=0;
   
	tmp=MFRC522_ReadFromRegister(addr);     
	MFRC522_Write2Register(addr,tmp|mask);  // set bit mask 
}

//功能描述清RC522寄存器位
//输入参数addr--寄存器地址;mask--清位值
void ClearBitMask(u8 addr, u8 mask)   
{     
	u8 tmp=0;

    tmp=MFRC522_ReadFromRegister(addr);     
	MFRC522_Write2Register(addr,tmp&(~mask));  // set bit mask 
}

//功能描述复位MFRC522
void MFRC522_Reset(void) 
{ 
	//外复位可以不用
	MF522_RST(1);
	Delay_us(10);
	MF522_RST(0);
	Delay_us(10);
	MF522_RST(1);
	Delay_us(10); 
	//内复位   
	MFRC522_Write2Register(CommandReg, PCD_RESETPHASE); 
}

//开启天线,每次启动或关闭天线发射之间应至少有1ms的间隔
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

//关闭天线,每次启动或关闭天线发射之间应至少有1ms的间隔
void MFRC522_AntennaOff(void) 
{  
	ClearBitMask(TxControlReg,0x03);
}

//MFRC522模块初始化
void MFRC522_Initializtion(void) 
{
    //对模拟SPI引脚进行初始化
	MFRC522_SPI_Init();  
    
    //复位MFRC522
	MFRC522_Reset();    
    
	//Timer: TPrescaler*TreloadVal/6.78MHz = 0xD3E*0x32/6.78=25ms     
	MFRC522_Write2Register(TModeReg,0x8D);				        //TAuto=1为自动计数模式，受通信协议影向。低4位为预分频值的高4位
	MFRC522_Write2Register(TPrescalerReg,0x3E); 	            //预分频值的低8位     
	MFRC522_Write2Register(TReloadRegL,0x32);		            //计数器的低8位                
	MFRC522_Write2Register(TReloadRegH,0x00);		            //计数器的高8位       
	MFRC522_Write2Register(TxAutoReg,0x40); 			        //100%ASK     
	MFRC522_Write2Register(ModeReg,0x3D); 				        //CRC初始值0x6363
	MFRC522_Write2Register(CommandReg,0x00);			        //启动MFRC522     
	MFRC522_AntennaOn();          							    //打开天线 
}


//功能描述RC522和ISO14443卡通讯
//输入参数command--MF522命令字
//					sendData--通过RC522发送到卡片的数据
//					sendLen--发送的数据长度
//					BackData--接收到的卡片返回数据
//					BackLen--返回数据的位长度
//返 回 值成功返回MI_OK
u8 MFRC522_ToCard(u8 command, u8 *sendData, u8 sendLen, u8 *backData, u16 *backLen) 
{
	u8  status=MI_ERR;
	u8  irqEn=0x00;
	u8  waitIRq=0x00;
	u8  lastBits;
	u8  n;
	u16 i;
	//根据命预设中断参数
	switch (command)     
	{         
		case PCD_AUTHENT:  		//认证卡密   
			irqEn 	= 0x12;			//    
			waitIRq = 0x10;			//    
			break;
		case PCD_TRANSCEIVE: 	//发送FIFO中数据      
			irqEn 	= 0x77;			//    
			waitIRq = 0x30;			//    
			break;      
		default:    
			break;     
	}

	MFRC522_Write2Register(ComIEnReg, irqEn|0x80);		//允许中断请求     
	ClearBitMask(ComIrqReg, 0x80);  				//清除所有中断请求位               	
	SetBitMask(FIFOLevelReg, 0x80);  				//FlushBuffer=1, FIFO初始化
	MFRC522_Write2Register(CommandReg, PCD_IDLE); 		//使MFRC522空闲   
    
	//向FIFO中写入数据     
	for (i=0; i<sendLen; i++)
    {
        MFRC522_Write2Register(FIFODataReg, sendData[i]);
    }
    
	//执行命令
	MFRC522_Write2Register(CommandReg, command);
	//天线发送数据     
	if (command == PCD_TRANSCEIVE)					//如果是卡片通信命令，MFRC522开始向天线发送数据      
		SetBitMask(BitFramingReg, 0x80);  		//StartSend=1,transmission of data starts      
	//等待接收数据完成     
	i = 10000; //i根据时钟频率调整操作M1卡最大等待时间25ms     
	do      
	{        
		n = MFRC522_ReadFromRegister(ComIrqReg);
		//irq_regdata=n;	//test         
		i--;
		//wait_count=i;		//test		     
	}while ((i!=0) && !(n&0x01) && !(n&waitIRq));	//接收完就退出n=0x64
	//停止发送
	ClearBitMask(BitFramingReg, 0x80);   		//StartSend=0
	//如果在25ms内读到卡
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



//功能描述寻卡读取卡类型号
//输入参数reqMode--寻卡方式
//					TagType--返回卡片类型
//					0x4400 = Mifare_UltraLight
//					0x0400 = Mifare_One(S50)
//					0x0200 = Mifare_One(S70)
//					0x0800 = Mifare_Pro(X)
//					0x4403 = Mifare_DESFire
//返 回 值成功返回MI_OK	
u8 MFRC522_Request(u8 reqMode, u8 *TagType)
{  
	u8  status;    
	u16 backBits;   //接收到的数据位数
    
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

//功能描述防冲突检测读取选中卡片的卡序列号
//输入参数serNum--返回4字节卡序列号,第5字节为校验字节
//返 回 值成功返回MI_OK
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
		//校验卡序列号   
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

//功能描述用MF522计算CRC
//输入参数pIndata--要读数CRC的数据len--数据长度pOutData--计算的CRC结果
void CalulateCRC(u8 *pIndata, u8 len, u8 *pOutData) 
{     
	u16 i;
	u8  n;
	//      
	ClearBitMask(DivIrqReg, 0x04);   			//CRCIrq = 0     
	SetBitMask(FIFOLevelReg, 0x80);   		//清FIFO指针     
	MFRC522_Write2Register(CommandReg, PCD_IDLE);   
	//向FIFO中写入数据      
	for (i=0; i<len; i++)
		MFRC522_Write2Register(FIFODataReg, *(pIndata+i));
	//开始RCR计算
	MFRC522_Write2Register(CommandReg, PCD_CALCCRC);
	//等待CRC计算完成     
	i = 1000;     
	do      
	{         
		n = MFRC522_ReadFromRegister(DivIrqReg);         
		i--;     
	}while ((i!=0) && !(n&0x04));   //CRCIrq = 1
	//读取CRC计算结果     
	pOutData[0] = MFRC522_ReadFromRegister(CRCResultRegL);     
	pOutData[1] = MFRC522_ReadFromRegister(CRCResultRegH);
	MFRC522_Write2Register(CommandReg, PCD_IDLE);
}

//功能描述选卡读取卡存储器容量
//输入参数serNum--传入卡序列号
//返 回 值成功返回卡容量
u8 MFRC522_SelectTag(u8 *serNum) 
{     
	u8  i;     
	u8  status;     
	u8  size;     
	u16 recvBits;     
	u8  buffer[9];
	//     
	buffer[0] = PICC_ANTICOLL1;	//防撞码1     
	buffer[1] = 0x70;
	buffer[6] = 0x00;						     
	for (i=0; i<4; i++)					
	{
		buffer[i+2] = *(serNum+i);	//buffer[2]-buffer[5]为卡序列号
		buffer[6]  ^=	*(serNum+i);	//卡校验码
	}
	//
	CalulateCRC(buffer, 7, &buffer[7]);	//buffer[7]-buffer[8]为RCR校验码
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


//功能描述验证卡片密码
//输入参数authMode--密码验证模式
//					0x60 = 验证A密钥
//					0x61 = 验证B密钥
//					BlockAddr--块地址
//					Sectorkey--扇区密码
//					serNum--卡片序列号4字节
//返 回 值成功返回MI_OK
u8 MFRC522_Auth(u8 authMode, u8 BlockAddr, u8 *Sectorkey, u8 *serNum) 
{     
	u8  status;     
	u16 recvBits;     
	u8  i;  
	u8  buff[12];    
	//验证模式+块地址+扇区密码+卡序列号     
	buff[0] = authMode;		//验证模式     
	buff[1] = BlockAddr;	//块地址     
	for (i=0; i<6; i++)
		buff[i+2] = *(Sectorkey+i);	//扇区密码
	//
	for (i=0; i<4; i++)
		buff[i+8] = *(serNum+i);		//卡序列号
	//
	status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
	//      
	if ((status != MI_OK) || (!(MFRC522_ReadFromRegister(Status2Reg) & 0x08)))
		status = MI_ERR;
	//
	return status;
}

//功能描述读块数据
//输入参数blockAddr--块地址;recvData--读出的块数据
//返 回 值成功返回MI_OK
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

//功能描述写块数据
//输入参数blockAddr--块地址;writeData--向块写16字节数据
//返 回 值成功返回MI_OK
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
		for (i=0; i<16; i++)  //向FIFO写16Byte数据                     
			buff[i] = *(writeData+i);
		//                     
		CalulateCRC(buff, 16, &buff[16]);         
		status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);           
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))               
			status = MI_ERR;         
	}          
	return status;
}

//功能描述命令卡片进入休眠状态
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
