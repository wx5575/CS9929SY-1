/**
  ******************************************************************************
  * @file    7_start_win.c
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
#include "7_module_win.h"
#include "module_win.h"
#include "string.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SIZE_7INCH_WIDTH	800
#define SIZE_7INCH_HEIGH	480
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  初始化7寸屏中主界面位置尺寸信息
  */
WIDGET_POS_SIZE_T _7_module_windows=
{
	0/*x*/, 0/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};

#define MODULE_BASE_T_X      5
#define MODULE_BASE_T_Y      5
#define MODULE_BASE_T_W      120

/** 
  * @brief 7寸屏模块管理界面文本对象自动布局结构
  */
TEXT_ELE_AUTO_LAYOUT_T _7_module_text_ele_auto_layout_inf=
{
   MODULE_BASE_T_X/*base_x*/,MODULE_BASE_T_Y/*base_y*/,MODULE_BASE_T_W/*width*/,30/*height*/,
    1/*rows*/,8/*column*/,50/*row_spacing*/,65/*column_spacing*/,
    {&GUI_Fonthz_24}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_VCENTER,20
};

/** 
  * @brief 7寸屏模块管理界面文本对象调整布局信息池
  */
//    MODULE_WIN_COM1_NAME,///<串口1名称
//    MODULE_WIN_COM1_ADDR,///<串口1地址
//    MODULE_WIN_COM2_NAME,///<串口2名称
//    MODULE_WIN_COM2_ADDR,///<串口2地址
//    MODULE_WIN_COM3_NAME,///<串口3名称
//    MODULE_WIN_COM3_ADDR,///<串口3地址
//    MODULE_WIN_COM4_NAME,///<串口4名称
//    MODULE_WIN_COM4_ADDR,///<串口4地址
static ADJUST_TEXT_ELE_LAYOUT _7_module_win_text_ele_adjust_layout[]=
{
    {
        MODULE_WIN_COM1_ADDR, {GUI_TA_CENTER, CS_FALSE},
        {
            {0, CS_FALSE},/*base_x*/
            {0, CS_FALSE},/*x*/
            {80, CS_TRUE},/*width*/
        },
    },
};
/** 
  * @brief 7寸屏模块管理界面文本对象调整布局信息
  */
ADJUST_TEXT_ELE_LAYOUT_INF _7_module_win_adjust_text_ele_layout_inf=
{
    _7_module_win_text_ele_adjust_layout,
    ARRAY_SIZE(_7_module_win_text_ele_adjust_layout)
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
