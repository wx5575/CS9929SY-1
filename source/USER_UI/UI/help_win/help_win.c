/**
  ******************************************************************************
  * @file    help_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   启动窗口
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "FILE_SYS.H"
#include "image/user_image.h"
#include "image/logo.h"
#include "UI_COM/com_ui_info.h"
#include "rtc_config.h"
#include "7_help_win.h"
#include "help_win.h"
#include "PROGBAR.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void help_win_f1_cb(KEY_MESSAGE *key_msg);
static void help_win_f2_cb(KEY_MESSAGE *key_msg);
static void help_win_f3_cb(KEY_MESSAGE *key_msg);
static void help_win_f4_cb(KEY_MESSAGE *key_msg);
static void help_win_f5_cb(KEY_MESSAGE *key_msg);
static void help_win_f6_cb(KEY_MESSAGE *key_msg);


static void help_win_update_key_inf(WM_HWIN hWin);
static void help_win_cb(WM_MESSAGE * pMsg);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* help_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_help_windows,/*4.3寸屏*/
    &_7_help_windows,/*5.6寸屏*/
    &_7_help_windows,/*7寸屏*/
};


/** 帮助界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	help_win_menu_key_inf[] = 
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, help_win_f1_cb, SYS_KEY_EN},//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, help_win_f2_cb, SYS_KEY_EN},//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, help_win_f3_cb, SYS_KEY_EN},//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, help_win_f4_cb, SYS_KEY_EN},//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, help_win_f5_cb, SYS_KEY_EN},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, help_win_f6_cb, SYS_KEY_EN},//f6
};

/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T help_windows=
{
    {"帮助窗口", "Help Window"},
    help_win_cb,    help_win_update_key_inf,
	{0},
    {0},
    {0},
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    help_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void help_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
static void help_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
static void help_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void help_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void help_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void help_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  更新主界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_help_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(help_win_menu_key_inf, ARRAY_SIZE(help_win_menu_key_inf), hWin);
}
/**
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void help_win_update_key_inf(WM_HWIN hWin)
{
    update_help_win_menu_key_inf(hWin);
//    update_help_win_system_key_inf(hWin);
}
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void help_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_user_window_handle(hWin);
            create_logo_imagex(hWin);
			win = get_user_window_info(hWin);
            
            if(win->init_key_fun != NULL)
            {
                win->init_key_fun(hWin);
            }
            
            help_win_update_key_inf(hWin);
			break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case WM_KEY:
			break;
		case WM_DELETE:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建启动窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_help_win(int hWin)
{
    create_user_window(&help_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
