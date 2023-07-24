#include "rtc.h"


void rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	//RTCӵ�б��ù��磨Ŧ�۵�أ����������ȡ���ݼĴ��������ݣ����ж�RTC�Ƿ�һֱ����
	//������ݼĴ��������ݶ�ʧ����Ҫ����RTC������д�뱸�ݼĴ���������(20��32λ�Ĵ���)
	if( BKP_DATA !=  RTC_ReadBackupRegister(RTC_BKP_DR0) )
	{
		//printf("��������RTC...\r\n");
		
		//����PWRʱ�ӣ�RTC�ļĴ���������PWR���ƣ�
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		//���RTCд��������Ҫ�õ�PWR�ٷ��������ļ���
		PWR_BackupAccessCmd(ENABLE);
	
#if LSE_OR_LSI	//ѡ��RTCʱ��Դ
		
		//���ⲿ����ʱ������
		RCC_LSEConfig(RCC_LSE_ON);	//23.768kHz
		
		//�ȴ�ʱ��Դ�ȶ�
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
		//����LSEΪRTC��ʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#else
		//ʹ���ڲ�����ʱ��Դ�����ȶ���//32kHz
		RCC_LSICmd(ENABLE);
		
		//�ȴ�ʱ��Դ�ȶ�
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0);
		
		//����LSIΪRTC��ʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#endif
		
		//ʹ��RTCʱ��
		RCC_RTCCLKCmd(ENABLE);
		
		//�ȴ�RTC��ؼĴ�������
		RTC_WaitForSynchro();
		
		//����RTC --> 32.768/128/256 = 1Hz --> 1s
		RTC_InitStruct.RTC_AsynchPrediv = 0x7F;	//�첽Ԥ��Ƶֵ:128
		RTC_InitStruct.RTC_SynchPrediv  = 0xFF;	//ͬ��Ԥ��Ƶֵ:256��LSI�ĳ�0xF9��
		RTC_InitStruct.RTC_HourFormat   = RTC_HourFormat_24;	//24Сʱ��
		RTC_Init(&RTC_InitStruct);
		
		//����RTCʱ��
		RTC_TimeStruct.RTC_H12     = RTC_H12_PM;	//����
		RTC_TimeStruct.RTC_Hours   = 14;	//Сʱ
		RTC_TimeStruct.RTC_Minutes = 20;	//����
		RTC_TimeStruct.RTC_Seconds = 0;	//��
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

		//����RTC����
		RTC_DateStruct.RTC_Year    = 23;
		RTC_DateStruct.RTC_Month   = RTC_Month_July;
		RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Friday;
		RTC_DateStruct.RTC_Date    = 21;
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);	//RTC_Format_BCD����8421BCD�����ʾʮ��������ʽ��ֱ�ۣ���ռ�ÿռ䣩
														//RTC_Format_BIN���Զ�������ʽ��ʾ��ʮ������������洢��������㣩
		
		//����д�뱸�ݼĴ����������´ο������
		RTC_WriteBackupRegister(RTC_BKP_DR0,BKP_DATA);
		
	}
	else	//��ȡ���ݼĴ����������δ�ı䣬˵��RTCδ���磬��������������RTC
	{
		//printf("��������RTC...\r\n");
		
		//����PWRʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		//���RTCд����
		PWR_BackupAccessCmd(ENABLE);
		
		//��������жϱ�־λ
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		
		//�ȴ�ͬ��
		RTC_WaitForSynchro();
	}
	
	//����RTC���ӣ�һ��������ѡ�䣩
	//RTC_AlarmConfig();
	
	//��RTC�����жϣ�ֻ��һ����ѡ�䣩
	//RTC_WakeUpConfig();

}


void RTC_WakeUpConfig(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�ж������ã�17ΪRTC�����¼���21ΪRTC���ֺ�ʱ����¼���22ΪRTC�����¼�����23��EXTI_Line��
	EXTI_ClearITPendingBit(EXTI_Line22);	//�ⲿ�ж��ߵĴ���
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//����RTC�����ж�Դ�������жϵȼ�
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;	//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//�رջ����ж�������
	RTC_WakeUpCmd(DISABLE);
	
	//RTC�����ж�ʱ��Դ�����ã�CK_SPRE(32768/2^(16-1)=1Hz)
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//��ʼ��RTC���Ѽ�������д��ֵĬ����0���ݼ�һ�δ���(���16λ)
	RTC_SetWakeUpCounter(0x0000);
	
	//�����жϱ�־λ���
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//�����жϵĿ���
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	//�����жϵ�ʹ��
	RTC_WakeUpCmd(ENABLE);
}
	
//��������A
void RTC_AlarmConfig(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//�ж������ã�17ΪRTC�����¼���21ΪRTC���ֺ�ʱ����¼���22ΪRTC�����¼�����23��EXTI_Line��
	EXTI_ClearITPendingBit(EXTI_Line17);	//�ⲿ�ж��ߵĴ���
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//����RTC�����ж�Դ�������жϵȼ�
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;	//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//�ر������ж�������
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	//��������A������
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel 	 = RTC_AlarmDateWeekDaySel_WeekDay;	//ָ�����ڼ���Ч�������ڣ�
	RTC_AlarmStructure.RTC_AlarmDateWeekDay 	 = RTC_Weekday_Monday;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours	 = 1;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;
	RTC_AlarmStructure.RTC_AlarmMask 			 = RTC_AlarmMask_DateWeekDay;	//������һ������ڵ�����(����)
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

	//��������A���벢ʹ����������:ÿ�����8���ж�
	//RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);
	
	//����RTC������A�����ж�
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	//�����жϱ�־λ���
	//RTC_ClearITPendingBit(RTC_IT_ALRA);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
	
	//��RTC������A����
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	
}

static uint8_t ShowTime[12] = {0};

//ʱ��Ļ�ȡ����ʾʱ����
void RTC_TimeShow(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	
	snprintf((char*)ShowTime, sizeof(ShowTime), "%02d:%02d:%02d  ", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	
	printf("Time: %s ", ShowTime);
}

static uint8_t ShowData[32] = {0};

//���ڵĻ�ȡ����ʾ������
void RTC_DataShow(void)
{
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	snprintf((char*)ShowData, sizeof(ShowData), "%02d-%02d-%02d", RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	
	printf("Data: %s \n", ShowData);
}


//�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		printf(" Alarm������\r\n");
		
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}


//�����жϷ�������ÿ1s����ʱ������ڷ�����������ʾ����
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{	
		//��ʾʱ��
		RTC_TimeShow();
		
		//��ʾ����
		RTC_DataShow();
		
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
}
