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

typedef CS_ERR (*SEND_CMD_FUN)(uint8_t addr, uint8_t*data, uint32_t len);

extern CS_BOOL com1_send_is_over(void);
extern CS_BOOL com2_send_is_over(void);
extern CS_BOOL com3_send_is_over(void);
extern CS_BOOL com4_send_is_over(void);

extern CS_BOOL all_com_send_is_over(void);

extern MODULE_ADDR_T get_com1_send_addr(void);
extern MODULE_ADDR_T get_com2_send_addr(void);
extern MODULE_ADDR_T get_com3_send_addr(void);
extern MODULE_ADDR_T get_com4_send_addr(void);

extern void send_cmd_to_all_module(uint8_t *data, uint32_t len, SEND_CMD_FUN fun);
extern CS_ERR send_cmd_to_one_module(ROAD_INDEX road, uint8_t *data, uint32_t len, SEND_CMD_FUN fun);

#endif //__SEND_CMD_H__

/************************ (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
