/**
  ******************************************************************************
  * @file    tools.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   软件工具函数
  ******************************************************************************
  */
#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "sys_level.h"

#define COUNT_ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])
#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

extern uint32_t ten_power(u8 n);
extern void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value);
extern void mysprintf_2(uint8_t *buf, const uint8_t* unit, uint8_t format, uint32_t value);
extern float myatof(uint8_t *buf);

#endif //__TOOLS_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
