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

/************  ��������  *************/
//���Ժ���
void pwm_test(void);
void beep_test(void);
void rfid_test(void);
void reset_test(void);
void fr1002_test(void);
void w25q128_test(void);
void at24c02_test(void);
void keyboard_test(void);
void fpm_test(void);


//��Ҫ����
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
/************	��ʼ��	*************/
	
	//�����жϷ���Ϊ��2����ռ���ȼ��ȼ���2^2��4�����ȼ�����Ӧ���ȼ��ȼ���4���ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//��ʼ��(���ã��������
	key_board_init();
	
	//��ʼ��(���ã�systick���ж���ʱ��
    SysTick_Init();
	
	//����Systickʱ��ԴΪ168MHZ����������ʱ��
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
	//��ʼ��(���ã�IWDG(�������Ź�)
	iwdg_init(IWDG_Prescaler_128, 250);	//��ʱΪ1��ࣨ32KHZ~60KHZ��
	
	//��ʼ��(���ã�TIM���ⲿ��ʱ����
	tim7_init();
	
	//��ʼ��(���ã�PWM�������ȵ��ƣ�
	pwm_SG90_init();
	//pwm_beep_init();
	
	//��ʼ��(���ã�UART������ͨ�ţ�
	UART1_Config(9600);
	
	//��ʼ��fpm����ָ�ƴ�����������2��TIM7��EXIT6��
	fpm_power_init();
	fpm_init();
	
	
	//��ʼ��fr1002����ʶ�𣬸�3D����ʶ��ģ��������֣�����3��TIM7��
	while(fr_init())
	{
		Delay_ms(1000);
		printf("3D����ʶ��ģ�������� ...\r\n");		
	}
	
	//��ʼ��(���ã�RTC��ʵʱʱ�ӣ�
	rtc_init();
	
	//��ʼ��(���ã�OLED��IIC���ߣ�
	OLED_Power_Init();	//����ģ���Դ
	OLED_Config();	//��������IC��SSD1306
	
	//��ʼ�������ã�AT24��ģ��IIC���ߣ�
	AT24C02_Init();
	
	//��ʼ�������ã�W25Qxx��Ӳ��SPI���ߣ�
	//W25Q128_Init();
	
	//��ʼ�������ã�MFRC522��ģ��SPI���ߣ�
	MFRC522_Power_Init();
	MFRC522_Initializtion();
	
	//��ʼ��(���ã�ESP8266��WIFI���ӣ�
	//Wifi_STA_Config();	//�����ȵ�
	//Get_Network_Weather();	//���ӷ������˿ڣ�TCP�������ݱ�
	
	
/************	�� ��	**************/

	//reset_test();	//��λ��⺯��
	
	printf("��ʼ�����������...\r\n");	//��Linuxϵͳ�ϻس����з���Ϊ�� \n ; ��Windowsϵͳ�ϻس����з���Ϊ�� \r\n 
	OLED_OFF();
	
	while(1)
	{
		
		menu_func(); //�û��˵�����
		root_func(); //��������Ա����
		
	}
}

/************  ���Ժ���	 *************/

//���Ź�����
void reset_test(void)
{
	//ͨ����ȡRCC_CSR���λ29 IWDGRSTF ���ж�����ֵ�Ƿ�Ϊ1�����жϸ�λ��־��״̬��ȷ����λԭ�򣩣����Ϊ1����ʾ�����������ڿ��Ź��������˸�λ
	if( SET == RCC_GetFlagStatus(RCC_FLAG_IWDGRST) )
	{
		//�����־λ
		RCC_ClearFlag();
		
		//����ص�������¼�����Ϣ
		printf("�������Ź���λ...\r\n");		
	}
	else if( SET == RCC_GetFlagStatus(RCC_FLAG_WWDGRST) )
	{
		//�����־λ
		RCC_ClearFlag();
		
		//����ص�������¼�����Ϣ
		printf("���ڿ��Ź���λ...\r\n");		
	}
	else if( SET == RCC_GetFlagStatus(RCC_FLAG_PORRST) )
	{
		//����ص�������¼�����Ϣ
		printf("��Դ��λ...\r\n");	
	}
	else if( SET == RCC_GetFlagStatus(RCC_FLAG_PINRST) )
	{
		//����ص�������¼�����Ϣ
		printf("�ⲿ��λ...\r\n");	
	}
	else
	{
		//����ص�������¼�����Ϣ
		printf("������λ...\r\n");	
	}
	
}

uint32_t pwm_num = 0; //pwmǿ�ȣ�ȡֵ��0-100��
uint8_t pwm_flag = 0; //pwm��־���ж������߻��ǽ���

//pwm���Ժ���
void pwm_test(void)
{
	Delay_ms(50);
	if(pwm_num == 0 || pwm_num == 100)
		pwm_flag = !pwm_flag;	//pwm��־���ж��ǵ������ǵݼ�
	
	if(pwm_flag)
	{
		pwm_num += 10;
	}else
	{
		pwm_num -= 10;
	}
	
	//������PF9---TIM14_CH1
	//TIM_SetCompare1(TIM14, pwm_num);	//ȡֵ��Χ��0-TIM_Period����0-100
	
	//������PF8---TIM13_CH1��ռ�ձ�->��ȱ仯��Ƶ��->������
	//TIM_SetCompare1(TIM13, pwm_num);	//ȡֵ��Χ��0-TIM_Period����0-100
	
	//SG90���PC8---TIM3_CH3��angle��Χ0-180��
	//TIM_SetCompare3(TIM3, angle(pwm_num));	//ȡֵ��Χ��0-TIM_Period����0-100
}

uint8_t send[16]; //д���������
uint8_t recv[16]; //��ȡ��������

//at24c02���Ժ���
void at24c02_test(void)
{
	memset(send, 0, sizeof(send));
	strcpy((char *)send,"hello at24c02\r\n");
	
	AT24C02_WriteData(0x00, send, sizeof(send));
	Delay_ms(1000);
	
	memset(recv, 0, sizeof(recv));
	AT24C02_ReadData(0x00, recv, sizeof(recv));
	Delay_ms(1000);
	
//	printf("AT24C02 send count��%d\r\n",AT24C02_WRITE_PAGE(0x00, send));
//	Delay_ms(10);
//	printf("AT24C02 recv count��%d\r\n",AT24C02_READ_PAGE(0x00, recv));
//	Delay_ms(10);
	
	printf("AT24C02: %s",recv);
}

//w25q128���Ժ���
void w25q128_test(void)
{
	//��ȡW25Q128�豸ID
	printf("W25Q128 ID: %#hx\r\n", W25Q128_ReadID());
	Delay_ms(1000);
	
	memset(send, 0, sizeof(send));
	strcpy((char *)send,"hello w25q128\r\n");
	
	W25Q128_SectorErase(0x000000);	//д֮ǰ�������
	W25Q128_PageProgram(0x000000, send, sizeof(send));
	Delay_ms(1000);
	
	memset(recv, 0, sizeof(recv));
	W25Q128_ReadData(0x000000, recv, sizeof(recv));
	Delay_ms(1000);
	
	printf("W25Q128: %s",recv);
}

int32_t distance = 0; //sr04������������

//sr04���������Ժ���
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

//���������ֲ��Ժ���
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
	/* �ر� */
	pwm_beep_set(0, 0); Delay_ms(2000);
}

//�������
uint32_t key_sta=0;
uint8_t key_old=0;
uint8_t key_cur=0;

//������̲��Ժ���
void keyboard_test(void)
{
	
	/* ʹ��״̬��˼��õ�������״̬ */
	switch(key_sta)
	{
		case 0://��ȡ���µİ���
		{
			
			key_cur = get_key_board();	

			if(key_cur != 'N')
			{
				key_old = key_cur;
				key_sta=1;
			}
				
		}break;
		
		
		case 1://ȷ�ϰ��µİ���������������
		{
			
			key_cur = get_key_board();	
				
			if((key_cur != 'N') && (key_cur == key_old))
			{
				printf("KEY %c Down\r\n",key_cur);
				key_sta=2;
			}				
		
		}break;
	
		case 2://��ȡ�ͷŵİ���
		{
			
			key_cur = get_key_board();
			
			if(key_cur == 'N')
			{
				printf("KEY %c Up\r\n",key_old);
				
				//�������
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

//fpm����ָ�ƴ�����
uint8_t fpm_error_code=0;
uint16_t id=0;
uint16_t id_total=0;
uint32_t run_mode=0;

//fpm����ָ�ƴ��������ԣ���Ҫ�õ�������̣�
void fpm_test(void)
{
	/* Ĭ����ˢָ����֤ģʽ */
	if(run_mode == 0)
	{
		if(g_fpm_touch_event)
		{
		
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			printf("ִ��ˢָ�Ʋ���,�뽫��ָ�ŵ�ָ��ģ�鴥����Ӧ��\r\n");
			
			/* ����Ϊ0xFFFF����1:Nƥ�� */
			id = 0xFFFF;
			
			fpm_error_code = fpm_idenify_auto(&id);
			
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("�Զ���ָ֤��%04d�ɹ�!\r\n", id);
				
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
	
	/* �����л���ǰ����ִ��ģʽ */
	if(0 == key_value())
	{
		run_mode ^= 1;
		
		/* ��run_modeΪ1����Ϊ����ָ��ģ��ģʽ */
		if(run_mode == 1)
			PFout(9) = 0;	
		/* ��run_modeΪ0����Ϊˢָ����֤ģʽ */
		else
			PFout(9) = 1;
		
		
		Delay_ms(500);
		
	}		
	
	/* ���밴������ָ��ģ��ģʽ */
	if(run_mode == 1)
	{
	
		/* ���ָ�� */
		if(1 == key_value())
		{
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			printf("ִ�����ָ�Ʋ���,�뽫��ָ�ŵ�ָ��ģ�鴥����Ӧ��\r\n");
			
			fpm_error_code = fpm_id_total(&id_total);
	
			if(fpm_error_code == 0)
			{
				printf("��ȡָ��������%04d\r\n", id_total);
				
				/* ���ָ��*/
				fpm_error_code = fpm_enroll_auto(id_total+1);
	
				if(fpm_error_code == 0)
				{
					fpm_ctrl_led(FPM_LED_GREEN);					
					
					printf("�Զ�ע��ָ�Ƴɹ�\r\n");
					
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
		
		
		/* ��ȡ�û����� */
		if(2 == key_value())
		{
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			
			fpm_error_code = fpm_id_total(&id_total);
	
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("��ȡָ��������%04d\r\n", id_total);
				
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
	
		
		/* ���ָ�� */
		if(3 == key_value())
		{
			fpm_ctrl_led(FPM_LED_BLUE);
			
			printf("\r\n\r\n=====================================\r\n");
			
			fpm_error_code = fpm_empty();
			
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("���ָ�Ƴɹ�\r\n");
				
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

//fr1002����ʶ��ģ��
uint8_t user_buf[64]={0};
int32_t user_total;
uint8_t user_error_code=0;

void fr1002_test(void)
{		
		/* ������� */
		if(0 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");
			printf("ִ��������Ӳ���,�뽫������׼����ͷ�������� 0.4m ~ 1m \r\n");
			
			if(0 == fr_reg_user("lee"))
			{
				printf("ע���û��ɹ�!\r\n");
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				printf("ע���û�ʧ��!\r\n");
			}
			
			/* �������ģʽ */
			fr_power_down();
			
			Delay_ms(1000);
		}
		
		/* ˢ���� */
		if(1 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");
			printf("ִ������ƥ�����,�뽫������׼����ͷ�������� 0.4m ~ 1m \r\n");
			
			if(0 == fr_match(user_buf))
			{
				printf("����ƥ��ɹ�!\r\n");
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				printf("����ƥ��ʧ��!\r\n");
			}	
			
			/* �������ģʽ */
			fr_power_down();
			
			Delay_ms(1000);			
		}
		
		/* ��ȡ�û����� */
		if(2 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");
	
			user_total = fr_get_user_total();
			
			if(user_total < 0 )
			{
				printf("��ȡ��ע����û�����ʧ��!\r\n");
			}
			else
			{
				printf("��ȡ��ע����û�����:%d\r\n",user_total);
				pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			
			/* �������ģʽ */
			fr_power_down();
			
			Delay_ms(1000);
		}	

		
		/* ɾ�������û� */
		if(3 == key_value())
		{
			printf("\r\n\r\n=====================================\r\n");

			if(0 == fr_del_user_all())
			{
				printf("ɾ�������û��ɹ�!\r\n");
				//pwm_beep_set(TONE_1_M, 15); Delay_ms(50); pwm_beep_set(0, 0);
			}
			else
			{
				printf("ɾ�������û�ʧ��!\r\n");
			}
			
			/* �������ģʽ */
			fr_power_down();
			
			Delay_ms(1000);	
		}
}

//MFRC522������

u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];                                            //���濨������
u8  card_numberbuf[5];                                          //���濨�����к�
u8  card_key0Abuf[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
u8  card_writebuf[16]={15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};  //����д��Ŀ�����
u8  card_readbuf[16]={0};

uint8_t i=0;
uint8_t rfid_status = 0;    //���濨״̬
uint8_t card_size = 0;      //���濨����

//mfrc522���Ժ���
void rfid_test(void)
{
	//2.Ѱ��
    rfid_status = MFRC522_Request(0x52, card_pydebuf);
    
    //3.��⿨Ƭ
    if(rfid_status==0)
    {
        printf("RFID Check OK\n");
        
        rfid_status=MFRC522_Anticoll(card_numberbuf);			            //��ײ����
        card_size=MFRC522_SelectTag(card_numberbuf);	                    //ѡ��
        rfid_status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
        rfid_status=MFRC522_Write(4,card_writebuf);				            //д����д��ҪС�ģ��ر��Ǹ����Ŀ�3��
        rfid_status=MFRC522_Read(4,card_readbuf);					        //����
        
        //��������
        printf("Card Type is %02x%02x \n",card_pydebuf[0],card_pydebuf[1]); //0400 Mifare_One(S50)
        
        //��������  ���һ�ֽ�Ϊ����У����
        printf("Card ID Is %02x%02x%02x%02x \n",card_numberbuf[0],card_numberbuf[1],card_numberbuf[2],card_numberbuf[3]);
        
        //��������
        printf("Card Size is %d Kbits\n",card_size);
        
        //��������
        printf("Card Data Is ");
        for(i=0;i<16;i++)
        {
            printf("%d ",card_readbuf[i]);
        }
        printf("\n");
        
        Delay_ms(500);
    }
}

/**************** ��Ŀ���ܺ��� ****************/

uint8_t pos = 0;
uint8_t mode = 'A';
uint8_t level = 0;

//����λ����0-5��
uint8_t PWSend[7]={0};	//���һλΪ\0
uint8_t PWRecv[7]={0};  //���һλΪ\0
int8_t num = 0;	

//�����ܲ˵�����
void menu_func(void)
{
	pos = get_key_board();
	
	if(level == 0) //���˵�
	{
		if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D')
		{
			mode = pos;	//ģʽѡ��
			
			OLED_ON();	//OLED����ģʽ
			Main_Menu(mode);
		}
		
		
		if(pos == '*')	OLED_OFF();	//OLED����ģʽ
		if(pos == '#')	level = 1;	//ȷ�Ͻ�������˵�
	}
	else
	{
		switch(mode)	//��ͬģʽ���Ӳ˵�
		{
			case 'A':
				password_func();	//���̿���
				
			break;
			
			case 'B':
				rfid_func();		//ID������
			
			break;
			
			case 'C':	
				finger_func();		//ָ�ƿ���
			
			break;
			
			case 'D':	
				face_func();		//��������
			
			break;
			
			default:break;
		}
		
	}
	
	
}

//����Ա���ܣ�������
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

//���빦�ܲ˵�
void password_func(void)
{
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			memset(PWRecv, 0 ,sizeof(PWRecv)); //��������ĳ���
			memset(PWSend, 0 ,sizeof(PWSend)); //��������ĳ���;
			
			level = 0;	//�������˵�
			num = 0;	//��������
			
			Main_Menu(mode);
			return;
		}
		else if(pos == '#')	num--;
		else if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D'|| pos == 'N') continue;
		else	PWSend[num++] = pos;
		
		//num��0-5֮��
		if(num < 0)	num = 0;
		if(num > 5)	
		{
			memset(PWRecv, 0, sizeof(PWRecv));
			AT24C02_ReadData(0x00, PWRecv, sizeof(PWRecv));
			Delay_ms(10);
			
			if(!strcmp((char *)PWRecv, (char *)PWSend))	
			{
				printf("password unlock\n");
				RTC_TimeShow();RTC_DataShow(); //��ʾʱ������
				unlock_func();
			}
			else	lock_func();
			
			num = 0;
		}
		
		Password_Menu(num);
	}
}

//�޸�����
void password_change(void)
{	
	if(Change_Password)
	{
		//���������
		Password_Menu(0);
		OLED_ShowString(16, 2, (u8*)"new password", 16);
		
		while(1)
		{
			pos = get_key_board();
			
			if(pos == '*')	
			{
				memset(PWSend, 0, sizeof(PWSend)); //��������ĳ���
				memset(PWSend, 0, sizeof(PWSend)); //��������ĳ���
				
				level = 0;	//�������˵�
				num = 0;	//��������
				
				Main_Menu(mode);
				Change_Password = 0;
				return;
			}
			else if(pos == '#')	num--;
			else if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D'|| pos == 'N') continue;
			else	PWSend[num++] = pos;
			
			//num��0-5֮��
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
		
		
		//ȷ���������
		Password_Menu(num);
		OLED_ShowString(32, 2, (u8*)"confirm", 16);
		
		while(1)
		{
			pos = get_key_board();
			
			if(pos == '*')	
			{
				memset(PWRecv, 0 ,sizeof(PWRecv)); //��������ĳ���
				memset(PWSend, 0 ,sizeof(PWSend)); //��������ĳ���;
				
				level = 0;	//�������˵�
				num = 0;	//��������
				
				Main_Menu(mode);
				Change_Password = 0;
				return;
			}
			else if(pos == '#')	num--;
			else if(pos == 'A' || pos == 'B' || pos == 'C' || pos == 'D'|| pos == 'N') continue;
			else	PWRecv[num++] = pos;
			
			//num��0-5֮��
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
			AT24C02_WriteData(0x00, PWRecv, sizeof(PWRecv));	//����������
			
			OLED_Clear();
			OLED_ShowString(56, 2, (u8*)"OK", 16);
			Delay_ms(1000);		
			
			memset(PWRecv, 0 ,sizeof(PWRecv)); //��������ĳ���
			memset(PWSend, 0 ,sizeof(PWSend)); //��������ĳ���;
			
			level = 0;	//�������˵�
			num = 0;	//��������
			
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

//����ǿ�ƿ���
void lock_force(void)
{
	if(Lock_Flag)
	{
		printf("Bluetooth unlock\n");
		RTC_TimeShow();RTC_DataShow(); //��ʾʱ������
		unlock_func();
		
		Main_Menu(mode);
		Lock_Flag = 0;
	}
}

//ָ�ƹ��ܲ˵�
void finger_func(void)
{
	OLED_Finger();
	
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			level = 0;	//�������˵�	
			Main_Menu(mode);
			return;
		}
		
		if(g_fpm_touch_event)
		{
		
			fpm_ctrl_led(FPM_LED_BLUE);
			Delay_ms(100);
			
			/* ����Ϊ0xFFFF����1:Nƥ�� */
			id = 0xFFFF;
			
			fpm_error_code = fpm_idenify_auto(&id);
			Delay_ms(100);
			
			if(fpm_error_code == 0)
			{
				fpm_ctrl_led(FPM_LED_GREEN);
				
				printf("finger unlock\n");
				RTC_TimeShow();RTC_DataShow(); //��ʾʱ������
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

//���ָ��
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
			//printf("��ȡָ��������%04d\r\n", id_total);
			
			/* ���ָ��*/
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

//���ָ��
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

//�������ܲ˵�
void face_func(void)
{
	OLED_Face();
	
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			level = 0;	//�������˵�	
			Main_Menu(mode);
			
			/* �������ģʽ */
			fr_power_down();
			return;
		}
		
		if(0 == fr_match(user_buf))
		{
			printf("face unlock\n");
			RTC_TimeShow();RTC_DataShow(); //��ʾʱ������
			unlock_func();
			OLED_Face();
		}
	}
}

//�������
void face_add(void)
{	
	if(Add_Face)
	{
		OLED_Face();
		
		user_error_code = fr_reg_user("lee");
		/* �������ģʽ */
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

//�������
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
		
		/* �������ģʽ */
		fr_power_down();
		
		Delay_ms(1000);
		
		Clean_Face = 0;
		Main_Menu(mode);
	}
}

//ID�����ܲ˵�
void rfid_func(void)
{
	OLED_Lock();
	
	while(1)
	{
		pos = get_key_board();
		
		if(pos == '*')	
		{
			level = 0;	//�������˵�	
			Main_Menu(mode);
			return;
		}
		
		//Ѱ��
		rfid_status = MFRC522_Request(0x52, card_pydebuf);
		
		//��⿨Ƭ
		if(rfid_status==0)
		{	
			rfid_status=MFRC522_Anticoll(card_numberbuf);			            //��ײ����
			card_size=MFRC522_SelectTag(card_numberbuf);	                    //ѡ��
			rfid_status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
			
			memset(card_readbuf, 0, sizeof(card_readbuf));
			rfid_status=MFRC522_Read(4,card_readbuf);					        //����
			
//			//�������ݣ����ԣ�
//			printf("Card Data Is ");
//			for(i=0;i<16;i++)
//			{
//				printf("%d ",card_readbuf[i]);
//			}
//			printf("\n");
			
			if(!strcmp((char *)card_readbuf, (char *)card_writebuf))	//�ж��Ƿ�Ϊע�Ῠ
			{
				printf("rfid unlock\n");
				RTC_TimeShow();RTC_DataShow(); //��ʾʱ������
				unlock_func();
				OLED_Lock();
			}
			else	lock_func();
			
		}

	}
}

//���ID��
void rfid_add(void)
{
	if(Change_Rfid)
	{
		OLED_Clear();
		OLED_ShowString(32, 2, (u8*)"new rfid", 16);
		
		while(1)
		{
			//Ѱ��
			rfid_status = MFRC522_Request(0x52, card_pydebuf);
		
			//��⿨Ƭ
			if(rfid_status==0)
			{
				//printf("RFID Check OK\n");				
				rfid_status=MFRC522_Anticoll(card_numberbuf);			            //��ײ����
				card_size=MFRC522_SelectTag(card_numberbuf);	                    //ѡ��
				rfid_status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
				rfid_status=MFRC522_Write(4,card_writebuf);				            //д����д��ҪС�ģ��ر��Ǹ����Ŀ�3��
				
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

//��������
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

//��������
void lock_func(void)
{
	OLED_Lock();
	pwm_beep_set(TONE_5_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_4_M, 15); Delay_ms(500);
	pwm_beep_set(TONE_3_M, 15); Delay_ms(500);
	pwm_beep_set(0, 0);
	Delay_ms(1000);

}
