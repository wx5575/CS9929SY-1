/**
  ******************************************************************************
  * @file    UART4.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   串口4的驱动程序
  ******************************************************************************
  */

#ifndef __UART4_H__
#define __UART4_H__

#include "sys_level.h"


extern void uart4_close(void);
extern void uart4_clear_receive_count(void);
extern uint32_t uart4_sub_timeout(void);
extern uint32_t uart4_get_timeout(void);
extern void uart4_judge_timout(void);
extern void uart4_reset_timeout(void);
extern uint8_t * uart4_get_send_buf(void);
extern void uart4_clear_receive_data_count(void);
extern uint32_t uart4_get_receive_data_count(void);
extern uint8_t* uart4_get_receive_data(void);
extern uint8_t uart4_get_send_status(void);
extern void uart4_send_data(uint8_t *data, uint32_t len);
extern void uart4_resend_data(void);
extern void uart4_config(uint32_t baud_rate);
extern uint8_t get_uart4_receive_over_flag(void);
extern void clear_uart4_receive_over_flag(void);



#endif //__UART4_H__



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/

