/**
  ******************************************************************************
  * @file    7_result_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   7寸屏结果管理窗口
  ******************************************************************************
  */

#include "stdio.h"
#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"
#include "result_win.h"


WIDGET_POS_SIZE_T _7_result_windows=
{
	0/*x*/,0/*y*/,690/*width*/,455/*height*/,
};

/**
  * @brief  7寸屏结果管理界面布局1,对文本控件的位置尺寸信息进行初始化
  * @param  [in] pool 文本控件结构数组
  * @retval 无
  */
void _7_init_result_win_layout1_text_ele_pos(TEXT_ELE_T *pool)
{
    int32_t i = 0;
    TEXT_ELE_T *ele;
    #define FONT_RES_WIN &GUI_Fonthz_20
    
    ele = &pool[RESULT_WIN_PRODUCT_NUM];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 5;
    ele->dis_info.pos_size.y = 30;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 190;
    
    for(i = 0; i < 16; i++)
    {
        ele = &pool[RESULT_WIN_NUM01 + i];
        
        ele->dis_info.base_x = 0;
        ele->dis_info.base_y = 0;
        ele->dis_info.pos_size.x = 5;
        ele->dis_info.pos_size.y = 60 + 23 * i;
        ele->dis_info.back_color = GUI_INVALID_COLOR;
        ele->dis_info.font = FONT_RES_WIN;
        ele->dis_info.font_color = GUI_WHITE;
        ele->dis_info.max_len = 100;
        ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
        ele->dis_info.pos_size.height = 30;
        ele->dis_info.pos_size.width = 180;
    }
    
    ele = &pool[RESULT_WIN_TEST_RES_INF];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 215;
    ele->dis_info.pos_size.y = 30;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 675 - 215;
    
    for(i = 0; i < 8; i++)
    {
        ele = &pool[RESULT_WIN_TEST_RES_1 + i];
        
        ele->dis_info.base_x = 0;
        ele->dis_info.base_y = 0;
        ele->dis_info.pos_size.x = 180;
        ele->dis_info.pos_size.y = 60 + 23 * i;
        ele->dis_info.back_color = GUI_INVALID_COLOR;
        ele->dis_info.font = FONT_RES_WIN;
        ele->dis_info.font_color = GUI_WHITE;
        ele->dis_info.max_len = 100;
        ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
        ele->dis_info.pos_size.height = 30;
        ele->dis_info.pos_size.width = 675 - 180;
    }
    
    ele = &pool[RESULT_WIN_SETTING_PAR];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 180;
    ele->dis_info.pos_size.y = 260;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 395 - 180;
    
    ele = &pool[RESULT_WIN_SETTING_PAR_C];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 180;
    ele->dis_info.pos_size.y = 300;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_LEFT | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 140;
    ele->dis_info.pos_size.width = 395 - 180;
    
    ele = &pool[RESULT_WIN_TEST_DATA];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 370;
    ele->dis_info.pos_size.y = 260;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_CENTER | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 30;
    ele->dis_info.pos_size.width = 670 - 370;
    
    ele = &pool[RESULT_WIN_TEST_DATA_C];
    
    ele->dis_info.base_x = 0;
    ele->dis_info.base_y = 0;
    ele->dis_info.pos_size.x = 370;
    ele->dis_info.pos_size.y = 300;
    ele->dis_info.back_color = GUI_INVALID_COLOR;
    ele->dis_info.font = FONT_RES_WIN;
    ele->dis_info.font_color = GUI_WHITE;
    ele->dis_info.max_len = 100;
    ele->dis_info.align = GUI_TA_LEFT | GUI_TA_TOP;
    ele->dis_info.pos_size.height = 140;
    ele->dis_info.pos_size.width = 670 - 370;
}    

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
