/**
  ******************************************************************************
  * @file    7_sys_time_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.18
  * @brief   7寸屏系统时间设置窗口位置尺寸相关定义
  ******************************************************************************
  */

#include "UI_COM/com_ui_info.h"


#define SYS_TIME_WIN_W   400
#define SYS_TIME_WIN_H   300
#define SYS_TIME_WIN_X   (800 - 110 - SYS_TIME_WIN_W) / 2
#define SYS_TIME_WIN_Y   50
/**
  * @brief  7寸屏系统时间设置窗口位置尺寸数据结构定义
  */
WIDGET_POS_SIZE_T _7_sys_time_windows=
{
	SYS_TIME_WIN_X/*x*/, SYS_TIME_WIN_Y/*y*/,
    SYS_TIME_WIN_W/*width*/, SYS_TIME_WIN_H/*height*/,
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
