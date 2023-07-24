#ifndef __ES8266_MQTT_H
#define __ES8266_MQTT_H

#include "stm32f4xx.h"
#include "systick.h"
#include "esp8266.h"

//�˴��ǰ����Ʒ������Ĺ���ʵ����½����-------------------------------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������


#define MQTT_BROKERADDRESS 		"i0o7ybwV21d.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define MQTT_CLIENTID 			"0001|securemode=3,signmethod=hmacsha1|"
#define MQTT_USARNAME 			"smartdevice&i0o7ybwV21d"
#define MQTT_PASSWD 			"DC02941C71A9A15C6BA4B36E2858D65CC5B1F508"
#define	MQTT_PUBLISH_TOPIC 		"/sys/i0o7ybwV21d/smartdevice/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC 	"/sys/i0o7ybwV21d/smartdevice/thing/service/property/set"

#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	

//MQTT���ӷ�����
extern int32_t mqtt_connect(char *client_id,char *user_name,char *password);

//MQTT��Ϣ����
extern int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);

//MQTT��Ϣ����
extern uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);

//MQTT����������
extern void mqtt_send_heart(void);

extern int32_t esp8266_mqtt_init(void);

extern void mqtt_report_devices_status(void);

#endif
