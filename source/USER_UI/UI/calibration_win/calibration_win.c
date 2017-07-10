/**
  ******************************************************************************
  * @file    calibration_win.c
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
#include "7_calibration_win.h"
#include "calibration_win.h"
#include "PROGBAR.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void calibration_win_cb(WM_MESSAGE * pMsg);
static void update_calibration_win_menu_key_inf(WM_HMEM hWin);

static void sys_exit_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_stop_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg);
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg);

static void calibration_win_f1_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f2_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f3_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f4_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f5_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f6_cb(KEY_MESSAGE *key_msg);

static void calibration_win_update_key_inf(WM_HWIN hWin);
/* Private variables ---------------------------------------------------------*/

static	WM_HWIN timer_handle;///<定时器句柄
static	WM_HWIN U_FLASH_1_handle;///<U盘图标1句柄
static	WM_HWIN U_FLASH_2_handle;///<U盘图标2句柄
static	WM_HWIN KEY_LOCK_handle;///<键盘锁图标句柄
static	WM_HWIN KEY_CAPITAL_SMALL_handle;///<大小写图标句柄
/** 主界面显示的文本索引表 */
//static CS_INDEX main_ui_text_ele_table[] =
//{
//	MAIN_UI_COM_ST,
//	MAIN_UI_SYS_TIME,
//};
/**
  * @brief  系统按键信息
  */
static CONFIG_FUNCTION_KEY_INFO_T calibration_win_sys_key_pool[]=
{
	{KEY_SHIFT	    , sys_shift_key_fun_cb      },
	{KEY_UNLOCK	    , sys_unlock_key_fun_cb     },
	{KEY_EXIT	    , sys_exit_key_fun_cb       },
	{KEY_STOP	    , sys_stop_key_fun_cb       },
	{KEY_F1 & KEY_0 , screen_capture_key_fun_cb },
};
/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* calibration_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_calibration_windows,/*4.3寸屏*/
    &_7_calibration_windows,/*5.6寸屏*/
    &_7_calibration_windows,/*7寸屏*/
};
/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	calibration_win_menu_key_inf[] = 
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, calibration_win_f1_cb },//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, calibration_win_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, calibration_win_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, calibration_win_f4_cb },//f4
    {"", F_KEY_ENTER    , KEY_F5 & _KEY_UP, calibration_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, calibration_win_f6_cb },//f6
};
/**
  * @brief  主界面的文本对象池
  */
//static TEXT_ELE_T main_ui_text_ele_pool[]=
//{
//	{{"本控","LOCAL"}, MAIN_UI_COM_ST },
//	{{"2017-04-07 08:59:00","2017-04-07 08:59:00"}, MAIN_UI_SYS_TIME },
//};
/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T calibration_windows=
{
    {"校准管理窗口", "Calibration Window"},
    calibration_win_cb, calibration_win_update_key_inf,
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
    calibration_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  主窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}
/**
  * @brief  根据屏幕尺寸初始化主界面的文本对象位置尺寸信息
  * @param  无
  * @retval 无
  */
static void init_main_ui_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
    case SCREEN_4_3INCH:
        break;
    case SCREEN_6_5INCH:
        break;
    default:
    case SCREEN_7INCH:
        break;
    }
}
/**
  * @brief  绘制主界面状态栏
  * @param  无
  * @retval 无
  */
static void draw_calibration_win_status_bar(void)
{
    GUI_SetColor(GUI_LIGHTGRAY);
    
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            break;
    }
}

/**
  * @brief  更新主界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_calibration_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(calibration_win_menu_key_inf, ARRAY_SIZE(calibration_win_menu_key_inf), hWin);
}



/**
  * @brief  系统EXIT按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_exit_key_fun_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  系统STOP按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_stop_key_fun_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  系统SHIFT按键回调函数
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg)
{
}



/**
  * @brief  系统键盘锁按键回调函数
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  截屏按键的
  * @param  [in] data 用户数据
  * @retval 无
  */
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void calibration_win_update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(calibration_win_sys_key_pool, ARRAY_SIZE(calibration_win_sys_key_pool), hWin);
}

/**
  * @brief  设置主界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_calibration_windows_handle(WM_HWIN hWin)
{
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
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void calibration_win_update_key_inf(WM_HWIN hWin)
{
    update_calibration_win_menu_key_inf(hWin);
    calibration_win_update_system_key_inf(hWin);
}


/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void calibration_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_calibration_windows_handle(hWin);
			win = get_user_window_info(hWin);
            
//            WM_CreateTimer(hWin, 0, 1000, 0);
			break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case WM_TIMER:
		{
//            back_win(hWin);
			break;
		}
		case WM_KEY:
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
void create_calibration_win(int hWin)
{
    create_user_window(&calibration_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/