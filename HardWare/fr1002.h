#ifndef __FR1002_H__
#define __FR1002_H__

#include "stm32f4xx.h"
#include "systick.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "uart.h"
#include "tim.h"
#include "sys.h"

#define FR_DEBUG_EN		0


extern int32_t fr_entry_standby(void);
extern int32_t fr_del_user_all(void);
extern int32_t fr_get_user_total(void);
extern int32_t fr_match(uint8_t *buf);
extern int32_t fr_power_down(void);
extern int32_t fr_reg_admin(const char *name);
extern int32_t fr_reg_user(const char *name);
extern int32_t fr_state_get(void);

extern int32_t fr_init(void);

#endif

