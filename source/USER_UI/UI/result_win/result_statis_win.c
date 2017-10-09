/**
  ******************************************************************************
  * @file    result_statis_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.8
  * @brief   结果统计界面
  ******************************************************************************
  */

#include "ui_com/com_ui_info.h"
#include "7_result_statis_win.h"
#include "result_statis_win.h"

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  界面要显示的文本对象索引枚举定义
  */
enum{
    RESULT_STATIS_WIN_CAPACITY,///<结果容量
    RESULT_STATIS_WIN_USED,///<已用条数
    RESULT_STATIS_WIN_RESIDUE,///<剩余条数
    RESULT_STATIS_WIN_PASS,///<合格条数
    RESULT_STATIS_WIN_FAIL,///<失败条数
    RESULT_STATIS_WIN_PERCENT///<合格率
};
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void result_statis_win_cb(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);

static void result_statis_win_f1_cb(KEY_MESSAGE *key_msg);
static void result_statis_win_f2_cb(KEY_MESSAGE *key_msg);
static void result_statis_win_f3_cb(KEY_MESSAGE *key_msg);
static void result_statis_win_f4_cb(KEY_MESSAGE *key_msg);
static void result_statis_win_f5_cb(KEY_MESSAGE *key_msg);
static void result_statis_win_f6_cb(KEY_MESSAGE *key_msg);

static void init_create_result_statis_win_text_ele(MYUSER_WINDOW_T* win);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  窗口的位置尺寸信息，根据不同屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* result_statis_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_result_statis_windows,/*4.3寸屏*/
    &_7_result_statis_windows,/*5.6寸屏*/
    &_7_result_statis_windows,/*7寸屏*/
};
/**
  * @brief  窗口的文本控件自动布局结构信息，根据不同屏幕尺寸进行初始化
  */
TEXT_ELE_AUTO_LAYOUT_T  *result_statis_win_text_ele_auto_layout[]=
{
    &_7_result_statis_win_text_ele_auto_layout_inf,//4.3寸屏
    &_7_result_statis_win_text_ele_auto_layout_inf,//5.6寸屏
    &_7_result_statis_win_text_ele_auto_layout_inf,//7寸屏
};
/**
  * @brief  窗口的菜单键信息
  */
static MENU_KEY_INFO_T 	result_statis_win_menu_key_info[] =
{
    {"", F_KEY_NULL , KEY_F1 & _KEY_UP, result_statis_win_f1_cb },//f1
    {"", F_KEY_NULL , KEY_F2 & _KEY_UP, result_statis_win_f2_cb },//f2
    {"", F_KEY_NULL , KEY_F3 & _KEY_UP, result_statis_win_f3_cb },//f3
    {"", F_KEY_NULL , KEY_F4 & _KEY_UP, result_statis_win_f4_cb },//f4
    {"", F_KEY_NULL , KEY_F5 & _KEY_UP, result_statis_win_f5_cb },//f5
    {"", F_KEY_BACK , KEY_F6 & _KEY_UP, result_statis_win_f6_cb },//f6
};

/**
  * @brief  界面要显示的文本对象索引表
  */
static CS_INDEX result_statis_win_text_ele_table[]=
{
    RESULT_STATIS_WIN_CAPACITY,///<结果容量
    RESULT_STATIS_WIN_USED,///<已用条数
    RESULT_STATIS_WIN_RESIDUE,///<剩余条数
    RESULT_STATIS_WIN_PASS,///<合格条数
    RESULT_STATIS_WIN_FAIL,///<失败条数
    RESULT_STATIS_WIN_PERCENT///<合格率
};
/**
  * @brief  界面的文本对象池初始化数组
  */
static TEXT_ELE_T result_statis_win_ui_text_ele_pool[]=
{
	{{"结果容量:10000", "Capacity:10000"}   , RESULT_STATIS_WIN_CAPACITY},
	{{"已用条数:10000", "Used:10000"}       , RESULT_STATIS_WIN_USED},
	{{"剩余条数:10000", "Residue:10000"}    , RESULT_STATIS_WIN_RESIDUE},
	{{"合格条数:10000", "PASS:10000"}       , RESULT_STATIS_WIN_PASS},
	{{"失败条数:10000", "FAIL:10000"}       , RESULT_STATIS_WIN_FAIL},
	{{"合格率:100%", "Percent of pass:10000"}, RESULT_STATIS_WIN_PERCENT},
};
/**
  * @brief  窗口文本控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static TEXT_ELE_AUTO_LAYOUT_T  *res_statis_text_ele_auto_layout[]=
{
    &_7_result_statis_win_text_ele_auto_layout_inf,//4.3寸屏
    &_7_result_statis_win_text_ele_auto_layout_inf,//5.6寸屏
    &_7_result_statis_win_text_ele_auto_layout_inf,//7寸屏
};
/**
  * @brief  恢复出厂设置窗口结构
  */
MYUSER_WINDOW_T result_statis_windows=
{
    {"结果统计","The results of statistical"},
    result_statis_win_cb, update_menu_key_inf,
	{
        result_statis_win_ui_text_ele_pool, ARRAY_SIZE(result_statis_win_ui_text_ele_pool),
        (CS_INDEX*)result_statis_win_text_ele_table, ARRAY_SIZE(result_statis_win_text_ele_table),
        init_create_result_statis_win_text_ele
    },/*text*/
    {0},/*edit*/
    {0},/*com*/
    /* 自动布局 */
    {
        res_statis_text_ele_auto_layout,//文本自动布局信息池
        NULL,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    result_statis_win_pos_size_pool/*pos_size_pool*/
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  菜单键f1的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void result_statis_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f2的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void result_statis_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f3的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void result_statis_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f4的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void result_statis_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f5的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void result_statis_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f6的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void result_statis_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);//关闭对话框
}

/**
  * @brief  初始化并创建恢复出厂设置窗口所有的文本控件
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_result_statis_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_window_text_ele_list(win);
    init_window_text_ele_dis_inf(win, result_statis_win_text_ele_auto_layout[SCREEM_SIZE]);
    init_window_text_ele(win);
}
/**
  * @brief  更新菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(result_statis_win_menu_key_info, ARRAY_SIZE(result_statis_win_menu_key_info), hWin);//刷新菜单键显示
}
/**
  * @brief  窗口的回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void result_statis_win_cb(WM_MESSAGE* pMsg)
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
            update_win_menu_key(win);
            init_create_win_all_ele(win);
            
            break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			break;
		case WM_DELETE:
		{
			send_no_par_msg_to_parent(hWin, CM_CHILD_W_MSG);
			break;
		}
		default:
			WM_DefaultProc(pMsg);
	}
}
/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_result_statis_dialog(int hWin)
{
    create_user_dialog(&result_statis_windows, &windows_list, hWin);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
