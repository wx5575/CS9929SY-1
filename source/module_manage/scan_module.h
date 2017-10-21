/**
  ******************************************************************************
  * @file    scan_module.h
  * @author  ÍõöÎ
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   É¨ÃèÄ£¿é
  ******************************************************************************
  */
#ifndef __SCAN_MODULE_H__
#define __SCAN_MODULE_H__


/* Includes ------------------------------------------------------------------*/

#include "module_manage.h"

extern CS_BOOL com1_scan_is_over(void);
extern CS_BOOL com2_scan_is_over(void);
extern CS_BOOL com3_scan_is_over(void);
extern CS_BOOL com4_scan_is_over(void);


extern MODULE_ADDR_T get_com1_scan_addr(void);
extern MODULE_ADDR_T get_com2_scan_addr(void);
extern MODULE_ADDR_T get_com3_scan_addr(void);
extern MODULE_ADDR_T get_com4_scan_addr(void);

extern void scan_all_module(void);
extern void stop_scan_all_module(void);
extern void start_scan_all_module(void);

extern void syn_module_num(void);
extern void set_module_road_num(void);

extern CS_ERR connect_module(uint8_t addr);

#endif //__SCAN_MODULE_H__

/************************ (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
