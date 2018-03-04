/**
  ******************************************************************************
  * @file    7_result_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   7寸屏启动窗口的布局
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "ui_com/com_ui_info.h"
#include "7_result_win.h"
#include "result_win.h"
#include "string.h"
#include "stdio.h"
#include "LISTVIEW.H"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SIZE_7INCH_WIDTH	300
#define SIZE_7INCH_HEIGH	200
#define SIZE_7INCH_X	    ((690 - SIZE_7INCH_WIDTH) / 2)
#define SIZE_7INCH_Y	    100
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  初始化7寸屏中主界面位置尺寸信息
  */
WIDGET_POS_SIZE_T _7_set_goto_result_num_windows=
{
	SIZE_7INCH_X/*x*/, SIZE_7INCH_Y/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};

#define FONT    &GUI_Fonthz_26

#define _7_set_goto_result_num_WIN_WIDTH        230
#define _7_set_goto_result_num_WIN_HEIGHT       100
#define _7_set_goto_result_num_EDIT_WIDTH       140
#define _7_set_goto_result_num_WIN_X            (800 - 110 - _7_set_goto_result_num_WIN_WIDTH) / 2
#define _7_set_goto_result_num_WIN_Y            150
/* 密码界面文本对象自动布局结构 */
EDIT_ELE_AUTO_LAYOUT_T _7_set_goto_result_num_edit_ele_auto_layout_inf=
{
    (_7_set_goto_result_num_WIN_WIDTH - _7_set_goto_result_num_EDIT_WIDTH) / 2/*base_x*/,50/*base_y*/,
    1/*name_w*/,_7_set_goto_result_num_EDIT_WIDTH/*edit_w*/,1/*uint_w*/,30/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_RIGHT | GUI_TA_VCENTER, GUI_TA_CENTER | GUI_TA_VCENTER, GUI_TA_LEFT | GUI_TA_VCENTER,
    20
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
