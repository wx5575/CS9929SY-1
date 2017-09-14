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
#include "bit_banding.h"

typedef struct{
	uint32_t buzzer_time;/* ��������ʱ */
    uint32_t pass_led_time;///<PASS_LED ����ʱ��
    uint32_t fail_led_time;///<FAIL_LED ����ʱ��
    uint32_t test_led_time;///<TEST_LED ����ʱ��
    uint32_t test_led_flicker_time;///<TEST_LED ��˸���ʱ��
}DEVICE_CTRL_INFO;

#define   ON     (1)
#define   OFF    (0)

#define BUZZER_ON	1
#define BUZZER_OFF	0

#define LED_ON	    1
#define LED_OFF	    0

#define BUZZER_ON_T(T)	{set_buzzer_on_time(T);} //������������ʱ��

#define TEST_LED_PIN    PFout(8)    ///<���Ե�

#define KEY_BUZZER_TIME 30

extern void key_led_buzzer_init(void);
extern void set_pass_led_state(uint8_t state);
extern void set_fail_led_state(uint8_t state);
extern void set_test_led_state(uint8_t state);
extern void set_buzzer(uint8_t state);
extern uint32_t read_key_value(void);
extern void set_buzzer_on_time(uint32_t time);
extern void key_start_stop_gpio_init(void);
extern void sub_led_time(void);
extern uint32_t get_buzzer_time(void);
extern void sub_buzzer_time(void);
extern void test_led_flicker(void);

#endif //__KEY_LED_BUZZER__H__




/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
