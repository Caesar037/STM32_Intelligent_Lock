#include "wifi.h"

/*
 *����Ϊ�򵥿������WiFiģ��_Esp8266ΪAPģʽ���й�����������AP������
 */
 
void Wifi_AP_Config(void)
{
	//Usart_SendString(USARTx_WiFi," AT+CWMODE=3\r\n");//����Ϊ SoftAP+Station ����ģʽ
	Usart_SendString(USARTx_WiFi,"AT+CWMODE=2\r\n");
	DELAY_MS(500);
	
	Usart_SendString(USARTx_WiFi,"AT+RST\r\n");//����
	DELAY_MS(1500);
	
	Usart_SendString(USARTx_WiFi,"AT+CIPAP=\"192.168.1.1\"\r\n");//����IP:192.168.1.1
	DELAY_MS(1500);

	Usart_SendString(USARTx_WiFi,"AT+CWSAP=\"ESP8266\",\"1234567890\",1,0\r\n");//����WiFi������ESP8266������1234567890�����5����ͬʱ���ӣ�����ʱ�������룻
	DELAY_MS(500);
	
	Usart_SendString(USARTx_WiFi,"AT+CIPMUX=1\r\n");//����������
	DELAY_MS(500);
	
	Usart_SendString(USARTx_WiFi,"AT+CIPSERVER=1,8080\r\n");//���ö˿�8080
	DELAY_MS(500);
}

//����WiFi��STA����ģʽ����
void Wifi_STA_Config(void)
{
	char buf[256]={0};
	
	Usart_SendString(USARTx_WiFi, "+++");//�˳�͸��ģʽ
	DELAY_MS(1000);

	Usart_SendString(USARTx_WiFi, "AT+RST\r\n");//����
	DELAY_MS(1000);

	Usart_SendString(USARTx_WiFi, "AT\r\n");//����A����
	DELAY_MS(1000);
	
	Usart_SendString(USARTx_WiFi, "ATE0\r\n");//�رջ���
	DELAY_MS(1000);
	
	Usart_SendString(USARTx_WiFi, "AT+CWMODE_CUR=1\r\n");//����Ϊ Station ģʽ	
	DELAY_MS(100);
	
	Usart_SendString(USARTx_WiFi, "AT+CIPMUX=0\r\n");//��·����ģʽ
	DELAY_MS(100);
	
	//�����ȵ�AP����صȴ�����ָ������ӳɹ����ٷ��������ָ��
	sprintf(buf, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
	Usart_SendString(USARTx_WiFi, buf);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
}
