#include "esp8266_mqtt.h"


//连接成功服务器回应 20 02 00 00
//客户端主动断开连接 e0 00
const uint8_t g_packet_connect_ack[4] = {0x20,0x02,0x00,0x00};
const uint8_t g_packet_disconnect[2]  = {0xe0,0x00};
const uint8_t g_packet_heart[2]       = {0xc0,0x00};
const uint8_t g_packet_heart_reply[2] = {0xc0,0x00};
const uint8_t g_packet_sub_ack[2]     = {0x90,0x03};

char  g_mqtt_msg[526];

uint32_t g_mqtt_tx_len;

//MQTT发送数据
void mqtt_send_bytes(uint8_t *buf,uint32_t len)
{
    esp8266_send_bytes(buf,len);
}

//发送心跳包
void mqtt_send_heart(void)
{
    mqtt_send_bytes((uint8_t *)g_packet_heart,sizeof(g_packet_heart));
}

//MQTT无条件断开
void mqtt_disconnect()
{
    mqtt_send_bytes((uint8_t *)g_packet_disconnect,sizeof(g_packet_disconnect));
}

//MQTT初始化
void mqtt_init(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen)
{
    memset(g_esp8266_tx_buf,0,sizeof(g_esp8266_tx_buf)); //清空发送缓冲
    memset((void *)g_esp8266_rx_buf,0,sizeof(g_esp8266_rx_buf)); //清空接收缓冲

    //无条件先主动断开
    mqtt_disconnect();
    Delay_ms(100);
	
    mqtt_disconnect();
    Delay_ms(100);
}

//MQTT连接服务器的打包函数
int32_t mqtt_connect(char *client_id,char *user_name,char *password)
{
    uint32_t client_id_len = strlen(client_id);
    uint32_t user_name_len = strlen(user_name);
    uint32_t password_len = strlen(password);
    uint32_t data_len;
    uint32_t cnt=2;
    uint32_t wait=0;
    g_mqtt_tx_len=0;
	
    //可变报头+Payload  每个字段包含两个字节的长度标识
    data_len = 10 + (client_id_len+2) + (user_name_len+2) + (password_len+2);

    //固定报头
    //控制报文类型
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x10;		//MQTT Message Type CONNECT
    //剩余长度(不包括固定头部)
    do
    {
        uint8_t encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    //可变报头
    //协议名
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0;        		// Protocol Name Length MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 4;        		// Protocol Name Length LSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'M';        	// ASCII Code for M
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'Q';        	// ASCII Code for Q
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'T';        	// ASCII Code for T
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 'T';        	// ASCII Code for T
    //协议级别
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 4;        		// MQTT Protocol version = 4
    //连接标志
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0xc2;        	// conn flags
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0;        		// Keep-alive Time Length MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 60;        	// Keep-alive Time Length LSB  60S心跳包

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
		
        wait=3000;//等待3s时间
		
        while(wait--)
        {
			Delay_ms(1);
            //CONNECT
            if((g_esp8266_rx_buf[0]==g_packet_connect_ack[0]) && (g_esp8266_rx_buf[1]==g_packet_connect_ack[1])) //连接成功
            {
                return 0;//连接成功
            }
			
            
        }
    }
	
    return -1;
}

//MQTT订阅/取消订阅数据打包函数
//topic       主题
//qos         消息等级
//whether     订阅/取消订阅请求包
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether)
{
    
	
    uint32_t cnt=2;
    uint32_t wait=0;
	
    uint32_t topiclen = strlen(topic);

    uint32_t data_len = 2 + (topiclen+2) + (whether?1:0);//可变报头的长度（2字节）加上有效载荷的长度
	
	g_mqtt_tx_len=0;
	
    //固定报头
    //控制报文类型
    if(whether) 
		g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x82; //消息类型和标志订阅
    else	
		g_esp8266_tx_buf[g_mqtt_tx_len++] = 0xA2; //取消订阅

    //剩余长度
    do
    {
        uint8_t encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    //可变报头
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0;				//消息标识符 MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x01;           //消息标识符 LSB
	
    //有效载荷
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topiclen);//主题长度 MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topiclen);//主题长度 LSB
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topiclen);
	
    g_mqtt_tx_len += topiclen;

    if(whether)
    {
        g_esp8266_tx_buf[g_mqtt_tx_len++] = qos;//QoS级别
    }


    while(cnt--)
    {
		g_esp8266_rx_cnt=0;
        memset((void *)g_esp8266_rx_buf,0,sizeof(g_esp8266_rx_buf));
        mqtt_send_bytes(g_esp8266_tx_buf,g_mqtt_tx_len);
		
        wait=3000;//等待3s时间
        while(wait--)
        {
			Delay_ms(1);
            if((g_esp8266_rx_buf[0]==g_packet_sub_ack[0]) && (g_esp8266_rx_buf[1]==g_packet_sub_ack[1])) //订阅成功
            {
                return 0;//订阅成功
            }
            
        }
    }
	
    if(cnt) 
		return 0;	//订阅成功
	
    return -1;
}

//MQTT发布数据打包函数
//topic   主题
//message 消息
//qos     消息等级
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos)
{
static 
	uint16_t id=0;	
    uint32_t topicLength = strlen(topic);
    uint32_t messageLength = strlen(message);

    uint32_t data_len;
	uint8_t encodedByte;
    g_mqtt_tx_len=0;
    //有效载荷的长度这样计算：用固定报头中的剩余长度字段的值减去可变报头的长度
    //QOS为0时没有标识符
    //数据长度             主题名   报文标识符   有效载荷
    if(qos)	data_len = (2+topicLength) + 2 + messageLength;
    else	data_len = (2+topicLength) + messageLength;

    //固定报头
    //控制报文类型
    g_esp8266_tx_buf[g_mqtt_tx_len++] = 0x30;    // MQTT Message Type PUBLISH

    //剩余长度
    do
    {
        encodedByte = data_len % 128;
        data_len = data_len / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( data_len > 0 )
            encodedByte = encodedByte | 128;
        g_esp8266_tx_buf[g_mqtt_tx_len++] = encodedByte;
    } while ( data_len > 0 );

    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE1(topicLength);//主题长度MSB
    g_esp8266_tx_buf[g_mqtt_tx_len++] = BYTE0(topicLength);//主题长度LSB
	
    memcpy(&g_esp8266_tx_buf[g_mqtt_tx_len],topic,topicLength);//拷贝主题
	
    g_mqtt_tx_len += topicLength;

    //报文标识符
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

//设备状态上报
void mqtt_report_devices_status(void)
{
//    uint8_t led_1_sta = GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_9) ? 0:1;

    //把开发板相关的状态变量利用sprintf函数存放到一个数组里，再把该数组利用MQTT协议打包成消息报文
    //sprintf(str,"a=%d",a);
    //需要更改“temperature”和“CurrentHumidity”为对应的平台设备信息；
    sprintf(g_mqtt_msg,
            "{\"method\":\"thing.service.property.set\",\"id\":\"0001\",\"params\":{\
			  \"switch_lock\":%d,\
			  },\"version\":\"1.0.0\"}",
             0);

    //上报信息到平台服务器
    mqtt_publish_data(MQTT_PUBLISH_TOPIC,g_mqtt_msg,0);
}

int32_t esp8266_mqtt_init(void)
{
	int32_t rt;
	
	//esp8266初始化
	esp8266_init();


	//退出透传模式，才能输入AT指令
	rt=esp8266_exit_transparent_transmission();
	
	if(rt)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	Delay_ms(2000);
	
	//复位模块
	rt=esp8266_reset();
	if(rt)
	{
		printf("esp8266_reset fail\r\n");
		return -2;
	}
	Delay_ms(2000);	
	
	//关闭回显
	rt=esp8266_enable_echo(0);
	if(rt)
	{
		printf("esp8266_enable_echo(0) fail\r\n");
		return -3;
	}	
	printf("esp8266_enable_echo(0)success\r\n");
	Delay_ms(2000);	
		
	//连接热点
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
	
	//进入透传模式
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
