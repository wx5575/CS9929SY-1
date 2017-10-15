/**
  ******************************************************************************
  * @file    board.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.5.12
  * @brief   �弶֧��
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
  * @brief  ���RCCʱ����Ϣ�������˽�ʱ������
  */
static RCC_ClocksTypeDef clock;

/**
  * @brief  �弶��ʼ��
  * @param  ��
  * @retval ��
  */
void bsp_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
    fsmc_peripheral_config();//FSMC����
    fsmc_gpio_config();//FSMC GPIO����
	key_led_buzzer_init();//����LED������
    register_tim3_server_fun(sub_buzzer_time);//ע�ᶨʱ��������
    register_tim3_server_fun(sub_led_time);//ע�ᶨʱ��������
    key_start_stop_gpio_init();//������λ����GPIO��ʼ��
	ra8875_bsp_Init();//RA8875Ӳ����ʼ��
	rtc_init();//RTC��ʼ��
    coded_disc_init();//���̳�ʼ��
    init_flash();//����FLASH��ʼ��
    tim3_init(10 - 1, 4200 - 1);//10 * 0.1ms = 1ms //��ʱ��3��ʼ��
    RCC_GetClocksFreq(&clock);//��ȡʱ��Ƶ��
    
    /* �������� */
    usart1_config(115200 * 6);
    usart2_config(115200 * 6);
    usart3_config(115200 * 6);
    uart4_config (115200 * 6);
    usart6_config (9600);
}


/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
