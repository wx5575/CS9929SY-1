/**
  ******************************************************************************
  * @file    tim3.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   定时器3驱动层定义
  ******************************************************************************
  */
#ifndef __TIM3_H__
#define __TIM3_H__

#include "sys_level.h"


extern void tim3_init(uint16_t arr,uint16_t psc);
extern uint8_t register_tim3_server_fun(void (*fun)(void));
extern uint8_t unregister_tim3_server_fun(void (*fun)(void));

#endif //__TIM3_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
