/**
  ******************************************************************************
  * @file    sel_cal_module_win.c
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
#include "ui_com/com_edit_api.h"
#include "rtc_config.h"
#include "7_sel_cal_module_win.h"
#include "sel_cal_module_win.h"
#include "PROGBAR.h"
#include "LISTVIEW.H"
#include "module_manage.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void sel_cal_module_win_cb(WM_MESSAGE * pMsg);
static void update_sel_cal_module_win_menu_key_inf(WM_HMEM hWin);

static void sel_cal_module_win_f1_cb(KEY_MESSAGE *key_msg);
static void sel_cal_module_win_f2_cb(KEY_MESSAGE *key_msg);
static void sel_cal_module_win_f3_cb(KEY_MESSAGE *key_msg);
static void sel_cal_module_win_f4_cb(KEY_MESSAGE *key_msg);
static void sel_cal_module_win_f5_cb(KEY_MESSAGE *key_msg);
static void sel_cal_module_win_f6_cb(KEY_MESSAGE *key_msg);


static void sel_cal_module_direct_key_up_cb	  (KEY_MESSAGE *key_msg);
static void sel_cal_module_direct_key_down_cb( KEY_MESSAGE *key_msg);
static void sel_cal_module_direct_key_left_cb( KEY_MESSAGE *key_msg);
static void sel_cal_module_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void sel_cal_module_enter_key_up_cb	  (KEY_MESSAGE *key_msg);


static void sel_cal_module_win_update_key_inf(WM_HWIN hWin);
static void reg_sel_cal_module_menu_key(WM_HMEM hWin);
static void reg_sel_cal_module_sys_key(WM_HMEM hWin);
static void init_create_sel_cal_module_edit_ele(MYUSER_WINDOW_T* win);
static void init_create_sel_cal_module_win_com_ele(MYUSER_WINDOW_T* win);
/* Private variables ---------------------------------------------------------*/

//static	WM_HWIN timer_handle;///<定时器句柄
//static	WM_HWIN U_FLASH_1_handle;///<U盘图标1句柄
//static	WM_HWIN U_FLASH_2_handle;///<U盘图标2句柄
//static	WM_HWIN KEY_LOCK_handle;///<键盘锁图标句柄
//static	WM_HWIN KEY_CAPITAL_SMALL_handle;///<大小写图标句柄
/** 主界面显示的文本索引表 */
//static CS_INDEX main_ui_text_ele_table[] =
//{
//	MAIN_UI_COM_ST,
//	MAIN_UI_SYS_TIME,
//};
//static	LISTVIEW_Handle sel_cal_module_list_handle;///<校准管理列表句柄
/**
  * @brief  系统按键信息
  */
//static CONFIG_FUNCTION_KEY_INFO_T sel_cal_module_win_sys_key_pool[]=
//{
//	{KEY_SHIFT	    , sys_shift_key_fun_cb      },
//	{KEY_UNLOCK	    , sys_unlock_key_fun_cb     },
//	{KEY_EXIT	    , sys_exit_key_fun_cb       },
//	{KEY_STOP	    , sys_stop_key_fun_cb       },
//	{KEY_F1 & KEY_0 , screen_capture_key_fun_cb },
//};
/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* sel_cal_module_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_sel_cal_module_windows,/*4.3寸屏*/
    &_7_sel_cal_module_windows,/*5.6寸屏*/
    &_7_sel_cal_module_windows,/*7寸屏*/
};

/**
  * @brief  密码管理窗口编辑控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static EDIT_ELE_AUTO_LAYOUT_T      *sel_cal_module_edit_ele_auto_layout[]=
{
    &_7_sel_cal_module_edit_ele_auto_layout_inf,//4.3寸屏
    &_7_sel_cal_module_edit_ele_auto_layout_inf,//5.6寸屏
    &_7_sel_cal_module_edit_ele_auto_layout_inf,//7寸屏
};
/** 主界面使用的菜单键信息的配置 */
//static MENU_KEY_INFO_T 	sel_cal_module_win_menu_key_inf[] = 
//{
//    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, sel_cal_module_win_f1_cb },//f1
//    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, sel_cal_module_win_f2_cb },//f2
//    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, sel_cal_module_win_f3_cb },//f3
//    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, sel_cal_module_win_f4_cb },//f4
//    {"", F_KEY_ENTER    , KEY_F5 & _KEY_UP, sel_cal_module_win_f5_cb },//f5
//    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, sel_cal_module_win_f6_cb },//f6
//};

/**
  * @brief  原始密码 新设密码使用的菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	sel_cal_module_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, sel_cal_module_win_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, sel_cal_module_win_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, sel_cal_module_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, sel_cal_module_win_f4_cb },//f4
    {"", F_KEY_ENTER    , KEY_F5 & _KEY_UP, sel_cal_module_win_f5_cb },//f5
    {"", F_KEY_BACK     , KEY_F6 & _KEY_UP, sel_cal_module_win_f6_cb },//f6
};

static CONFIG_FUNCTION_KEY_INFO_T sel_cal_module_sys_key_pool[]={
	{KEY_UP		, sel_cal_module_direct_key_up_cb		 },
	{KEY_DOWN	, sel_cal_module_direct_key_down_cb	 },
	{KEY_LEFT	, sel_cal_module_direct_key_left_cb	 },
	{KEY_RIGHT	, sel_cal_module_direct_key_right_cb	 },
	{KEY_ENTER	, sel_cal_module_enter_key_up_cb	     },
    
	{CODE_LEFT	, sel_cal_module_direct_key_down_cb    },
	{CODE_RIGH	, sel_cal_module_direct_key_up_cb	     },
};
enum{
    SEL_CAL_MODULE,///<选择校准模块
};
CS_INDEX sel_cal_module_win_edit_ele_index[]=
{
    SEL_CAL_MODULE,///<选择校准模块
};
/**
  * @brief  窗口中使用到的编辑控件初始化数组
  */
static EDIT_ELE_T sel_cal_module_ele_pool[]=
{
    {
        {"",""}, /* 名称 */
        SEL_CAL_MODULE,/* 通过枚举索引 */
        {"",""},/* 默认值 */
        {NULL, 1},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_NUM,},/*类型*/
        {0/*dec*/,2/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {15/*heigh*/,1/*low*/,{"",""}/*notice*/},/*range*/
        {reg_sel_cal_module_menu_key,reg_sel_cal_module_sys_key,keyboard_fun_num,},/*key_inf*/
        0,/*dis*/
    },
};
/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T sel_cal_module_windows=
{
    {"选择校准模块", "select module"},
    sel_cal_module_win_cb, sel_cal_module_win_update_key_inf,
	{0},/*text*/
    {
        sel_cal_module_ele_pool, ARRAY_SIZE(sel_cal_module_ele_pool),
        (CS_INDEX*)sel_cal_module_win_edit_ele_index,ARRAY_SIZE(sel_cal_module_win_edit_ele_index),
        init_create_sel_cal_module_edit_ele
    },/*edit*/
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_sel_cal_module_win_com_ele,
    },/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        sel_cal_module_edit_ele_auto_layout,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    sel_cal_module_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void init_create_sel_cal_module_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
//    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
//    update_group_inf(win);//更新记忆组信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}
static void sel_cal_module_direct_key_up_cb	  (KEY_MESSAGE *key_msg)
{
}
static void sel_cal_module_direct_key_down_cb( KEY_MESSAGE *key_msg)
{
}
static void sel_cal_module_direct_key_left_cb( KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
static void sel_cal_module_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}
static void sel_cal_module_enter_key_up_cb	  (KEY_MESSAGE *key_msg)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    back_win(key_msg->user_data);
}

/**
  * @brief  主窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sel_cal_module_win_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  主窗口中功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sel_cal_module_win_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  主窗口中功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sel_cal_module_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sel_cal_module_win_f4_cb(KEY_MESSAGE *key_msg)
{
    
}
/**
  * @brief  主窗口中功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sel_cal_module_win_f5_cb(KEY_MESSAGE *key_msg)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    back_win(key_msg->user_data);
}
/**
  * @brief  主窗口中功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sel_cal_module_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  原始密码和新密码的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void reg_sel_cal_module_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = sel_cal_module_menu_key_info;
    uint32_t size = ARRAY_SIZE(sel_cal_module_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
static void reg_sel_cal_module_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(sel_cal_module_sys_key_pool, ARRAY_SIZE(sel_cal_module_sys_key_pool), hWin);
}
uint8_t cur_cal_module_index = 1;///<当前校准路索引
uint8_t prev_cal_module_index = 1;///<上一路校准索引
uint8_t get_cur_cal_road(void)
{
    return cur_cal_module_index;
}

void set_cur_cal_road(uint8_t road)
{
    cur_cal_module_index = road;
}
static void set_sel_cal_module_par_win_ele_data(void)
{
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, SEL_CAL_MODULE, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    reg_edit_ele_data_inf(SEL_CAL_MODULE, &cur_cal_module_index, sizeof(cur_cal_module_index));
    edit_ele->range.high = get_total_roads_num();
    
    if(edit_ele->range.high < 1)
    {
        edit_ele->range.low = 0;
    }
    else
    {
        edit_ele->range.low = 1;
    }
}
/**
  * @brief  初始化并创建密码编辑对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_sel_cal_module_edit_ele(MYUSER_WINDOW_T* win)
{
    set_sel_cal_module_par_win_ele_data();//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    auto_layout_win_edit_ele(win);//自动布局窗口中的编辑对象
    init_window_edit_ele(win);//初始化创建编辑对象
}
/**
  * @brief  更新主界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_sel_cal_module_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(sel_cal_module_menu_key_info, ARRAY_SIZE(sel_cal_module_menu_key_info), hWin);
}

/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void sel_cal_module_win_update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(sel_cal_module_sys_key_pool, ARRAY_SIZE(sel_cal_module_sys_key_pool), hWin);
}

/**
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void sel_cal_module_win_update_key_inf(WM_HWIN hWin)
{
    update_sel_cal_module_win_menu_key_inf(hWin);
    sel_cal_module_win_update_system_key_inf(hWin);
}

/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void sel_cal_module_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
            
            init_dialog(win);
            
            init_create_win_all_ele(win);
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();
            if(g_cur_edit_ele != NULL)
            {
                select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
            }
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			break;
		case WM_NOTIFY_PARENT:
			break;
		case WM_DELETE:
		{
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建选择校准模块窗口
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_sel_cal_module_win(int hWin)
{
    create_user_dialog(&sel_cal_module_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
