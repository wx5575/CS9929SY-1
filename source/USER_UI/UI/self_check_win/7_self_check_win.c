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
    UI_ELE_DISPLAY_INFO_T inf={
		0/*base_x*/,0/*base_y*/,0/*x*/,0/*y*/,0/*width*/,30/*height*/,250,
		{&GUI_Fonthz_24}, GUI_WHITE, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP
    };
    uint8_t offset = 40;
    
    inf.base_x = 0;
    inf.base_y = 0;
    inf.pos_size.width = 700;
    inf.pos_size.x = 10;
    inf.pos_size.y = 100;
    
    //电源自检
    inf.pos_size.y += offset;
    memcpy(&pool[SELF_CHECK_UI_POWER].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //键盘自检
    inf.pos_size.y += offset;
    memcpy(&pool[SELF_CHECK_UI_KEY].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //存储器自检
    inf.pos_size.y += offset;
    memcpy(&pool[SELF_CHECK_UI_MEM].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //电压自检
    inf.pos_size.y += offset;
    memcpy(&pool[SELF_CHECK_UI_VOL].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //电流自检
    inf.pos_size.y += offset;
    memcpy(&pool[SELF_CHECK_UI_CUR].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    //系统参数自检
    inf.pos_size.y += offset;
    memcpy(&pool[SELF_CHECK_UI_SYS].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
