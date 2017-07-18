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

extern uint8_t com4_scan_is_over(void);
extern uint8_t com3_scan_is_over(void);
extern uint8_t com2_scan_is_over(void);
extern uint8_t com1_scan_is_over(void);


extern uint8_t get_com1_scan_addr(void);
extern uint8_t get_com2_scan_addr(void);
extern uint8_t get_com3_scan_addr(void);
extern uint8_t get_com4_scan_addr(void);

extern void scan_all_module(void);
extern void stop_scan_all_module(void);
extern void start_scan_all_module(void);


extern CS_ERR connect_module(uint8_t addr);

#endif //__SCAN_MODULE_H__

/************************ (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
