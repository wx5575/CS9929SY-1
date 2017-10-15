/**
  ******************************************************************************
  * @file    board.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.12
  * @brief   板级支持
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/

#include "board.h"
#include "rtc_config.h"
#include "keyboard.h"
#include "os.h"
#include "GUI.H"
#include "EDIT.h"
#include "key_fun_manage.h"
#include "LCD_RA8875.H"
#include "CODED_DISC/coded_disc.h"
#include "ui_com/com_edit_api.h"
#include "spi_flash.h"
#include "cs99xx_mem_api.h"
#include "mem_alloc.h"
//#include "led.h"
#include "tim3.h"
#include "tim4.h"
#include "GUI.h"
#include "ff.h"
#include "key_led_buzzer.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "bsp_tft_lcd.h"
#include "FSMC_CONFIG.H"


/**
  * @brief  存放RCC时钟信息，用来了解时钟配置
  */
static RCC_ClocksTypeDef clock;

/**
  * @brief  板级初始化
  * @param  无
  * @retval 无
  */
void bsp_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
    fsmc_peripheral_config();//FSMC配置
    fsmc_gpio_config();//FSMC GPIO配置
	key_led_buzzer_init();//按键LED蜂鸣器
    register_tim3_server_fun(sub_buzzer_time);//注册定时器服务函数
    register_tim3_server_fun(sub_led_time);//注册定时器服务函数
    key_start_stop_gpio_init();//启动复位按键GPIO初始化
	ra8875_bsp_Init();//RA8875硬件初始化
	rtc_init();//RTC初始化
    coded_disc_init();//码盘初始化
    init_flash();//外扩FLASH初始化
    tim3_init(10 - 1, 4200 - 1);//10 * 0.1ms = 1ms //定时器3初始化
    RCC_GetClocksFreq(&clock);//获取时钟频率
    
    /* 串口配置 */
    usart1_config(115200 * 6);
    usart2_config(115200 * 6);
    usart3_config(115200 * 6);
    uart4_config (115200 * 6);
    usart6_config (9600);
}


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
