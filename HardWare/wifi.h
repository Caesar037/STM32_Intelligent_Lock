#ifndef __WIFI_H
#define	__WIFI_H

#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"
#include <stdio.h>

#define WIFI_SSID 		"DESKTOP-Lee"
#define WIFI_PASSWORD	"1234567890"

#define DELAY_MS(nms)	Delay_ms(nms)


void Wifi_AP_Config(void);
void Wifi_STA_Config(void);

#endif
