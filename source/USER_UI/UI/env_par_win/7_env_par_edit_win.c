/**
  ******************************************************************************
  * @file    7_env_par_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   7寸屏环境变量编辑窗口位置尺寸定义
  ******************************************************************************
  */

#include "UI_COM/com_ui_info.h"

/**
  * @brief  7寸屏环境变量窗口位置尺寸结构初始化数据
  */
WIDGET_POS_SIZE_T _7_env_par_windows=
{
	0/*x*/, 0/*y*/, 690/*width*/, 455/*height*/,
};


#define FONT    &GUI_Fonthz_26
/**
  * @brief  环境变量窗口界面编辑对象自动布局结构
  */
EDIT_ELE_AUTO_LAYOUT_T _7_env_par_edit_ele_auto_layout_inf=
{
    10/*base_x*/,50/*base_y*/,
    155/*name_w*/,150/*edit_w*/,25/*uint_w*/,30/*height*/,
    5/*rows*/,2/*column*/,45/*row_spacing*/,340/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_LEFT | GUI_TA_VCENTER, GUI_TA_CENTER | GUI_TA_VCENTER, GUI_TA_LEFT | GUI_TA_VCENTER,
    20/*max_len*/
};



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
