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
static void init_create_help_win_text_ele(MYUSER_WINDOW_T* win);
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
CS_INDEX help_win_text_ele_index_pool[]=
{
    HELP_WIN_SOFT_VER,///<软件版本号
    HELP_WIN_HARD_VER,///<硬件版本号
    HELP_WIN_NOTICE,///<注意事项
    HELP_WIN_ADDRESS,///<地址
    HELP_WIN_LTD,///<公司名称
};

/**
  * @brief  界面的文本对象池
  */
static TEXT_ELE_T help_win_text_ele_pool[]=
{
	{{"软件版本号:V1.0.0", "SOFTWARE VERSION:V1.0.0"}, HELP_WIN_SOFT_VER },
	{{"硬件版本号:V1.0.0", "HARDWARE VERSION:V1.0.0"}, HELP_WIN_HARD_VER },
	{{"地址:南京江宁滨江开发区飞鹰路10#",
        "ADD:No.10 Feiying Road,Jiangning BinJiang\n    Economical Development Zone,"
        "Nanjing,China"}, HELP_WIN_ADDRESS },
	{{"南京长盛仪器有限公司", "NANJING CHANGSHENG INSTRUMENT CO.,LTD"}, HELP_WIN_LTD },
	{{"仪器操作注意事项:\n"
      "1.请勿触摸仪器输出端口及其测试物;\n"
      "2.仪器如有异常状况必须关闭电源;\n"
      "3.仪器必须妥善接地，请详阅说明书.\n\n"
        
      "系统快捷键说明:\n"
      "有效范围: 主界面和帮助界面下 \n"
      "Enter+0: 系统中英文快速切换键\n"
      "Enter+1: 系统初始密码快速恢复键",
        
      "Operation Notice:\n"
      "1.Don't touch H.V.port and DUT;\n"
      "2.Power off once an exception occrred;\n"
      "3.The instrument must be well grounded.\n\n"
      
      "System shortcutkeys:\n"
      "Valid range: main and help windows\n"
	  "Enter+0: language quick shift key\n"
	  "Enter+1: init.password recover key"
      }, HELP_WIN_NOTICE },
};
/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T help_windows=
{
    {"帮助窗口", "Help Window"},
    help_win_cb,    help_win_update_key_inf,
	{
        help_win_text_ele_pool,COUNT_ARRAY_SIZE(help_win_text_ele_pool),
        (CS_INDEX*)help_win_text_ele_index_pool,
        COUNT_ARRAY_SIZE(help_win_text_ele_index_pool),
        init_create_help_win_text_ele
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
    help_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void init_help_win_text_ele_pos_inf(MYUSER_WINDOW_T* win)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_help_win_layout1_text_ele_pos(win->text.pool);
            break;
    }
}
/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_help_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_help_win_text_ele_pos_inf(win);
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
//    update_result_win_text_ele_text(win);
}
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
static void help_win_paint_frame(void) 
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
            
            init_create_win_all_ele(win);
			break;
		case WM_PAINT:
			help_win_paint_frame();
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
