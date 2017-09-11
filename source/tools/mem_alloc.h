/**
  ******************************************************************************
  * @file    mem_alloc.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   外部内存分配
  ******************************************************************************
  */

#ifndef __MEM_ALLOC_H__
#define __MEM_ALLOC_H__
/* Includes ------------------------------------------------------------------*/
#include "sys_level.h"


extern void free_ex_mem(void *p);
extern void* malloc_ex_mem(uint32_t bytes);




#endif //__MEM_ALLOC_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
