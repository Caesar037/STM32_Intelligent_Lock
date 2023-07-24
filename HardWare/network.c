#include "network.h"

void Get_Network_Weather(void)
{
	char buf[256]={0};
	
	sprintf(buf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n", MODE, NETTIME_SERVERIP, NETTIME_PORTNUM);
	printf("%s\n", buf);

	//连接到目标TCP服务器
	Usart_SendString(USARTx_WiFi, buf);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	
	//检测连接状态
	Usart_SendString(USARTx_WiFi, "AT+CIPSTATUS\r\n");
	DELAY_MS(1000);
	
	//进入透传模式
	Usart_SendString(USARTx_WiFi, "AT+CIPMODE=1\r\n");  
	DELAY_MS(1000);

	//开启发送状态
	Usart_SendString(USARTx_WiFi, "AT+CIPSEND\r\n");
	DELAY_MS(1000);
	DELAY_MS(1000);
	
	//清空数组,用以接收数据
	Rx3Counter = 0;
	memset( (void *)Rx3Buffer, 0, sizeof(Rx3Buffer));
	
	//向目标提出获取时间信息请求
	Usart_SendString(USARTx_WiFi, "GET /v3/weather/now.json?key=S2rXjc-39fUM9SQEF&location=beijing&language=zh-Hans&unit=c HTTP/1.1\r\nHost:api.seniverse.com\r\n\r\n");

	//等待数据接收完毕
	DELAY_MS(1000);
	DELAY_MS(1000);	
	
	//打印目标网站反馈的信息，数据通过串口3接收的
	printf("\r\nRxBuffer:%s\r\n", Rx3Buffer);
	
	//退出透传模式
	Usart_SendString(USARTx_WiFi, "+++");
	DELAY_MS(1000);
	
	//断开服务器
	Usart_SendString(USARTx_WiFi, "AT+CIPCLOSE\r\n");
	DELAY_MS(1000);
	
}
