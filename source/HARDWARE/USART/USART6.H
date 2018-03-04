/**
  ******************************************************************************
  * @file    USART6.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   串口6的驱动程序
  ******************************************************************************
  */
  
  
#include "sys_level.h"


extern void usart6_close(void);
extern void usart6_clear_receive_count(void);
extern uint32_t usart6_sub_timeout(void);
extern uint32_t usart6_get_timeout(void);
extern void usart6_judge_timout(void);
extern void usart6_reset_timeout(void);
extern uint8_t * usart6_get_send_buf(void);
extern void usart6_clear_receive_data_count(void);
extern uint32_t usart6_get_receive_data_count(void);
extern uint8_t* usart6_get_receive_data(void);
extern uint8_t usart6_get_send_status(void);
extern void usart6_send_data(uint8_t *data, uint32_t len);
extern void usart6_resend_data(void);
extern void usart6_config(uint32_t baud_rate);
extern uint8_t get_usart6_receive_over_flag(void);
extern void clear_usart6_receive_over_flag(void);


  
  
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
