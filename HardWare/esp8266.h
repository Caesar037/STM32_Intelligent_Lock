#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "string.h"
#include "uart.h"

#define EN_DEBUG_ESP8266	0

//添加WIFI热点宏定义，此处根据自己的wifi作调整
#define WIFI_USART 			USART3
#define WIFI_SSID 			"Lee"
#define WIFI_PASSWORD		"1234567890"

extern uint8_t  g_esp8266_tx_buf[512];
extern volatile uint8_t  g_esp8266_rx_buf[512];
extern volatile uint32_t g_esp8266_rx_cnt;


extern volatile uint32_t g_esp8266_transparent_transmission_sta;

extern void 	esp8266_init(void);
extern int32_t  esp8266_self_test(void);
extern int32_t 	esp8266_exit_transparent_transmission (void);
extern int32_t 	esp8266_entry_transparent_transmission(void);
extern int32_t 	esp8266_connect_ap(char* ssid,char* pswd);
extern int32_t 	esp8266_connect_server(char* mode,char* ip,uint16_t port);
extern int32_t 	esp8266_disconnect_server(void);
extern void 	esp8266_send_bytes(uint8_t *buf,uint32_t len);
extern void 	esp8266_send_str(char *buf);
extern void 	esp8266_send_at(char *str);
extern int32_t  esp8266_enable_multiple_id(uint32_t b);
extern int32_t 	esp8266_create_server(uint16_t port);
extern int32_t 	esp8266_close_server(uint16_t port);
extern int32_t 	esp8266_enable_echo(uint32_t b);
extern int32_t 	esp8266_reset(void);

#endif




