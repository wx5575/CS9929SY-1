/**
  ******************************************************************************
  * @file    key_led_buzzer.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ������LED��������������
  ******************************************************************************
  */
  
#ifndef __KEY_LED_BUZZER__H__
#define __KEY_LED_BUZZER__H__


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "sys_level.h"

typedef struct{
	uint32_t buzzer_time;/* ��������ʱ */
}DEVICE_CTRL_INFO;

#define   ON     (1)
#define   OFF    (0)

#define BUZZER_ON	1
#define BUZZER_OFF	0

#define BUZZER_ON_T(T)	{set_buzzer_on_time(T);} //������������ʱ��


extern void key_led_buzzer_init(void);
extern void Set_LED_PASSLED(uint8_t state);
extern void Set_LED_FAILLED(uint8_t state);
extern void Set_LED_TESTLED(uint8_t state);
extern void set_buzzer(uint8_t state);
extern uint32_t KeyValue_Read(void);
extern void set_buzzer_on_time(uint32_t time);
extern void key_start_stop_gpio_init(void);

uint32_t get_buzzer_time(void);
uint32_t sub_buzzer_time(void);

#endif //__KEY_LED_BUZZER__H__



