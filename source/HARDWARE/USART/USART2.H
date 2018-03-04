/**
  ******************************************************************************
  * @file    USART2.H
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   串口2的驱动程序
  ******************************************************************************
  */

#ifndef __USART2_H__
#define __USART2_H__

#include "sys_level.h"


extern void usart2_close(void);
extern void usart2_clear_receive_count(void);
extern uint32_t usart2_sub_timeout(void);
extern uint32_t usart2_get_timeout(void);
extern void usart2_judge_timout(void);
extern void usart2_reset_timeout(void);
extern uint8_t * usart2_get_send_buf(void);
extern void usart2_clear_receive_data_count(void);
extern uint32_t usart2_get_receive_data_count(void);
extern uint8_t* usart2_get_receive_data(void);
extern uint8_t usart2_get_send_status(void);
extern void usart2_send_data(uint8_t *data, uint32_t len);
extern void usart2_resend_data(void);
extern void usart2_config(uint32_t baud_rate);
extern uint8_t get_usart2_receive_over_flag(void);
extern void clear_usart2_receive_over_flag(void);



#endif //__USART2_H__



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/

