/**
  ******************************************************************************
  * @file    calibration_win.c
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
#include "ui_com/com_edit_api.h"
#include "rtc_config.h"
#include "7_calibration_win.h"
#include "calibration_win.h"
#include "PROGBAR.h"
#include "LISTVIEW.H"
#include "sel_cal_module_win.h"
#include "module_manage.h"
#include "send_cmd.h"

/* Private typedef -----------------------------------------------------------*/

enum{
    CAL_ST_IDLE,///<校准空闲状态
    GET_CAL_POINTS,///<获取校准点总个数
    SLAVE_INTO_CAL,///<从机进入校准状态
    SLAVE_EXIT_CAL,///<从机退出校准状态
    GET_ALL_CAL_POINTS_INF,///<获取所有校准点的信息
    AUTO_INIT_SLAVE_CAL_ENV,///<自动初始化从机校准环境
    EXIT_CAL_WIN,///<退出校准窗口
    CAL_START,///<启动校准
    CAL_STOP,///<停止校准
    SET_MEASURE_VALUE,///<设置测试值
    LOAD_CAL_POINT,///<加载校准点作为当前校准点
};
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void calibration_win_cb(WM_MESSAGE * pMsg);
static void update_calibration_win_menu_key_inf(WM_HMEM hWin);


static void calibration_win_f1_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f2_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f3_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f4_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f5_cb(KEY_MESSAGE *key_msg);
static void calibration_win_f6_cb(KEY_MESSAGE *key_msg);

static void calibration_win_update_key_inf(WM_HWIN hWin);
static void init_create_calibration_win_edit_ele(MYUSER_WINDOW_T *win);
static void init_calibration_win_edit_ele_inf(void);
static void set_calibration_win_ele_data(void);


static void cal_measured_value_f1_cb(KEY_MESSAGE *key_msg);
static void cal_measured_value_f2_cb(KEY_MESSAGE *key_msg);
static void cal_measured_value_f3_cb(KEY_MESSAGE *key_msg);
static void cal_measured_value_f4_cb(KEY_MESSAGE *key_msg);
static void cal_measured_value_f5_cb(KEY_MESSAGE *key_msg);
static void cal_measured_value_f6_cb(KEY_MESSAGE *key_msg);


static void cal_measured_value_direct_key_up_cb	  (KEY_MESSAGE *key_msg);
static void cal_measured_value_direct_key_down_cb (KEY_MESSAGE *key_msg);
static void cal_measured_value_direct_key_left_cb (KEY_MESSAGE *key_msg);
static void cal_measured_value_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void cal_measured_value_enter_key_up_cb	  (KEY_MESSAGE *key_msg);


static void calibration_win_direct_key_up_cb   (KEY_MESSAGE *key_msg);
static void calibration_win_direct_key_down_cb (KEY_MESSAGE *key_msg);
static void calibration_win_direct_key_left_cb (KEY_MESSAGE *key_msg);
static void calibration_win_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void calibration_win_enter_key_up_cb	   (KEY_MESSAGE *key_msg);


static void cal_measured_value_sys_key(WM_HMEM hWin);
static void cal_measured_value_menu_key(WM_HMEM hWin);
static void cal_measured_value_press_enter(WM_HMEM hWin);
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

static uint8_t cur_cal_point_num;///<当前校准点的编号
static uint8_t cal_status = CAL_ST_IDLE;
static uint8_t auto_init_flag = CAL_ST_IDLE;
static	WM_HWIN cal_win_timer_handle;///<定时器句柄
static	LISTVIEW_Handle calibration_list_handle;///<校准管理列表句柄
/**
  * @brief  系统按键信息
  */
static CONFIG_FUNCTION_KEY_INFO_T calibration_win_sys_key_pool[]=
{
	{KEY_UP		, calibration_win_direct_key_up_cb		 },
	{KEY_DOWN	, calibration_win_direct_key_down_cb	 },
	{KEY_LEFT	, calibration_win_direct_key_left_cb	 },
	{KEY_RIGHT	, calibration_win_direct_key_right_cb	 },
	{KEY_ENTER	, calibration_win_enter_key_up_cb	     },
    
	{CODE_LEFT	, calibration_win_direct_key_down_cb    },
	{CODE_RIGH	, calibration_win_direct_key_up_cb	     },
};

/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* calibration_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_calibration_windows,/*4.3寸屏*/
    &_7_calibration_windows,/*5.6寸屏*/
    &_7_calibration_windows,/*7寸屏*/
};
/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	calibration_win_menu_key_inf[] = 
{
    {"", F_KEY_START    , KEY_F1 & _KEY_UP, calibration_win_f1_cb },//f1
    {"", F_KEY_F2		, KEY_F2 & _KEY_UP, calibration_win_f2_cb },//f2
    {"", F_KEY_F3		, KEY_F3 & _KEY_UP, calibration_win_f3_cb },//f3
    {"", F_KEY_MODULE   , KEY_F4 & _KEY_UP, calibration_win_f4_cb },//f4
    {"", F_KEY_ENTER    , KEY_F5 & _KEY_UP, calibration_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, calibration_win_f6_cb },//f6
};
/**
  * @brief  校准管理窗口文本控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static TEXT_ELE_AUTO_LAYOUT_T  *calibration_win_text_ele_auto_layout[]=
{
    &_7_calibration_text_ele_auto_layout_inf,//4.3寸屏
    &_7_calibration_text_ele_auto_layout_inf,//5.6寸屏
    &_7_calibration_text_ele_auto_layout_inf,//7寸屏
};

/**
  * @brief  校准管理窗口文本控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static ADJUST_TEXT_ELE_LAYOUT_INF  *calibration_win_adjust_text_ele_layout_inf[]=
{
    &_7_calibration_win_adjust_text_ele_layout_inf,//4.3寸屏
    &_7_calibration_win_adjust_text_ele_layout_inf,//5.6寸屏
    &_7_calibration_win_adjust_text_ele_layout_inf,//7寸屏
};
static CS_INDEX calibration_win_text_index_pool[]=
{
    CAL_WIN_MODULE_NUM,///<校准模块编号
    CAL_WIN_MODULE_NUM_V,///<校准模块编号值
    CAL_WIN_MODULE_ADDR,///<校准模块地址
    CAL_WIN_MODULE_ADDR_V,///<校准模块地址值
    CAL_WIN_MODULE_PORT,///<校准模块串口号
    CAL_WIN_MODULE_PORT_V,///<校准模块串口号值
    CAL_WIN_MODULE_TOTAL,///<校准模块校准点总数
    CAL_WIN_MODULE_TOTAL_V,///<校准模块校准点总数值
};
/**
  * @brief  校准管理界面的文本对象池
  */
static TEXT_ELE_T calibration_win_text_ele_pool[]=
{
	{{"编号:"  ,"MODULE:"}   , CAL_WIN_MODULE_NUM },
	{{"01"     ,"01"     }    , CAL_WIN_MODULE_NUM_V },
	{{"地址:"  ,"ADDR:"}  , CAL_WIN_MODULE_ADDR },
	{{"01"     ,"01"   }            , CAL_WIN_MODULE_ADDR_V  },
	{{"串口号:","PORT:"}           , CAL_WIN_MODULE_PORT  },
	{{"COM1"   ,"COM1"   }        , CAL_WIN_MODULE_PORT_V  },
	{{"校准点:","POINT:"}           , CAL_WIN_MODULE_TOTAL  },
	{{"1/0"    ,"1/0"   }        , CAL_WIN_MODULE_TOTAL_V  },
};

enum{
    CAL_WIN_EDIT_INDEX,///<校准窗口中的编辑对象索引
};
CS_INDEX calibration_win_edit_index_table[]=
{
    CAL_WIN_EDIT_INDEX,///<校准窗口中的编辑对象索引
};

/**
  * @brief  原始密码 新设密码使用的菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	cal_measured_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, cal_measured_value_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, cal_measured_value_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, cal_measured_value_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, cal_measured_value_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP, cal_measured_value_f5_cb },//f5
    {"", F_KEY_ENTER    , KEY_F6 & _KEY_UP, cal_measured_value_f6_cb },//f6
};

static CONFIG_FUNCTION_KEY_INFO_T cal_measured_value_sys_key_pool[]={
	{KEY_UP		, cal_measured_value_direct_key_up_cb		 },
	{KEY_DOWN	, cal_measured_value_direct_key_down_cb	 },
	{KEY_LEFT	, cal_measured_value_direct_key_left_cb	 },
	{KEY_RIGHT	, cal_measured_value_direct_key_right_cb	 },
	{KEY_ENTER	, cal_measured_value_enter_key_up_cb	     },
    
	{CODE_LEFT	, cal_measured_value_direct_key_down_cb    },
	{CODE_RIGH	, cal_measured_value_direct_key_up_cb	     },
};
/**
  * @brief 界面编辑对象池数组
  */
static EDIT_ELE_T calibration_win_edit_ele_pool[]=
{
    {
        {"",""}, /* 名称 */
        CAL_WIN_EDIT_INDEX,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 0/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, },/*类型*/
        {3/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {cal_measured_value_sys_key, cal_measured_value_menu_key, keyboard_fun_num,},/*key_inf*/
    },
};
/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T calibration_windows=
{
    {"校准管理窗口", "Calibration Window"},
    calibration_win_cb, calibration_win_update_key_inf,
	{
        calibration_win_text_ele_pool, ARRAY_SIZE(calibration_win_text_ele_pool),
        (CS_INDEX*)calibration_win_text_index_pool,ARRAY_SIZE(calibration_win_text_index_pool)
    },/*text*/
    {
        calibration_win_edit_ele_pool, ARRAY_SIZE(calibration_win_edit_ele_pool),
        calibration_win_edit_index_table, ARRAY_SIZE(calibration_win_edit_index_table),
        NULL,
    },
    {0},
    /* 自动布局 */
    {
        calibration_win_text_ele_auto_layout,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        calibration_win_adjust_text_ele_layout_inf,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    calibration_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

static void init_create_calibration_win_edit_ele(MYUSER_WINDOW_T *win)
{
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    init_calibration_win_edit_ele_inf();
    set_calibration_win_ele_data();
    init_window_edit_ele(win);//初始化创建编辑对象
    
    g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
    select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
}

static void update_cal_win_title_point_inf(void)
{
    uint8_t buf[20] = {0};
    uint32_t row = 0;
    
	row = LISTVIEW_GetSel(calibration_list_handle);
    
    cur_cal_point_num = row + 1;
    
    sprintf((char*)buf, "%d/%d", cur_cal_point_num, cur_module_cal_points);
    update_text_ele((CS_INDEX)CAL_WIN_MODULE_TOTAL_V, g_cur_win, buf);
    
    cal_status = LOAD_CAL_POINT;
}

static void calibration_win_direct_key_up_cb   (KEY_MESSAGE *key_msg)
{
	LISTVIEW_DecSel(calibration_list_handle);
    update_cal_win_title_point_inf();
}

static void calibration_win_direct_key_down_cb (KEY_MESSAGE *key_msg)
{
	LISTVIEW_IncSel(calibration_list_handle);
    update_cal_win_title_point_inf();
}
static void calibration_win_direct_key_left_cb (KEY_MESSAGE *key_msg)
{
}
static void calibration_win_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
}
static void calibration_win_enter_key_up_cb	   (KEY_MESSAGE *key_msg)
{
}

static void cal_measured_value_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
static void cal_measured_value_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
static void cal_measured_value_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_f6_cb(KEY_MESSAGE *key_msg)
{
    cal_measured_value_press_enter(key_msg->user_data);
}

static void cal_measured_value_direct_key_up_cb	  (KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_direct_key_down_cb (KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_direct_key_left_cb (KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
}
static void cal_measured_value_enter_key_up_cb	  (KEY_MESSAGE *key_msg)
{
    cal_measured_value_press_enter(key_msg->user_data);
}
static void cal_measured_value_press_enter(WM_HMEM hWin)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    delete_win_edit_ele(g_cur_win);
    set_cur_edit_ele(NULL);//将当前编辑对象置为空
    g_cur_win->init_key_fun(g_cur_win->handle);
    cal_status = SET_MEASURE_VALUE;
}
static void cal_measured_value_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = cal_measured_menu_key_info;
    uint32_t size = ARRAY_SIZE(cal_measured_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
static void cal_measured_value_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(cal_measured_value_sys_key_pool, ARRAY_SIZE(cal_measured_value_sys_key_pool), hWin);
}

/**
  * @brief  主窗口中功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f1_cb(KEY_MESSAGE *key_msg)
{
    cal_status = CAL_START;
    init_create_calibration_win_edit_ele(g_cur_win);
}
/**
  * @brief  主窗口中功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f2_cb(KEY_MESSAGE *key_msg)
{
    cal_status = CAL_STOP;
}
uint64_t cur_measured_value;
uint32_t cur_mul_power = 1;///<当前校准点的倍率
static void set_calibration_win_ele_data(void)
{
    reg_edit_ele_data_inf(CAL_WIN_EDIT_INDEX, &cur_measured_value, sizeof(cur_measured_value));
}
static void init_calibration_win_edit_ele_inf(void)
{
    GUI_RECT rect;
    uint32_t row = 0;
    uint32_t col = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    CAL_POINT_INF *point_inf;
    
	row = LISTVIEW_GetSel(calibration_list_handle);
    col = 5;
    
    point_inf = &cal_point_inf_pool[row];
    
    LISTVIEW_GetItemRect(calibration_list_handle, col, row, &rect);
    x = rect.x0;
    y = rect.y0;
    width = rect.x1 - rect.x0;
    height = rect.y1 - rect.y0;
    
    {
        EDIT_ELE_T *edit_ele = NULL;
        CS_ERR err;
        EDIT_ELE_T *pool;
        uint32_t n;
        
        pool = g_cur_win->edit.pool;
        n = g_cur_win->edit.pool_size;
        
        edit_ele = get_edit_ele_inf(pool, n, CAL_WIN_EDIT_INDEX, &err);
        
        if(err != CS_ERR_NONE)
        {
            return;
        }
        
        edit_ele->dis.name.width = 1;
        edit_ele->dis.name.height = 1;
        edit_ele->dis.x = x;
        edit_ele->dis.y = y + 30;
        edit_ele->dis.edit.font = &GUI_Fonthz_24;
        edit_ele->dis.edit.back_color = GUI_INVALID_COLOR;
        edit_ele->dis.edit.font_color = GUI_BLACK;
        edit_ele->dis.edit.height = height;
        edit_ele->dis.edit.width = width - 40;
        edit_ele->dis.edit.align = GUI_TA_LEFT | GUI_TA_VCENTER;
        edit_ele->dis.edit.max_len = edit_ele->format.lon;
        edit_ele->dis.unit.width = 30;
        edit_ele->dis.unit.height = height;
        edit_ele->dis.unit.font = &GUI_Fonthz_24;
        edit_ele->dis.unit.back_color = GUI_INVALID_COLOR;
        edit_ele->dis.unit.font_color = GUI_BLACK;
        
        edit_ele->format.unit = point_inf->unit;
        edit_ele->format.lon = point_inf->lon;
        edit_ele->format.dec = point_inf->decs;
        cur_mul_power = point_inf->mul_power;
    }
}
/**
  * @brief  主窗口中功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f3_cb(KEY_MESSAGE *key_msg)
{
    init_create_calibration_win_edit_ele(g_cur_win);
}
/**
  * @brief  主窗口中功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f4_cb(KEY_MESSAGE *key_msg)
{
    create_sel_cal_module_win(key_msg->user_data);
}
/**
  * @brief  主窗口中功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  主窗口中功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void calibration_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  更新主界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_calibration_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(calibration_win_menu_key_inf, ARRAY_SIZE(calibration_win_menu_key_inf), hWin);
}

/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void calibration_win_update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(calibration_win_sys_key_pool, ARRAY_SIZE(calibration_win_sys_key_pool), hWin);
}

/**
  * @brief  设置主界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_calibration_windows_handle(WM_HWIN hWin)
{
    calibration_windows.handle = hWin;
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
static void calibration_win_update_key_inf(WM_HWIN hWin)
{
    update_calibration_win_menu_key_inf(hWin);
    calibration_win_update_system_key_inf(hWin);
}

void create_calibration_win_listview(WM_HWIN hWin)
{
    
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            calibration_list_handle = _7_create_calibration_listview(hWin);
            break;
    }
}

void update_cal_point_dis_inf(uint8_t num)
{
    uint8_t *str = NULL;
//    uint8_t index = 0;
    uint8_t row = 0;
    uint8_t col = 0;
    int32_t i = 0;
    
	for(i = 0; i < num; i++)
	{
        row = i;
        col = 1;
        str = cal_point_inf_pool[i].mode;
        if(strlen((const char*)str) > 8)
        {
            str = "";
        }
		LISTVIEW_SetItemText(calibration_list_handle, col, row, (const char*)str);
        col = 2;
        str = cal_point_inf_pool[i].point;
        if(strlen((const char*)str) > 8)
        {
            str = "";
        }
		LISTVIEW_SetItemText(calibration_list_handle, col, row, (const char*)str);
    }
}

static void update_cal_win_title_inf(uint8_t num)
{
    uint8_t buf[20] = {0};
    ROAD_INDEX road;
    
    road = (ROAD_INDEX)get_cur_cal_road();
    
    sprintf((char*)buf, "COM%d", syn_test_port[road - 1].com->com_num + 1);
    update_text_ele((CS_INDEX)CAL_WIN_MODULE_PORT_V, g_cur_win, buf);
    
    sprintf((char*)buf, "%d", syn_test_port[road - 1].addr - syn_test_port[road - 1].offset_addr);
    update_text_ele((CS_INDEX)CAL_WIN_MODULE_ADDR_V, g_cur_win, buf);
    
    sprintf((char*)buf, "%d", road);
    update_text_ele((CS_INDEX)CAL_WIN_MODULE_NUM_V, g_cur_win, buf);
    
    sprintf((char*)buf, "%d/%d", 1, num);
    update_text_ele((CS_INDEX)CAL_WIN_MODULE_TOTAL_V, g_cur_win, buf);
}
static void clear_cal_listview(void)
{
    int32_t row = 0;
    int32_t column = 0;
    uint32_t n = 60;
    
    for(row = 0; row < n; row++)
    {
        for(column = 1; column < 6; column++)
        {
            LISTVIEW_SetItemText(calibration_list_handle, column, row, "");//清空单元格的显示信息
        }
    }
}

void update_cur_cal_point_inf(void)
{
    uint8_t cur_road;
    uint8_t pre_road;
    
    cur_road = get_cur_cal_road();
    pre_road = get_prev_cal_road();
    
    if(pre_road != cur_road)
    {
        cal_status = AUTO_INIT_SLAVE_CAL_ENV;
        auto_init_flag = EXIT_CAL_WIN;
    }
}
static void cal_status_machine(void)
{
    ROAD_INDEX road;
    CS_ERR err;
    uint8_t index = 0;
    static int32_t cur_point;
    
    switch(cal_status)
    {
        case CAL_ST_IDLE:
            key_scan_on();
            break;
        case LOAD_CAL_POINT:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            
            err = send_cmd_to_one_module(road, &cur_cal_point_num, sizeof(cur_cal_point_num),
                                    send_slave_load_cur_point);
            if(err == CS_ERR_SEND_SUCCESS)
            {
                cal_status = CAL_ST_IDLE;
            }
            
            break;
        }
        case CAL_START:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            
            err = send_cmd_to_one_module(road, NULL, 0, send_slave_start_cal);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                cal_status = CAL_ST_IDLE;
            }
            
            break;
        }
        case CAL_STOP:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            
            err = send_cmd_to_one_module(road, NULL, 0, send_slave_stop_cal);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                if(ROAD_STOPPING ==  read_road_test_status(road))
                {
                    cal_status = CAL_ST_IDLE;
                }
            }
            
            break;
        }
        case SET_MEASURE_VALUE:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            cur_measured_value *= cur_mul_power;
            err = send_cmd_to_one_module(road, (void*)&cur_measured_value, sizeof(cur_measured_value),
                                        send_slave_set_measure_value);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                cal_status = CAL_STOP;
            }
            
            break;
        }
        case GET_CAL_POINTS:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            cur_module_cal_points = 0;
            cur_module_cal_points_bk = 0;
            cur_point = 0;
            clear_cal_listview();
            err = send_cmd_to_one_module(road, NULL, 0, send_query_cal_points);
            
            if(err == CS_ERR_SEND_SUCCESS)
            {
                cal_status = CAL_ST_IDLE;
            }
            
            break;
        }
        case GET_ALL_CAL_POINTS_INF:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            
            {
                index = cur_point;
                err = send_cmd_to_one_module(road, &index, sizeof(index), send_query_cal_point_inf);
                
                if(err == CS_ERR_SEND_SUCCESS)
                {
                    update_cal_point_dis_inf(cur_point);
                    update_cal_win_title_inf(cur_point);
                    cur_point++;
                }
            }
            
            if(cur_point > cur_module_cal_points)
            {
                cal_status = CAL_ST_IDLE;
            }
            break;
        }
        case SLAVE_INTO_CAL:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            
            send_cmd_to_one_module(road, NULL, 0, send_slave_enter_cal_st);
            cal_status = CAL_ST_IDLE;
            
            break;
        }
        case EXIT_CAL_WIN:
        {
            road = (ROAD_INDEX)get_cur_cal_road();
            
            send_cmd_to_one_module(road, NULL, 0, send_slave_exit_cal_st);
            cal_status = CAL_ST_IDLE;
            
            break;
        }
        case AUTO_INIT_SLAVE_CAL_ENV:
        {
            switch(auto_init_flag)
            {
                case EXIT_CAL_WIN:
                {
                    road = (ROAD_INDEX)get_prev_cal_road();
                    
                    err = send_cmd_to_one_module(road, NULL, 0, send_slave_exit_cal_st);
                    
                    if(err == CS_ERR_SEND_SUCCESS)
                    {
                        auto_init_flag = SLAVE_INTO_CAL;
                        road = (ROAD_INDEX)get_cur_cal_road();
                        set_prev_cal_road(road);
                    }
                    break;
                }
                case SLAVE_INTO_CAL:
                {
                    key_scan_off();
                    road = (ROAD_INDEX)get_cur_cal_road();
                    
                    err = send_cmd_to_one_module(road, NULL, 0, send_slave_enter_cal_st);
                    
                    if(err == CS_ERR_SEND_SUCCESS)
                    {
                        auto_init_flag = GET_CAL_POINTS;
                    }
                    break;
                }
                case GET_CAL_POINTS:
                {
                    road = (ROAD_INDEX)get_cur_cal_road();
                    cur_module_cal_points = 0;
                    cur_module_cal_points_bk = 0;
                    cur_point = 0;
                    clear_cal_listview();
                    err = send_cmd_to_one_module(road, NULL, 0, send_query_cal_points);
                    
                    if(err == CS_ERR_SEND_SUCCESS)
                    {
                        cal_status = GET_ALL_CAL_POINTS_INF;
                    }
                    break;
                }
            }
            break;
        }
    }
}
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void calibration_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_calibration_windows_handle(hWin);
			win = get_user_window_info(hWin);
            create_calibration_win_listview(hWin);
            init_create_win_all_ele(win);
			cal_win_timer_handle = WM_CreateTimer(hWin, 0, 100, 0);
            set_cur_cal_road(1);
            cal_status = AUTO_INIT_SLAVE_CAL_ENV;
            auto_init_flag = SLAVE_INTO_CAL;
			break;
		case WM_PAINT:
			_PaintFrame();
            draw_group_inf_area();
			break;
		case WM_TIMER:
		{
            cal_status_machine();
			WM_RestartTimer(cal_win_timer_handle, 10);
			break;
		}
		case WM_KEY:
			break;
		case WM_DELETE:
            WM_DeleteTimer(cal_win_timer_handle);
            cal_status = EXIT_CAL_WIN;
            cal_status_machine();
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
void create_calibration_win(int hWin)
{
    create_user_window(&calibration_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
