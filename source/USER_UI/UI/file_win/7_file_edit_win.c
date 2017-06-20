/**
  ******************************************************************************
  * @file    7_file_win_edit.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   文件编辑界面
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ff.h"
#include "OS.H"
#include "app.h"
#include "cs99xx_struct.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "UI_COM/com_ui_info.h"
#include "7_file_edit_win.h"
#include "file_edit_win.h"
#include "string.h"

/**
  * @brief  7寸屏文件编辑窗口尺寸
  */
WIDGET_POS_SIZE_T _7_file_edit_windows=
{
	150/*x*/, 50/*y*/, 400/*width*/, 320/*height*/,
};

/**
  * @brief  7寸屏文件编辑窗口尺寸
  */
WIDGET_POS_SIZE_T _7_file_save_as_windows=
{
	150/*x*/, 50/*y*/, 400/*width*/, 150/*height*/,
};

#define FONT    &GUI_Fonthz_24

/**
  * @brief  7寸屏文件编辑窗口编辑对象自动布局结构
  */
EDIT_ELE_AUTO_LAYOUT_T _7_file_edit_ele_auto_layout_inf=
{
    10/*base_x*/,50/*base_y*/,
    120/*name_w*/,120/*edit_w*/,25/*uint_w*/,30/*height*/,
    5/*rows*/,1/*column*/,42/*row_spacing*/,0/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_RIGHT | GUI_TA_VCENTER, GUI_TA_CENTER | GUI_TA_VCENTER, GUI_TA_LEFT | GUI_TA_VCENTER,
    20/*max_len*/
};

static ADJUST_EDIT_ELE_LAYOUT  _7_file_edit_win_edit_ele_adjust_layout_pool[]=
{
    /* 调整文件名的布局信息，因为自动布局时的编辑框的宽度不够 */
    {
        FSAVE_UI_FNAME/*index*/,
        {
            {0/*name*/, CS_FALSE}, {220/*edit*/, CS_TRUE}, {0/*unit*/, CS_FALSE},
        }/*width*/,
    },
};

ADJUST_EDIT_ELE_LAYOUT_INF _7_file_edit_win_edit_ele_adjust_layout_inf =
{
    _7_file_edit_win_edit_ele_adjust_layout_pool,
    ARRAY_SIZE(_7_file_edit_win_edit_ele_adjust_layout_pool)
};
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
