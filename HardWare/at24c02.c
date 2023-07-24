#include "at24c02.h"

//AT24C02一共有32页，每页一共有8个字节，所以AT24C02的内存空间一共有256字节，而256字节一共有2048bit，也就是AT24C02的2K指的是bit的数目

/*
 * 功  能：AT24C02初始化  
 * 参  数：无
 * 返回值：无
 */
void AT24C02_Init(void)
{
	IIC_Sim_Init();
}

/*
 * 功  能：AT24C02实现页写 
 * 参  数：
 *		addr	---> 将数据写入 AT24C02 的 addr 这个地址中
 * 		pBuf	---> 保存要写入的数据的首地址
 * 		length	---> 你要写入的字节数
 * 返回值：
 *		失败	---> 1
 * 		成功	---> 成功写入的字节数
 */
uint8_t AT24C02_WriteData(uint8_t addr, uint8_t *pBuf, uint8_t length)
{
	uint8_t count = 0;			//统计写入的字节数
	uint8_t tempAddr = addr;	//临时存储地址
	uint8_t i;
	
	for(i=0; i<length; i++)
	{
		//当第一次进入 或者 遇到页边界地址就重新喊开始(8字节1页，共32页)
		if( (i == 0) || (tempAddr%8 == 0) )
		{
			//STM32发送停止信号 关闭上一次的起始信号
			IIC_Sim_Stop();
			Delay_ms(10);
			
			//1、STM32发送起始信号
			IIC_Sim_Start();
			
			//2、STM32发送器件地址选择相应设备并且说明待会儿要执行写操作
			IIC_Sim_SendByte(0xA0);
			if( IIC_Sim_WaitSlavAck() )  //表达式满足说明接收到不应答信号
			{
				IIC_Sim_Stop();
				printf("发送器件地址失败(0xA0)...\r\n");
				return 1;
			}
			//3、STM32发送字地址说明待会儿要把数据写入这个字地址
			IIC_Sim_SendByte(tempAddr);
			if( IIC_Sim_WaitSlavAck() )  //表达式满足说明接收到不应答信号
			{
				IIC_Sim_Stop();
				printf("发送字地址失败(%#hhx)...\r\n", addr);
				return 1;
			}
		}
		
		//4、STM32发送 数据 给 AT24C02
		IIC_Sim_SendByte(pBuf[count]);
		if( IIC_Sim_WaitSlavAck() )  //表达式满足说明接收到不应答信号
		{
			IIC_Sim_Stop();
			printf("发送数据失败...\r\n");
			return 1;
		}else{
			count++;		//统计写入的字节数
			tempAddr++;   		//将地址往后偏移
		}
	}
	
	//5、STM32发送停止信号
	IIC_Sim_Stop();
	//printf("发送数据完成...\r\n");
	
	return count;
}


/*
 * 功  能：AT24C02实现随机+顺序读
 *		addr	---> 将从 AT24C02 读取数据的的地址
 * 		pBuf	---> 读取到的数据所要存放的内存区域首地址
 * 		length	---> 要读取的字节数
 * 返回值：
 *		失败	---> 1
 * 		成功	---> 成功读取的字节数
 */
uint8_t AT24C02_ReadData(uint8_t addr ,uint8_t *pBuf ,uint8_t length)
{
    uint8_t count = 0; //统计写入的字节数
    uint8_t *p =pBuf;
    
    //STM32发送起始信号
    IIC_Sim_Start();
    
    //设备寻址，带有写访问
    IIC_Sim_SendByte(0xA0);
    
    //等待对方应答
    if(IIC_Sim_WaitSlavAck())   //若 ack = 1 说明接收到不应答信号
    {
        IIC_Sim_Stop();
        printf("发送器件地址失败(0xA0)...\r\n");
        return  1;        
    }
    
    //告诉对方从哪个地址读取数据
    IIC_Sim_SendByte(addr);
    
    //等待对方应答
    if(IIC_Sim_WaitSlavAck())   //若 ack = 1 说明接收到不应答信号
    {
        IIC_Sim_Stop();
        printf("发送字地址失败(%#hhx)...\r\n", addr);
        return  1;        
    }    
    
    //STM32再次发送起始信号
    IIC_Sim_Start();
    
    //设备寻址，带有读访问
    IIC_Sim_SendByte(0xA1);
    
    //等待对方应答
    if(IIC_Sim_WaitSlavAck())   //若 ack = 1 说明接收到不应答信号
    {
        IIC_Sim_Stop();
        printf("发送器件地址失败(0xA1)...\r\n");
        return  1;        
    }    
    
    //开始读取数据操作
    length = length-1;   //预留最后一次
    
    while(length--)
    {
        //读取从机发过来的数据
        *p++ = IIC_Sim_ReadByte();
        count++;
        //STM32向从机发送有应答信号
        IIC_Sim_MasterAck(0);
    
    }
    
    //读取最后一个数据
    *p = IIC_Sim_ReadByte();
    count++;
    //STM32向从机发送无应答信号，也就是说我已经读取完了，你不用再发数据了
    IIC_Sim_MasterAck(1);
    
    //停止信号
    IIC_Sim_Stop();
	//printf("接收数据完成...\r\n");
    
    return count;
    
}

