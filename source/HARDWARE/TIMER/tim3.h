#ifndef __TIM3_H__
#define __TIM3_H__

#include "sys_level.h"


void tim3_init(uint16_t arr,uint16_t psc);
uint8_t register_tim3_server_fun(void (*fun)(void));

#endif //__TIM3_H__
