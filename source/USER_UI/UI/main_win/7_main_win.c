/**
  ******************************************************************************
  * @file    7_main_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   7寸屏的布局
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "ui_com/com_ui_info.h"
#include "7_main_win.h"
#include "main_win.h"
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
WIDGET_POS_SIZE_T _7_main_windows=
{
	0/*x*/, 0/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};
/**
  * @brief  初始化7寸屏中主界面系统状态信息区
  */
const GUI_RECT _7_sys_st_info_area =
{
    0, SIZE_7INCH_HEIGH - 25, 0 + SIZE_7INCH_WIDTH - 110, SIZE_7INCH_HEIGH - 1
};
/**
  * @brief  初始化7寸屏中主界面文本对象的显示信息
  * @param  [out] pool 文本对象显示信息数组
  * @retval 无
  */
void _7_init_main_ui_text_ele_pos(TEXT_ELE_T *pool)
{
    uint8_t *str;
    TEXT_ELE_T *ele;
    UI_ELE_DISPLAY_INFO_T inf={	//主界面的通信状态
		0/*base_x*/,0/*base_y*/,0/*x*/,0/*y*/,0/*width*/,25/*height*/,100,
		{&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP
    };
    
    //主界面的系统时间
    inf.base_x = 0;
    inf.base_y = 0;
    inf.pos_size.width = 210;
    inf.pos_size.x = SIZE_7INCH_WIDTH - 115 - inf.pos_size.width;
    inf.pos_size.y = SIZE_7INCH_HEIGH - inf.pos_size.height;
    memcpy(&pool[MAIN_UI_SYS_TIME].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    str = get_time_str(0);
    pool[MAIN_UI_SYS_TIME].text[0] = str;
    pool[MAIN_UI_SYS_TIME].text[1] = str;
    
    //主界面的通信状态
    inf.pos_size.width = 80;
    inf.pos_size.x -= inf.pos_size.width;
    memcpy(&pool[MAIN_UI_COM_ST].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    ele = &pool[MAIN_UI_CSALLWIN];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 5;
    ele->dis_info.pos_size.y = 50;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = &GUI_Fontxingkai;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 90;
    ele->dis_info.pos_size.width = 600;
    
    ele = &pool[MAIN_UI_VERSION];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 200;
    ele->dis_info.pos_size.y = 200;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = &GUI_Fonthz_26;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_LEFT | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 300;
    
    ele = &pool[MAIN_UI_TYPE];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 200;
    ele->dis_info.pos_size.y = 240;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = &GUI_Fonthz_26;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_LEFT | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 300;
    
    ele = &pool[MAIN_UI_WWW];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 20;
    ele->dis_info.pos_size.y = 405;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = &GUI_Fonthz_20;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 100;
    
    ele = &pool[MAIN_UI_WTS];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 210;
    ele->dis_info.pos_size.y = 405;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = &GUI_Fonthz_20;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 130;
}


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
