#include "network.h"

void Get_Network_Weather(void)
{
	char buf[256]={0};
	
	sprintf(buf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n", MODE, NETTIME_SERVERIP, NETTIME_PORTNUM);
	printf("%s\n", buf);

	//���ӵ�Ŀ��TCP������
	Usart_SendString(USARTx_WiFi, buf);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	
	//�������״̬
	Usart_SendString(USARTx_WiFi, "AT+CIPSTATUS\r\n");
	DELAY_MS(1000);
	
	//����͸��ģʽ
	Usart_SendString(USARTx_WiFi, "AT+CIPMODE=1\r\n");  
	DELAY_MS(1000);

	//��������״̬
	Usart_SendString(USARTx_WiFi, "AT+CIPSEND\r\n");
	DELAY_MS(1000);
	DELAY_MS(1000);
	
	//�������,���Խ�������
	Rx3Counter = 0;
	memset( (void *)Rx3Buffer, 0, sizeof(Rx3Buffer));
	
	//��Ŀ�������ȡʱ����Ϣ����
	Usart_SendString(USARTx_WiFi, "GET /v3/weather/now.json?key=S2rXjc-39fUM9SQEF&location=beijing&language=zh-Hans&unit=c HTTP/1.1\r\nHost:api.seniverse.com\r\n\r\n");

	//�ȴ����ݽ������
	DELAY_MS(1000);
	DELAY_MS(1000);	
	
	//��ӡĿ����վ��������Ϣ������ͨ������3���յ�
	printf("\r\nRxBuffer:%s\r\n", Rx3Buffer);
	
	//�˳�͸��ģʽ
	Usart_SendString(USARTx_WiFi, "+++");
	DELAY_MS(1000);
	
	//�Ͽ�������
	Usart_SendString(USARTx_WiFi, "AT+CIPCLOSE\r\n");
	DELAY_MS(1000);
	
}
