/**
  ******************************************************************************
  * @file    7_input_password_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   7寸屏密码输入窗口布局配置
  ******************************************************************************
  */

#include "UI_COM/com_ui_info.h"


#define _7_INPUT_PWD_WIN_WIDTH        230
#define _7_INPUT_PWD_WIN_HEIGHT       100
#define _7_INPUT_PWD_EDIT_WIDTH       140
#define _7_INPUT_PWD_WIN_X            (800 - 110 - _7_INPUT_PWD_WIN_WIDTH) / 2
#define _7_INPUT_PWD_WIN_Y            150

WIDGET_POS_SIZE_T _7_input_password_windows=
{
	_7_INPUT_PWD_WIN_X/*x*/, _7_INPUT_PWD_WIN_Y/*y*/,
    _7_INPUT_PWD_WIN_WIDTH/*width*/, _7_INPUT_PWD_WIN_HEIGHT/*height*/,
};

#define FONT    &GUI_Fonthz_26

/* 密码界面文本对象自动布局结构 */
EDIT_ELE_AUTO_LAYOUT_T _7_input_pwd_edit_ele_auto_layout_inf=
{
    (_7_INPUT_PWD_WIN_WIDTH - _7_INPUT_PWD_EDIT_WIDTH) / 2/*base_x*/,50/*base_y*/,
    1/*name_w*/,_7_INPUT_PWD_EDIT_WIDTH/*edit_w*/,1/*uint_w*/,30/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_RIGHT | GUI_TA_VCENTER, GUI_TA_CENTER | GUI_TA_VCENTER, GUI_TA_LEFT | GUI_TA_VCENTER,
    20
};


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
