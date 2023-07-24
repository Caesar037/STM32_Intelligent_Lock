#ifndef __NETWORK_H
#define __NETWORK_H

//包含头文件
#include "stm32f4xx.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "uart.h"
#include "wifi.h"

#define MODE				"TCP"
#define NETTIME_PORTNUM 	80						//网络时间端口
#define NETTIME_SERVERIP 	"api.seniverse.com"		//网络时间IP


//变量声明

//函数声明
void Get_Network_Weather(void);

#endif
