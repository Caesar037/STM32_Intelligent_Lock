#include "rtc.h"


void rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	//RTC拥有备用供电（纽扣电池），开机后读取备份寄存器的数据，以判断RTC是否一直工作
	//如果备份寄存器的数据丢失，需要更新RTC并重新写入备份寄存器的数据(20个32位寄存器)
	if( BKP_DATA !=  RTC_ReadBackupRegister(RTC_BKP_DR0) )
	{
		//printf("重新配置RTC...\r\n");
		
		//设置PWR时钟（RTC的寄存器保护由PWR控制）
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		//解除RTC写保护（需要用到PWR官方驱动库文件）
		PWR_BackupAccessCmd(ENABLE);
	
#if LSE_OR_LSI	//选择RTC时钟源
		
		//打开外部低速时钟震荡器
		RCC_LSEConfig(RCC_LSE_ON);	//23.768kHz
		
		//等待时钟源稳定
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
		//设置LSE为RTC的时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#else
		//使能内部低速时钟源（不稳定）//32kHz
		RCC_LSICmd(ENABLE);
		
		//等待时钟源稳定
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0);
		
		//设置LSI为RTC的时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#endif
		
		//使能RTC时钟
		RCC_RTCCLKCmd(ENABLE);
		
		//等待RTC相关寄存器就绪
		RTC_WaitForSynchro();
		
		//配置RTC --> 32.768/128/256 = 1Hz --> 1s
		RTC_InitStruct.RTC_AsynchPrediv = 0x7F;	//异步预分频值:128
		RTC_InitStruct.RTC_SynchPrediv  = 0xFF;	//同步预分频值:256（LSI改成0xF9）
		RTC_InitStruct.RTC_HourFormat   = RTC_HourFormat_24;	//24小时制
		RTC_Init(&RTC_InitStruct);
		
		//设置RTC时间
		RTC_TimeStruct.RTC_H12     = RTC_H12_PM;	//上午
		RTC_TimeStruct.RTC_Hours   = 14;	//小时
		RTC_TimeStruct.RTC_Minutes = 20;	//分钟
		RTC_TimeStruct.RTC_Seconds = 0;	//秒
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

		//设置RTC日期
		RTC_DateStruct.RTC_Year    = 23;
		RTC_DateStruct.RTC_Month   = RTC_Month_July;
		RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Friday;
		RTC_DateStruct.RTC_Date    = 21;
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);	//RTC_Format_BCD：以8421BCD编码表示十进制数格式（直观，但占用空间）
														//RTC_Format_BIN：以二进制形式表示六十进制数（方便存储，方便计算）
		
		//重新写入备份寄存器，用于下次开机检测
		RTC_WriteBackupRegister(RTC_BKP_DR0,BKP_DATA);
		
	}
	else	//读取备份寄存器里的数据未改变，说明RTC未掉电，则无需重新配置RTC
	{
		//printf("无需配置RTC...\r\n");
		
		//设置PWR时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		//解除RTC写保护
		PWR_BackupAccessCmd(ENABLE);
		
		//清除闹钟中断标志位
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		
		//等待同步
		RTC_WaitForSynchro();
	}
	
	//设置RTC闹钟（一共两个，选配）
	//RTC_AlarmConfig();
	
	//配RTC唤醒中断（只有一个，选配）
	//RTC_WakeUpConfig();

}


void RTC_WakeUpConfig(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//中断线配置，17为RTC闹钟事件，21为RTC入侵和时间戳事件，22为RTC唤醒事件（共23个EXTI_Line）
	EXTI_ClearITPendingBit(EXTI_Line22);	//外部中断线的处理
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//设置RTC唤醒中断源、设置中断等级
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;	//唤醒中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//关闭唤醒中断再配置
	RTC_WakeUpCmd(DISABLE);
	
	//RTC唤醒中断时钟源的配置：CK_SPRE(32768/2^(16-1)=1Hz)
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//初始化RTC唤醒计数器，写入值默认是0，递减一次触发(最大16位)
	RTC_SetWakeUpCounter(0x0000);
	
	//唤醒中断标志位清除
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//唤醒中断的开启
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	//唤醒中断的使能
	RTC_WakeUpCmd(ENABLE);
}
	
//配置闹钟A
void RTC_AlarmConfig(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//中断线配置，17为RTC闹钟事件，21为RTC入侵和时间戳事件，22为RTC唤醒事件（共23个EXTI_Line）
	EXTI_ClearITPendingBit(EXTI_Line17);	//外部中断线的处理
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//设置RTC闹钟中断源、设置中断等级
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;	//闹钟中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//关闭闹钟中断再配置
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	//设置闹钟A的掩码
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel 	 = RTC_AlarmDateWeekDaySel_WeekDay;	//指定星期几生效（或日期）
	RTC_AlarmStructure.RTC_AlarmDateWeekDay 	 = RTC_Weekday_Monday;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours	 = 1;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;
	RTC_AlarmStructure.RTC_AlarmMask 			 = RTC_AlarmMask_DateWeekDay;	//屏蔽哪一天或星期的配置(掩码)
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

	//设置闹钟A亚秒并使能亚秒闹钟:每秒产生8次中断
	//RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);
	
	//允许RTC的闹钟A触发中断
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	//闹钟中断标志位清除
	//RTC_ClearITPendingBit(RTC_IT_ALRA);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
	
	//让RTC的闹钟A工作
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	
}

static uint8_t ShowTime[12] = {0};

//时间的获取，显示时分秒
void RTC_TimeShow(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	
	snprintf((char*)ShowTime, sizeof(ShowTime), "%02d:%02d:%02d  ", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	
	printf("Time: %s ", ShowTime);
}

static uint8_t ShowData[32] = {0};

//日期的获取，显示年月日
void RTC_DataShow(void)
{
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	snprintf((char*)ShowData, sizeof(ShowData), "%02d-%02d-%02d", RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	
	printf("Data: %s \n", ShowData);
}


//闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		printf(" Alarm！！！\r\n");
		
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}


//唤醒中断服务函数：每1s，把时间和日期发送至串口显示出来
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{	
		//显示时间
		RTC_TimeShow();
		
		//显示日期
		RTC_DataShow();
		
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
}
