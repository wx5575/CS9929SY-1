/**
  ******************************************************************************
  * @file    main_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   主窗口文件
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "FILE_SYS.H"
#include "image/user_image.h"
#include "image/logo.h"
#include "UI_COM/com_ui_info.h"
#include "rtc_config.h"
#include "7_main_win.h"
#include "main_win.h"
#include "PROGBAR.h"
#include "mem_alloc.h"
#include "app.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void main_win_cb(WM_MESSAGE * pMsg);
static void update_main_ui_menu_key_inf(WM_HMEM hWin);

static void sys_exit_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_stop_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg);
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg);
static void copy_file_to_sdcard_key_fun_cb(KEY_MESSAGE *key_msg);
static void reset_cpu_key_fun_cb(KEY_MESSAGE *key_msg);

static void main_win_f1_cb(KEY_MESSAGE *key_msg);
static void main_win_f2_cb(KEY_MESSAGE *key_msg);
static void main_win_f3_cb(KEY_MESSAGE *key_msg);
static void main_win_f4_cb(KEY_MESSAGE *key_msg);
static void main_win_f5_cb(KEY_MESSAGE *key_msg);
static void main_win_f6_cb(KEY_MESSAGE *key_msg);

static void main_win_update_key_inf(WM_HWIN hWin);
/* Private variables ---------------------------------------------------------*/

/** 主界面显示的文本索引表 */
static CS_INDEX main_ui_text_ele_table[] =
{
    MAIN_UI_CSALLWIN,
    MAIN_UI_VERSION,
    MAIN_UI_TYPE,
    MAIN_UI_WWW,///<主界面的公司网站
    MAIN_UI_WTS,///<主界面的微信技术服务
};
/**
  * @brief  系统按键信息
  */
static CONFIG_FUNCTION_KEY_INFO_T main_win_sys_key_pool[]=
{
	{KEY_SHIFT	    , sys_shift_key_fun_cb      },
	{KEY_UNLOCK	    , sys_unlock_key_fun_cb     },
	{KEY_EXIT	    , sys_exit_key_fun_cb       },
	{KEY_STOP	    , sys_stop_key_fun_cb       },
	{KEY_F1 & KEY_0 , screen_capture_key_fun_cb },
	{KEY_F1 & KEY_1 , copy_file_to_sdcard_key_fun_cb },
	{KEY_UNLOCK & KEY_OFFSET , reset_cpu_key_fun_cb },
};
/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* main_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_main_windows,/*4.3寸屏*/
    &_7_main_windows,/*5.6寸屏*/
    &_7_main_windows,/*7寸屏*/
};
/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	main_ui_menu_key_inf[] = 
{
    {"", F_KEY_DISPLAY  , KEY_F0 & _KEY_UP, 0             , SYS_KEY_DIS},//f0
    {"", F_KEY_FILE		, KEY_F1 & _KEY_UP, main_win_f1_cb, SYS_KEY_EN },//f1
    {"", F_KEY_STEP		, KEY_F2 & _KEY_UP, main_win_f2_cb, SYS_KEY_EN},//f2
    {"", F_KEY_SYS		, KEY_F3 & _KEY_UP, main_win_f3_cb, SYS_KEY_EN },//f3
    {"", F_KEY_RESULT   , KEY_F4 & _KEY_UP, main_win_f4_cb, SYS_KEY_EN},//f4
    {"", F_KEY_TEST		, KEY_F5 & _KEY_UP, main_win_f5_cb, SYS_KEY_EN},//f5
    {"", F_KEY_HELP		, KEY_F6 & _KEY_UP, main_win_f6_cb, SYS_KEY_DIS },//f6
};
/**
  * @brief  主界面的文本对象池
  */
static TEXT_ELE_T main_ui_text_ele_pool[]=
{
	{{"南京长盛仪器","CSALLWIN"}, MAIN_UI_CSALLWIN },
	{{"版本:1.0.0","Ver:1.0.0"}, MAIN_UI_VERSION },
	{{"型号:CS9929SY","Type:CS9929SY"}, MAIN_UI_TYPE },
	{{"公司网站","web sites"}, MAIN_UI_WWW },
	{{"微信技术服务","WeChat services"}, MAIN_UI_WTS },
};
/**
  * @brief  主窗口结构体初始化
  */
MYUSER_WINDOW_T main_windows=
{
    {"主窗口", "Main Window"},
    main_win_cb, main_win_update_key_inf,
	{
        main_ui_text_ele_pool, ARRAY_SIZE(main_ui_text_ele_pool),
        (CS_INDEX*)main_ui_text_ele_table, ARRAY_SIZE(main_ui_text_ele_table)
    },
    {0},
    {0},
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    main_win_pos_size_pool,/*pos_size_pool */
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  主窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void main_win_f1_cb(KEY_MESSAGE *key_msg)
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
static void main_win_f2_cb(KEY_MESSAGE *key_msg)
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
static void main_win_f3_cb(KEY_MESSAGE *key_msg)
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
static void main_win_f4_cb(KEY_MESSAGE *key_msg)
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
static void main_win_f5_cb(KEY_MESSAGE *key_msg)
{
    create_test_win(key_msg->user_data);
}
/**
  * @brief  主窗口中功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void main_win_f6_cb(KEY_MESSAGE *key_msg)
{
    create_help_win(key_msg->user_data);
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
        _7_init_main_ui_text_ele_pos(main_ui_text_ele_pool);
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
  * @brief  系统EXIT按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_exit_key_fun_cb(KEY_MESSAGE *key_msg)
{
	MYUSER_WINDOW_T *node;
	CS_LIST *index = NULL;
	CS_LIST *list = &windows_list;
    
	list_for_each_reverse(index, list)
	{
		node = list_entry(index, MYUSER_WINDOW_T, w_list);
        
        if(node->handle == main_windows.handle)
        {
            break;
        }
        
        del_cur_window();
	}
    
    main_win_update_key_inf(main_windows.handle);
    show_cur_window();
}
/**
  * @brief  系统STOP按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_stop_key_fun_cb(KEY_MESSAGE *key_msg)
{
	MYUSER_WINDOW_T *node;
	CS_LIST *index = NULL;
	CS_LIST *list = &windows_list;
    
	list_for_each_reverse(index, list)
	{
		node = list_entry(index, MYUSER_WINDOW_T, w_list);
        
        if(node->handle == main_windows.handle)
        {
            break;
        }
        
        del_cur_window();
	}
    
    create_test_win(main_windows.handle);//进入测试界面
}
/**
  * @brief  系统SHIFT按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_shift_status();
    
    set_shift_status(!flag);
    update_shift_bmp();
}

/**
  * @brief  改变键盘锁状态
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void change_key_lock_status(int data)
{
    uint8_t flag = get_key_lock_flag();
    
    set_key_lock_flag(!flag);
    update_unlock_bmp();
    save_sys_par();
}
/**
  * @brief  系统键盘锁按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg)
{
    init_back_up_will_enter_win_inf(change_key_lock_status, main_windows.handle);
}

/**
  * @brief  截屏按键的
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg)
{
    set_usb_disk_task(USB_SCREEN_CAPTURE);
    
    //创建进度条
    create_status_bar_win_progbar();
}
/**
  * @brief  截屏按键的
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void copy_file_to_sdcard_key_fun_cb(KEY_MESSAGE *key_msg)
{
    set_usb_disk_task(USB_COPY_FILE);
    
    //创建进度条
    create_status_bar_win_progbar();
}
/**
  * @brief  重启仪器
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void reset_cpu_key_fun_cb(KEY_MESSAGE *key_msg)
{
    NVIC_SystemReset();
}

/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(main_win_sys_key_pool, ARRAY_SIZE(main_win_sys_key_pool), hWin);
}

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void main_win_paintframe(void) 
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
static void main_win_update_key_inf(WM_HWIN hWin)
{
    update_main_ui_menu_key_inf(hWin);
    update_system_key_inf(hWin);
}

void test_ttf_font(void);
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void main_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
            
			WM_SetFocus(hWin);/* 设置聚焦 */
            main_win_update_key_inf(hWin);
            
            init_main_ui_text_ele_pos_inf();//初始化文本对象的位置信息
            init_window_text_ele_list(win);//初始化窗口文本对象链表
			init_window_text_ele(win);
            
            create_slogo_image(hWin);
            create_www_qr_code_image(hWin);
            create_wts_qr_code_image(hWin);
			break;
		case WM_PAINT:
			main_win_paintframe();
			break;
		case WM_KEY:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/**
  * @brief  隐藏主界面
  * @param  无
  * @retval 无
  */
void hide_main_win(void)
{
    WM_HideWindow(main_windows.handle);
}
/**
  * @brief  显示主界面
  * @param  无
  * @retval 无
  */
void show_main_win(void)
{
    WM_ShowWindow(main_windows.handle);
}
/**
  * @brief  创建主界面
  * @param  无
  * @retval 无
  */
void create_main_windows(void)
{
    create_user_window(&main_windows, &windows_list, 0);
}
/**
  * @brief  初始化用户窗口环境
  * @param  无
  * @retval 无
  */
static void init_user_window_env(void)
{
	list_init(&windows_list);//初始化窗口链表
}
/**
  * @brief  进入自检窗口
  * @param  无
  * @retval 无
  */
static void into_self_check_win(void)//进入自检窗口
{
    create_self_check_win(0);//创建自检窗口
}
/**
  * @brief  进入启动窗口
  * @param  无
  * @retval 无
  */
static void into_start_win(void)//进入启动窗口
{
    create_start_win(0);//创建启动窗口
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  主窗口的入口
  * @param  无
  * @retval 无
  */

void main_ui_enter(void)
{
	SCREEM_SIZE = SCREEN_7INCH;//设置为7寸屏
	id_base = GUI_ID_USER;//窗口控件ID
    init_user_window_env();//初始化用户窗口环境
    into_start_win();//进入启动窗口
    GUI_Delay(1000);//调用这个函数可以刷新界面
    into_self_check_win();//进入自检窗口
    GUI_Delay(2000);//调用这个函数可以刷新界面
    while(self_check_ok == 0)
    {
        GUI_Delay(1);//调用这个函数可以刷新界面
    }
    create_key_menu_window();//创建按键界面
    create_status_bar_windows();
    create_main_windows();//创建主界面
    read_par_from_memory();//从存储器读取参数
    
	
	while(1)
	{
		GUI_Delay(1);//调用这个函数可以刷新界面
		
		/* 响应按键回调函数 */
        if(golbal_key_info.fun != NULL && golbal_key_info.en == SYS_KEY_EN)
        {
            golbal_key_info.fun(&golbal_key_info.msg);
            golbal_key_info.fun = NULL;
        }
	}
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
