/**
  ******************************************************************************
  * @file    7_auto_cal_win.c
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
#include "7_auto_cal_win.h"
#include "auto_cal_win.h"
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
WIDGET_POS_SIZE_T _7_auto_cal_windows=
{
	0/*x*/, 0/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};

/**
  * @brief  7寸屏结果管理界面布局1,对文本控件的位置尺寸信息进行初始化
  * @param  [in] pool 文本控件结构数组
  * @retval 无
  */
void _7_init_auto_cal_win_layout1_text_ele_pos(TEXT_ELE_T *pool)
{
    UI_ELE_DISPLAY_INFO_T inf={
		0/*base_x*/,0/*base_y*/,0/*x*/,0/*y*/,0/*width*/,30/*height*/,250,
		{&GUI_Fonthz_20}, GUI_WHITE, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP
    };
    uint8_t offset = 30;
    
    inf.base_x = 0;
    inf.base_y = 0;
    inf.pos_size.width = 700;
    inf.pos_size.x = 120;
    inf.pos_size.y = 10;
    
    inf.pos_size.height = 30;
    //软件版本号
//    inf.pos_size.y += 40;
    memcpy(&pool[AUTO_CAL_WIN_SOFT_VER].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //硬件版本号
    inf.pos_size.y += 22;
//    inf.pos_size.x += 230;
    memcpy(&pool[AUTO_CAL_WIN_HARD_VER].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //公司名称
    inf.pos_size.y += 22;
    memcpy(&pool[AUTO_CAL_WIN_LTD].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //地址
    inf.pos_size.y += 22;
    inf.pos_size.height = 60;
    memcpy(&pool[AUTO_CAL_WIN_ADDRESS].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = 10;
    inf.pos_size.y = 100;
    inf.pos_size.height = 300;
    //注意事项
    inf.pos_size.y += offset;
    memcpy(&pool[AUTO_CAL_WIN_NOTICE].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
