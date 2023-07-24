/**************************************************************************
 *	   _         _   _                   ____                            
 *	  / \  _   _| |_| |__   ___  _ __   / ___|__ _  ___  ___  __ _ _ __  
 *   / _ \| | | | __| '_ \ / _ \| '__| | |   / _` |/ _ \/ __|/ _` | '__| 
 *  / ___ \ |_| | |_| | | | (_) | |    | |__| (_| |  __/\__ \ (_| | |    
 * /_/   \_\__,_|\__|_| |_|\___/|_|     \____\__,_|\___||___/\__,_|_|    
 *	
 **************************************************************************/

#include "stm32f4xx.h"

#include "keyboard.h"
#include "systick.h"
#include "at24c02.h"
#include "w25q128.h"
#include "network.h"
#include "mfrc522.h"
#include "fr1002.h"
#include "fpm383.h"
#include "dht11.h"
#include "wifi.h"
#include "oled.h"
#include "exti.h"
#include "uart.h"
#include "iwdg.h"
#include "wwdg.h"
#include "sr04.h"
#include "rtc.h"
#include "adc.h"
#include "tim.h"
#include "pwm.h"
#include "led.h"
#include "key.h"
#include "sys.h"

/************  函数声明  *************/
//测试函数
void pwm_test(void);
void beep_test(void);
void rfid_test(void);
void reset_test(void);
void fr1002_test(void);
void w25q128_test(void);
void at24c02_test(void);
void keyboard_test(void);
void fpm_test(void);


//主要功能
void unlock_func(void);
void lock_func(void);

void menu_func(void);
void root_func(void);
void password_func(void);
void password_change(void);
void lock_force(void);
void finger_func(void);
void finger_add(void);
void finger_clear(void);
void face_func(void);
void face_add(void);
void face_clear(void);
void rfid_func(void);
void rfid_add(void);

int main(void)
{
/************	初始化	*************/
	
	//设置中断分组为组2（抢占优先级等级有2^2（4个）等级，响应优先级等级有4个等级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//初始化(配置）矩阵键盘
	key_board_init();
	
	//初始化(配置）systick（中断延时）
    SysTick_Init();
	
	//设置Systick时钟源为168MHZ（计数器延时）
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
	//初始化(配置）IWDG(独立看门狗)
	iwdg_init(IWDG_Prescaler_128, 250);	//超时为1秒多（32KHZ~60KHZ）
	
	//初始化(配置）TIM（外部定时器）
	tim7_init();
	
	//初始化(配置）PWM（脉冲宽度调制）
	pwm_SG90_init();
	//pwm_beep_init();
	
	//初始化(配置）UART（串口通信）
	UART1_Config(9600);
	
	//初始化fpm电容指纹传感器（串口2，TIM7，EXIT6）
	fpm_power_init();
	fpm_init();
	
	
	//初始化fr1002人脸识别，跟3D人脸识别模块进行握手（串口3，TIM7）
	while(fr_init())
	{
		Delay_ms(1000);
		printf("3D人脸识别模块连接中 ...\r\n");		
	}
	
	//初始化(配置）RTC（实时时钟）
	rtc_init();
	
	//初始化(配置）OLED（IIC总线）
	OLED_Power_Init();	//启动模拟电源
	OLED_Config();	//配置驱动IC：SSD1306
	
	//初始化（配置）AT24（模拟IIC总线）
	AT24C02_Init();
	
	//初始化（配置）W25Qxx（硬件SPI总线）
	//W25Q128_Init();
	
	//初始化（配置）MFRC522（模拟SPI总线）
	MFRC522_Power_Init();
	MFRC522_Initializtion();
	
	//初始化(配置）ESP8266（WIFI连接）
	//Wifi_STA_Config();	//连接热点
	//Get_Network_Weather();	//连接服务器端口，TCP发送数据报
	
	
/************	运 行	**************/

	//reset_test();	//复位检测函数
	
	printf("初始化外设已完成...\r\n");	//在Linux系统上回车换行符号为： \n ; 在Windows系统上回车换行符号为： \r\n 
	OLED_OFF();
	
	while(1)
	{
		
		menu_func(); //用户菜单功能
		root_func(); //蓝牙管理员功能
		
	}
}

/************  测试函数	 *************/

//看门狗测试
void reset_test(void)
{
	//通过读取RCC_CSR里的位29 IWDGRSTF ，判断它的值是否为1（即判断复位标志的状态来确定复位原因），如果为1，表示它（独立窗口看门狗）引起了复位
	if( SET == RCC_GetFlagStatus(RCC_FLAG_IWDGRST) )
	{
		//清除标志位
		RCC_ClearFlag();
		
		//做相关的事情或记录相关信息
		printf("独立看门狗复位...\r\n");		
	}
	else if( SET == RCC_GetFlagStatus(RCC_FLAG_WWDGRST) )
	{
		//清除标志位
		RCC_ClearFlag();
		
		//做相关的事情或记录相关信息
		printf("窗口看门狗复位...\r\n");		
	}
	else if( SET == RCC_GetFlagStatus(RCC_FLAG_PORRST) )
	{
		//做相关的事情或记录相关信息
		printf("电源复位...\r\n");	
	}
	else if( SET == RCC_GetFlagStatus(RCC_FLAG_PINRST) )
	{
		//做相关的事情或记录相关信息
		printf("外部复位...\r\n");	
	}
	else
	{
		//做相关的事情或记录相关信息
		printf("其他复位...\r\n");	
	}
	
}

uint32_t pwm_num = 0; //pwm强度（取值：0-100）
uint8_t pwm_flag = 0; //pwm标志，判断是升高还是降低

//pwm测试函数
void pwm_test(void)
{
	Delay_ms(50);
	if(pwm_num == 0 || pwm_num == 100)
		pwm_flag = !pwm_flag;	//pwm标志，判断是递增还是递减
	
	if(pwm_flag)
	{
		pwm_num += 10;
	}else
	{
		pwm_num -= 10;
	}
	
	//呼吸灯PF9---TIM14_CH1
	//TIM_SetCompare1(TIM14, pwm_num);	//取值范围：0-TIM_Period，即0-100
	
	//蜂鸣器PF8---TIM13_CH1（占空比->响度变化，频率->音调）
	//TIM_SetCompare1(TIM13, pwm_num);	//取值范围：0-TIM_Period，即0-100
	
	//SG90舵机PC8---TIM3_CH3（angle范围0-180）
	//TIM_SetCompare3(TIM3, angle(pwm_num));	//取值范围：0-TIM_Period，即0-100
}

uint8_t send[16]; //写入测试数据
uint8_t recv[16]; //读取测试数据

//at24c02测试函数
void at24c02_test(void)
{
	memset(send, 0, sizeof(send));
	strcpy((char *)send,"hello at24c02\r\n");
	
	AT24C02_WriteData(0x00, send, sizeof(send));
	Delay_ms(1000);
	
	memset(recv, 0, sizeof(recv));
	AT24C02_ReadData(0x00, recv, sizeof(recv));
	Delay_ms(1000);
	
//	printf("AT24C02 send count：%d\r\n",AT24C02_WRITE_PAGE(0x00, send));
//	Delay_ms(10);
//	printf("AT24C02 recv count：%d\r\n",AT24C02_READ_PAGE(0x00, recv));
//	Delay_ms(10);
	
	printf("AT24C02: %s",recv);
}

//w25q128测试函数
void w25q128_test(void)
{
	//读取W25Q128设备ID
	printf("W25Q128 ID: %#hx\r\n", W25Q128_ReadID());
	Delay_ms(1000);
	
	memset(send, 0, sizeof(send));
	strcpy((char *)send,"hello w25q128\r\n");
	
	W25Q128_SectorErase(0x000000);	//写之前清除扇区
	W25Q128_PageProgram(0x000000, send, sizeof(send));
	Delay_ms(1000);
	
	memset(recv, 0, sizeof(recv));
	W25Q128_ReadData(0x000000, recv, sizeof(recv));
	Delay_ms(1000);
	
	printf("W25Q128: %s",recv);
}

int32_t distance = 0; //sr04超声波测距距离

//sr04超声波测试函数
void sr04_test(void)
{
	distance = sr04_get_distance();
		
	if(distance>=20 && distance<=4000)
	{
		printf("distance is %d mm\r\n",distance);	
	}
	else
	{
		printf("distance is invalid\r\n");
	}
	
	Delay_ms(1000);
}

//蜂鸣器音乐测试函数
void beep_test(void)
{
	/* 4 5 3 6 2 5 1 */
	pwm_beep_set(TONE_4_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_5_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_3_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_6_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_2_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_5_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_1_M, 15); Delay_ms(500);
	/* 关闭 */
	pwm_beep_set(0, 0); Delay_ms(2000);
}

//矩阵键盘
uint32_t key_sta=0;
uint8_t key_old=0;
uint8_t key_cur=0;

//矩阵键盘测试函数
void keyboard_test(void)
{
	
	/* 使用状态机思想得到按键的状态 */
	switch(key_sta)
	{
		case 0://获取按下的按键
		{
			
			key_cur = get_key_board();	

			if(key_cur != 'N')
			{
				key_old = key_cur;
				key_sta=1;
			}
				
		}break;
		
		
		case 1://确认按下的按键（按键消抖）
		{
			
			key_cur = get_key_board();	
				
			if((key_cur != 'N') && (key_cur == key_old))
			{
				printf("KEY %c Down\r\n",key_cur);
				key_sta=2;
			}				
		
		}break;
	
		case 2://获取释放的按键
		{
			
			key_cur = get_key_board();
			
			if(key_cur == 'N')
			{
				printf("KEY %c Up\r\n",key_old);
				
				//舵机测试
				if(key_old == 'A'){Rotation(0);}
				else if(key_old == 'B'){Rotation(60);}
				else if(key_old == 'C'){Rotation(120);}
				else if(key_old == 'D'){Rotation(180);}
				
				key_sta=0;
				key_old = 'N';
			}
		
		}break;

		default:break;
	}
	

	
}

//fpm电容指纹传感器
uint8_t fpm_error_code=0;
uint16_t id=0;
uint16_t id_total=0;
uint32_t run_mode=0;

//fpm电容指纹传感器测试（需要用到矩阵键盘）
void fpm_test(void)
{
	/* 默认是刷指纹认证模式 */
	if(run_mode == 0)
	{
		if(g_fpm_touch_event)
		{
		
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			printf("执行刷指纹操作,请将手指放到指纹模块触摸感应区\r\n");
			
			/* 参数为0xFFFF进行1:N匹配 */
			id = 0xFFFF;
			
			fpm_error_code = fpm_idenify_auto(&id);
			
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("自动验证指纹%04d成功!\r\n", id);
				
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				fpm_ctrl_led(FPM_LED_RED);
			}
			
			
			
			Delay_ms(1000);		
		
			g_fpm_touch_event = 0;
			
			fpm_sleep();
			
			Delay_ms(1000);
		}		
	
	}
	
	/* 用于切换当前程序执行模式 */
	if(0 == key_value())
	{
		run_mode ^= 1;
		
		/* 若run_mode为1，即为设置指纹模块模式 */
		if(run_mode == 1)
			PFout(9) = 0;	
		/* 若run_mode为0，即为刷指纹认证模式 */
		else
			PFout(9) = 1;
		
		
		Delay_ms(500);
		
	}		
	
	/* 进入按键设置指纹模块模式 */
	if(run_mode == 1)
	{
	
		/* 添加指纹 */
		if(1 == key_value())
		{
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			printf("执行添加指纹操作,请将手指放到指纹模块触摸感应区\r\n");
			
			fpm_error_code = fpm_id_total(&id_total);
	
			if(fpm_error_code == 0)
			{
				printf("获取指纹总数：%04d\r\n", id_total);
				
				/* 添加指纹*/
				fpm_error_code = fpm_enroll_auto(id_total+1);
	
				if(fpm_error_code == 0)
				{
					fpm_ctrl_led(FPM_LED_GREEN);					
					
					printf("自动注册指纹成功\r\n");
					
					pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);				
				}
				else
				{
					fpm_ctrl_led(FPM_LED_RED);
				}
				
				Delay_ms(100);	
				
				fpm_sleep();
				
				Delay_ms(1000);					
				
			}
		}
		
		
		/* 获取用户总数 */
		if(2 == key_value())
		{
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			
			fpm_error_code = fpm_id_total(&id_total);
	
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("获取指纹总数：%04d\r\n", id_total);
				
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				fpm_ctrl_led(FPM_LED_RED);
			}			
			
			Delay_ms(100);		
			
			fpm_sleep();	
	
			Delay_ms(1000);				
		}	
	
		
		/* 清空指纹 */
		if(3 == key_value())
		{
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			
			fpm_error_code = fpm_empty();
			
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("清空指纹成功\r\n");
				
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				fpm_ctrl_led(FPM_LED_RED);
			}			
			Delay_ms(100);		
			
			fpm_sleep();
	
			Delay_ms(1000);								
		}		
	
	}
}

//fr1002人脸识别模块
uint8_t user_buf[64]={0};
int32_t user_total;
uint8_t user_error_code=0;

void fr1002_test(void)
{		
		/* 添加人脸 */
		if(0 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");
			printf("执行人脸添加操作,请将脸部对准摄像头区，距离 0.4m ~ 1m \r\n");
			
			if(0 == fr_reg_user("lee"))
			{
				printf("注册用户成功!\r\n");
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				printf("注册用户失败!\r\n");
			}
			
			/* 进入掉电模式 */
			fr_power_down();
			
			Delay_ms(1000);
		}
		
		/* 刷人脸 */
		if(1 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");
			printf("执行人脸匹配操作,请将脸部对准摄像头区，距离 0.4m ~ 1m \r\n");
			
			if(0 == fr_match(user_buf))
			{
				printf("人脸匹配成功!\r\n");
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				printf("人脸匹配失败!\r\n");
			}	
			
			/* 进入掉电模式 */
			fr_power_down();
			
			Delay_ms(1000);			
		}
		
		/* 获取用户总数 */
		if(2 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");
	
			user_total = fr_get_user_total();
			
			if(user_total < 0 )
			{
				printf("获取已注册的用户总数失败!\r\n");
			}
			else
			{
				printf("获取已注册的用户总数:%d\r\n",user_total);
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			
			/* 进入掉电模式 */
			fr_power_down();
			
			Delay_ms(1000);
		}	

		
		/* 删除所有用户 */
		if(3 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");

			if(0 == fr_del_user_all())
			{
				printf("删除所有用户成功!\r\n");
				//pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				printf("删除所有用户失败!\r\n");
			}
			
			/* 进入掉电模式 */
			fr_power_down();
			
			Delay_ms(1000);	
		}
}

//MFRC522数据区

u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];                                            //保存卡的类型
u8  card_numberbuf[5];                                          //保存卡的序列号
u8  card_key0Abuf[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
u8  card_writebuf[16]={15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};  //保存写入的卡内容
u8  card_readbuf[16]={0};

uint8_t i=0;
uint8_t rfid_status = 0;    //保存卡状态
uint8_t card_size = 0;      //保存卡容量

//mfrc522测试函数
void rfid_test(void)
{
	//2.寻卡
    rfid_status = MFRC522_Request(0x52, card_pydebuf);
    
    //3.检测卡片
    if(rfid_status==0)
    {
        printf("RFID Check OK\n");
        
        rfid_status=MFRC522_Anticoll(card_numberbuf);			            //防撞处理
        card_size=MFRC522_SelectTag(card_numberbuf);	                    //选卡
        rfid_status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
        rfid_status=MFRC522_Write(4,card_writebuf);				            //写卡（写卡要小心，特别是各区的块3）
        rfid_status=MFRC522_Read(4,card_readbuf);					        //读卡
        
        //卡的类型
        printf("Card Type is %02x%02x \n",card_pydebuf[0],card_pydebuf[1]); //0400 Mifare_One(S50)
        
        //卡的序列  最后一字节为卡的校验码
        printf("Card ID Is %02x%02x%02x%02x \n",card_numberbuf[0],card_numberbuf[1],card_numberbuf[2],card_numberbuf[3]);
        
        //卡的容量
        printf("Card Size is %d Kbits\n",card_size);
        
        //读卡内容
        printf("Card Data Is ");
        for(i=0;i<16;i++)
        {
            printf("%d ",card_readbuf[i]);
        }
        printf("\n");
        
        Delay_ms(500);
    }
}

/**************** 项目功能函数 ****************/

uint8_t pos = 0;
uint8_t mode = 'A';
uint8_t level = 0;

//密码位数（0-5）
uint8_t PWSend[7]={0};	//最后一位为\0
uint8_t PWRecv[7]={0};  //最后一位为\0
int8_t num = 0;	

//主功能菜单函数
void menu_func(void)
{
	pos = get_key_board();
	
	if(level == 0) //主菜单
	{
		if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D')
		{
			mode = pos;	//模式选择
			
			OLED_ON();	//OLED工作模式
			Main_Menu(mode);
		}
		
		
		if(pos == '*')	OLED_OFF();	//OLED休眠模式
		if(pos == '#')	level = 1;	//确认进入二级菜单
	}
	else
	{
		switch(mode)	//不同模式的子菜单
		{
			case 'A':
				password_func();	//键盘开锁
				
			break;
			
			case 'B':
				rfid_func();		//ID卡开锁
			
			break;
			
			case 'C':	
				finger_func();		//指纹开锁
			
			break;
			
			case 'D':	
				face_func();		//人脸开锁
			
			break;
			
			default:break;
		}
		
	}
	
	
}

//管理员功能（蓝牙）
void root_func(void)
{
	password_change();
	lock_force();
	finger_add();
	finger_clear();
	face_add();
	face_clear();
	rfid_add();
}

//密码功能菜单
void password_func(void)
{
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			memset(PWRecv, 0 ,sizeof(PWRecv)); //密码数组的长度
			memset(PWSend, 0 ,sizeof(PWSend)); //密码数组的长度;
			
			level = 0;	//返回主菜单
			num = 0;	//计数清零
			
			Main_Menu(mode);
			return;
		}
		else if(pos == '#')	num--;
		else if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D'|| pos == 'N') continue;
		else	PWSend[num++] = pos;
		
		//num在0-5之间
		if(num < 0)	num = 0;
		if(num > 5)	
		{
			memset(PWRecv, 0, sizeof(PWRecv));
			AT24C02_ReadData(0x00, PWRecv, sizeof(PWRecv));
			Delay_ms(10);
			
			if(!strcmp((char *)PWRecv, (char *)PWSend))	
			{
				printf("password unlock\n");
				RTC_TimeShow();RTC_DataShow(); //显示时间日期
				unlock_func();
			}
			else	lock_func();
			
			num = 0;
		}
		
		Password_Menu(num);
	}
}

//修改密码
void password_change(void)
{	
	if(Change_Password)
	{
		//新密码界面
		Password_Menu(0);
		OLED_ShowString(16, 2, (u8*)"new password", 16);
		
		while(1)
		{
			pos = get_key_board();
			
			if(pos == '*')	
			{
				memset(PWSend, 0, sizeof(PWSend)); //密码数组的长度
				memset(PWSend, 0, sizeof(PWSend)); //密码数组的长度
				
				level = 0;	//返回主菜单
				num = 0;	//计数清零
				
				Main_Menu(mode);
				Change_Password = 0;
				return;
			}
			else if(pos == '#')	num--;
			else if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D'|| pos == 'N') continue;
			else	PWSend[num++] = pos;
			
			//num在0-5之间
			if(num < 0)	num = 0;
			if(num > 5)	
			{
				num = 0;
				break;
			}
			
			Password_Menu(num);
			OLED_ShowString(16, 2, (u8*)"new password", 16);
		}
		
		OLED_Clear();
		OLED_ShowString(56, 2, (u8*)"OK", 16);
		Delay_ms(1000);
		
		
		//确认密码界面
		Password_Menu(num);
		OLED_ShowString(32, 2, (u8*)"confirm", 16);
		
		while(1)
		{
			pos = get_key_board();
			
			if(pos == '*')	
			{
				memset(PWRecv, 0 ,sizeof(PWRecv)); //密码数组的长度
				memset(PWSend, 0 ,sizeof(PWSend)); //密码数组的长度;
				
				level = 0;	//返回主菜单
				num = 0;	//计数清零
				
				Main_Menu(mode);
				Change_Password = 0;
				return;
			}
			else if(pos == '#')	num--;
			else if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D'|| pos == 'N') continue;
			else	PWRecv[num++] = pos;
			
			//num在0-5之间
			if(num < 0)	num = 0;
			if(num > 5)	
			{
				num = 0;
				break;
			}
			
			Password_Menu(num);
			OLED_ShowString(32, 2, (u8*)"confirm", 16);
			
		}
		
		if(!strcmp((char *)PWRecv, (char *)PWSend))	
		{
			AT24C02_WriteData(0x00, PWRecv, sizeof(PWRecv));	//保存新密码
			
			OLED_Clear();
			OLED_ShowString(56, 2, (u8*)"OK", 16);
			Delay_ms(1000);		
			
			memset(PWRecv, 0 ,sizeof(PWRecv)); //密码数组的长度
			memset(PWSend, 0 ,sizeof(PWSend)); //密码数组的长度;
			
			level = 0;	//返回主菜单
			num = 0;	//计数清零
			
			Main_Menu(mode);
			
			Change_Password = 0;	
		}
		else
		{
			OLED_Clear();
			OLED_ShowString(48, 2, (u8*)"ERRER", 16);
			Delay_ms(1000);
		}
	}
}

//蓝牙强制开锁
void lock_force(void)
{
	if(Lock_Flag)
	{
		printf("Bluetooth unlock\n");
		RTC_TimeShow();RTC_DataShow(); //显示时间日期
		unlock_func();
		
		Main_Menu(mode);
		Lock_Flag = 0;
	}
}

//指纹功能菜单
void finger_func(void)
{
	OLED_Finger();
	
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			level = 0;	//返回主菜单	
			Main_Menu(mode);
			return;
		}
		
		if(g_fpm_touch_event)
		{
		
			fpm_ctrl_led(FPM_LED_BLUE);
			Delay_ms(100);
			
			/* 参数为0xFFFF进行1:N匹配 */
			id = 0xFFFF;
			
			fpm_error_code = fpm_idenify_auto(&id);
			Delay_ms(100);
			
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("finger unlock\n");
				RTC_TimeShow();RTC_DataShow(); //显示时间日期
				unlock_func();
				OLED_Finger();
			}
			else
			{
				fpm_ctrl_led(FPM_LED_RED);
				lock_func();
				OLED_Finger();
			}
			
			Delay_ms(100);		
		
			g_fpm_touch_event = 0;
			fpm_sleep();
			
			Delay_ms(1000);
			
		}
		
	}
}

//添加指纹
void finger_add(void)
{	
	if(Add_Finger)
	{
		fpm_ctrl_led(FPM_LED_BLUE);
		Delay_ms(100);
		
		fpm_error_code = fpm_id_total(&id_total);
		Delay_ms(100);
	
		if(fpm_error_code == 0)
		{
			//printf("获取指纹总数：%04d\r\n", id_total);
			
			/* 添加指纹*/
			fpm_error_code = fpm_enroll_auto(id_total+1);
	
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				OLED_Clear();
				OLED_ShowString(56, 2, (u8*)"OK", 16);
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				fpm_ctrl_led(FPM_LED_RED);
				
				OLED_Clear();
				OLED_ShowString(48, 2, (u8*)"ERRER", 16);
			}
			
			Delay_ms(100);
			
			g_fpm_touch_event = 0;
			
			fpm_sleep();
			
			Delay_ms(1000);
			
			Add_Finger = 0;
			Main_Menu(mode);
		}
	}
}

//清除指纹
void finger_clear(void)
{
	if(Clean_Finger)
	{
		fpm_ctrl_led(FPM_LED_BLUE);
		Delay_ms(100);
		
		fpm_error_code = fpm_empty();
		Delay_ms(100);
		
		if(fpm_error_code == 0)
		{
			fpm_ctrl_led(FPM_LED_GREEN);
			
			OLED_Clear();
			OLED_ShowString(56, 2, (u8*)"OK", 16);
			pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
		}
		else
		{
			fpm_ctrl_led(FPM_LED_RED);
			
			OLED_Clear();
			OLED_ShowString(48, 2, (u8*)"ERRER", 16);
		}			
		Delay_ms(100);		
		
		fpm_sleep();
	
		Delay_ms(1000);
		
		Clean_Finger = 0;
		Main_Menu(mode);
	}
}

//人脸功能菜单
void face_func(void)
{
	OLED_Face();
	
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			level = 0;	//返回主菜单	
			Main_Menu(mode);
			
			/* 进入掉电模式 */
			fr_power_down();
			return;
		}
		
		if(0 == fr_match(user_buf))
		{
			printf("face unlock\n");
			RTC_TimeShow();RTC_DataShow(); //显示时间日期
			unlock_func();
			OLED_Face();
		}
	}
}

//添加人脸
void face_add(void)
{	
	if(Add_Face)
	{
		OLED_Face();
		
		user_error_code = fr_reg_user("lee");
		/* 进入掉电模式 */
		fr_power_down();
		
		Delay_ms(1000);
		
		if(0 == user_error_code)
		{
			printf("%d",user_error_code);
			OLED_Clear();
			OLED_ShowString(56, 2, (u8*)"OK", 16);
			pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			
		}
		else
		{
			printf("%d",user_error_code);
			OLED_Clear();
			OLED_ShowString(48, 2, (u8*)"ERRER", 16);
		}
		Delay_ms(1000);
		
		Add_Face = 0;
		Main_Menu(mode);
	}
}

//清除人脸
void face_clear(void)
{
	if(Clean_Face)
	{
		if(0 == fr_del_user_all())
		{
			OLED_Clear();
			OLED_ShowString(56, 2, (u8*)"OK", 16);
			pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
		}
		else
		{
			OLED_Clear();
			OLED_ShowString(48, 2, (u8*)"ERRER", 16);
		}
		
		/* 进入掉电模式 */
		fr_power_down();
		
		Delay_ms(1000);
		
		Clean_Face = 0;
		Main_Menu(mode);
	}
}

//ID卡功能菜单
void rfid_func(void)
{
	OLED_Lock();
	
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			level = 0;	//返回主菜单	
			Main_Menu(mode);
			return;
		}
		
		//寻卡
		rfid_status = MFRC522_Request(0x52, card_pydebuf);
		
		//检测卡片
		if(rfid_status==0)
		{	
			rfid_status=MFRC522_Anticoll(card_numberbuf);			            //防撞处理
			card_size=MFRC522_SelectTag(card_numberbuf);	                    //选卡
			rfid_status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
			
			memset(card_readbuf, 0, sizeof(card_readbuf));
			rfid_status=MFRC522_Read(4,card_readbuf);					        //读卡
			
//			//读卡内容（测试）
//			printf("Card Data Is ");
//			for(i=0;i<16;i++)
//			{
//				printf("%d ",card_readbuf[i]);
//			}
//			printf("\n");
			
			if(!strcmp((char *)card_readbuf, (char *)card_writebuf))	//判断是否为注册卡
			{
				printf("rfid unlock\n");
				RTC_TimeShow();RTC_DataShow(); //显示时间日期
				unlock_func();
				OLED_Lock();
			}
			else	lock_func();
			
		}

	}
}

//添加ID卡
void rfid_add(void)
{
	if(Change_Rfid)
	{
		OLED_Clear();
		OLED_ShowString(32, 2, (u8*)"new rfid", 16);
		
		while(1)
		{
			//寻卡
			rfid_status = MFRC522_Request(0x52, card_pydebuf);
		
			//检测卡片
			if(rfid_status==0)
			{
				//printf("RFID Check OK\n");				
				rfid_status=MFRC522_Anticoll(card_numberbuf);			            //防撞处理
				card_size=MFRC522_SelectTag(card_numberbuf);	                    //选卡
				rfid_status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
				rfid_status=MFRC522_Write(4,card_writebuf);				            //写卡（写卡要小心，特别是各区的块3）
				
				OLED_Clear();
				OLED_ShowString(56, 2, (u8*)"OK", 16);
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
				
				Delay_ms(500);
				Change_Rfid = 0;
				Main_Menu(mode);
				return;
			}
		}
	}
}

//解锁函数
void unlock_func(void)
{
	OLED_Unlock();
	Rotation(180);
	pwm_beep_set(TONE_1_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_4_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_7_M, 15); Delay_ms(500);
	pwm_beep_set(0, 0);
	
	Delay_ms(3000);
	
	Rotation(0);
	pwm_beep_set(TONE_7_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_4_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_1_M, 15); Delay_ms(500);
	pwm_beep_set(0, 0);

}

//关锁函数
void lock_func(void)
{
	OLED_Lock();
	pwm_beep_set(TONE_5_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_4_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_3_M, 15); Delay_ms(500);
	pwm_beep_set(0, 0);
	Delay_ms(1000);

}
