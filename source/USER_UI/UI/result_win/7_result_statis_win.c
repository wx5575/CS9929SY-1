/**
  ******************************************************************************
  * @file    7_result_statis_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   7寸屏结果统计窗口布局信息定义
  ******************************************************************************
  */
  
#include "UI_COM/com_ui_info.h"

#define RES_STATIS_W   650
#define RES_STATIS_H   400
#define RES_STATIS_X   (800 - 110 - RES_STATIS_W) / 2
#define RES_STATIS_Y   40
/**
  * @brief  7寸屏窗口的位置尺寸信息
  */
WIDGET_POS_SIZE_T _7_result_statis_windows=
{
	RES_STATIS_X/*x*/, RES_STATIS_Y/*y*/,
    RES_STATIS_W/*width*/, RES_STATIS_H/*height*/,
};

#define FONT    &GUI_Fonthz_20

#define RES_STATIS_BASE_T_X      100
#define RES_STATIS_BASE_T_Y      50
#define RES_STATIS_ELE_T_W       200 //RES_STATIS_W - RES_STATIS_BASE_T_X * 2
#define RES_STATIS_ELE_T_H       30 //RES_STATIS_H - RES_STATIS_BASE_T_Y - 10

/**
  * @brief  7寸屏窗口文本对象自动布局结构
  */
TEXT_ELE_AUTO_LAYOUT_T _7_result_statis_win_text_ele_auto_layout_inf=
{
   RES_STATIS_BASE_T_X/*base_x*/,RES_STATIS_BASE_T_Y/*base_y*/,
    RES_STATIS_ELE_T_W/*width*/,RES_STATIS_ELE_T_H/*height*/,
    6/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    {FONT}, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_LEFT | GUI_TA_VCENTER,100
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
