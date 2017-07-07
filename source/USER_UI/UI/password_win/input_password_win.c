/**
  ******************************************************************************
  * @file    input_password_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   密码验证界面
  ******************************************************************************
  */

#include "DIALOG.h"
#include "string.h"
#include "ui_com/com_edit_api.h"
#include "input_password_win.h"
#include "7_input_password_win.h"

/* Private typedef -----------------------------------------------------------*/

/** 
  * @brief 备份将要进入窗口的信息,当需要进行密码验证的窗口时要备份一下信息，当正确输入密码后再恢复调用进入窗口
  */
typedef struct{
    void (*into_win_fun)(int);//进入窗口的函数
    int data;///<携带的参数
}BACK_UP_WILL_ENTER_WIN_INF;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void input_password_win_cb(WM_MESSAGE* pMsg);
static CONFIG_FUNCTION_KEY_INFO_T 	sys_key_pool[];
static void input_pwd_menu_key_ok(WM_HMEM hWin);

static void init_create_pwd_edit_ele(MYUSER_WINDOW_T* win);

static void input_pwd_win_f1_cb(KEY_MESSAGE *key_msg);
static void input_pwd_win_f2_cb(KEY_MESSAGE *key_msg);
static void input_pwd_win_f3_cb(KEY_MESSAGE *key_msg);
static void input_pwd_win_f4_cb(KEY_MESSAGE *key_msg);
static void input_pwd_win_f5_cb(KEY_MESSAGE *key_msg);
static void input_pwd_win_f6_cb(KEY_MESSAGE *key_msg);

static void reg_input_pwd_sys_key(WM_HMEM data);

static void reg_input_pwd_menu_key(WM_HMEM hWin);

static void input_pwd_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void input_pwd_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void input_pwd_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void input_pwd_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void input_pwd_enter_key_up_cb(KEY_MESSAGE *key_msg);

/* Private variables ---------------------------------------------------------*/
static BACK_UP_WILL_ENTER_WIN_INF   back_up_will_enter_win_inf;
/**
  * @brief  临时存放输入密码
  */
static uint8_t tmp_input_pwd[PWD_MAX_LEN + 1];
/**
  * @brief  密码管理窗口位置尺寸信息，根据不同的屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* password_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_input_password_windows,/*4.3寸屏*/
    &_7_input_password_windows,/*5.6寸屏*/
    &_7_input_password_windows,/*7寸屏*/
};
/**
  * @brief  密码管理窗口编辑控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static EDIT_ELE_AUTO_LAYOUT_T      *input_pwd_edit_ele_auto_layout[]=
{
    &_7_input_pwd_edit_ele_auto_layout_inf,//4.3寸屏
    &_7_input_pwd_edit_ele_auto_layout_inf,//5.6寸屏
    &_7_input_pwd_edit_ele_auto_layout_inf,//7寸屏
};

/**
  * @brief  原始密码 新设密码使用的菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	input_pwd_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, input_pwd_win_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, input_pwd_win_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, input_pwd_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, input_pwd_win_f4_cb },//f4
    {"", F_KEY_ENTER       , KEY_F5 & _KEY_UP, input_pwd_win_f5_cb },//f5
    {"", F_KEY_BACK     , KEY_F6 & _KEY_UP, input_pwd_win_f6_cb },//f6
};

/**
  * @brief  窗口中使用到的编辑控件初始化数组
  */
static EDIT_ELE_T input_password_ele_pool[]=
{
    {
        {"",""}, /* 名称 */
        INPUT_PWD,/* 通过枚举索引 */
        {"",""},/* 默认值 */
        {NULL, D_N_BYTES},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_STR, E_STRING_T},/*类型*/
        {0/*dec*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"1-8个数字","1-8 Numbers"}/*notice*/},/*range*/
        {reg_input_pwd_menu_key,reg_input_pwd_sys_key,keyboard_fun_pwd,},/*key_inf*/
        0,/*dis*/
    },
};

//static PWD_T pdw_inf;

static CONFIG_FUNCTION_KEY_INFO_T sys_key_pool[]={
	{KEY_UP		, input_pwd_direct_key_up_cb		 },
	{KEY_DOWN	, input_pwd_direct_key_down_cb	 },
	{KEY_LEFT	, input_pwd_direct_key_left_cb	 },
	{KEY_RIGHT	, input_pwd_direct_key_right_cb	 },
	{KEY_ENTER	, input_pwd_enter_key_up_cb	     },
    
	{CODE_LEFT	, input_pwd_direct_key_down_cb    },
	{CODE_RIGH	, input_pwd_direct_key_up_cb	     },
};


static CS_INDEX input_password_ui_ele_table[]=
{
    INPUT_PWD,
};

/**
  * @brief  密码界面窗口结构
  */
static MYUSER_WINDOW_T input_password_windows=
{
    {"密码验证","Input Password"},
    input_password_win_cb, NULL,
	{
        0
    },
    {
        input_password_ele_pool, ARRAY_SIZE(input_password_ele_pool),
        (CS_INDEX*)input_password_ui_ele_table,ARRAY_SIZE(input_password_ui_ele_table),
        init_create_pwd_edit_ele
    },
    {
        0
//        com_text_ele_pool, ARRAY_SIZE(com_text_ele_pool),
//        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
//        init_create_input_pwd_com_ele
    },
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        input_pwd_edit_ele_auto_layout,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    password_win_pos_size_pool /*pos_size_pool*/
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  清空编辑对象的密码，注意密码是在内存中的，不是在控件中显示的*
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void clear_pwd_edit_ele(int hWin)
{
    WM_HMEM handle = 0;
    uint8_t *str;
    
    str = g_cur_edit_ele->data.data;
    str[0] = 0;
    
    handle = g_cur_edit_ele->dis.edit.handle;
    clear_edit_ele(handle);
}
/**
  * @brief  退格编辑密码，注意密码是在内存中的，不是在控件中显示的*
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void backspace_pwd_edit_ele(int hWin)
{
    WM_HMEM handle = 0;
    uint8_t cur;
    
    handle = g_cur_edit_ele->dis.edit.handle;
    cur = EDIT_GetCursorCharPos(handle);
    
    del_a_char_from_edit_str(cur);
    menu_key_backspace(hWin);
}
/**
  * @brief  原始密码和新密码共用的功能键F1的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_win_f1_cb(KEY_MESSAGE *key_msg)
{
    backspace_pwd_edit_ele(key_msg->user_data);
}
/**
  * @brief  原始密码和新密码共用的功能键F2的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_win_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_pwd_edit_ele(key_msg->user_data);
}
/**
  * @brief  原始密码和新密码共用的功能键F3的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  原始密码和新密码共用的功能键F4的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  原始密码和新密码共用的功能键F5的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_win_f5_cb(KEY_MESSAGE *key_msg)
{
    input_pwd_menu_key_ok(key_msg->user_data);
}
/**
  * @brief  原始密码和新密码共用的功能键F6的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_win_f6_cb(KEY_MESSAGE *key_msg)
{
    g_custom_msg.msg = CM_DIALOG_RETURN_CANCLE;
    back_win(key_msg->user_data);
}


/**
  * @brief  原始密码和新密码的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void reg_input_pwd_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = input_pwd_menu_key_info;
    uint32_t size = ARRAY_SIZE(input_pwd_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  向上功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  向下功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
}


/**
  * @brief  向左功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
/**
  * @brief  向右功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}
/**
  * @brief  向右功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void input_pwd_enter_key_up_cb(KEY_MESSAGE *key_msg)
{
    input_pwd_menu_key_ok(key_msg->user_data);
}

/**
  * @brief  密码设置窗口中系统功能键的初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void reg_input_pwd_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}

/**
  * @brief  设置密码时按下确认键后调用这个函数，进行密码合法性判断，如果合法就保存密码
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void input_pwd_menu_key_ok(WM_HMEM hWin)
{
    if(0 == strcmp((const char*)tmp_input_pwd, (const char*)sys_par.password))
    {
        g_custom_msg.msg = CM_DIALOG_RETURN_OK;
        back_win(hWin);
    }
    else
    {
        clear_pwd_edit_ele(hWin);
    }
}

static void set_input_pwd_par_win_ele_data(void)
{
    reg_edit_ele_data_inf(INPUT_PWD, tmp_input_pwd, sizeof(tmp_input_pwd));
}
/**
  * @brief  初始化并创建密码编辑对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_pwd_edit_ele(MYUSER_WINDOW_T* win)
{
    set_input_pwd_par_win_ele_data();//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    auto_layout_win_edit_ele(win);//自动布局窗口中的编辑对象
    init_window_edit_ele(win);//初始化创建编辑对象
}

static void input_pwd_win_into_win(void)
{
    if(NULL != back_up_will_enter_win_inf.into_win_fun)
    {
        back_up_will_enter_win_inf.into_win_fun(back_up_will_enter_win_inf.data);
    }
}

static void clear_input_pwd_win_into_win_inf(void)
{
    back_up_will_enter_win_inf.into_win_fun = NULL;
    back_up_will_enter_win_inf.data = 0;
}
/**
  * @brief  密码窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void input_password_win_cb(WM_MESSAGE* pMsg)
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
            clear_pwd_edit_ele(hWin);
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
            /* 恢复按键信息 */
            if(recover_key_inf_fun != NULL)
            {
                recover_key_inf_fun();
            }
            
            /* 如果按下ENTER键就进入要进入的窗口 */
            if(g_custom_msg.msg == CM_DIALOG_RETURN_OK)
            {
                input_pwd_win_into_win();
            }
            
            clear_input_pwd_win_into_win_inf();
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/* Public functions ---------------------------------------------------------*/

void init_back_up_will_enter_win_inf(void (*fun)(int), int data)
{
    back_up_will_enter_win_inf.into_win_fun = fun;
    back_up_will_enter_win_inf.data = g_cur_win->handle;
    create_input_password_ui(data);
}
/**
  * @brief  创建密码窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_input_password_ui(int hWin)
{
    create_user_dialog(&input_password_windows, &windows_list, hWin);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
