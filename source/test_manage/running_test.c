/**
  ******************************************************************************
  * @file    running_test.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.23
  * @brief   运行测试
  ******************************************************************************
  */

#define RUNNING_GLOBALS

#include "sys_level.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "ff.h"
#include "OS.H"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "cs99xx_type.h"
//#include "UI_COM/com_ui_info.h"
#include "running_test.h"

CS_BOOL judge_road_work(ROAD_NUM_T road)
{
    uint16_t work_port_16;
    
    work_port_16 = *((uint16_t*)&cur_work_port);
    
    /* 判断是否参与测试 */
    if(((work_port_16 >> ((road - 1) * 2)) & 3) == 2)
    {
        return CS_TRUE;
    }
    
    return CS_FALSE;
}

uint8_t get_work_roads(void)
{
    uint16_t work_port_16;
    uint8_t work_count = 0;
    int32_t i = 0;
    
    work_port_16 = *((uint16_t*)&cur_work_port);
    
    for(i = 0; i < MAX_SYN_ROADS_NUM; i++)
    {
        /* 判断是否参与测试 */
        if(((work_port_16 >> (i * 2)) & 3) == 2)
        {
            work_count++;
        }
    }
    
    return work_count;
}

/*
 * 函数名：load_cur_frequency
 * 描述  ：计算当前步输出频率
 * 输入  ：frequency 频率设置值
 * 输出  ：无
 * 返回  ：返回对应的频率值 50hz--400hz 或 40.0hz - 400.0hz
 */
uint16_t load_cur_frequency(uint16_t frequency)
{
    uint16_t freq_buf[10] = {50,50,60,100,150,200,250,300,350,400};
    uint16_t temp = 0;
    
    if(cur_mode == GR)
    {
        temp = freq_buf[frequency % 10];/* 开正弦波 */
    }
    else
    {
        if(type_spe.hz_type == HZ_TYPE_GRADE)
        {
            temp = freq_buf[frequency % 10];/* 开正弦波 */
        }
        else
        {
            temp = frequency/10.0;
        }
    }
    
    return temp;
}
void load_data(void)
{
	UN_STRUCT *pun = &g_cur_step->one_step;
	
	cur_step = pun->com.step;/* 更新当前步号 */
	cur_mode = pun->com.mode;/* 更新当前步模式 */
    
    /* 偏移测试无输出延时 */
    zeo_t = 0;
    
    dly_t = sys_par.output_delay;/* 输出延时 */
    
	switch(cur_mode)
	{
		case ACW:
		{
            tes_t = pun->acw.test_time;
            ris_t = pun->acw.rise_time;
            fal_t = pun->acw.fall_time;
            int_t = pun->acw.inter_time;
			steps_con = pun->acw.step_con;
			steps_pass = pun->acw.step_pass;
			
            cur_gear = pun->acw.range;
            cur_vol = pun->acw.output_vol;
			cur_high = pun->acw.upper_limit;
			cur_low = pun->acw.lower_limit;
			cur_frequency_gear = pun->acw.output_freq;
			cur_arc_gear = pun->acw.arc_sur;/* 电弧侦测 */
			cur_port = pun->acw.port;
            cur_work_port = pun->acw.work_port;
            
            if(type_spe.hz_type == HZ_TYPE_CONT)
            {
                cur_frequency = cur_frequency_gear;
            }
            else
            {
                cur_frequency = load_cur_frequency(cur_frequency_gear);
            }
            
			break;
		}
		case DCW:
		{
			steps_con = pun->dcw.step_con;
			steps_pass = pun->dcw.step_pass;
			
            cur_vol = pun->dcw.output_vol;
            cur_gear = pun->dcw.range;
			cur_high = pun->dcw.upper_limit;
			cur_low  = pun->dcw.lower_limit;
			cur_charge_cur = pun->dcw.charge_cur;	/* 当前充电电流 */
			cur_delay_t    = pun->dcw.delay_time;
			cur_arc_gear   = pun->dcw.arc_sur;/* 电弧侦测 */
			cur_port = pun->dcw.port;
            cur_work_port = pun->dcw.work_port;
			break;
		}
		case IR:
		{
            cur_vol = pun->ir.output_vol;
            
			steps_con = pun->ir.step_con;
			steps_pass = pun->ir.step_pass;
			cur_frequency_gear = 9;/* 选择400赫兹 */
			cur_frequency = 400;/* 选择400赫兹 */
			cur_high = pun->ir.upper_limit;
			cur_low = pun->ir.lower_limit;
			cur_delay_t = pun->ir.delay_time;
			cur_port = pun->ir.port;
            cur_work_port = pun->ir.work_port;
            
			/* 计算出电阻的档位 */
			if(cur_low < 10)
			{
				cur_gear = 1;
			}
			else if(cur_low < 100)
			{
				cur_gear = 2;
			}
			else if(cur_low < 1000)
			{
				cur_gear = 3;
			}
			else if(cur_low < 10*1000)
			{
				cur_gear = 4;
			}
			else
			{
				cur_gear = 5;
			}
            
			break;
		}
		case GR:
		{
            cur_vol = pun->gr.output_cur;
			steps_con = pun->gr.step_con;
			steps_pass = pun->gr.step_pass;
			cur_gear = pun->gr.voltage_gear;
			cur_high = pun->gr.upper_limit;
			cur_low = pun->gr.lower_limit;
			cur_frequency_gear = pun->gr.output_freq;
			cur_method = pun->gr.test_method;
            cur_work_port = pun->gr.work_port;
			break;
		}
		case BBD:
		{
			steps_con = pun->bbd.step_con;
			steps_pass = pun->bbd.step_pass;
			cur_cap = g_cur_step->one_step.bbd.cap_value;
            cur_getc_result = g_cur_step->one_step.bbd.get_cs_ok;
			cur_gear = g_cur_step->one_step.bbd.gear;
            cur_open_ratio = g_cur_step->one_step.bbd.open_ratio;
            cur_short_ratio = g_cur_step->one_step.bbd.short_ratio;
			cur_port = pun->bbd.port;
            cur_work_port = pun->bbd.work_port;
			break;
		}
		case CC:
		{
			steps_con = pun->cc.step_con;
			steps_pass = pun->cc.step_pass;
			cur_high = pun->cc.upper_limit;
			cur_low = pun->cc.lower_limit;
			cur_frequency_gear = pun->cc.output_freq;
			cur_intensity = pun->cc.cur_intensity;/* 电流强度 */
			cur_port = pun->cc.port;
            cur_work_port = pun->cc.work_port;
			break;
		}
	}
}


/**
  * @brief  获取测试状态显示用的字符串
  * @param  [in] status 测试状态
  * @retval 无
  */
const uint8_t* get_test_status_str(uint8_t status)
{
    const uint8_t *str = NULL;
    CS_INDEX index = 0;
    uint8_t test_st_buf[][2]={
        {ST_VOL_RISE  , TEST_RISE_INDEX},
        {ST_TESTING   , TEST_TEST_INDEX},
        {ST_VOL_FALL  , TEST_FALL_INDEX},
        {ST_INTER_WAIT, TEST_INTER_INDEX},
        {ST_WAIT      , TEST_WAIT_INDEX},
        {ST_PASS      , TEST_PASS_INDEX},
        {ST_STOP      , TEST_WAIT_INDEX},    /* 复位状态 */
        {ST_ERR_H     , ERR_HIGH_INDEX},
        {ST_ERR_L     , ERR_LOW_INDEX},
        {ST_ERR_SHORT , ERR_SHORT_INDEX},
        {ST_ERR_VOL_ABNORMAL, ERR_VOL_INDEX},     /* 电压异常 */
        {ST_ERR_ARC     , ERR_ARC_INDEX},
        {ST_ERR_GFI     , ERR_GFI_INDEX},
        {ST_ERR_FAIL    , TEST_FAIL_INDEX},
        {ST_ERR_REAL    , ERR_REAL_INDEX},     /* 真实电流报警 */
        {ST_ERR_CHAR    , ERR_CHARGE_INDEX},     /* 充电报警 */
        {ST_ERR_GEAR    , ERR_GEAR_INDEX},     /* 量程报警 / 档位报警 */
        {ST_ERR_AMP     , ERR_AMP_INDEX},
        {ST_ERR_OPEN    , ERR_OPEN_INDEX},     /* 开路报警 */
        {ST_OUTPUT_DELAY, TEST_OUTPUT_DELAY},
    };
    uint32_t num = 0;
    
    num = ARRAY_SIZE(test_st_buf);
    
    if(status < num)
    {
        index = test_st_buf[status][1];
    }
    else
    {
        index = TEST_WAIT_INDEX;
    }
    
    str = status_str[index][SYS_LANGUAGE];
    
    return str;
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
