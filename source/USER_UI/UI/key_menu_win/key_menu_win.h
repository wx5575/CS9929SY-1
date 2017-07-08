/**
  ******************************************************************************
  * @file    key_menu_win.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
#ifndef __KEY_MENU_WIN_H__
#define __KEY_MENU_WIN_H__

#include "UI_COM/com_ui_info.h"

/**
  * @brief  菜单键项枚举索引
  */
typedef enum{
    KEY_MENU_F0,
    KEY_MENU_F1,
    KEY_MENU_F2,
    KEY_MENU_F3,
    KEY_MENU_F4,
    KEY_MENU_F5,
    KEY_MENU_F6,
}KEY_MENU_ENUM;


#define MENU_KEY_NUM			7 /* 按键个数 */


/**
  * @brief  系统标准菜单键初始化信息结构
  */
typedef struct{
	uint8_t *text[2];///<初始化菜单键文本，包含中英文
	CS_INDEX index;///<对初始化信息结构数组的索引
}STAND_MENU_KEY_INFO_T;


#endif //__KEY_MENU_WIN_H__


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
