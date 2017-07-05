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

#include "key_fun_manage.h"
#include "cs99xx_struct.h"

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


/** 系统标准菜单键初始化信息结构 */
typedef struct{
	uint8_t *text[2];///<初始化菜单键文本，包含中英文
	CS_INDEX index;///<对初始化信息结构数组的索引
}STAND_MENU_KEY_INFO_T;

extern void create_key_menu_window(void);
extern void unregister_system_key_fun(CONFIG_FUNCTION_KEY_INFO_T info[], uint32_t n);
extern void register_system_key_fun(CONFIG_FUNCTION_KEY_INFO_T info[], uint32_t n, int data);
extern void init_menu_key_info(MENU_KEY_INFO_T * info, uint32_t n, int data);
extern void set_menu_function_status(uint32_t key_value, SYS_KEY_ST_ENUM st);
extern void set_menu_key_config_st(MENU_KEY_INFO_T * inf, uint32_t size,
                        CS_INDEX index, SYS_KEY_ST_ENUM st, CS_ERR *err);
extern void change_menu_key_font_color(uint32_t key_value, GUI_COLOR color);

#endif //__KEY_MENU_WIN_H__


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
