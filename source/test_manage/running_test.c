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
//#include "UI_COM/com_ui_info.h"
#include "running_test.h"


uint8_t get_cur_port_work_st(WORK_PORT *work_port)
{
    uint16_t work_port_16;
    
    work_port_16 = *((uint16_t*)&work_port->ports[0]);
    
//    /* 判断是否参与测试 */
//    if(((work_port_16 >> ((g_module_num - 1) * 2)) & 3) != 2)
//    {
//        return 0;
//    }
    
    return 1;
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
			steps_con = pun->acw.step_con;
			steps_pass = pun->acw.step_pass;
			
			cur_high = pun->acw.upper_limit;
			cur_low = pun->acw.lower_limit;
			cur_frequency_gear = pun->acw.output_freq;
			cur_arc_gear = pun->acw.arc_sur;/* 电弧侦测 */
			cur_port = pun->acw.port;
            cur_work_port = pun->acw.work_port;
			break;
		}
		case DCW:
		{
			steps_con = pun->dcw.step_con;
			steps_pass = pun->dcw.step_pass;
			
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
			steps_con = pun->ir.step_con;
			steps_pass = pun->ir.step_pass;
			cur_frequency_gear = 9;/* 选择400赫兹 */
			cur_frequency = 400;/* 选择400赫兹 */
			cur_high = pun->ir.upper_limit;
			cur_low = pun->ir.lower_limit;
			cur_delay_t = pun->ir.delay_time;
			cur_port = pun->ir.port;
            cur_work_port = pun->ir.work_port;
			break;
		}
		case GR:
		{
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
	
    cur_work_st = get_cur_port_work_st(&cur_work_port);
}


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
