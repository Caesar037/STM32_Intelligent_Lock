#include "esp8266.h"

uint8_t  g_esp8266_tx_buf[512];
volatile uint8_t  g_esp8266_rx_buf[512];
volatile uint32_t g_esp8266_rx_cnt=0;
volatile uint32_t g_esp8266_transparent_transmission_sta=0;


void esp8266_send_bytes(uint8_t *buf, uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(WIFI_USART, *p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(WIFI_USART, USART_FLAG_TXE) == RESET);
	}
}

void esp8266_send_str(char *buf)
{	
	char *p = buf;
	
	while(*p != '\0')
	{
		USART_SendData(WIFI_USART, *p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(WIFI_USART,USART_FLAG_TXE) == RESET);
	}
}

void esp8266_init(void)
{
	UART3_Config(115200);
}


void esp8266_send_at(char *str)
{
	//��ս��ջ�����
	memset((void *)g_esp8266_rx_buf, 0, sizeof g_esp8266_rx_buf);
	
	//��ս��ռ���ֵ
	g_esp8266_rx_cnt = 0;	
	
	//����3��������
	esp8266_send_str(str);
}

/* ���ҽ������ݰ��е��ַ��� */
int32_t esp8266_find_str_in_rx_packet(char *str, uint32_t timeout)
{
	char *dest = str;
	char *src  = (char *)&g_esp8266_rx_buf;
	//�ȴ����ڽ�����ϻ�ʱ�˳�
	while((strstr(src,dest)==NULL) && timeout)
	{		
		Delay_ms(1);
		timeout--;
	}
#if EN_DEBUG_ESP8266	
	printf("[find str]%s ,timeout=%d\r\n",(const char *)g_esp8266_rx_buf,timeout);
#endif
	if(timeout) return 0; 
	            
	return -1; 
}


/* �Լ���� */
int32_t  esp8266_self_test(void)
{
	esp8266_send_at("AT\r\n");
	
	return esp8266_find_str_in_rx_packet("OK", 1000);
}

/**
 * ���ܣ������ȵ�
 * ������
 *         ssid:�ȵ���
 *         pwd:�ȵ�����
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. WIFI�������벻��ȷ
 *         2. ·���������豸̫��,δ�ܸ�ESP8266����IP
 */
int32_t esp8266_connect_ap(char* ssid,char* pswd)
{
#if 0
	//������ʹ������sprintf��ռ�ù����ջ
	char buf[128]={0};
	
	sprintf(buf,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);

#endif
    //����ΪSTATIONģʽ	
	esp8266_send_at("AT+CWMODE_CUR=1\r\n"); 
	
	if(esp8266_find_str_in_rx_packet("OK", 1000)) return -1;

	//����Ŀ��AP
	//sprintf(buf,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);
	esp8266_send_at("AT+CWJAP_CUR="); 
	esp8266_send_at("\"");esp8266_send_at(ssid);esp8266_send_at("\"");	
	esp8266_send_at(",");	
	esp8266_send_at("\"");esp8266_send_at(pswd);esp8266_send_at("\"");	
	esp8266_send_at("\r\n");
	if(esp8266_find_str_in_rx_packet("OK",5000))
		if(esp8266_find_str_in_rx_packet("CONNECT",5000)) return -2;

	return 0;
}



/* �˳�͸��ģʽ */
int32_t esp8266_exit_transparent_transmission (void)
{
	//�˳�͸��ģʽ��������һ��ATָ��Ҫ���1��
	Delay_ms ( 1000 ); 

	esp8266_send_at ("+++");
	
	//�˳�͸��ģʽ��������һ��ATָ��Ҫ���1��
	Delay_ms ( 1000 ); 

	return 0;
}

/* ����͸��ģʽ */
int32_t  esp8266_entry_transparent_transmission(void)
{
	//����͸��ģʽ
	esp8266_send_at("AT+CIPMODE=1\r\n");  
	if(esp8266_find_str_in_rx_packet("OK",5000)) return -1;
	
	Delay_ms(2000);
	//��������״̬
	esp8266_send_at("AT+CIPSEND\r\n");
	if(esp8266_find_str_in_rx_packet(">",5000))  return -2;

	return 0;
}


/**
 * ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
 * ������
 *         mode:Э������ "TCP","UDP"
 *         ip:Ŀ�������IP
 *         port:Ŀ���Ƿ������˿ں�
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. Զ�̷�����IP�Ͷ˿ں�����
 *         2. δ����AP
 *         3. �������˽�ֹ���(һ�㲻�ᷢ��)
 */
int32_t esp8266_connect_server(char* mode,char* ip,uint16_t port)
{

#if 0	
	//ʹ��MQTT���ݵ�ip��ַ������������ʹ�����·�����������ջ���
	//AT+CIPSTART="TCP","a10tC4OAAPc.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883�����ַ���ռ���ڴ������
	
	char buf[128]={0};
	
	//���ӷ�����
	sprintf((char*)buf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
	
	esp8266_send_at(buf);
#else
	
	char buf[16]={0};
	esp8266_send_at("AT+CIPSTART=");
	esp8266_send_at("\"");	esp8266_send_at(mode);	esp8266_send_at("\"");
	esp8266_send_at(",");
	esp8266_send_at("\"");	esp8266_send_at(ip);	esp8266_send_at("\"");	
	esp8266_send_at(",");
	sprintf(buf,"%d",port);
	esp8266_send_at(buf);	
	esp8266_send_at("\r\n");
	
#endif
	
	if(esp8266_find_str_in_rx_packet("CONNECT",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000)) return -1;
	
	return 0;
}

/* �Ͽ������� */
int32_t esp8266_disconnect_server(void)
{
	esp8266_send_at("AT+CIPCLOSE\r\n");
		
	if(esp8266_find_str_in_rx_packet("CLOSED",5000))
		if(esp8266_find_str_in_rx_packet("OK",5000)) return -1;
	
	return 0;	
}


/* ʹ�ܶ����� */
int32_t esp8266_enable_multiple_id(uint32_t b)
{

	char buf[32]={0};
	
	sprintf(buf,"AT+CIPMUX=%d\r\n", b);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000)) return -1;
	
	return 0;
}

/* ���������� */
int32_t esp8266_create_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=1,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000)) return -1;
	
	return 0;
}

/* �رշ����� */
int32_t esp8266_close_server(uint16_t port)
{
	char buf[32]={0};
	
	sprintf(buf,"AT+CIPSERVER=0,%d\r\n", port);
	esp8266_send_at(buf);
	
	if(esp8266_find_str_in_rx_packet("OK",5000))  return -1;
	
	return 0;
}

/* ���Դ򿪻�ر� */
int32_t esp8266_enable_echo(uint32_t b)
{
	if(b)
		esp8266_send_at("ATE1\r\n"); 
	else
		esp8266_send_at("ATE0\r\n"); 
	
	if(esp8266_find_str_in_rx_packet("OK",5000))  return -1;

	return 0;
}

/* ��λ */
int32_t esp8266_reset(void)
{
	esp8266_send_at("AT+RST\r\n");
	
	if(esp8266_find_str_in_rx_packet("OK",10000)) return -1;

	return 0;
}

