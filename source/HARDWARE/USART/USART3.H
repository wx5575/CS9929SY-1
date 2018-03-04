/**
  ******************************************************************************
  * @file    USART3.H
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   串口3的驱动程序
  ******************************************************************************
  */

#ifndef __usart3_H__
#define __usart3_H__

#include "sys_level.h"


extern void usart3_close(void);
extern void usart3_clear_receive_count(void);
extern uint32_t usart3_sub_timeout(void);
extern uint32_t usart3_get_timeout(void);
extern void usart3_judge_timout(void);
extern void usart3_reset_timeout(void);
extern uint8_t * usart3_get_send_buf(void);
extern void usart3_clear_receive_data_count(void);
extern uint32_t usart3_get_receive_data_count(void);
extern uint8_t* usart3_get_receive_data(void);
extern uint8_t usart3_get_send_status(void);
extern void usart3_send_data(uint8_t *data, uint32_t len);
extern void usart3_resend_data(void);
extern void usart3_config(uint32_t baud_rate);
extern uint8_t get_usart3_receive_over_flag(void);
extern void clear_usart3_receive_over_flag(void);



#endif //__usart3_H__



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/

