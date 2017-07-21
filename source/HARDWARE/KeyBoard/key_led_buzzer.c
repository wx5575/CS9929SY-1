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
  * @brief  PASS灯控制函数
  * @param  [in] state：目标状态  ON or OFF
  * @retval 无
  */
void Set_LED_PASSLED(uint8_t state)
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
void Set_LED_FAILLED(uint8_t state)
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
void Set_LED_TESTLED(uint8_t state)
{
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
