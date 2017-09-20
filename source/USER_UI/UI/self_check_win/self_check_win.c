/**
  ******************************************************************************
  * @file    self_check_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   自检窗口
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
#include "7_self_check_win.h"
#include "self_check_win.h"
#include "PROGBAR.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void self_check_win_cb(WM_MESSAGE * pMsg);
static void update_main_ui_menu_key_inf(WM_HMEM hWin);


static void self_check_win_f1_cb(KEY_MESSAGE *key_msg);
static void self_check_win_f2_cb(KEY_MESSAGE *key_msg);
static void self_check_win_f3_cb(KEY_MESSAGE *key_msg);
static void self_check_win_f4_cb(KEY_MESSAGE *key_msg);
static void self_check_win_f5_cb(KEY_MESSAGE *key_msg);
static void self_check_win_f6_cb(KEY_MESSAGE *key_msg);

static void update_key_inf(WM_HWIN hWin);
static void init_create_self_check_win_text_ele(MYUSER_WINDOW_T* win);
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
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* self_check_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_self_check_windows,/*4.3寸屏*/
    &_7_self_check_windows,/*5.6寸屏*/
    &_7_self_check_windows,/*7寸屏*/
};
/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	main_ui_menu_key_inf[] = 
{
    {"", F_KEY_DISPLAY  , KEY_F0 & _KEY_UP, 0             , SYS_KEY_DIS},//f0
    {"", F_KEY_FILE		, KEY_F1 & _KEY_UP, self_check_win_f1_cb, SYS_KEY_EN },//f1
    {"", F_KEY_STEP		, KEY_F2 & _KEY_UP, self_check_win_f2_cb, SYS_KEY_EN},//f2
    {"", F_KEY_SYS		, KEY_F3 & _KEY_UP, self_check_win_f3_cb, SYS_KEY_EN },//f3
    {"", F_KEY_RESULT   , KEY_F4 & _KEY_UP, self_check_win_f4_cb, SYS_KEY_EN},//f4
    {"", F_KEY_TEST		, KEY_F5 & _KEY_UP, self_check_win_f5_cb, SYS_KEY_EN},//f5
    {"", F_KEY_HELP		, KEY_F6 & _KEY_UP, self_check_win_f6_cb, SYS_KEY_DIS },//f6
};
CS_INDEX self_check_win_test_ele_index_pool[]=
{
    SELF_CHECK_UI_CONTENT,///<自检内容文本
};
/**
  * @brief  界面的文本对象池
  */
static TEXT_ELE_T self_check_ui_text_ele_pool[]=
{
//	{{"本控","LOCAL"}, MAIN_UI_COM_ST },
	{{"1.电源检查     .............. OK\n\n"
      "2.键盘检查     .............. OK\n\n"
      "3.存储检查     .............. OK\n\n"
      "4.电压检查     .............. OK\n\n"
      "5.电流检查     .............. OK\n\n"
      "6.系统参数检查 .............. OK\n\n"
        ,
      "1.Power check  .............. OK\n"
      "2.Key check    .............. OK\n"
      "3.Memory check .............. OK\n"
      "4.Voltage check.............. OK\n"
      "5.Current check.............. OK\n"
      "5.Sys.Par.check.............. OK\n"
    }, SELF_CHECK_UI_CONTENT },
};
/**
  * @brief  自检窗口结构体初始化
  */
MYUSER_WINDOW_T self_check_windows=
{
    {"自检窗口", "Self check Window"},
    self_check_win_cb, NULL,
	{
        self_check_ui_text_ele_pool,COUNT_ARRAY_SIZE(self_check_ui_text_ele_pool),
        (CS_INDEX*)self_check_win_test_ele_index_pool,
        COUNT_ARRAY_SIZE(self_check_win_test_ele_index_pool),
        init_create_self_check_win_text_ele
    },/* text */
    {0},
    {0},
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    self_check_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void init_self_check_win_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_self_check_win_layout1_text_ele_pos(self_check_ui_text_ele_pool);
            break;
    }
}
/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_self_check_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_self_check_win_text_ele_pos_inf();
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
//    update_result_win_text_ele_text(win);
}
/**
  * @brief  主窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void self_check_win_f1_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(create_file_win, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        create_file_win(key_msg->user_data);
    }
}
/**
  * @brief  主窗口中功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void self_check_win_f2_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(create_step_par_win, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        create_step_par_win(key_msg->user_data);
    }
}
/**
  * @brief  主窗口中功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void self_check_win_f3_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(create_sys_manager_win, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        create_sys_manager_win(key_msg->user_data);
    }
}
/**
  * @brief  主窗口中功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void self_check_win_f4_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(create_result_win, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        create_result_win(key_msg->user_data);
    }
}
/**
  * @brief  主窗口中功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void self_check_win_f5_cb(KEY_MESSAGE *key_msg)
{
    create_test_win(key_msg->user_data);
}
/**
  * @brief  主窗口中功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void self_check_win_f6_cb(KEY_MESSAGE *key_msg)
{
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
static void draw_self_check_win_status_bar(void)
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
static void update_main_ui_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(main_ui_menu_key_inf, ARRAY_SIZE(main_ui_menu_key_inf), hWin);
}



/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_system_key_inf(WM_HMEM hWin)
{
}

/**
  * @brief  设置主界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_self_check_windows_handle(WM_HWIN hWin)
{
}

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void self_check_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_main_ui_menu_key_inf(hWin);
    update_system_key_inf(hWin);
}

/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void self_check_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
            init_create_win_all_ele(win);
			WM_SetFocus(hWin);/* 设置聚焦 */
			break;
		case WM_PAINT:
			self_check_win_paint_frame();
            create_slogo_image(hWin);
			break;
		case WM_KEY:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  创建自检窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_self_check_win(int hWin)
{
    create_user_window(&self_check_windows, &windows_list, hWin);//创建文件管理界面
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
