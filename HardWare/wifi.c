#include "wifi.h"

/*
 *以下为简单框架设置WiFi模块_Esp8266为AP模式进行工作，即设置AP局域网
 */
 
void Wifi_AP_Config(void)
{
	//Usart_SendString(USARTx_WiFi," AT+CWMODE=3\r\n");//设置为 SoftAP+Station 共存模式
	Usart_SendString(USARTx_WiFi,"AT+CWMODE=2\r\n");
	DELAY_MS(500);
	
	Usart_SendString(USARTx_WiFi,"AT+RST\r\n");//重启
	DELAY_MS(1500);
	
	Usart_SendString(USARTx_WiFi,"AT+CIPAP=\"192.168.1.1\"\r\n");//设置IP:192.168.1.1
	DELAY_MS(1500);

	Usart_SendString(USARTx_WiFi,"AT+CWSAP=\"ESP8266\",\"1234567890\",1,0\r\n");//设置WiFi名称是ESP8266，密码1234567890，最多5个人同时连接，连接时无需密码；
	DELAY_MS(500);
	
	Usart_SendString(USARTx_WiFi,"AT+CIPMUX=1\r\n");//启动多连接
	DELAY_MS(500);
	
	Usart_SendString(USARTx_WiFi,"AT+CIPSERVER=1,8080\r\n");//设置端口8080
	DELAY_MS(500);
}

//配置WiFi以STA联网模式工作
void Wifi_STA_Config(void)
{
	char buf[256]={0};
	
	Usart_SendString(USARTx_WiFi, "+++");//退出透传模式
	DELAY_MS(1000);

	Usart_SendString(USARTx_WiFi, "AT+RST\r\n");//重启
	DELAY_MS(1000);

	Usart_SendString(USARTx_WiFi, "AT\r\n");//测试A启动
	DELAY_MS(1000);
	
	Usart_SendString(USARTx_WiFi, "ATE0\r\n");//关闭回显
	DELAY_MS(1000);
	
	Usart_SendString(USARTx_WiFi, "AT+CWMODE_CUR=1\r\n");//设置为 Station 模式	
	DELAY_MS(100);
	
	Usart_SendString(USARTx_WiFi, "AT+CIPMUX=0\r\n");//单路连接模式
	DELAY_MS(100);
	
	//连接热点AP，务必等待该条指令返回连接成功后，再发送下面的指令
	sprintf(buf, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
	Usart_SendString(USARTx_WiFi, buf);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
}
