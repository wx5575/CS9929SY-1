/**
  ******************************************************************************
  * @file    FSMC_CONFIG.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.9.10
  * @brief   LCD\SRAM FSMC总线初始化
  ******************************************************************************
  */
  
#ifndef __FSMC_CONFIG_H__
#define __FSMC_CONFIG_H__


#include <stm32f4xx.h>



extern void fsmc_gpio_config(void);
extern void fsmc_peripheral_config(void);


#endif //__FSMC_CONFIG_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
