

#ifndef __USART1_H__
#define __USART1_H__

#include "sys_level.h"


extern void usart1_close(void);
extern void usart1_clear_receive_count(void);
extern uint32_t usart1_sub_timeout(void);
extern uint32_t usart1_get_timeout(void);
extern void usart1_judge_timout(void);
extern void usart1_reset_timeout(void);
extern uint8_t * usart1_get_send_buf(void);
extern void usart1_clear_receive_data_count(void);
extern uint32_t usart1_get_receive_data_count(void);
extern uint8_t* usart1_get_receive_data(void);
extern uint8_t usart1_get_send_status(void);
extern void usart1_send_data(uint8_t *data, uint32_t len);
extern void usart1_config(uint32_t baud_rate);
extern uint8_t get_usart1_receive_over_flag(void);
extern void clear_usart1_receive_over_flag(void);



#endif //__USART1_H__



