/**
  ******************************************************************************
  * @file    key_led_buzzer.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键、LED、蜂鸣器驱动层
  ******************************************************************************
  */
  
#ifndef __PANEL_4094__H__
#define __PANEL_4094__H__


/* Includes ------------------------------------------------------------------*/

#include "sys_level.h"

#define PANEL_4094_CHIP_1   0 ///<面板第1片4094
#define PANEL_4094_CHIP_2   1 ///<面板第2片4094

#define  PASS_LED_CHIP_2_POS    (1<<0)  ///<PASS_LED
#define  FAIL_LED_CHIP_2_POS    (1<<1)  ///<FAIL_LED
#define  BUZZER_CHIP_2_POS      (1<<2)  ///<BUZZER

extern void panel_4094_write(uint8_t index,uint8_t data);
extern void panel_4094_init(void);
extern uint8_t read_row_gpio(void);
extern void update_panel_4094(void);
extern uint8_t panel_4094_read(uint8_t index);

#endif //__PANEL_4094__H__




/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
