#ifndef __SR04_H__
#define __SR04_H__

#include "stm32f4xx.h"
#include "systick.h"
#include "sys.h"

//º¯ÊıÉùÃ÷
void sr04_init(void);
int32_t sr04_get_distance(void);	

#endif
