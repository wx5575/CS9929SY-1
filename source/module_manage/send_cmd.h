/**
  ******************************************************************************
  * @file    send_cmd.h
  * @author  ÍõöÎ
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   ·¢ËÍÃüÁî
  ******************************************************************************
  */
#ifndef __SEND_CMD_H__
#define __SEND_CMD_H__


/* Includes ------------------------------------------------------------------*/

#include "module_manage.h"

extern CS_BOOL com1_send_is_over(void);
extern CS_BOOL com2_send_is_over(void);
extern CS_BOOL com3_send_is_over(void);
extern CS_BOOL com4_send_is_over(void);

extern CS_BOOL all_com_send_is_over(void);

extern uint8_t get_com1_send_addr(void);
extern uint8_t get_com2_send_addr(void);
extern uint8_t get_com3_send_addr(void);
extern uint8_t get_com4_send_addr(void);

extern void send_all_module(uint8_t *data, uint32_t len);
extern void stop_send_all_module(void);
extern void start_send_all_module(void);
extern void init_send_module_connect(void);

extern void init_send_set_road_num(void);
extern void init_send_module_connect(void);
extern CS_ERR connect_module(uint8_t addr);

#endif //__SEND_CMD_H__

/************************ (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
