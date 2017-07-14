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
#include "FILE_SYS.H"
#include "image/user_image.h"
#include "image/logo.h"
#include "UI_COM/com_ui_info.h"
#include "rtc_config.h"
#include "7_module_win.h"
#include "module_win.h"
#include "PROGBAR.h"
#include "module_manage.h"


/* Private typedef -----------------------------------------------------------*/

typedef enum{

    SCAN_STOP,///<停止扫描
    SCAN_START,///<启动扫描
    SCAN_OVER,///<启动扫描
    SCAN_INIT,///<扫描初始化值
}SCAN_MODULE_ST;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void module_win_cb(WM_MESSAGE * pMsg);
static void update_module_win_menu_key_inf(WM_HMEM hWin);
static void update_module_win_stop_menu_key_inf(WM_HMEM hWin);
static void update_module_win_start_menu_key_inf(WM_HMEM hWin);

static void sys_exit_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_stop_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg);
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg);
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg);

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
/* Private variables ---------------------------------------------------------*/

static	WM_HWIN timer_handle;///<定时器句柄
//static	WM_HWIN U_FLASH_1_handle;///<U盘图标1句柄
//static	WM_HWIN U_FLASH_2_handle;///<U盘图标2句柄
//static	WM_HWIN KEY_LOCK_handle;///<键盘锁图标句柄
//static	WM_HWIN KEY_CAPITAL_SMALL_handle;///<大小写图标句柄
static SCAN_MODULE_ST scan_status;///<扫描模块运行状态
static SCAN_MODULE_ST com1_scan_status;///<扫描模块运行状态
static SCAN_MODULE_ST com2_scan_status;///<扫描模块运行状态
static SCAN_MODULE_ST com3_scan_status;///<扫描模块运行状态
static SCAN_MODULE_ST com4_scan_status;///<扫描模块运行状态
static int8_t com1_scan_addr = 1;///<第1路串口扫描地址
static int8_t com2_scan_addr = 1;///<第2路串口扫描地址
static int8_t com3_scan_addr = 1;///<第3路串口扫描地址
static int8_t com4_scan_addr = 1;///<第4路串口扫描地址


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
    scan_status = SCAN_START;
    com1_scan_addr = 1;
    com2_scan_addr = 17;
    com3_scan_addr = 33;
    com4_scan_addr = 49;
    com1_scan_status = SCAN_START;
    com2_scan_status = SCAN_START;
    com3_scan_status = SCAN_START;
    com4_scan_status = SCAN_START;
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
    scan_status = SCAN_STOP;
    com1_scan_status = SCAN_STOP;
    com2_scan_status = SCAN_STOP;
    com3_scan_status = SCAN_STOP;
    com4_scan_status = SCAN_STOP;
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
  * @brief  系统EXIT按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_exit_key_fun_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  系统STOP按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void sys_stop_key_fun_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  系统SHIFT按键回调函数
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_shift_key_fun_cb(KEY_MESSAGE *key_msg)
{
}



/**
  * @brief  系统键盘锁按键回调函数
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_unlock_key_fun_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  截屏按键的
  * @param  [in] data 用户数据
  * @retval 无
  */
static void screen_capture_key_fun_cb(KEY_MESSAGE *key_msg)
{
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

typedef struct{
    uint8_t status;///<扫描状态
    uint8_t addr;///<当前的扫描地址
    uint8_t start_addr;///<起始地址
    uint8_t end_addr;///<结束地址
    uint8_t offset_addr;///<偏移地址 串口1的偏移地址是0 串口2的偏移地址是16 串口3是32 串口4是48
    CS_INDEX text_index;///<界面对应地址的文本对象索引，用来刷新界面显示
}SCAN_MODULE_T;

SCAN_MODULE_T com1_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    15,///<结束地址
    0,///<偏移地址
    MODULE_WIN_COM1_ADDR///<界面对应地址的文本对象索引
};

void scan_module(SCAN_MODULE_T *inf, MYUSER_WINDOW_T* win)
{
    CS_ERR err;
    uint8_t buf[5]= {0};
    uint8_t addr = 0;
    
    if(inf->status == SCAN_START)
    {
        err = com_module_connect(inf->addr);
        
        if(err == CS_ERR_SEND_SUCCESS)
        {
            sprintf((char*)buf, "%02d", inf->addr - inf->offset_addr);
            update_text_ele((CS_INDEX)inf->text_index, win, buf);//更新界面显示
            inf->addr++;
            111
            if(inf->addr > inf->end_addr)
            {
                inf->status = SCAN_OVER;
            }
        }
    }
}

static module_scan_manage(WM_HWIN hWin, MYUSER_WINDOW_T* win)
{
    CS_ERR err;
    uint8_t buf[5]= {0};
    
    if(scan_status == SCAN_START)
    {
        if(com1_scan_status == SCAN_START)
        {
            err = com_module_connect(com1_scan_addr);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                sprintf((char*)buf, "%02d", com1_scan_addr - 16 * 0);
                update_text_ele((CS_INDEX)MODULE_WIN_COM1_ADDR, win, buf);
                com1_scan_addr++;
                
                if(com1_scan_addr >= 16)
                {
                    com1_scan_status = SCAN_OVER;
                }
            }
        }
        
        if(com2_scan_status == SCAN_START)
        {
            err = com_module_connect(com2_scan_addr);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                sprintf((char*)buf, "%02d", com2_scan_addr - 16 * 1);
                update_text_ele((CS_INDEX)MODULE_WIN_COM2_ADDR, win, buf);
                com2_scan_addr++;
                
                if(com2_scan_addr >= 32)
                {
                    com2_scan_status = SCAN_OVER;
                }
            }
        }
        
        if(com3_scan_status == SCAN_START)
        {
            err = com_module_connect(com3_scan_addr);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                sprintf((char*)buf, "%02d", com3_scan_addr - 16 * 2);
                update_text_ele((CS_INDEX)MODULE_WIN_COM3_ADDR, win, buf);
                com3_scan_addr++;
                
                if(com3_scan_addr >= 48)
                {
                    com3_scan_status = SCAN_OVER;
                }
            }
        }
        
        if(com4_scan_status == SCAN_START)
        {
            err = com_module_connect(com4_scan_addr);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                sprintf((char*)buf, "%02d", com4_scan_addr - 16 * 3);
                update_text_ele((CS_INDEX)MODULE_WIN_COM4_ADDR, win, buf);
                com4_scan_addr++;
                
                if(com4_scan_addr >= 64)
                {
                    com4_scan_status = SCAN_OVER;
                }
            }
        }
        
        if(com1_scan_status == SCAN_OVER
            && com2_scan_status == SCAN_OVER
            && com3_scan_status == SCAN_OVER
            && com4_scan_status == SCAN_OVER)
        {
            scan_status = SCAN_OVER;
            update_module_win_start_menu_key_inf(hWin);
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
            
            init_create_win_all_ele(win);
            
            com1_scan_addr = 1;
            com2_scan_addr = 17;
            com3_scan_addr = 33;
            com4_scan_addr = 49;
            scan_status = SCAN_INIT;
            com1_scan_status = SCAN_INIT;
            com2_scan_status = SCAN_INIT;
            com3_scan_status = SCAN_INIT;
            com4_scan_status = SCAN_INIT;
            timer_handle = WM_CreateTimer(hWin, 0, 10, 0);
			break;
		case WM_PAINT:
			_PaintFrame();
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
