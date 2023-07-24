#ifndef __FPM383C_H__
#define __FPM383C_H__

#include "stm32f4xx.h"
#include "systick.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "uart.h"
#include "tim.h"
#include "sys.h"


//fpm指纹模块提示灯
#define FPM_LED_RED		1
#define FPM_LED_GREEN	2
#define FPM_LED_BLUE	3


//fpm指纹模块触摸中断事件
extern volatile uint8_t g_fpm_touch_event;

void fpm_power_init(void);
extern void fpm_init(void);
extern void fpm_send_data(int length,uint8_t FPM383C_Databuffer[]);
extern void fpm_sleep(void);
extern int32_t fpm_empty(void);
extern int32_t fpm_id_total(uint16_t *total);
extern uint8_t fpm_ctrl_led(uint8_t color);
extern const char *fpm_error_code_auto_enroll(uint8_t error_code);
extern int32_t fpm_idenify_auto(uint16_t *id);
extern int32_t fpm_enroll_auto(uint16_t id);

#endif
