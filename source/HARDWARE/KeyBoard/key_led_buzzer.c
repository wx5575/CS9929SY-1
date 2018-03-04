/**
  ******************************************************************************
  * @file    key_led_buzzer.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键、LED、蜂鸣器驱动层
  ******************************************************************************
  */
  
#include "key_led_buzzer.h"
#include "panel_4094.h"

/**
  * @brief  设备控件信息结构
  */
static DEVICE_CTRL_INFO	device_ctrl_info;

/**
  * @brief  开蜂鸣器并设置延时关闭时间
  * @param  [in] 延时关闭时间
  * @retval 无
  */
void set_buzzer_on_time(uint32_t time)
{
	device_ctrl_info.buzzer_time = time;
	set_buzzer(BUZZER_ON);
}

/**
  * @brief  获取蜂鸣器的剩余延时关闭时间
  * @param  无
  * @retval 剩余的延时时间
  */
uint32_t get_buzzer_time(void)
{
	return device_ctrl_info.buzzer_time;
}

/**
  * @brief  给外部定时器提供的定时服务函数，定时时间到就关闭蜂鸣器
  * @param  无
  * @retval 无
  */
void sub_buzzer_time(void)
{
	if(device_ctrl_info.buzzer_time > 0)
	{
		if(--device_ctrl_info.buzzer_time == 0)
		{
			set_buzzer(BUZZER_OFF);
		}
	}
}

/**
  * @brief  提供给定时器调用的用于保持时间到后自动关闭LED灯
  * @param  无
  * @retval 无
  */
void sub_led_time(void)
{
	if(device_ctrl_info.pass_led_time > 0)
	{
		if(--device_ctrl_info.pass_led_time == 0)
		{
			set_pass_led_state(LED_OFF);
		}
	}
    
	if(device_ctrl_info.fail_led_time > 0)
	{
		if(--device_ctrl_info.fail_led_time == 0)
		{
			set_fail_led_state(LED_OFF);
		}
	}
    
	if(device_ctrl_info.test_led_time > 0)
	{
		if(--device_ctrl_info.test_led_time == 0)
		{
			set_test_led_state(LED_OFF);
		}
	}
}

/**
  * @brief  打开 PASS_LED  保持指定时间后自动关闭
  * @param  [in] time 保持时间 单位 ms
  * @retval 无
  */
void set_pass_led_on_time(uint32_t time)
{
	device_ctrl_info.pass_led_time = time;
	set_pass_led_state(LED_ON);
}

/**
  * @brief  打开 FAIL_LED  保持指定时间后自动关闭
  * @param  [in] time 保持时间 单位 ms
  * @retval 无
  */
void set_fail_led_on_time(uint32_t time)
{
	device_ctrl_info.fail_led_time = time;
	set_fail_led_state(LED_ON);
}

/**
  * @brief  打开 TEST_LED  保持指定时间后自动关闭
  * @param  [in] time 保持时间 单位 ms
  * @retval 无
  */
void set_test_led_on_time(uint32_t time)
{
	device_ctrl_info.test_led_time = time;
	set_test_led_state(LED_ON);
}
/**
  * @brief  打开所有LED灯 PASS_LED、TEST_LED、FAIL_LED 保持指定时间后自动关闭
  * @param  [in] ms 保持时间 单位 ms
  * @retval 无
  */
void led_all_hold_on_t(uint32_t ms)
{
    set_pass_led_on_time(ms);
    set_fail_led_on_time(ms);
    set_test_led_on_time(ms);
}

/**
  * @brief  测试灯闪烁
  * @param  无
  * @retval 无
  */
void test_led_flicker(void)
{
	if(++device_ctrl_info.test_led_flicker_time >= 200)
    {
        device_ctrl_info.test_led_flicker_time = 0;
        TEST_LED_PIN++;
    }
}
/**
  * @brief  PASS灯控制函数
  * @param  [in] state：目标状态  ON or OFF
  * @retval 无
  */
void set_pass_led_state(uint8_t state)
{
    uint8_t tmp = 0;
    
    tmp = panel_4094_read(PANEL_4094_CHIP_2);
    
	if(state == LED_ON)
	{
        tmp |= PASS_LED_CHIP_2_POS;
    }
    else
    {
        tmp &= ~PASS_LED_CHIP_2_POS;
    }
    
    panel_4094_write(PANEL_4094_CHIP_2, tmp);
    update_panel_4094();
}

/**
  * @brief  FAIL灯控制函数
  * @param  [in] state：目标状态  ON or OFF
  * @retval 无
  */
void set_fail_led_state(uint8_t state)
{
    uint8_t tmp = 0;
    
    tmp = panel_4094_read(PANEL_4094_CHIP_2);
    
	if(state == LED_ON)
	{
        tmp |= FAIL_LED_CHIP_2_POS;
    }
    else
    {
        tmp &= ~FAIL_LED_CHIP_2_POS;
    }
    
    panel_4094_write(PANEL_4094_CHIP_2, tmp);
    update_panel_4094();
}

/**
  * @brief  TEST灯控制函数
  * @param  [in] state：目标状态  ON or OFF
  * @retval 无
  */
void set_test_led_state(uint8_t state)
{
	if(state == LED_ON)
	{
        TEST_LED_PIN = LED_ON;
    }
    else
    {
        TEST_LED_PIN = LED_OFF;
    }
}

/**
  * @brief  蜂鸣器控制函数
  * @param  [in] state：目标状态  ON or OFF
  * @retval 无
  */
void set_buzzer(uint8_t state)
{
    uint8_t tmp = 0;
    
    tmp = panel_4094_read(PANEL_4094_CHIP_2);
    
	if(state == BUZZER_ON)
	{
        tmp |= BUZZER_CHIP_2_POS;
    }
    else
    {
        tmp &= ~BUZZER_CHIP_2_POS;
    }
    
    panel_4094_write(PANEL_4094_CHIP_2, tmp);
    update_panel_4094();
}

/**
  * @brief  延时函数
  * @param  t 延时值
  * @retval 无
  */
static void key_led_buzzer_delay(uint32_t t)
{
	while(t--);
}

/**
  * @brief  键值读取函数
  * @param  无
  * @retval 键值32位数据，8行4列矩阵键盘
  */
uint32_t read_key_value(void)
{
	uint8_t read_temp = 0;
    int32_t i = 0;
	uint32_t key_value = 0;
    uint8_t colum = 4;//4列
    uint8_t row = 8;//8行
    
	for(i = 0; i < row; i++)
	{
		panel_4094_write(PANEL_4094_CHIP_1, ~(1<<i));//设置行状态
        update_panel_4094();//更新到4094
        key_led_buzzer_delay(10);//延时
        
		read_temp = 0;
        read_temp = read_row_gpio();//读列引脚值
		key_value <<= colum;
		key_value |= read_temp;
		read_temp = 0;
	}
	
	return key_value;
}

/**
  * @brief  键盘、LED、蜂鸣器驱动初始化
  * @param  无
  * @retval 无
  */
void key_led_buzzer_init(void)
{
	panel_4094_init();
    panel_4094_write(PANEL_4094_CHIP_1, 0);
    panel_4094_write(PANEL_4094_CHIP_2, 0);
    update_panel_4094();
}


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
