/**
  ******************************************************************************
  * @file    7_sys_manage_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   菜单键界面
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
#include "cs99xx_struct.h"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"
#include "password_win.h"


#define _7_PWD_WIN_WIDTH        400
#define _7_PWD_WIN_HEIGHT       300

WIDGET_POS_SIZE_T _7_password_windows=
{
	80/*x*/, 50/*y*/, _7_PWD_WIN_WIDTH/*width*/, _7_PWD_WIN_HEIGHT/*height*/,
};

#define FONT    &GUI_Fonthz_24

/* 密码界面文本对象自动布局结构 */
EDIT_ELE_AUTO_LAYOUT_T _7_pwd_edit_ele_auto_layout_inf=
{
    10/*base_x*/,50/*base_y*/,
    120/*name_w*/,140/*edit_w*/,10/*uint_w*/,30/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_RIGHT | GUI_TA_VCENTER, GUI_TA_CENTER | GUI_TA_VCENTER, GUI_TA_LEFT | GUI_TA_VCENTER,
    20
};

#define PWD_BASE_T_X      280
#define PWD_BASE_T_Y      50
#define PWD_ELE_T_W       120

/* 密码界面文本对象自动布局结构 */
TEXT_ELE_AUTO_LAYOUT_T _7_pwd_text_ele_auto_layout_inf=
{
   PWD_BASE_T_X/*base_x*/,PWD_BASE_T_Y/*base_y*/,PWD_ELE_T_W/*width*/,30/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_VCENTER,20
};


static ADJUST_TEXT_ELE_LAYOUT _7_pwd_win_text_ele_adjust_layout[]=
{
    {
        PWD_UI_CHANGE_RESULT, {GUI_TA_CENTER, CS_TRUE},
        {
            {0, CS_TRUE},/*base_x*/
            {0, CS_TRUE},/*x*/
            {_7_PWD_WIN_WIDTH, CS_TRUE},/*width*/
        },
    },
};

ADJUST_TEXT_ELE_LAYOUT_INF _7_pwd_win_adjust_text_ele_layout_inf=
{
    _7_pwd_win_text_ele_adjust_layout,
    ARRAY_SIZE(_7_pwd_win_text_ele_adjust_layout)
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
