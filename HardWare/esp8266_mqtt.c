#include "esp8266_mqtt.h"


//���ӳɹ���������Ӧ 20 02 00 00
//�ͻ��������Ͽ����� e0 00
const uint8_t g_packet_connect_ack[4] = {0x20,0x02,0x00,0x00};
const uint8_t g_packet_disconnect[2]  = {0xe0,0x00};
const uint8_t g_packet_heart[2]       = {0xc0,0x00};
const uint8_t g_packet_heart_reply[2] = {0xc0,0x00};
const uint8_t g_packet_sub_ack[2]     = {0x90,0x03};

char  g_mqtt_msg[526];

uint32_t g_mqtt_tx_len;

//MQTT��������
void mqtt_send_bytes(uint8_t *buf,uint32_t len)
{
    esp8266_send_bytes(buf,len);
}

//����������
void mqtt_send_heart(void)
{
    mqtt_send_bytes((uint8_t *)g_packet_heart,sizeof(g_packet_heart));
}

//MQTT�������Ͽ�
void mqtt_disconnect()
{
    mqtt_send_bytes((uint8_t *)g_packet_disconnect,sizeof(g_packet_disconnect));
}

//MQTT��ʼ��
void mqtt_init(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen)
{
    memset(g_esp8266_tx_buf,0,sizeof(g_esp8266_tx_buf)); //��շ��ͻ���
    memset((void *)g_esp8266_rx_buf,0,sizeof(g_esp8266_rx_buf)); //��ս��ջ���

    //�������������Ͽ�
    mqtt_disconnect();
    Delay_ms(100);
	
    mqtt_disconnect();
    Delay_ms(100);
}

//MQTT���ӷ������Ĵ������
int32_t mqtt_connect(char *client_id,char *user_name,char *password)
{
    uint32_t client_id_len = strlen(client_id);
    uint32_t user_name_len = strlen(user_name);
    uint32_t password_len = strlen(password);
    uint32_t data_len;
    uint32_t cnt=2;
    uint32_t wait=0;
    g_mqtt_tx_len=0;
	
    //�ɱ䱨ͷ+Payload  ÿ���ֶΰ��������ֽڵĳ��ȱ�ʶ
    data_len = 10 + (client_id_len+2) + (user_name_len+2) + (password_len+2);

    //�̶���ͷ
    //���Ʊ�������
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x10;		//MQTT Message Type CONNECT
    //ʣ�೤��(�������̶�ͷ��)
    do
    {
        uint8_t encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    //�ɱ䱨ͷ
    //Э����
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0;        		// Protocol Name Length MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 4;        		// Protocol Name Length LSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'M';        	// ASCII Code for M
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'Q';        	// ASCII Code for Q
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'T';        	// ASCII Code for T
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'T';        	// ASCII Code for T
    //Э�鼶��
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 4;        		// MQTT Protocol version = 4
    //���ӱ�־
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0xc2;        	// conn flags
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0;        		// Keep-alive Time Length MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 60;        	// Keep-alive Time Length LSB  60S������

    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(client_id_len);// Client ID length MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(client_id_len);// Client ID length LSB
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],client_id,client_id_len);
    g_mqtt_tx_len += client_id_len;

    if(user_name_len > 0)
    {
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(user_name_len);		//user_name length MSB
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(user_name_len);    	//user_name length LSB
        memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],user_name,user_name_len);
        g_mqtt_tx_len += user_name_len;
    }

    if(password_len > 0)
    {
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(password_len);		//password length MSB
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(password_len);    	//password length LSB
        memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],password,password_len);
        g_mqtt_tx_len += password_len;
    }

	
    while(cnt--)
    {
        memset((void *)g_esp8266_rx_buf,0,sizeof(g_esp8266_rx_buf));
		g_esp8266_rx_cnt=0;
		
        mqtt_send_bytes(g_esp8266_tx_buf,g_mqtt_tx_len);
		
        wait=3000;//�ȴ�3sʱ��
		
        while(wait--)
        {
			Delay_ms(1);
            //CONNECT
            if((g_esp8266_rx_buf[0]==g_packet_connect_ack[0]) && (g_esp8266_rx_buf[1]==g_packet_connect_ack[1])) //���ӳɹ�
            {
                return 0;//���ӳɹ�
            }
			
            
        }
    }
	
    return -1;
}

//MQTT����/ȡ���������ݴ������
//topic       ����
//qos         ��Ϣ�ȼ�
//whether     ����/ȡ�����������
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether)
{
    
	
    uint32_t cnt=2;
    uint32_t wait=0;
	
    uint32_t topiclen = strlen(topic);

    uint32_t data_len = 2 + (topiclen+2) + (whether?1:0);//�ɱ䱨ͷ�ĳ��ȣ�2�ֽڣ�������Ч�غɵĳ���
	
	g_mqtt_tx_len=0;
	
    //�̶���ͷ
    //���Ʊ�������
    if(whether) 
		g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x82; //��Ϣ���ͺͱ�־����
    else	
		g_esp8266_tx_buf[g_mqtt_tx_len++] = 0xA2; //ȡ������

    //ʣ�೤��
    do
    {
        uint8_t encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    //�ɱ䱨ͷ
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0;				//��Ϣ��ʶ�� MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x01;           //��Ϣ��ʶ�� LSB
	
    //��Ч�غ�
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topiclen);//���ⳤ�� MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topiclen);//���ⳤ�� LSB
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topiclen);
	
    g_mqtt_tx_len += topiclen;

    if(whether)
    {
        g_esp8266_tx_buf[g_mqtt_tx_len++] = qos;//QoS����
    }


    while(cnt--)
    {
		g_esp8266_rx_cnt=0;
        memset((void *)g_esp8266_rx_buf,0,sizeof(g_esp8266_rx_buf));
        mqtt_send_bytes(g_esp8266_tx_buf,g_mqtt_tx_len);
		
        wait=3000;//�ȴ�3sʱ��
        while(wait--)
        {
			Delay_ms(1);
            if((g_esp8266_rx_buf[0]==g_packet_sub_ack[0]) && (g_esp8266_rx_buf[1]==g_packet_sub_ack[1])) //���ĳɹ�
            {
                return 0;//���ĳɹ�
            }
            
        }
    }
	
    if(cnt) 
		return 0;	//���ĳɹ�
	
    return -1;
}

//MQTT�������ݴ������
//topic   ����
//message ��Ϣ
//qos     ��Ϣ�ȼ�
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos)
{
static 
	uint16_t id=0;	
    uint32_t topicLength = strlen(topic);
    uint32_t messageLength = strlen(message);

    uint32_t data_len;
	uint8_t encodedByte;
    g_mqtt_tx_len=0;
    //��Ч�غɵĳ����������㣺�ù̶���ͷ�е�ʣ�೤���ֶε�ֵ��ȥ�ɱ䱨ͷ�ĳ���
    //QOSΪ0ʱû�б�ʶ��
    //���ݳ���             ������   ���ı�ʶ��   ��Ч�غ�
    if(qos)	data_len = (2+topicLength) + 2 + messageLength;
    else	data_len = (2+topicLength) + messageLength;

    //�̶���ͷ
    //���Ʊ�������
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x30;    // MQTT Message Type PUBLISH

    //ʣ�೤��
    do
    {
        encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topicLength);//���ⳤ��MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topicLength);//���ⳤ��LSB
	
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topicLength);//��������
	
    g_mqtt_tx_len += topicLength;

    //���ı�ʶ��
    if(qos)
    {
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(id);
        g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(id);
        id++;
    }
	
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],message,messageLength);
	
    g_mqtt_tx_len += messageLength;

    mqtt_send_bytes(g_esp8266_tx_buf,g_mqtt_tx_len);
	
    return g_mqtt_tx_len;
}

//�豸״̬�ϱ�
void mqtt_report_devices_status(void)
{
//    uint8_t led_1_sta = GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_9) ? 0:1;

    //�ѿ�������ص�״̬��������sprintf������ŵ�һ��������ٰѸ���������MQTTЭ��������Ϣ����
    //sprintf(str,"a=%d",a);
    //��Ҫ���ġ�temperature���͡�CurrentHumidity��Ϊ��Ӧ��ƽ̨�豸��Ϣ��
    sprintf(g_mqtt_msg,
            "{\"method\":\"thing.service.property.set\",\"id\":\"0001\",\"params\":{\
			  \"switch_lock\":%d,\
			  },\"version\":\"1.0.0\"}",
             0);

    //�ϱ���Ϣ��ƽ̨������
    mqtt_publish_data(MQTT_PUBLISH_TOPIC,g_mqtt_msg,0);
}

int32_t esp8266_mqtt_init(void)
{
	int32_t rt;
	
	//esp8266��ʼ��
	esp8266_init();


	//�˳�͸��ģʽ����������ATָ��
	rt=esp8266_exit_transparent_transmission();
	
	if(rt)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	Delay_ms(2000);
	
	//��λģ��
	rt=esp8266_reset();
	if(rt)
	{
		printf("esp8266_reset fail\r\n");
		return -2;
	}
	Delay_ms(2000);	
	
	//�رջ���
	rt=esp8266_enable_echo(0);
	if(rt)
	{
		printf("esp8266_enable_echo(0) fail\r\n");
		return -3;
	}	
	printf("esp8266_enable_echo(0)success\r\n");
	Delay_ms(2000);	
		
	//�����ȵ�
	rt = esp8266_connect_ap(WIFI_SSID,WIFI_PASSWORD);
	if(rt)
	{
		printf("esp8266_connect_ap fail\r\n");
		return -4;
	}	
	printf("esp8266_connect_ap success\r\n");
	Delay_ms(2000);
	
	rt =esp8266_connect_server("TCP",MQTT_BROKERADDRESS,1883);
	if(rt)
	{
		printf("esp8266_connect_server fail\r\n");
		return -5;
	}	
	printf("esp8266_connect_server success\r\n");
	Delay_ms(2000);
	
	//����͸��ģʽ
	rt =esp8266_entry_transparent_transmission();
	if(rt)
	{
		printf("esp8266_entry_transparent_transmission fail\r\n");
		return -6;
	}	
	printf("esp8266_entry_transparent_transmission success\r\n");
	Delay_ms(2000);
	
	if(mqtt_connect(MQTT_CLIENTID, MQTT_USARNAME, MQTT_PASSWD))
	{
		printf("mqtt_connect fail\r\n");
		return -7;	
	
	}
	printf("mqtt_connect success\r\n");
	Delay_ms(2000);		
	
	if(mqtt_subscribe_topic(MQTT_SUBSCRIBE_TOPIC,0,1))
	{
		printf("mqtt_subscribe_topic fail\r\n");
		return -8;
	}	
	
	printf("mqtt_subscribe_topic success\r\n");
	
	return 0;
		
}
