/**
  ******************************************************************************
  * @file    run_log_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   运行日志
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
#include "7_run_log_win.h"
#include "run_log_win.h"
#include "PROGBAR.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void run_log_win_f1_cb(KEY_MESSAGE *key_msg);
static void run_log_win_f2_cb(KEY_MESSAGE *key_msg);
static void run_log_win_f3_cb(KEY_MESSAGE *key_msg);
static void run_log_win_f4_cb(KEY_MESSAGE *key_msg);
static void run_log_win_f5_cb(KEY_MESSAGE *key_msg);
static void run_log_win_f6_cb(KEY_MESSAGE *key_msg);

static void sys_checkout_key_fun_cb(KEY_MESSAGE *key_msg);

static void run_log_win_update_key_inf(WM_HWIN hWin);
static void run_log_win_cb(WM_MESSAGE * pMsg);
static void init_create_run_log_win_text_ele(MYUSER_WINDOW_T* win);
static void update_run_log_win_system_key_inf(WM_HMEM hWin);
/* Private variables ---------------------------------------------------------*/

static	WM_HWIN timer_handle;///<定时器句柄
/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* run_log_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_run_log_windows,/*4.3寸屏*/
    &_7_run_log_windows,/*5.6寸屏*/
    &_7_run_log_windows,/*7寸屏*/
};

/**
  * @brief  系统按键信息
  */
static CONFIG_FUNCTION_KEY_INFO_T run_log_win_sys_key_pool[]=
{
	{KEY_F4 & KEY_5 , sys_checkout_key_fun_cb },
};
/** 帮助界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	run_log_win_menu_key_inf[] = 
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, run_log_win_f1_cb, SYS_KEY_EN},//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, run_log_win_f2_cb, SYS_KEY_EN},//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, run_log_win_f3_cb, SYS_KEY_EN},//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, run_log_win_f4_cb, SYS_KEY_EN},//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, run_log_win_f5_cb, SYS_KEY_EN},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, run_log_win_f6_cb, SYS_KEY_EN},//f6
};
CS_INDEX run_log_win_text_ele_index_pool[]=
{
    RUN_LOG_WIN_SOFT_VER,///<软件版本号
    RUN_LOG_WIN_HARD_VER,///<硬件版本号
    RUN_LOG_WIN_ADDRESS,///<地址
    RUN_LOG_WIN_LTD,///<公司名称
    RUN_LOG_WIN_INSPECTION_DATE_N,///<出厂检验日期名字
    RUN_LOG_WIN_INSPECTION_DATE_V,///<出厂检验日期
    RUN_LOG_WIN_BOOT_TIMES_N,///<开机总计次数名字
    RUN_LOG_WIN_BOOT_TIMES_V,///<开机总计次数
    RUN_LOG_WIN_TESTING_TIMES_N,///<总计测量次数名字
    RUN_LOG_WIN_TESTING_TIMES_V,///<总计测量次数
    RUN_LOG_WIN_STARTUP_TIME_N,///<开机运行时间名字
    RUN_LOG_WIN_STARTUP_TIME_V,///<开机运行时间
    RUN_LOG_WIN_OPERATION_TIME_N,///<总计运行时间名字
    RUN_LOG_WIN_OPERATION_TIME_V,///<总计运行时间
};

/**
  * @brief  界面的文本对象池
  */
static TEXT_ELE_T run_log_win_text_ele_pool[]=
{
	{{"软件版本号:V1.0.0", "SOFTWARE VERSION:V1.0.0"}, RUN_LOG_WIN_SOFT_VER },
	{{"硬件版本号:V1.0.0", "HARDWARE VERSION:V1.0.0"}, RUN_LOG_WIN_HARD_VER },
	{{"地址:南京江宁滨江开发区飞鹰路10#",
        "ADD:No.10 Feiying Road,Jiangning BinJiang\n    Economical Development Zone,"
        "Nanjing,China"}, RUN_LOG_WIN_ADDRESS },
	{{"南京长盛仪器有限公司", "NANJING CHANGSHENG INSTRUMENT CO.,LTD"}, RUN_LOG_WIN_LTD },
	{{"出厂检验日期:", "Inspection date:"}, RUN_LOG_WIN_INSPECTION_DATE_N },
	{{"xxxx-xx-xx xx:xx:xx", "xxxx-xx-xx xx:xx:xx"}, RUN_LOG_WIN_INSPECTION_DATE_V },
	{{"总计开机次数:", "Total boot times:"}, RUN_LOG_WIN_BOOT_TIMES_N },
	{{"0次", "0 times"}, RUN_LOG_WIN_BOOT_TIMES_V },
	{{"总计测试次数:", "Total testing times:"}, RUN_LOG_WIN_TESTING_TIMES_N },
	{{"0次", "0 times"}, RUN_LOG_WIN_TESTING_TIMES_V },
	{{"开机运行时间:", "Startup time:"}, RUN_LOG_WIN_STARTUP_TIME_N },
	{{"0 天  00 小时  00 分钟  00 秒", "0 Days  00 Hours 00 Minutes  00 Seconds"}, RUN_LOG_WIN_STARTUP_TIME_V },
	{{"总计运行时间:", "Total run time:"}, RUN_LOG_WIN_OPERATION_TIME_N },
	{{"0 天  00 小时  00 分钟  00 秒", "0 Days  00 Hours 00 Minutes  00 Seconds"}, RUN_LOG_WIN_OPERATION_TIME_V },
};
/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T run_log_windows=
{
    {"运行日志窗口", "Run Log Window"},
    run_log_win_cb,    run_log_win_update_key_inf,
	{
        run_log_win_text_ele_pool,COUNT_ARRAY_SIZE(run_log_win_text_ele_pool),
        (CS_INDEX*)run_log_win_text_ele_index_pool,
        COUNT_ARRAY_SIZE(run_log_win_text_ele_index_pool),
        init_create_run_log_win_text_ele
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
    run_log_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void init_run_log_win_text_ele_pos_inf(MYUSER_WINDOW_T* win)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_run_log_win_layout1_text_ele_pos(win->text.pool);
            break;
    }
}
/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_run_log_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_run_log_win_text_ele_pos_inf(win);
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
//    update_result_win_text_ele_text(win);
}
static void run_log_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
static void run_log_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
static void run_log_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void run_log_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void run_log_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void run_log_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void run_log_win_paint_frame(void) 
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
static void update_run_log_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(run_log_win_menu_key_inf, ARRAY_SIZE(run_log_win_menu_key_inf), hWin);
}
static void update_run_log_win_system_key_inf(WM_HMEM hWin)
{
	register_system_key_fun(run_log_win_sys_key_pool, ARRAY_SIZE(run_log_win_sys_key_pool), hWin);
}
/**
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void run_log_win_update_key_inf(WM_HWIN hWin)
{
    update_run_log_win_menu_key_inf(hWin);
    update_run_log_win_system_key_inf(hWin);
}

static void sys_checkout_key_fun_cb(KEY_MESSAGE *key_msg)
{
    uint8_t *str = get_time_str(0);
    
    strcpy((char*)sys_par.data_of_survey, (const char*)str);
    sys_flag.power_on_count = 0;
    sys_flag.sys_run_time = 0;
    sys_flag.test_count = 0;
    save_sys_par();
    save_sys_flag();
}

void turn_seconds_to_dhms(uint32_t time, uint8_t *days,
    uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
{
    sprintf((char*)days     , "%d", time / (3600 * 24));
    sprintf((char*)hours    , "%02d", (time % (3600 * 24)) / 3600);
    sprintf((char*)minutes  , "%02d", ((time % (3600 * 24)) % 3600) / 60);
    sprintf((char*)seconds  , "%02d", ((time % (3600 * 24)) % 3600) % 60);
}

void update_run_log_win_text(MYUSER_WINDOW_T* win)
{
    uint8_t days[5] = {0};
    uint8_t hours[5] = {0};
    uint8_t minutes[5] = {0};
    uint8_t seconds[5] = {0};
    uint8_t buf[100] = {0};
    
    turn_seconds_to_dhms(sys_flag.sys_run_time, days, hours, minutes, seconds);
    
    sprintf((char*)buf, SYS_LANGUAGE==CHINESE?
            "%s 天  %s 小时  %s 分钟  %s 秒": "%s Days  %s Hours %s Minutes  %s Seconds",
            days, hours, minutes, seconds);
    update_text_ele((CS_INDEX)RUN_LOG_WIN_OPERATION_TIME_V, win, buf);
    
    turn_seconds_to_dhms(startup_time, days, hours, minutes, seconds);
    sprintf((char*)buf, SYS_LANGUAGE==CHINESE?
            "%s 天  %s 小时  %s 分钟  %s 秒": "%s Days  %s Hours %s Minutes  %s Seconds",
            days, hours, minutes, seconds);
    update_text_ele((CS_INDEX)RUN_LOG_WIN_STARTUP_TIME_V, win, buf);
    
    sprintf((char*)buf, SYS_LANGUAGE==CHINESE?"%d 次":"%d times", sys_flag.power_on_count);
    update_text_ele((CS_INDEX)RUN_LOG_WIN_BOOT_TIMES_V, win, buf);
    
    sprintf((char*)buf, SYS_LANGUAGE==CHINESE?"%d 次":"%d times", sys_flag.test_count);
    update_text_ele((CS_INDEX)RUN_LOG_WIN_TESTING_TIMES_V, win, buf);
    
    if(strlen((const char*)sys_par.data_of_survey) == 19)
    {
        update_text_ele((CS_INDEX)RUN_LOG_WIN_INSPECTION_DATE_V, win,
            (uint8_t *)sys_par.data_of_survey);
    }
}
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void run_log_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	static CS_IMAGE_T cs_image;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_user_window_handle(hWin);
            create_logo_imagex(hWin, &cs_image);
			win = get_user_window_info(hWin);
			timer_handle = WM_CreateTimer(hWin, 0, 1000, 0);
            
            if(win->init_key_fun != NULL)
            {
                win->init_key_fun(hWin);
            }
            
            init_create_win_all_ele(win);
            update_run_log_win_text(win);
			break;
		case WM_PAINT:
			run_log_win_paint_frame();
			break;
		case WM_TIMER:
		{
            win = get_user_window_info(hWin);
            update_run_log_win_text(win);
			WM_RestartTimer(timer_handle, 1000);
            break;
		}
		case WM_KEY:
			break;
		case WM_DELETE:
            delete_image(&cs_image);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建运行日志窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_run_log_win(int hWin)
{
    create_user_window(&run_log_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
