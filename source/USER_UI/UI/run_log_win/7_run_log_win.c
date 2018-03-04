/**
  ******************************************************************************
  * @file    7_run_log_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   7寸屏运行日志窗口布局
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "ui_com/com_ui_info.h"
#include "7_run_log_win.h"
#include "run_log_win.h"
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
WIDGET_POS_SIZE_T _7_run_log_windows=
{
	0/*x*/, 0/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};

/**
  * @brief  7寸屏结果管理界面布局1,对文本控件的位置尺寸信息进行初始化
  * @param  [in] pool 文本控件结构数组
  * @retval 无
  */
void _7_init_run_log_win_layout1_text_ele_pos(TEXT_ELE_T *pool)
{
    UI_ELE_DISPLAY_INFO_T inf={
		0/*base_x*/,0/*base_y*/,0/*x*/,0/*y*/,0/*width*/,30/*height*/,250,
		{&GUI_Fonthz_20}, GUI_WHITE, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP
    };
    uint8_t offset = 30;
    uint16_t name_x = 10;
    uint16_t value_x = 270;
    
    inf.base_x = 0;
    inf.base_y = 0;
    inf.pos_size.width = 700;
    inf.pos_size.x = 120;
    inf.pos_size.y = 10;
    
    inf.pos_size.height = 30;
    //软件版本号
//    inf.pos_size.y += 40;
    memcpy(&pool[RUN_LOG_WIN_SOFT_VER].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //硬件版本号
    inf.pos_size.y += 22;
//    inf.pos_size.x += 230;
    memcpy(&pool[RUN_LOG_WIN_HARD_VER].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //公司名称
    inf.pos_size.y += 22;
    memcpy(&pool[RUN_LOG_WIN_LTD].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //地址
    inf.pos_size.y += 22;
    inf.pos_size.height = 60;
    memcpy(&pool[RUN_LOG_WIN_ADDRESS].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = 10;
    inf.pos_size.y = 100;
    inf.pos_size.height = 300;
//    //注意事项
    inf.pos_size.y += offset;
    inf.pos_size.x = name_x;
    memcpy(&pool[RUN_LOG_WIN_INSPECTION_DATE_N].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.x = value_x - 60;
    memcpy(&pool[RUN_LOG_WIN_INSPECTION_DATE_V].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.y += offset;
    inf.pos_size.x = name_x;
    memcpy(&pool[RUN_LOG_WIN_BOOT_TIMES_N].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.x = value_x;
    memcpy(&pool[RUN_LOG_WIN_BOOT_TIMES_V].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.y += offset;
    inf.pos_size.x = name_x;
    memcpy(&pool[RUN_LOG_WIN_TESTING_TIMES_N].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.x = value_x;
    memcpy(&pool[RUN_LOG_WIN_TESTING_TIMES_V].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.y += offset;
    inf.pos_size.x = name_x;
    memcpy(&pool[RUN_LOG_WIN_STARTUP_TIME_N].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.x = value_x - 60;
    memcpy(&pool[RUN_LOG_WIN_STARTUP_TIME_V].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.y += offset;
    inf.pos_size.x = name_x;
    memcpy(&pool[RUN_LOG_WIN_OPERATION_TIME_N].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    inf.pos_size.x = value_x - 60;
    memcpy(&pool[RUN_LOG_WIN_OPERATION_TIME_V].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
