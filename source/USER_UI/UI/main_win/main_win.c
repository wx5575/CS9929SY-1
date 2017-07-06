/**
  ******************************************************************************
  * @file    main_ui_com.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   主窗口文件
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
#include "key_menu_win/key_menu_win.h"
#include "sys_win/sys_manage_win.h"
#include "file_win/file_win.h"
#include "test_win/test_win.h"
#include "cs99xx_usb_manager.h"
#include "step_par_win/step_win.h"
#include "result_win/result_win.h"
#include "password_win/input_password_win.h"
#include "key_server.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "7_main_win.h"
#include "main_win.h"
#include "PROGBAR.h"
#include "cs99xx_mem_api.h"
  
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

static void main_win_f1_cb(KEY_MESSAGE *key_msg);
static void main_win_f2_cb(KEY_MESSAGE *key_msg);
static void main_win_f3_cb(KEY_MESSAGE *key_msg);
static void main_win_f4_cb(KEY_MESSAGE *key_msg);
static void main_win_f5_cb(KEY_MESSAGE *key_msg);
static void main_win_f6_cb(KEY_MESSAGE *key_msg);

static void update_key_inf(WM_HWIN hWin);
/* Private variables ---------------------------------------------------------*/

static	WM_HWIN progbar_handle;///<进度条
static	WM_HWIN timer_handle;///<定时器句柄
static	WM_HWIN U_FLASH_1_handle;///<U盘图标1句柄
static	WM_HWIN U_FLASH_2_handle;///<U盘图标2句柄
static	WM_HWIN KEY_LOCK_handle;///<键盘锁图标句柄
static	WM_HWIN KEY_CAPITAL_SMALL_handle;///<大小写图标句柄
/** 主界面显示的文本索引表 */
static CS_INDEX main_ui_text_ele_table[] =
{
	MAIN_UI_COM_ST,
	MAIN_UI_SYS_TIME,
};
/**
  * @brief  系统按键信息
  */
static CONFIG_FUNCTION_KEY_INFO_T sys_key_pool[]=
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
	{{"本控","LOCAL"}, MAIN_UI_COM_ST },
	{{"2017-04-07 08:59:00","2017-04-07 08:59:00"}, MAIN_UI_SYS_TIME },
};
/**
  * @brief  主窗口结构体初始化
  */
MYUSER_WINDOW_T main_windows=
{
    {"主窗口", "Main Window"},
    main_win_cb, update_main_ui_menu_key_inf,
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
  * @brief  绘制主界面状态栏
  * @param  无
  * @retval 无
  */
static void draw_main_win_status_bar(void)
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
            GUI_FillRectEx(&_7_sys_st_info_area);
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
  * @brief  更新上档键的图标显示
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_shift_bmp(void)
{
    uint8_t flag = get_shift_status();
    
    if(flag)
    {
        set_capital_letter_image(KEY_CAPITAL_SMALL_handle);
    }
    else
    {
        set_small_letter_image(KEY_CAPITAL_SMALL_handle);
    }
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
    
    update_key_inf(main_windows.handle);
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
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_shift_status();
    
    set_shift_status(!flag);
    update_shift_bmp();
}

/**
  * @brief  更新键盘锁图标
  * @param  无
  * @retval 无
  */
void update_unlock_bmp(void)
{
    uint8_t flag = get_key_lock_flag();
    
    if(flag)
    {
        set_key_lock_image(KEY_LOCK_handle);
    }
    else
    {
        set_key_unlock_image(KEY_LOCK_handle);
    }
}

/**
  * @brief  改变键盘锁状态
  * @param  [in] data 用户数据
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
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg)
{
    init_back_up_will_enter_win_inf(change_key_lock_status, main_windows.handle);
}

/**
  * @brief  截屏按键的
  * @param  [in] data 用户数据
  * @retval 无
  */
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg)
{
    int32_t data = key_msg->user_data;
    
    set_usb_disk_task(USB_SCREEN_CAPTURE);
    //创建进度条
    progbar_handle = PROGBAR_CreateEx(100, 455, 50, 20, data, WM_CF_HIDE, 0, id_base++);
}
/**
  * @brief  删除主窗口进度条
  * @param  无
  * @retval 无
  */
void delete_main_win_progbar(void)
{
    WM_DeleteWindow(progbar_handle);//删除窗口控件
    progbar_handle = 0;//清除被删除窗口的句柄
}
/**
  * @brief  设置主窗口进度条的进度
  * @param  value 进度值
  * @retval 无
  */
void set_main_win_progbar_value(int32_t value)
{
    PROGBAR_SetValue(progbar_handle, value);
}
/**
  * @brief  设置主窗口进度条从隐藏变为可见
  * @param  无
  * @retval 无
  */
void set_main_win_progbar_show(void)
{
    WM_ShowWindow(progbar_handle);
}
/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}

/**
  * @brief  设置主界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_main_windows_handle(WM_HWIN hWin)
{
	main_windows.handle = hWin;
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
static void update_key_inf(WM_HWIN hWin)
{
    update_main_ui_menu_key_inf(hWin);
    update_system_key_inf(hWin);
}

/**
  * @brief  更新USB的显示状态
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void update_usb_dis_status(void)
{
    uint8_t st = 0;
    
    st = get_ch376_status(1);
    
    if(st == 1)
    {
        set_usb_disk_1_ok_image(U_FLASH_1_handle);
    }
    else
    {
        set_usb_disk_1_ng_image(U_FLASH_1_handle);
    }
    
    st = get_ch376_status(2);
    
    if(st == 1)
    {
        set_usb_disk_2_ok_image(U_FLASH_2_handle);
    }
    else
    {
        set_usb_disk_2_ng_image(U_FLASH_2_handle);
    }
    
    update_unlock_bmp();
    update_shift_bmp();
}

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
			set_main_windows_handle(hWin);
			win = get_user_window_info(hWin);
            
			WM_SetFocus(hWin);/* 设置聚焦 */
            update_key_inf(hWin);
            
            init_main_ui_text_ele_pos_inf();//初始化文本对象的位置信息
            init_window_text_ele_list(win);//初始化窗口文本对象链表
			init_window_text_ele(win);
			timer_handle = WM_CreateTimer(hWin, 0, 1000, 0);
            create_logo_image(hWin);
            U_FLASH_1_handle = create_u_flash_1_image(hWin, 5 + 25 * 0, 480 - 24);
            U_FLASH_2_handle = create_u_flash_2_image(hWin, 5 + 25 * 1, 480 - 24);
            KEY_LOCK_handle = create_key_lock_image(hWin, 5 + 25 * 2, 480 - 24);
            KEY_CAPITAL_SMALL_handle = create_capital_small_letter_image(hWin, 5 + 25 * 3, 480 - 24);
            
            set_usb_disk_1_ng_image(U_FLASH_1_handle);
            set_usb_disk_2_ng_image(U_FLASH_2_handle);
            set_key_unlock_image(KEY_LOCK_handle);
            set_small_letter_image(KEY_CAPITAL_SMALL_handle);
			break;
		case WM_PAINT:
			_PaintFrame();
            draw_main_win_status_bar();
			break;
		case WM_TIMER:
		{
            uint8_t *str = get_time_str(0);
			win = get_user_window_info(hWin);
            main_ui_text_ele_pool[MAIN_UI_SYS_TIME].text[CHINESE] = str;
            main_ui_text_ele_pool[MAIN_UI_SYS_TIME].text[ENGLISH] = str;
			update_text_ele((CS_INDEX)MAIN_UI_SYS_TIME, win, str);
			WM_RestartTimer(timer_handle, 1000);
			break;
		}
		case WM_KEY:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/**
  * @brief  创建主界面
  * @param  无
  * @retval 无
  */
static void create_main_windows(void)
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
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  主窗口的入口
  * @param  无
  * @retval 无
  */
void main_ui_enter(void)
{
	SCREEM_SIZE = SCREEN_7INCH;
	id_base = GUI_ID_USER;//窗口控件ID
    init_user_window_env();//初始化用户窗口环境
    create_key_menu_window();//创建按键界面
    create_main_windows();//创建主界面
	
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
