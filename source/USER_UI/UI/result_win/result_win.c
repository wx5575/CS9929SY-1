/**
  ******************************************************************************
  * @file    result_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   结果管理窗口
  ******************************************************************************
  */

#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"
#include "7_result_win.h"
#include "result_win.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define WINDOWS_BAK_COLOR	GUI_BLUE	//窗口背景色

/* Private function prototypes -----------------------------------------------*/

static LISTVIEW_Handle list_h;
static void result_win_cb(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);
//void pop_warning_dialog(int hWin);
//void into_save_file_dialog(int hWin);
static void result_exist_f1_cb(KEY_MESSAGE *key_msg);
static void result_exist_f2_cb(KEY_MESSAGE *key_msg);
static void result_exist_f3_cb(KEY_MESSAGE *key_msg);
static void result_exist_f4_cb(KEY_MESSAGE *key_msg);
static void result_exist_f5_cb(KEY_MESSAGE *key_msg);
static void result_exist_f6_cb(KEY_MESSAGE *key_msg);

static void result_no_exist_f1_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f2_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f3_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f4_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f5_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f6_cb(KEY_MESSAGE *key_msg);
static void direct_key_up_cb(KEY_MESSAGE *key_msg);
static void direct_key_down_cb(KEY_MESSAGE *key_msg);
static void update_cur_row_menu_key_st(WM_HWIN hWin);

static void init_create_result_win_text_ele(MYUSER_WINDOW_T* win);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  结果管理窗口位置尺寸信息,根据不同屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* result_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_result_windows,///<4.3寸屏
    &_7_result_windows,///<5.6寸屏
    &_7_result_windows,///<7寸屏
};
/**
  * @brief 界面文本对象池数组
  */
static TEXT_ELE_T result_win_ele_pool[]=
{
	{{"产品编号","Product number"}, RESULT_WIN_PRODUCT_NUM      },
	{{"000001","000001"}, RESULT_WIN_NUM01      },
	{{"000002","000002"}, RESULT_WIN_NUM02      },
	{{"000003","000003"}, RESULT_WIN_NUM03      },
	{{"000004","000004"}, RESULT_WIN_NUM04      },
	{{"000005","000005"}, RESULT_WIN_NUM05      },
	{{"000006","000006"}, RESULT_WIN_NUM06      },
	{{"000007","000007"}, RESULT_WIN_NUM07      },
	{{"000008","000008"}, RESULT_WIN_NUM08      },
	{{"000009","000009"}, RESULT_WIN_NUM09      },
	{{"000010","000010"}, RESULT_WIN_NUM10      },
	{{"000011","000011"}, RESULT_WIN_NUM11      },
	{{"000012","000012"}, RESULT_WIN_NUM12      },
	{{"000013","000013"}, RESULT_WIN_NUM13      },
	{{"000014","000014"}, RESULT_WIN_NUM14      },
	{{"000015","000015"}, RESULT_WIN_NUM15      },
	{{"000016","000016"}, RESULT_WIN_NUM16      },
	{{"测试结果信息","Test Result Information"}, RESULT_WIN_TEST_RES_INF},
	{{"第1路 1 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 1 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_1},
	{{"第1路 2 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 2 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_2},
	{{"第1路 3 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 3 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_3},
	{{"第2路 1 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 1 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_4},
	{{"第2路 2 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 2 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_5},
	{{"第2路 3 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 3 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_6},
	{{"第3路 1 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 1 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_7},
	{{"第3路 2 ACW 5.000kV 2.000mA --mA 003.0s 合格",
      "ROAD1 2 ACW 5.000kV 2.000mA --mA 003.0s PASS"}, RESULT_WIN_TEST_RES_8},
      
	{{"设置参数", "SETTING PAR."}, RESULT_WIN_SETTING_PAR},
	{{"测试模式:ACW\n"
       "输出电压:5.000kV\n"
       "电流上限:2.000mA\n"
       "电流下限:2.000mA\n"
       "测试时间:003.0s\n"
       "上升时间:003.0s\n",
       "TEST MODE:ACW\n"
       "Output Vol.:5.000kV\n"
       "Cur High:2.000mA\n"
       "Cur Low:2.000mA\n"
       "TEST TIME:003.0s\n"
       "RISE TIME:003.0s\n",}, RESULT_WIN_SETTING_PAR_C},
       
	{{"测试数据", "TEST DATA"}, RESULT_WIN_TEST_DATA},
	{{
       "输出电压:5.000kV\n"
       "测试电流:2.000mA\n"
       "测试时间:003.0s\n"
       "测试结果:合格\n"
       "记录时间:2017-9-15 12:12:12\n",
       "Output Vol.:5.000kV\n"
       "TEST CUR:2.000mA\n"
       "TEST TIME:3.0s\n"
       "TEST RESULT:PASS\n"
       "RECORD TIME:2017-9-15 12:12:12\n",}, RESULT_WIN_TEST_DATA_C},
};
CS_INDEX result_win_text_ele_index_pool[]=
{
    RESULT_WIN_PRODUCT_NUM,
    RESULT_WIN_NUM01,
    RESULT_WIN_NUM02,
    RESULT_WIN_NUM03,
    RESULT_WIN_NUM04,
    RESULT_WIN_NUM05,
    RESULT_WIN_NUM06,
    RESULT_WIN_NUM07,
    RESULT_WIN_NUM08,
    RESULT_WIN_NUM09,
    RESULT_WIN_NUM10,
    RESULT_WIN_NUM11,
    RESULT_WIN_NUM12,
    RESULT_WIN_NUM13,
    RESULT_WIN_NUM14,
    RESULT_WIN_NUM15,
    RESULT_WIN_NUM16,
    
    RESULT_WIN_TEST_RES_INF,
    RESULT_WIN_TEST_RES_1,
    RESULT_WIN_TEST_RES_2,
    RESULT_WIN_TEST_RES_3,
    RESULT_WIN_TEST_RES_4,
    RESULT_WIN_TEST_RES_5,
    RESULT_WIN_TEST_RES_6,
    RESULT_WIN_TEST_RES_7,
    RESULT_WIN_TEST_RES_8,
    
    RESULT_WIN_SETTING_PAR,
    RESULT_WIN_SETTING_PAR_C,
    RESULT_WIN_TEST_DATA,
    RESULT_WIN_TEST_DATA_C,
};
/**
  * @brief 结果管理界面文本对象池数组
  */
MYUSER_WINDOW_T result_windows =
{
    {"结果管理窗口", "Result window"},
    result_win_cb,update_menu_key_inf,
	{
        result_win_ele_pool,COUNT_ARRAY_SIZE(result_win_ele_pool),
        (CS_INDEX*)result_win_text_ele_index_pool,
        COUNT_ARRAY_SIZE(result_win_text_ele_index_pool),
        init_create_result_win_text_ele
    },/*text*/
    {0},/*edit*/
    {0},/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    result_win_pos_size_pool/*pos_size_pool*/
};

static MENU_KEY_INFO_T 	result_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP, result_exist_f1_cb },//f1
    {"", F_KEY_READ		, KEY_F2 & _KEY_UP, result_exist_f2_cb },//f2
    {"", F_KEY_EDIT		, KEY_F3 & _KEY_UP, result_exist_f3_cb },//f3
    {"", F_KEY_DEL		, KEY_F4 & _KEY_UP, result_exist_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, result_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, result_exist_f6_cb },//f6
};
static MENU_KEY_INFO_T 	result_no_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP, result_no_exist_f1_cb },//f1
    {"", F_KEY_NEW		, KEY_F2 & _KEY_UP, result_no_exist_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, result_no_exist_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, result_no_exist_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP, result_no_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, result_no_exist_f6_cb },//f6
};
static CONFIG_FUNCTION_KEY_INFO_T 	sys_key_pool[]={
	{KEY_UP		, direct_key_up_cb		},
	{KEY_DOWN	, direct_key_down_cb 	},
	{CODE_LEFT	, direct_key_down_cb	 },
	{CODE_RIGH	, direct_key_up_cb      },
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  结果存在菜单功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}


/**
  * @brief  结果不存在菜单功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  向上功能键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_up_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_DecSel(list_h);
    update_cur_row_menu_key_st(key_msg->user_data);
}
/**
  * @brief  向下功能键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_down_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_IncSel(list_h);
    update_cur_row_menu_key_st(key_msg->user_data);
}

/**
  * @brief  更新功能键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}
/**
  * @brief  更新功能键和菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_menu_key_inf(hWin);
    update_sys_key_inf(hWin);
}
/**
  * @brief  更新当前行的菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_cur_row_menu_key_st(WM_HWIN hWin)
{
    int row = 0;
    uint8_t size = 0;
    MENU_KEY_INFO_T *info = NULL;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 步骤存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        size = ARRAY_SIZE(result_exist_menu_key_info);
        info = result_exist_menu_key_info;
	}
	/* 步骤不存在 */
	else
	{
        size = ARRAY_SIZE(result_no_exist_menu_key_info);
        info = result_no_exist_menu_key_info;
	}
    
	init_menu_key_info(info, size, hWin);//刷新菜单键显示
}
/**
  * @brief  更新菜单键信息
  * @param  无
  * @retval 无
  */
static void update_menu_key_inf(WM_HMEM hWin)
{
    update_cur_row_menu_key_st(hWin);
}
/**
  * @brief  窗口重绘
  * @param  无
  * @retval 无
  */
static void result_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  创建结果列表
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static WM_HWIN create_result_listview(WM_HWIN hWin)
{
    WM_HWIN handle = 0;
    
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
//            handle = _7_create_result_listview(hWin);
            break;
    }
    
    return handle;
}
static void init_result_win_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_result_win_layout1_text_ele_pos(result_win_ele_pool);
            break;
    }
}

/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_result_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_result_win_text_ele_pos_inf();
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
//    update_result_win_text_ele_text(win);
}
/**
  * @brief  结果管理窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void result_win_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			WM_SetFocus(hWin);/* 设置聚焦 */
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
            init_create_win_all_ele(win);
            update_key_inf(hWin);
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			result_win_paint_frame();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建结果管理窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_result_win(int hWin)
{
    create_user_window(&result_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
