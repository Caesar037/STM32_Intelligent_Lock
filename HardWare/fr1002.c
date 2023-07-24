#include "fr1002.h"

static uint8_t bcc_check(uint8_t *buf,uint32_t len)
{
	uint8_t s=0;
	uint8_t i=0;
	uint8_t *p = buf;
	
	for(i=0; i<len; i++)
		s = s^p[i];

	return s;
}

static void fr_printf_recv(void)
{
	
#if FR_DEBUG_EN
	uint32_t i;
	
	printf("fr recv buf:");
	
	for(i=0;i<Rx3Counter;i++)
	{
		printf("%02X ",Rx3Buffer[i]);
	
	}
	
	printf("\r\n");
	
#else
	
	(void)0;
	
#endif
}


void fr_send(uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(USART3,*p++);
		
		while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));
	}
	
	Rx3End = 0;
	Rx3Counter = 0;
}


int32_t fr_entry_standby(void)
{
	uint8_t buf_tx[6]={0};
	
    uint32_t timeout=4000;	
	
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	buf_tx[2]=0x23;
	buf_tx[3]=0x00;
	buf_tx[4]=0x00;
	buf_tx[5]=0x23;	


	fr_send(buf_tx,6);
	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();	
		
	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0) && (Rx3Buffer[5]==0x23) && (Rx3Buffer[6]==0))
		{
			return 0;
		}	
	}
		
	return 0;
		
}


int32_t fr_reg_admin(const char *name)
{
	uint8_t buf_tx[64]={0};
	
    uint32_t timeout=10000;	
	
	/* ���ݰ�����ͷ */
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	
	/* ���ݰ������� */
	buf_tx[2]=0x26;
	
	/* ���ݰ������ݳ��� */	
	buf_tx[3]=0x00;
	buf_tx[4]=0x28;
	
	/* ���ݰ�������Ϊ����Ա��ɫ */	
	buf_tx[5]=0x01;

	/* ���ݰ����û��� */
	memcpy(&buf_tx[6],name,strlen(name));

	/* ���ݰ���ע�᷽�򣬵���ע�� */
	buf_tx[38]=0x01;
	
	/* ���ݰ���ע������ */
	buf_tx[39]=0x01;

	/* ���ݰ����������ظ�¼�� */
	buf_tx[40]=0x00;
	
	/* ���ݰ�����ʱʱ�� */
	buf_tx[41]=0x0A;	
	
	/* ���ݰ�������λ 0x00 0x00 0x00 */
	buf_tx[42]=0x00;
	buf_tx[43]=0x00;
	buf_tx[44]=0x00;
	
	/* ���ݰ���У��λ */
	buf_tx[45]=bcc_check(&buf_tx[2],43);
	
	fr_send(buf_tx,46);
	
	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();
		

	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0) && (Rx3Buffer[5]==0x26) && (Rx3Buffer[6]==0))
		{
			return 0;
		}	
	}

	return -1;
}

int32_t fr_reg_user(const char *name)
{
	uint8_t buf_tx[64]={0};
	
    uint32_t timeout=10000;	
	
	/* ���ݰ�����ͷ */
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	
	/* ���ݰ������� */
	buf_tx[2]=0x26;
	
	/* ���ݰ������ݳ��� */	
	buf_tx[3]=0x00;
	buf_tx[4]=0x28;
	
	/* ���ݰ�������Ϊ��ͨ�û���ɫ */	
	buf_tx[5]=0x00;

	/* ���ݰ����û��� */
	memcpy(&buf_tx[6],name,strlen(name));

	/* ���ݰ���ע�᷽�򣬵���ע�� */
	buf_tx[38]=0x01;
	
	/* ���ݰ���ע������ */
	buf_tx[39]=0x01;

	/* ���ݰ����������ظ�¼�� */
	buf_tx[40]=0x00;
	
	/* ���ݰ�����ʱʱ�� */
	buf_tx[41]=0x0A;	
	
	/* ���ݰ�������λ 0x00 0x00 0x00 */
	buf_tx[42]=0x00;
	buf_tx[43]=0x00;
	buf_tx[44]=0x00;
	
	/* ���ݰ���У��λ */
	buf_tx[45]=bcc_check(&buf_tx[2],43);
	
	fr_send(buf_tx,46);
	
	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();		

	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0x00)&& (Rx3Buffer[5]==0x26) && (Rx3Buffer[6]==0x00))
		{
			return 0;
		}	
	}

	return -1;
}

int32_t fr_del_user_all(void)
{
	uint8_t buf_tx[8]={0};
	
    uint32_t timeout=4000;	
	
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	
	buf_tx[2]=0x21;
	
	buf_tx[3]=0x00;
	buf_tx[4]=0x00;
	
	buf_tx[5]=0x21;	

	fr_send(buf_tx,6);
	
	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();		
		
	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0) && (Rx3Buffer[5]==0x21) && (Rx3Buffer[6]==0x00))
		{
			
			return 0;
		}	
	}

	return -1;		
}

int32_t fr_match(uint8_t *buf)
{
	uint8_t buf_tx[8]={0};
	
    uint32_t timeout=4000;	
	
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	
	buf_tx[2]=0x12;
	
	buf_tx[3]=0x00;
	buf_tx[4]=0x02;
	
	buf_tx[5]=0x0A;	
	buf_tx[6]=0x0A;	

	/* ���ݰ���У��λ */
	buf_tx[7]=bcc_check(&buf_tx[2],5);
	
	fr_send(buf_tx,8);
	
	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();		
		
	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0) && (Rx3Buffer[6]==0))
		{
			memcpy(buf,(void *)&Rx3Buffer[7],36);
			return 0;
		}	
	}

	return -1;		
}

int32_t fr_state_get(void)
{
	uint8_t buf_tx[6]={0};
	
    uint32_t timeout=4000;	
		
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	buf_tx[2]=0x11;
	buf_tx[3]=0x00;
	buf_tx[4]=0x00;
	buf_tx[5]=0x11;	

	fr_send(buf_tx,6);
	
	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();		

	
	return 0;

}


int32_t fr_get_user_total(void)
{
	int32_t user_total=0;
    uint32_t timeout=4000;	
	
	uint8_t buf_tx[6]={0};

		
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	
	buf_tx[2]=0x24;
	
	buf_tx[3]=0x00;
	buf_tx[4]=0x00;
	
	buf_tx[5]=0x24;	


	fr_send(buf_tx,6);
	

	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();	
	
	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0) && (Rx3Buffer[5]==0x24) && (Rx3Buffer[6]==0))
		{
			user_total = Rx3Buffer[7];
			
			return user_total;
		}	
	}

	return -1;	

}

int32_t fr_power_down(void)
{
	uint8_t buf_tx[6]={0};
	
    uint32_t timeout=4000;	
		
	buf_tx[0]=0xEF;
	buf_tx[1]=0xAA;
	buf_tx[2]=0xED;
	buf_tx[3]=0x00;
	buf_tx[4]=0x00;
	buf_tx[5]=0xED;	


	fr_send(buf_tx,6);

	
	while(!Rx3End && (--timeout))
	{
		Delay_ms(1);
	}
	
	fr_printf_recv();	
	
	if((Rx3Buffer[0]==0xEF) && (Rx3Buffer[1]==0xAA))
	{
		if((Rx3Buffer[2]==0) && (Rx3Buffer[5]==0xED) && (Rx3Buffer[6]==0x00))
		{
			return 0;
		}	
	}	
	
	
	return 0;
}

int32_t fr_init(void)
{
	/* ��ʱ�� ��ʼ�� */
	//tim7_init();
	/* ����3 ��ʼ�� */
	UART3_Config(115200);
	
	Delay_ms(500);
	
	return fr_power_down();
}

