/**
  ******************************************************************************
  * @file    running_test.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.23
  * @brief   运行测试
  ******************************************************************************
  */

#include "stm32f4xx.h"

#ifdef   RUNNING_GLOBALS
#define  RUNNING_EXT
#else
#define  RUNNING_EXT  extern
#endif

#define     TABLE_VALUE_NULL                ((uint16_t)0xffff)

RUNNING_EXT uint8_t cur_status;///<当前测试状态
RUNNING_EXT uint8_t cur_range;///<当前档位

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
