/**
  ******************************************************************************
  * @file    7_restore_setting_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   7寸屏恢复出厂设置窗口布局信息定义
  ******************************************************************************
  */
  
#include "UI_COM/com_ui_info.h"

#define RES_SET_WIN_W   350
#define RES_SET_WIN_H   200
#define RES_SET_WIN_X   (800 - 110 - RES_SET_WIN_W) / 2
#define RES_SET_WIN_Y   100
/**
  * @brief  7寸屏恢复出厂设置窗口的位置尺寸信息
  */
WIDGET_POS_SIZE_T _7_restore_setting_windows=
{
	RES_SET_WIN_X/*x*/, RES_SET_WIN_Y/*y*/,
    RES_SET_WIN_W/*width*/, RES_SET_WIN_H/*height*/,
};

#define FONT    &GUI_Fonthz_20

#define RES_SET_BASE_T_X      10
#define RES_SET_BASE_T_Y      30
#define RES_SET_ELE_T_W       RES_SET_WIN_W - RES_SET_BASE_T_X * 2
#define RES_SET_ELE_T_H       RES_SET_WIN_H - RES_SET_BASE_T_Y - 10

/**
  * @brief  7寸屏恢复出厂设置窗口文本对象自动布局结构
  */
TEXT_ELE_AUTO_LAYOUT_T _7_restore_setting_text_ele_auto_layout_inf=
{
   RES_SET_BASE_T_X/*base_x*/,RES_SET_BASE_T_Y/*base_y*/,
    RES_SET_ELE_T_W/*width*/,RES_SET_ELE_T_H/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    {FONT}, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_CENTER | GUI_TA_VCENTER,100
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
