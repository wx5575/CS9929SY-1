/**
  ******************************************************************************
  * @file    7_self_check_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   7寸屏自检窗口的布局
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "ui_com/com_ui_info.h"
#include "7_self_check_win.h"
#include "self_check_win.h"
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
WIDGET_POS_SIZE_T _7_self_check_windows=
{
	0/*x*/, 0/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};


/**
  * @brief  7寸屏结果管理界面布局1,对文本控件的位置尺寸信息进行初始化
  * @param  [in] pool 文本控件结构数组
  * @retval 无
  */
void _7_init_self_check_win_layout1_text_ele_pos(TEXT_ELE_T *pool)
{
    int32_t i = 0;
    TEXT_ELE_T *ele;
    #define FONT_RES_WIN &GUI_Fonthz_26
    
    ele = &pool[SELF_CHECK_UI_CONTENT];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 5;
    ele->dis_info.pos_size.y = 100;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_LEFT | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 380;
    ele->dis_info.pos_size.width = 800;
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
