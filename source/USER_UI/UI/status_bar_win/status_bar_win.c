/**
  ******************************************************************************
  * @file    status_bar_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   状态栏
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
#include "7_status_bar_win.h"
#include "status_bar_win.h"
#include "PROGBAR.h"
#include "mem_alloc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void status_bar_win_cb(WM_MESSAGE * pMsg);
/* Private variables ---------------------------------------------------------*/

static	WM_HWIN progbar_handle;///<进度条
static	WM_HWIN timer_handle;///<定时器句柄
static	WM_HWIN U_FLASH_1_handle;///<U盘图标1句柄
static	WM_HWIN U_FLASH_2_handle;///<U盘图标2句柄
static	WM_HWIN KEY_LOCK_handle;///<键盘锁图标句柄
static	WM_HWIN KEY_CAPITAL_SMALL_handle;///<大小写图标句柄
/** 主界面显示的文本索引表 */
static CS_INDEX status_bar_win_text_ele_table[] =
{
	STATUS_BAR_WIN_COM_ST,
	STATUS_BAR_WIN_SYS_TIME,
};

/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* status_bar_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_status_bar_windows,/*4.3寸屏*/
    &_7_status_bar_windows,/*5.6寸屏*/
    &_7_status_bar_windows,/*7寸屏*/
};

/**
  * @brief  主界面的文本对象池
  */
static TEXT_ELE_T status_bar_win_text_ele_pool[]=
{
	{{"本控","LOCAL"}, STATUS_BAR_WIN_COM_ST },
	{{"2017-04-07 08:59:00","2017-04-07 08:59:00"}, STATUS_BAR_WIN_SYS_TIME },
};
/**
  * @brief  状态栏结构体初始化
  */
MYUSER_WINDOW_T status_bar_windows=
{
    {"状态栏", "Status Bar"},
    status_bar_win_cb, 0,
	{
        status_bar_win_text_ele_pool, ARRAY_SIZE(status_bar_win_text_ele_pool),
        (CS_INDEX*)status_bar_win_text_ele_table, ARRAY_SIZE(status_bar_win_text_ele_table)
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
    status_bar_win_pos_size_pool,/*pos_size_pool */
};
/**
  * @brief  指向状态栏的this指针
  */
static MYUSER_WINDOW_T *this_win = &status_bar_windows;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  根据屏幕尺寸初始化主界面的文本对象位置尺寸信息
  * @param  无
  * @retval 无
  */
static void init_status_bar_win_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_status_bar_win_text_ele_pos(status_bar_win_text_ele_pool);
            break;
    }
}
/**
  * @brief  绘制主界面状态栏
  * @param  无
  * @retval 无
  */
static void draw_status_bar_win_status_bar(void)
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
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void status_bar_win_paintframe(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void status_bar_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
            WM_SetStayOnTop(hWin, 1);//窗口保持在顶部
			set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
            
            init_status_bar_win_text_ele_pos_inf();//初始化文本对象的位置信息
            init_window_text_ele_list(win);//初始化窗口文本对象链表
			init_window_text_ele(win);
			timer_handle = WM_CreateTimer(hWin, 0, 1000, 0);
            
            /* 状态栏图标 */
            U_FLASH_1_handle = create_u_flash_1_image(hWin, 5 + 25 * 0, 2);
            U_FLASH_2_handle = create_u_flash_2_image(hWin, 5 + 25 * 1, 2);
            KEY_LOCK_handle = create_key_lock_image(hWin, 5 + 25 * 2, 2);
            KEY_CAPITAL_SMALL_handle = create_capital_small_letter_image(hWin, 5 + 25 * 3, 2);
            
            /* 设置图标内容 */
            set_usb_disk_1_ng_image(U_FLASH_1_handle);
            set_usb_disk_2_ng_image(U_FLASH_2_handle);
            set_key_unlock_image(KEY_LOCK_handle);
            set_small_letter_image(KEY_CAPITAL_SMALL_handle);
			break;
		case WM_PAINT:
			status_bar_win_paintframe();
            draw_status_bar_win_status_bar();
			break;
		case WM_TIMER:
		{
            uint8_t *str = get_time_str(0);
			win = get_user_window_info(hWin);
            status_bar_win_text_ele_pool[STATUS_BAR_WIN_SYS_TIME].text[CHINESE] = str;
            status_bar_win_text_ele_pool[STATUS_BAR_WIN_SYS_TIME].text[ENGLISH] = str;
			update_text_ele((CS_INDEX)STATUS_BAR_WIN_SYS_TIME, win, str);
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

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  更新上档键的图标显示
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
void update_shift_bmp(void)
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
  * @brief  创建状态栏进度条
  * @param  无
  * @retval 无
  */
void create_status_bar_win_progbar(void)
{
    progbar_handle = PROGBAR_CreateEx(105, 0, 50, 20, this_win->handle,
            WM_CF_HIDE, 0, id_base++);
}
/**
  * @brief  删除状态栏进度条
  * @param  无
  * @retval 无
  */
void delete_status_bar_win_progbar(void)
{
    WM_DeleteWindow(progbar_handle);//删除窗口控件
    progbar_handle = 0;//清除被删除窗口的句柄
}
/**
  * @brief  设置状态栏进度条的进度
  * @param  value 进度值
  * @retval 无
  */
void set_status_bar_win_progbar_value(int32_t value)
{
    PROGBAR_SetValue(progbar_handle, value);
}
/**
  * @brief  设置状态栏进度条从隐藏变为可见
  * @param  无
  * @retval 无
  */
void set_status_bar_win_progbar_show(void)
{
    WM_ShowWindow(progbar_handle);
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
  * @brief  通信进入远控状态
  * @param  无
  * @retval 无
  */
void com_st_into_remote(void)
{
    status_bar_win_text_ele_pool[STATUS_BAR_WIN_COM_ST].text[CHINESE] = "远控";
    status_bar_win_text_ele_pool[STATUS_BAR_WIN_COM_ST].text[ENGLISH] = "REMOTE";
    set_text_ele_font_color(STATUS_BAR_WIN_COM_ST, this_win, GUI_RED);
    update_text_ele(STATUS_BAR_WIN_COM_ST, this_win, NULL);
}
/**
  * @brief  通信退出远控状态
  * @param  无
  * @retval 无
  */
void com_st_exit_remote(void)
{
    status_bar_win_text_ele_pool[STATUS_BAR_WIN_COM_ST].text[CHINESE] = "本控";
    status_bar_win_text_ele_pool[STATUS_BAR_WIN_COM_ST].text[ENGLISH] = "LOCAL";
    set_text_ele_font_color(STATUS_BAR_WIN_COM_ST, this_win, GUI_BLACK);
    update_text_ele(STATUS_BAR_WIN_COM_ST, this_win, NULL);
}
/**
  * @brief  隐藏状态栏界面
  * @param  无
  * @retval 无
  */
void hide_status_bar_win(void)
{
    WM_HideWindow(status_bar_windows.handle);
}
/**
  * @brief  显示状态栏界面
  * @param  无
  * @retval 无
  */
void show_status_bar_win(void)
{
    WM_ShowWindow(status_bar_windows.handle);
}
/**
  * @brief  创建状态栏界面
  * @param  无
  * @retval 无
  */
void create_status_bar_windows(void)
{
    create_user_window(&status_bar_windows, &windows_list, 0);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
