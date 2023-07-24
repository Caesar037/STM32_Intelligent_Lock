#ifndef __WWDG_H__
#define __WWDG_H__

//头文件作用：作函数的声明，外部变量的声明，宏&typedef的定义，结构体&枚举&共用体类型定义，static修饰的函数尽量写在头文件， \
	inline修饰的函数写在头文件，作其他头文件的包含；
	
#include "stm32f4xx.h"

void wwdg_init(uint32_t WWDG_Prescaler, uint8_t WindowValue);

#endif
