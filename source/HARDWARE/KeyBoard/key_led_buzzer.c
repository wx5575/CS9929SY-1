/**
  ******************************************************************************
  * @file    key_led_buzzer.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ������LED��������������
  ******************************************************************************
  */
  
#include "key_led_buzzer.h"
#include "panel_4094.h"

/**
  * @brief  �豸�ؼ���Ϣ�ṹ
  */
static DEVICE_CTRL_INFO	device_ctrl_info;

/**
  * @brief  ����������������ʱ�ر�ʱ��
  * @param  [in] ��ʱ�ر�ʱ��
  * @retval ��
  */
void set_buzzer_on_time(uint32_t time)
{
	device_ctrl_info.buzzer_time = time;
	set_buzzer(BUZZER_ON);
}

/**
  * @brief  ��ȡ��������ʣ����ʱ�ر�ʱ��
  * @param  ��
  * @retval ʣ�����ʱʱ��
  */
uint32_t get_buzzer_time(void)
{
	return device_ctrl_info.buzzer_time;
}

/**
  * @brief  ���ⲿ��ʱ���ṩ�Ķ�ʱ����������ʱʱ�䵽�͹رշ�����
  * @param  ��
  * @retval ��
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
  * @brief  PASS�ƿ��ƺ���
  * @param  [in] state��Ŀ��״̬  ON or OFF
  * @retval ��
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
  * @brief  FAIL�ƿ��ƺ���
  * @param  [in] state��Ŀ��״̬  ON or OFF
  * @retval ��
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
  * @brief  TEST�ƿ��ƺ���
  * @param  [in] state��Ŀ��״̬  ON or OFF
  * @retval ��
  */
void Set_LED_TESTLED(uint8_t state)
{
}

/**
  * @brief  ���������ƺ���
  * @param  [in] state��Ŀ��״̬  ON or OFF
  * @retval ��
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
  * @brief  ��ʱ����
  * @param  t ��ʱֵ
  * @retval ��
  */
static void key_led_buzzer_delay(uint32_t t)
{
	while(t--);
}

/**
  * @brief  ��ֵ��ȡ����
  * @param  ��
  * @retval ��ֵ32λ���ݣ�8��4�о������
  */
uint32_t read_key_value(void)
{
	uint8_t read_temp = 0;
    int32_t i = 0;
	uint32_t key_value = 0;
    uint8_t colum = 4;//4��
    uint8_t row = 8;//8��
    
	for(i = 0; i < row; i++)
	{
		panel_4094_write(PANEL_4094_CHIP_1, ~(1<<i));//������״̬
        update_panel_4094();//���µ�4094
        key_led_buzzer_delay(10);//��ʱ
        
		read_temp = 0;
        read_temp = read_row_gpio();//��������ֵ
		key_value <<= colum;
		key_value |= read_temp;
		read_temp = 0;
	}
	
	return key_value;
}

/**
  * @brief  ���̡�LED��������������ʼ��
  * @param  ��
  * @retval ��
  */
void key_led_buzzer_init(void)
{
	panel_4094_init();
    panel_4094_write(PANEL_4094_CHIP_1, 0);
    panel_4094_write(PANEL_4094_CHIP_2, 0);
    update_panel_4094();
}


/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/