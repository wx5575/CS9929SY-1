/**
  ******************************************************************************
  * @file    module_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   启动窗口
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "LISTVIEW.H"
#include "FILE_SYS.H"
#include "image/user_image.h"
#include "image/logo.h"
#include "UI_COM/com_ui_info.h"
#include "rtc_config.h"
#include "7_module_win.h"
#include "module_win.h"
#include "PROGBAR.h"
#include "module_manage.h"
#include "scan_module.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void module_win_cb(WM_MESSAGE * pMsg);
static void update_module_win_menu_key_inf(WM_HMEM hWin);
static void update_module_win_stop_menu_key_inf(WM_HMEM hWin);
static void update_module_win_start_menu_key_inf(WM_HMEM hWin);

static void module_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void module_win_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void module_win_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void module_win_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void module_win_sys_key_enter_cb(KEY_MESSAGE *key_msg);

static void module_win_start_f1_cb(KEY_MESSAGE *key_msg);
static void module_win_start_f2_cb(KEY_MESSAGE *key_msg);
static void module_win_start_f3_cb(KEY_MESSAGE *key_msg);
static void module_win_start_f4_cb(KEY_MESSAGE *key_msg);
static void module_win_start_f5_cb(KEY_MESSAGE *key_msg);
static void module_win_start_f6_cb(KEY_MESSAGE *key_msg);

static void module_win_stop_f1_cb(KEY_MESSAGE *key_msg);
static void module_win_stop_f2_cb(KEY_MESSAGE *key_msg);
static void module_win_stop_f3_cb(KEY_MESSAGE *key_msg);
static void module_win_stop_f4_cb(KEY_MESSAGE *key_msg);
static void module_win_stop_f5_cb(KEY_MESSAGE *key_msg);
static void module_win_stop_f6_cb(KEY_MESSAGE *key_msg);

static void module_win_update_key_inf(WM_HWIN hWin);
static void init_module_listview(void);
static void clear_module_listview(void);
/* Private variables ---------------------------------------------------------*/

static	LISTVIEW_Handle module_list_handle;///<模块管理列表句柄
static	WM_HWIN timer_handle;///<定时器句柄
/**
  * @brief  模块管理窗口文本控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static TEXT_ELE_AUTO_LAYOUT_T  *module_win_text_ele_auto_layout[]=
{
    &_7_module_text_ele_auto_layout_inf,//4.3寸屏
    &_7_module_text_ele_auto_layout_inf,//5.6寸屏
    &_7_module_text_ele_auto_layout_inf,//7寸屏
};
/**
  * @brief  模块管理窗口文本控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static ADJUST_TEXT_ELE_LAYOUT_INF  *module_win_adjust_text_ele_layout_inf[]=
{
    &_7_module_win_adjust_text_ele_layout_inf,//4.3寸屏
    &_7_module_win_adjust_text_ele_layout_inf,//5.6寸屏
    &_7_module_win_adjust_text_ele_layout_inf,//7寸屏
};
/** 模块管理界面显示的文本索引表 */
static CS_INDEX module_win_text_ele_table[] =
{
    MODULE_WIN_COM1_NAME,///<串口1名称
    MODULE_WIN_COM1_ADDR,///<串口1地址
    MODULE_WIN_COM2_NAME,///<串口2名称
    MODULE_WIN_COM2_ADDR,///<串口2地址
    MODULE_WIN_COM3_NAME,///<串口3名称
    MODULE_WIN_COM3_ADDR,///<串口3地址
    MODULE_WIN_COM4_NAME,///<串口4名称
    MODULE_WIN_COM4_ADDR,///<串口4地址
};
/**
  * @brief  系统按键信息
  */
static CONFIG_FUNCTION_KEY_INFO_T module_win_sys_key_pool[]=
{
	{KEY_UP		, module_win_direct_key_up_cb      },
	{KEY_DOWN	, module_win_direct_key_down_cb    },
	{KEY_LEFT	, module_win_direct_key_left_cb    },
	{KEY_RIGHT	, module_win_direct_key_right_cb   },
	{KEY_ENTER	, module_win_sys_key_enter_cb      },
	{CODE_LEFT	, module_win_direct_key_down_cb    },
	{CODE_RIGH	, module_win_direct_key_up_cb      },
};
/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* module_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_module_windows,/*4.3寸屏*/
    &_7_module_windows,/*5.6寸屏*/
    &_7_module_windows,/*7寸屏*/
};
/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	module_win_start_menu_key_inf[] = 
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, module_win_start_f1_cb },//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, module_win_start_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, module_win_start_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, module_win_start_f4_cb },//f4
    {"", F_KEY_START    , KEY_F5 & _KEY_UP, module_win_start_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, module_win_start_f6_cb },//f6
};
/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	module_win_stop_menu_key_inf[] = 
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, module_win_stop_f1_cb },//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, module_win_stop_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, module_win_stop_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, module_win_stop_f4_cb },//f4
    {"", F_KEY_STOP     , KEY_F5 & _KEY_UP, module_win_stop_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, module_win_stop_f6_cb },//f6
};
/**
  * @brief  模块管理界面的文本对象池
  */
static TEXT_ELE_T module_win_text_ele_pool[]=
{
	{{"串口1:" ,"COM1:"}, MODULE_WIN_COM1_NAME },
	{{"01"     ,"01"   }, MODULE_WIN_COM1_ADDR },
	{{"串口2:" ,"COM2:"}, MODULE_WIN_COM2_NAME  },
	{{"01"     ,"01"   }, MODULE_WIN_COM2_ADDR  },
	{{"串口3:" ,"COM3:"}, MODULE_WIN_COM3_NAME  },
	{{"01"     ,"01"   }, MODULE_WIN_COM3_ADDR  },
	{{"串口4:" ,"COM4:"}, MODULE_WIN_COM4_NAME  },
	{{"01"     ,"01"   }, MODULE_WIN_COM4_ADDR  },
};
/**
  * @brief  模块管理窗口结构体初始化
  */
MYUSER_WINDOW_T module_windows=
{
    {"模块管理窗口", "Module Window"},
    module_win_cb, module_win_update_key_inf,
	{
        module_win_text_ele_pool, ARRAY_SIZE(module_win_text_ele_pool),
        (CS_INDEX*)module_win_text_ele_table,ARRAY_SIZE(module_win_text_ele_table)
    },/*text*/
    {0},
    {0},
    /* 自动布局 */
    {
        module_win_text_ele_auto_layout,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        module_win_adjust_text_ele_layout_inf,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    module_win_pos_size_pool,/*pos_size_pool */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  初始化四路串口的扫描模块状态
  * @param  [in] inf 扫描模块信息
  * @retval 无
  */
void init_all_scan_mode_st(void)
{
    stop_scan_all_module();
}
/**
  * @brief  启动四路串口的扫描模块状态
  * @param  [in] inf 扫描模块信息
  * @retval 无
  */
void start_all_scan_mode_st(void)
{
    start_scan_all_module();
}
/**
  * @brief  模块管理窗口功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_start_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_start_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_start_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_start_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_start_f5_cb(KEY_MESSAGE *key_msg)
{
    update_module_win_stop_menu_key_inf(key_msg->user_data);
    start_all_scan_mode_st();
    clear_module_inf();//清空模块信息
    clear_module_listview();//清空模块表
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_start_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_stop_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_stop_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_stop_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_stop_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_stop_f5_cb(KEY_MESSAGE *key_msg)
{
    update_module_win_start_menu_key_inf(key_msg->user_data);
    init_all_scan_mode_st();
    save_roads_flag();
}
/**
  * @brief  模块管理窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void module_win_stop_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  更新模块管理界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_module_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(module_win_start_menu_key_inf, ARRAY_SIZE(module_win_start_menu_key_inf), hWin);
}
/**
  * @brief  更新模块管理界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_module_win_start_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(module_win_start_menu_key_inf, ARRAY_SIZE(module_win_start_menu_key_inf), hWin);
}
/**
  * @brief  更新模块管理界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_module_win_stop_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(module_win_stop_menu_key_inf, ARRAY_SIZE(module_win_stop_menu_key_inf), hWin);
}
/**
  * @brief  模块管理窗口向上键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void module_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_DecSel(module_list_handle);
}
/**
  * @brief  模块管理窗口向下键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void module_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_IncSel(module_list_handle);
}
/**
  * @brief  模块管理窗口向左键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void module_win_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口向右键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void module_win_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  模块管理窗口确认键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void module_win_sys_key_enter_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(module_win_sys_key_pool, ARRAY_SIZE(module_win_sys_key_pool), hWin);
}

/**
  * @brief  设置模块管理界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_module_windows_handle(WM_HWIN hWin)
{
    module_windows.handle = hWin;
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
static void module_win_update_key_inf(WM_HWIN hWin)
{
    update_module_win_menu_key_inf(hWin);
    update_system_key_inf(hWin);
}

/**
  * @brief  清空模块表
  * @param  无
  * @retval 无
  */
static void clear_module_listview(void)
{
    int32_t row = 0;
    int32_t column = 0;
    uint32_t n = MAX_MODULE_NUM;
    
    for(row = 0; row < n; row++)
    {
        for(column = 1; column < 5; column++)
        {
            LISTVIEW_SetItemText(module_list_handle, column, row, "");//清空单元格的显示信息
        }
    }
}
static module_scan_manage(WM_HWIN hWin, MYUSER_WINDOW_T* win)
{
    uint8_t addr = 0;
    uint8_t buf[10];
    uint8_t com1_scan_over = 0;
    uint8_t com2_scan_over = 0;
    uint8_t com3_scan_over = 0;
    uint8_t com4_scan_over = 0;
    
    scan_all_module();
    init_module_listview();//刷新扫到的模块信息
    
    /* 刷新扫描地址 */
    addr = get_com1_scan_addr();
    sprintf((char*)buf, "%02d", addr);
    update_text_ele((CS_INDEX)MODULE_WIN_COM1_ADDR, win, buf);
    
    /* 刷新扫描地址 */
    addr = get_com2_scan_addr();
    sprintf((char*)buf, "%02d", addr);
    update_text_ele((CS_INDEX)MODULE_WIN_COM2_ADDR, win, buf);
    
    addr = get_com3_scan_addr();
    sprintf((char*)buf, "%02d", addr);
    update_text_ele((CS_INDEX)MODULE_WIN_COM3_ADDR, win, buf);
    
    /* 刷新扫描地址 */
    addr = get_com4_scan_addr();
    sprintf((char*)buf, "%02d", addr);
    update_text_ele((CS_INDEX)MODULE_WIN_COM4_ADDR, win, buf);
    
    com1_scan_over = com1_scan_is_over();
    com2_scan_over = com2_scan_is_over();
    com3_scan_over = com3_scan_is_over();
    com4_scan_over = com4_scan_is_over();
    
    /* 四路串口扫描全部结束 */
    if(com1_scan_over == 1 && com2_scan_over == 1 
        && com3_scan_over == 1 && com4_scan_over == 1)
    {
        update_module_win_start_menu_key_inf(hWin);
        save_roads_flag();
    }
}
void create_module_win_listview(WM_HWIN hWin)
{
    
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            module_list_handle = _7_create_module_listview(hWin);
            break;
    }
}

void draw_one_module_inf(uint8_t addr, uint8_t row)
{
    uint8_t buf[5][100];
    int32_t i = 0;
    uint8_t k = 0;
    uint8_t colum = 4;
//    uint8_t row = 0;
    
    sprintf((char*)buf[i++], "COM%d", module_inf_pool[addr].com_num + 1);
    sprintf((char*)buf[i++], "%d", module_inf_pool[addr].module_inf.id);
    sprintf((char*)buf[i++], "%s", module_inf_pool[addr].module_inf.name);
    sprintf((char*)buf[i++], "%s", module_inf_pool[addr].module_inf.version);
    
    for(k = 0; k < colum; k++)
    {
        LISTVIEW_SetItemText(module_list_handle, k + 1, row, (const char*)buf[k]);
    }
    
    row++;
}
static void init_module_listview(void)
{
    uint8_t addr = 0;
    int32_t i = 0;
    int32_t j = 0;
    uint8_t row = 0;
    
    for(j = 0; j < SYN_MAX_COM_NUM; j++)
    {
        for(i = 0; i <= SLAVE_ADDR_MAX; i++)
        {
            addr = roads_flag.flag[j].road_buf[i];
            
            if(addr > 0 && addr < MASTER_ADDR_RANGE)
            {
                draw_one_module_inf(addr, row++);
            }
        }
    }
    
}
/**
  * @brief  模块管理测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void module_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_module_windows_handle(hWin);
			win = get_user_window_info(hWin);
            create_module_win_listview(hWin);
            init_module_listview();
            init_create_win_all_ele(win);
            
            init_all_scan_mode_st();
            timer_handle = WM_CreateTimer(hWin, 0, 10, 0);
			break;
		case WM_PAINT:
			_PaintFrame();
            draw_group_inf_area();
			break;
		case WM_TIMER:
		{
			win = get_user_window_info(hWin);
            module_scan_manage(hWin, win);
            WM_RestartTimer(timer_handle, 10);
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
  * @brief  创建模块管理窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_module_win(int hWin)
{
    create_user_window(&module_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
