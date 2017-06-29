
/**
  ******************************************************************************
  * @file    env_par_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   环境变量编辑窗口
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "ff.h"
#include "OS.H"
#include "cs99xx_struct.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "cs99xx_mem_api.h"
#include "UI_COM/com_edit_api.h"
#include "UI_COM/com_ui_info.h"
#include "file_win/file_edit_win.h"
#include "key_menu_win/key_menu_win.h"
#include "7_env_par_edit_win.h"
#include "env_par_edit_win.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum{
    ENV_PAR_LANGUAGE,///<语言选择
    ENV_PAR_REMAIN,///<余量提示
    ENV_PAR_RES_SAVE,///<结果保存
    ENV_PAR_OVER_COVER,///<溢出覆盖
    ENV_PAR_LIST_DISPLAY,///<列表显示
    ENV_PAR_SELF_CHECK,///<自检允许
    ENV_PAR_OUT_DELAY,///<输出延时
    ENV_PAR_TEST_PORT,///<测试端口
    ENV_PAR_BUZZER_SW,///<蜂鸣开关
    ENV_PAR_NO_RULE,///<编号规则
}ENV_PAR_UI_INDEX;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void env_par_edit_win_cb(WM_MESSAGE * pMsg);
static void env_par_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void env_par_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void env_par_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void env_par_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void env_win_menu_key_ok(WM_HMEM hWin);
static void env_win_edit_sw_menu_key_init(WM_HMEM hWin);

static void edit_language_f1_cb(KEY_MESSAGE *key_msg);
static void edit_language_f2_cb(KEY_MESSAGE *key_msg);
static void edit_language_f3_cb(KEY_MESSAGE *key_msg);
static void edit_language_f4_cb(KEY_MESSAGE *key_msg);
static void edit_language_f5_cb(KEY_MESSAGE *key_msg);
static void edit_language_f6_cb(KEY_MESSAGE *key_msg);

static void env_win_edit_num_f1_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_num_f2_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_num_f3_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_num_f4_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_num_f5_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_num_f6_cb(KEY_MESSAGE *key_msg);

static void env_win_edit_sw_f1_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_sw_f2_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_sw_f3_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_sw_f4_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_sw_f5_cb(KEY_MESSAGE *key_msg);
static void env_win_edit_sw_f6_cb(KEY_MESSAGE *key_msg);

static void init_create_env_par_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_env_par_win_edit_ele(MYUSER_WINDOW_T* win);
static void reg_env_language_sys_key(WM_HMEM hWin);
static void env_win_edit_num_menu_key_init(WM_HMEM hWin);
static void env_win_edit_language_menu_key_init(WM_HMEM hWin);
static void env_win_re_init_create_win_all_ele(void);
/* Private variables ---------------------------------------------------------*/

static uint8_t language_table[]=
{
    CHINESE,
    ENGLISH,
};
/**
  * @brief  环境变量设置窗口的位置尺寸信息
  */
static WIDGET_POS_SIZE_T* env_par_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_env_par_windows,/*4.3寸屏*/
    &_7_env_par_windows,/*5.6寸屏*/
    &_7_env_par_windows,/*7寸屏*/
};

/**
  * @brief  编辑系统语言时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF env_win_language_custom_menu_inf[]=
{
    { CHINESE_STR , CHINESE	, edit_language_f1_cb},
    { ENGLISH_STR , ENGLISH	, edit_language_f2_cb},
};
/**
  * @brief  编辑语言使用的菜单键初始化信息
  */
static MENU_KEY_INFO_T 	env_par_language_menu_key_inf[] =
{
    {"", F_KEY_CUSTOM, KEY_F1 & _KEY_UP, edit_language_f1_cb },//f1
    {"", F_KEY_CUSTOM, KEY_F2 & _KEY_UP, edit_language_f2_cb },//f2
    {"", F_KEY_NULL  , KEY_F3 & _KEY_UP, edit_language_f3_cb },//f3
    {"", F_KEY_NULL  , KEY_F4 & _KEY_UP, edit_language_f4_cb },//f4
    {"", F_KEY_NULL  , KEY_F5 & _KEY_UP, edit_language_f5_cb },//f5
    {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, edit_language_f6_cb },//f6
};

/**
  * @brief  编辑数字编辑对象时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	env_win_edit_num_menu_key_init_info[] =
{
    {"", F_KEY_DEL		, KEY_F1 & _KEY_UP, env_win_edit_num_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, env_win_edit_num_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, env_win_edit_num_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, env_win_edit_num_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, env_win_edit_num_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, env_win_edit_num_f6_cb },//f6
};
/**
  * @brief  编辑开关变量时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	env_win_edit_sw_menu_key_init_info[] =
{
    {"", F_KEY_ON   , KEY_F1 & _KEY_UP, env_win_edit_sw_f1_cb },//f1
    {"", F_KEY_OFF  , KEY_F2 & _KEY_UP, env_win_edit_sw_f2_cb },//f2
    {"", F_KEY_NULL , KEY_F3 & _KEY_UP, env_win_edit_sw_f3_cb },//f3
    {"", F_KEY_NULL , KEY_F4 & _KEY_UP, env_win_edit_sw_f4_cb },//f4
    {"", F_KEY_NULL , KEY_F5 & _KEY_UP, env_win_edit_sw_f5_cb },//f5
    {"", F_KEY_BACK , KEY_F6 & _KEY_UP, env_win_edit_sw_f6_cb },//f6
};
/**
  * @brief  环境变量窗口系统按键初始化数组
  */
static FUNCTION_KEY_INFO_T env_par_sys_key_pool[]={
	{KEY_UP		, env_par_direct_key_up_cb		 },
	{KEY_DOWN	, env_par_direct_key_down_cb	 },
	{KEY_LEFT	, env_par_direct_key_left_cb	 },
	{KEY_RIGHT	, env_par_direct_key_right_cb	 },
    
	{CODE_LEFT	, env_par_direct_key_down_cb     },
	{CODE_RIGH	, env_par_direct_key_up_cb	     },
};

/**
  * @brief  环境变量窗口显示的编辑对象池
  */
static EDIT_ELE_T env_par_ele_pool[]=
{
    {
        {"语言选择:","Language:"}, /* 名称 */
        ENV_PAR_LANGUAGE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {language_pool, ARRAY_SIZE(language_pool)},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {reg_env_language_sys_key,env_win_edit_language_menu_key_init,keyboard_fun_num,},/*key_inf*/
    },
    {
        {"余量提示:","RemainHint:"}, /* 名称 */
        ENV_PAR_REMAIN,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*dec*/,1/*lon*/,U_PER_CENT/*unit*/,},/*format*/
        {9/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"结果保存:","ResultSave:"}, /* 名称 */
        ENV_PAR_RES_SAVE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"溢出覆盖:","OverCover:"}, /* 名称 */
        ENV_PAR_OVER_COVER,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"列表显示:","ListDisplay:"}, /* 名称 */
        ENV_PAR_LIST_DISPLAY,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"自检允许:","SelfCheck:"}, /* 名称 */
        ENV_PAR_SELF_CHECK,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出延时:","OutDelay:"}, /* 名称 */
        ENV_PAR_OUT_DELAY,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {1/*dec*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试端口:","TestPort:"}, /* 名称 */
        ENV_PAR_TEST_PORT,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"蜂鸣开关:","Buzzer SW.:"}, /* 名称 */
        ENV_PAR_BUZZER_SW,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"编号规则:","NO.Rule:"}, /* 名称 */
        ENV_PAR_NO_RULE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*dec*/,1/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, env_win_edit_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
};

/**
  * @brief  环境变量窗口显示的编辑对象索引表
  */
static CS_INDEX env_par_ui_ele_table[]=
{
    ENV_PAR_LANGUAGE,///<语言选择
    ENV_PAR_REMAIN,///<余量提示
    ENV_PAR_RES_SAVE,///<结果保存
    ENV_PAR_OVER_COVER,///<溢出覆盖
    ENV_PAR_LIST_DISPLAY,///<列表显示
    ENV_PAR_SELF_CHECK,///<自检允许
    ENV_PAR_OUT_DELAY,///<输出延时
    ENV_PAR_TEST_PORT,///<测试端口
    ENV_PAR_BUZZER_SW,///<蜂鸣开关
    ENV_PAR_NO_RULE,///<编号规则
};

/**
  * @brief  步骤编辑窗口的位置尺寸信息数组，根据不同的屏幕尺寸进行初始化
  */
static EDIT_ELE_AUTO_LAYOUT_T *env_par_win_edit_ele_auto_layout_pool[SCREEN_NUM]=
{
    &_7_env_par_edit_ele_auto_layout_inf,/*4.3寸屏*/
    &_7_env_par_edit_ele_auto_layout_inf,/*5.6寸屏*/
    &_7_env_par_edit_ele_auto_layout_inf,/*7寸屏*/
};
/**
  * @brief  环境变量窗口结构定义
  */
static MYUSER_WINDOW_T env_par_window=
{
    {"环境参数","Env.Par"},
    env_par_edit_win_cb,NULL,
    {0},/*text*/
    {
        env_par_ele_pool,ARRAY_SIZE(env_par_ele_pool),
        (CS_INDEX*)env_par_ui_ele_table,ARRAY_SIZE(env_par_ui_ele_table),
        init_create_env_par_win_edit_ele,
    },/*edit*/
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_env_par_win_com_ele,
    },/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        env_par_win_edit_ele_auto_layout_pool,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    env_par_win_pos_size_pool/*pos_size_pool*/
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  编辑步骤编号使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void env_win_edit_num_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = env_win_edit_num_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(env_win_edit_num_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  编辑电流档位时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void env_win_edit_language_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = env_par_language_menu_key_inf;
    uint32_t size = ARRAY_SIZE(env_par_language_menu_key_inf);
    CUSTOM_MENU_KEY_INF *cus_inf = env_win_language_custom_menu_inf;
    uint16_t cus_size = ARRAY_SIZE(env_win_language_custom_menu_inf);
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t pool_size;
    
    pool = g_cur_win->edit.pool;
    pool_size = g_cur_win->edit.pool_size;
    
    ele = get_edit_ele_inf(pool, pool_size, ENV_PAR_LANGUAGE, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_menu_key_custom_inf(cus_inf, cus_size, ele, info, size);
        init_menu_key_info(info, size, hWin);
    }
}
/**
  * @brief  设置系统语言为中文
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_env_language_chinese(WM_HMEM hWin)
{
    uint8_t size = g_cur_edit_ele->data.bytes;
    uint32_t value = CHINESE;
    WM_HMEM handle = g_cur_edit_ele->dis.edit.handle;
    
    DROPDOWN_SetSel(handle, value);
    DROPDOWN_SetUserData(handle, &value, size);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    update_all_windows_text();
    init_dialog(g_cur_win);//为了更新对话框名字
    env_win_re_init_create_win_all_ele();
}

/**
  * @brief  设置系统语言为英文
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_env_language_english(WM_HMEM hWin)
{
    uint8_t size = g_cur_edit_ele->data.bytes;
    uint32_t value = ENGLISH;
    WM_HMEM handle = g_cur_edit_ele->dis.edit.handle;
    
    DROPDOWN_SetSel(handle, value);
    DROPDOWN_SetUserData(handle, &value, size);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    update_all_windows_text();
    init_dialog(g_cur_win);//为了更新对话框名字
    env_win_re_init_create_win_all_ele();
}

/**
  * @brief  重新初始化并创建窗口中的对象
  * @param  [in] step 要加载的步骤编号
  * @retval 无
  */
static void env_win_re_init_create_win_all_ele(void)
{
    delete_win_all_ele(g_cur_win);
    init_create_win_all_ele(g_cur_win);
    select_edit_ele(g_cur_edit_ele);//重新选重当前编辑控件
    update_default_range_name();//更新默认的范围显示
}
/**
  * @brief  编辑语言时功能键F1的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f1_cb(KEY_MESSAGE *key_msg)
{
    set_env_language_chinese(key_msg->user_data);
}
/**
  * @brief  编辑语言时功能键F2的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f2_cb(KEY_MESSAGE *key_msg)
{
    set_env_language_english(key_msg->user_data);
}
/**
  * @brief  编辑语言时功能键F3的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑语言时功能键F4的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑语言时功能键F5的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑语言时功能键F6的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f6_cb(KEY_MESSAGE *key_msg)
{
    env_win_menu_key_ok(key_msg->user_data);
}

static void env_win_edit_num_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
static void env_win_edit_num_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
static void env_win_edit_num_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void env_win_edit_num_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void env_win_edit_num_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void env_win_edit_num_f6_cb(KEY_MESSAGE *key_msg)
{
    env_win_menu_key_ok(key_msg->user_data);
}

static void env_win_edit_sw_f1_cb(KEY_MESSAGE *key_msg)
{
    set_sw_status_on(key_msg->user_data);
}
static void env_win_edit_sw_f2_cb(KEY_MESSAGE *key_msg)
{
    set_sw_status_off(key_msg->user_data);
}
static void env_win_edit_sw_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void env_win_edit_sw_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void env_win_edit_sw_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void env_win_edit_sw_f6_cb(KEY_MESSAGE *key_msg)
{
    env_win_menu_key_ok(key_msg->user_data);
}

/**
  * @brief  编辑开关变量使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void env_win_edit_sw_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = env_win_edit_sw_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(env_win_edit_sw_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  向上键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    com_edit_win_direct_key_up_cb(key_msg);//调用通用的向下键回调
}
/**
  * @brief  向下键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    com_edit_win_direct_key_down_cb(key_msg);//调用通用的向下键回调
}
/**
  * @brief  向左键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
    com_win_direct_key_left_cb(key_msg);
//	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
/**
  * @brief  向右键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
    com_win_direct_key_right_cb(key_msg);
//	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}

/**
  * @brief  注册编辑语言时功能键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void reg_env_language_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(env_par_sys_key_pool, ARRAY_SIZE(env_par_sys_key_pool), hWin);
}
/**
  * @brief  注册编辑语言时菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
//static void env_language_menu_key(WM_HMEM hWin)
//{
//    MENU_KEY_INFO_T * info = env_par_language_menu_key_inf;
//    uint32_t size = ARRAY_SIZE(env_par_language_menu_key_inf);
//    int32_t data = g_cur_edit_ele->dis.edit.handle;
//    
//	init_menu_key_info(info, size, data);
//}
/**
  * @brief  按下确认键后调用这个函数进行保存数据并返回上级窗口
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void env_win_menu_key_ok(WM_HMEM hWin)
{
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    save_sys_par();//保存系统参数
    back_win(hWin);//关闭对话框
}

/**
  * @brief  设置环境变量参数的编辑对象数据
  * @param  [in] par 系统参数
  * @retval 无
  */
static void set_env_par_window_ele_data(SYS_PAR *par)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    //系统语言
    reg_edit_ele_data(g_cur_win, ENV_PAR_LANGUAGE, &par->language,  sizeof(par->language));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_LANGUAGE, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->resource.user_data = language_table;
        ele->resource.user_data_size = ARRAY_SIZE(language_table);
    }
    
    /* 余量提示 */
    reg_edit_ele_data_inf(ENV_PAR_REMAIN, &par->allowance,  sizeof(par->allowance));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_REMAIN, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 结果保存 */
    reg_edit_ele_data_inf(ENV_PAR_RES_SAVE, &par->is_save_res,  sizeof(par->is_save_res));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_RES_SAVE, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 溢出覆盖 */
    reg_edit_ele_data_inf(ENV_PAR_OVER_COVER, &par->is_overflow_cover,  sizeof(par->is_overflow_cover));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_OVER_COVER, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 列表显示 */
    reg_edit_ele_data_inf(ENV_PAR_LIST_DISPLAY, &par->is_table_dis,  sizeof(par->is_table_dis));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_LIST_DISPLAY, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 自检允许 */
    reg_edit_ele_data_inf(ENV_PAR_SELF_CHECK, &par->is_self_check,  sizeof(par->is_self_check));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_SELF_CHECK, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 输出延时 */
    reg_edit_ele_data_inf(ENV_PAR_OUT_DELAY, &par->output_delay,  sizeof(par->output_delay));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_OUT_DELAY, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 测试端口 */
    reg_edit_ele_data_inf(ENV_PAR_TEST_PORT, &par->test_method,  sizeof(par->test_method));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_TEST_PORT, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 蜂鸣开关 */
    reg_edit_ele_data_inf(ENV_PAR_BUZZER_SW, &par->buzzer_sw,  sizeof(par->buzzer_sw));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_BUZZER_SW, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 编号规则 */
    reg_edit_ele_data_inf(ENV_PAR_NO_RULE, &par->num_rule,  sizeof(par->num_rule));
    ele = get_edit_ele_inf(pool, size, ENV_PAR_NO_RULE, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
}

/**
  * @brief  初始化并创建编辑对象
  * @param  [in] win 窗口结构数据
  * @retval 无
  */
static void init_create_env_par_win_edit_ele(MYUSER_WINDOW_T* win)
{
    set_env_par_window_ele_data(&sys_par);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    auto_layout_win_edit_ele(win);//自动布局窗口中的编辑对象
    init_window_edit_ele(win);//初始化创建编辑对象
}

/**
  * @brief  初始化并创建公共文本对象
  * @param  [in] win 窗口结构数据
  * @retval 无
  */
static void init_create_env_par_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

/**
  * @brief  回调函数
  * @param  [in] pMsg 窗口消息指针
  * @retval 无
  */
static void env_par_edit_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
		case CM_EDIT_WIDGET_CHANGE:
		{
            win = get_user_window_info(hWin);
            update_win_menu_key(win);
			break;
		}
        case WM_PAINT:
            break;
        case WM_INIT_DIALOG:
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(win);
            init_create_win_all_ele(win);
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            update_default_range_name();//更新默认的范围显示
            break;
		case WM_DELETE:
		{
			send_no_par_msg_to_parent(hWin, CM_CHILD_W_MSG);
			break;
		}
        case WM_KEY:
        {
            break;
        }
        default:
            WM_DefaultProc(pMsg);
    }
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  创建对话框用来编辑环境变量
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_env_par_dialog(int hWin)
{
    create_user_dialog(&env_par_window, &windows_list, hWin);//创建主界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
