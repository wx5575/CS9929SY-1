/**
  ******************************************************************************
  * @file    set_goto_result_num_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.9.29
  * @brief   跳转到指定的结果
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
#include "7_set_goto_result_num_win.h"
#include "set_goto_result_num_win.h"
#include "PROGBAR.h"
#include "LISTVIEW.H"
#include "module_manage.h"
#include "result_win.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void set_goto_result_num_win_cb(WM_MESSAGE * pMsg);

static void set_goto_result_num_win_f1_cb(KEY_MESSAGE *key_msg);
static void set_goto_result_num_win_f2_cb(KEY_MESSAGE *key_msg);
static void set_goto_result_num_win_f3_cb(KEY_MESSAGE *key_msg);
static void set_goto_result_num_win_f4_cb(KEY_MESSAGE *key_msg);
static void set_goto_result_num_win_f5_cb(KEY_MESSAGE *key_msg);
static void set_goto_result_num_win_f6_cb(KEY_MESSAGE *key_msg);

static void set_goto_result_num_direct_key_up_cb	  (KEY_MESSAGE *key_msg);
static void set_goto_result_num_direct_key_down_cb( KEY_MESSAGE *key_msg);
static void set_goto_result_num_direct_key_left_cb( KEY_MESSAGE *key_msg);
static void set_goto_result_num_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void set_goto_result_num_enter_key_up_cb	  (KEY_MESSAGE *key_msg);

static void reg_set_goto_result_num_menu_key(WM_HMEM hWin);
static void reg_set_goto_result_num_sys_key(WM_HMEM hWin);
static void init_create_set_goto_result_num_edit_ele(MYUSER_WINDOW_T* win);
static void init_create_set_goto_result_num_win_com_ele(MYUSER_WINDOW_T* win);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  当前要跳转的结果条数
  */
static uint32_t cur_goto_result_num;

/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* set_goto_result_num_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_set_goto_result_num_windows,/*4.3寸屏*/
    &_7_set_goto_result_num_windows,/*5.6寸屏*/
    &_7_set_goto_result_num_windows,/*7寸屏*/
};

/**
  * @brief  密码管理窗口编辑控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static EDIT_ELE_AUTO_LAYOUT_T      *set_goto_result_num_edit_ele_auto_layout[]=
{
    &_7_set_goto_result_num_edit_ele_auto_layout_inf,//4.3寸屏
    &_7_set_goto_result_num_edit_ele_auto_layout_inf,//5.6寸屏
    &_7_set_goto_result_num_edit_ele_auto_layout_inf,//7寸屏
};

/**
  * @brief  原始密码 新设密码使用的菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	set_goto_result_num_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, set_goto_result_num_win_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, set_goto_result_num_win_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, set_goto_result_num_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, set_goto_result_num_win_f4_cb },//f4
    {"", F_KEY_ENTER    , KEY_F5 & _KEY_UP, set_goto_result_num_win_f5_cb },//f5
    {"", F_KEY_BACK     , KEY_F6 & _KEY_UP, set_goto_result_num_win_f6_cb },//f6
};

static CONFIG_FUNCTION_KEY_INFO_T set_goto_result_num_sys_key_pool[]={
	{KEY_UP		, set_goto_result_num_direct_key_up_cb		 },
	{KEY_DOWN	, set_goto_result_num_direct_key_down_cb	 },
	{KEY_LEFT	, set_goto_result_num_direct_key_left_cb	 },
	{KEY_RIGHT	, set_goto_result_num_direct_key_right_cb	 },
	{KEY_ENTER	, set_goto_result_num_enter_key_up_cb	     },
    
	{CODE_LEFT	, set_goto_result_num_direct_key_down_cb    },
	{CODE_RIGH	, set_goto_result_num_direct_key_up_cb	     },
};
enum{
    SET_GOTO_RESULT_NUM,///<选择校准模块
};
CS_INDEX set_goto_result_num_win_edit_ele_index[]=
{
    SET_GOTO_RESULT_NUM,///<选择校准模块
};
/**
  * @brief  窗口中使用到的编辑控件初始化数组
  */
static EDIT_ELE_T set_goto_result_num_ele_pool[]=
{
    {
        {"",""}, /* 名称 */
        SET_GOTO_RESULT_NUM,/* 通过枚举索引 */
        {"",""},/* 默认值 */
        {NULL, 1},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_NUM,},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {15/*heigh*/,1/*low*/,{"",""}/*notice*/},/*range*/
        {reg_set_goto_result_num_menu_key,reg_set_goto_result_num_sys_key,keyboard_fun_num,},/*key_inf*/
        0,/*dis*/
    },
};
/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T set_goto_result_num_windows=
{
    {"设置跳转的结果编号", "Set Goto result No."},
    set_goto_result_num_win_cb, NULL,
	{0},/*text*/
    {
        set_goto_result_num_ele_pool, ARRAY_SIZE(set_goto_result_num_ele_pool),
        (CS_INDEX*)set_goto_result_num_win_edit_ele_index,ARRAY_SIZE(set_goto_result_num_win_edit_ele_index),
        init_create_set_goto_result_num_edit_ele
    },/*edit*/
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_set_goto_result_num_win_com_ele,
    },/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        set_goto_result_num_edit_ele_auto_layout,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    set_goto_result_num_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void init_create_set_goto_result_num_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}
static void set_goto_result_num_direct_key_up_cb	  (KEY_MESSAGE *key_msg)
{
}
static void set_goto_result_num_direct_key_down_cb( KEY_MESSAGE *key_msg)
{
}
static void set_goto_result_num_direct_key_left_cb( KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
static void set_goto_result_num_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}
static void set_goto_result_num_enter_key_up_cb	  (KEY_MESSAGE *key_msg)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    back_win(key_msg->user_data);
}

/**
  * @brief  窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_goto_result_num_win_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  窗口中功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_goto_result_num_win_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  窗口中功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_goto_result_num_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  窗口中功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_goto_result_num_win_f4_cb(KEY_MESSAGE *key_msg)
{
    
}
/**
  * @brief  窗口中功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_goto_result_num_win_f5_cb(KEY_MESSAGE *key_msg)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    back_win(key_msg->user_data);
}
/**
  * @brief  窗口中功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_goto_result_num_win_f6_cb(KEY_MESSAGE *key_msg)
{
    cur_goto_result_num = 0;
    back_win(key_msg->user_data);
}

/**
  * @brief  原始密码和新密码的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void reg_set_goto_result_num_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = set_goto_result_num_menu_key_info;
    uint32_t size = ARRAY_SIZE(set_goto_result_num_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
static void reg_set_goto_result_num_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(set_goto_result_num_sys_key_pool,
        ARRAY_SIZE(set_goto_result_num_sys_key_pool), hWin);
}

static void set_set_goto_result_num_par_win_ele_data(void)
{
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, SET_GOTO_RESULT_NUM, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    cur_goto_result_num = 1;
    reg_edit_ele_data_inf(SET_GOTO_RESULT_NUM, &cur_goto_result_num, sizeof(cur_goto_result_num));
    edit_ele->range.high = get_result_max_num();
    edit_ele->range.low = 1;
}
/**
  * @brief  初始化并创建密码编辑对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_set_goto_result_num_edit_ele(MYUSER_WINDOW_T* win)
{
    set_set_goto_result_num_par_win_ele_data();//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    auto_layout_win_edit_ele(win);//自动布局窗口中的编辑对象
    init_window_edit_ele(win);//初始化创建编辑对象
}
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void set_goto_result_num_win_cb(WM_MESSAGE * pMsg)
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
            update_default_range_name();
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
            update_cur_result_inf(cur_goto_result_num);
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
void create_set_goto_result_num_win(int hWin)
{
    create_user_dialog(&set_goto_result_num_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
