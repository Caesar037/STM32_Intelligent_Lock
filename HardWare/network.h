#ifndef __NETWORK_H
#define __NETWORK_H

//����ͷ�ļ�
#include "stm32f4xx.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "uart.h"
#include "wifi.h"

#define MODE				"TCP"
#define NETTIME_PORTNUM 	80						//����ʱ��˿�
#define NETTIME_SERVERIP 	"api.seniverse.com"		//����ʱ��IP


//��������

//��������
void Get_Network_Weather(void);

#endif
