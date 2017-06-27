/**
  ******************************************************************************
  * @file    step_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.13
  * @brief   步骤编辑窗口
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ff.h"
#include "OS.H"
#include "cs99xx_struct.h"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"
#include "key_menu_win/key_menu_win.h"
#include "ui_com/com_ui_info.h"
#include "cs99xx_mem_api.h"
#include "ui_com/com_edit_api.h"
#include "step_par_win/7_step_edit_win.h"
#include "type/cs99xx_type.h"

#include "keyboard.h"
#include "app.h"

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  步骤编辑窗口中编辑对象的枚举索引
  */
typedef enum{
    STEP_EDIT_WIN_STEP,///<测试步骤
    STEP_EDIT_WIN_MODE,///<测试模式
    STEP_EDIT_WIN_VOL,///<输出电压
    STEP_EDIT_WIN_CUR,///<输出电流
    STEP_EDIT_WIN_RANGE,///<电流档位
    STEP_EDIT_WIN_UPPER,///<电流上限
    STEP_EDIT_WIN_LOWER,///<电流下限
    STEP_EDIT_WIN_AUTO_IR,///<绝缘电阻自动换档
    STEP_EDIT_WIN_UPPER_IR,///<绝缘电阻上限
    STEP_EDIT_WIN_LOWER_IR,///<绝缘电阻下限
    STEP_EDIT_WIN_UPPER_GR,///<接地电阻上限
    STEP_EDIT_WIN_LOWER_GR,///<接地电阻下限
    
    STEP_EDIT_WIN_ARC,///<电弧侦测
    STEP_EDIT_WIN_REAL_C,///<真实电流
    STEP_EDIT_WIN_FREQ,///<输出频率
    STEP_EDIT_WIN_DELAY_T,///<延时时间
    STEP_EDIT_WIN_RAISE_T,///<上升时间
    STEP_EDIT_WIN_TEST_T,///<测试时间
    STEP_EDIT_WIN_FALL_T,///<下降时间
    STEP_EDIT_WIN_INTER_T,///<间隔时间
    STEP_EDIT_WIN_CONT,///<步间连续
    STEP_EDIT_WIN_PASS,///<步间PASS
    STEP_EDIT_WIN_PORT,///<输出端口
    
    
    STEP_EDIT_UI_END,
}STEP_EDIT_UI_INDEX;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void step_edit_windows_cb(WM_MESSAGE* pMsg);
static void update_key_inf(WM_HMEM hWin);
static void step_edit_win_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void step_edit_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void step_edit_win_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void step_edit_win_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void step_edit_win_sys_key_enter_cb(KEY_MESSAGE *key_msg);

static void edit_test_port_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void edit_test_port_direct_key_right_cb(KEY_MESSAGE *key_msg);


static void edit_step_num_f1_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f2_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f3_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f4_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f5_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f6_cb(KEY_MESSAGE *key_msg);

static void edit_mode_f1_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f2_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f3_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f4_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f5_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f6_cb(KEY_MESSAGE *key_msg);

static void edit_range_f1_cb(KEY_MESSAGE *key_msg);
static void edit_range_f2_cb(KEY_MESSAGE *key_msg);
static void edit_range_f3_cb(KEY_MESSAGE *key_msg);
static void edit_range_f4_cb(KEY_MESSAGE *key_msg);
static void edit_range_f5_cb(KEY_MESSAGE *key_msg);
static void edit_range_f6_cb(KEY_MESSAGE *key_msg);

static void edit_sw_f1_cb(KEY_MESSAGE *key_msg);
static void edit_sw_f2_cb(KEY_MESSAGE *key_msg);
static void edit_sw_f3_cb(KEY_MESSAGE *key_msg);
static void edit_sw_f4_cb(KEY_MESSAGE *key_msg);
static void edit_sw_f5_cb(KEY_MESSAGE *key_msg);
static void edit_sw_f6_cb(KEY_MESSAGE *key_msg);

static void edit_step_num_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_direct_key_enter_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_direct_key_enter_down_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_direct_key_enter_up_cb(KEY_MESSAGE *key_msg);

static void edit_mode_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void edit_mode_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void edit_mode_direct_key_enter_cb(KEY_MESSAGE *key_msg);

static void select_test_mode_key_cb(KEY_MESSAGE *key_msg);
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_step_edit_win_edit_ele(MYUSER_WINDOW_T* win);
static void edit_step_num_sys_key(WM_HMEM hWin);
static void step_edit_win_sys_key_init(WM_HMEM hWin);
static void edit_test_port_sys_key_init(WM_HMEM hWin);
static void edit_step_num_menu_key_init(WM_HMEM hWin);
static void edit_mode_menu_key_init(WM_HMEM hWin);
static void edit_mode_win_sys_key_init(WM_HMEM hWin);
static void set_step_par_window_ele_data(UN_STRUCT *step);
static void update_and_init_mode(void);
static void edit_range_menu_key_init(WM_HMEM hWin);
static void edit_sw_menu_key_init(WM_HMEM hWin);
static void provided_dis_test_time_range_fun(EDIT_ELE_T* ele);
static void check_test_time_value_validity(EDIT_ELE_T* ele, uint32_t *value);

static void check_acw_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_acw_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_dcw_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_dcw_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_ir_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_ir_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_acw_dcw_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_acw_dcw_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_gr_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_gr_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value);

static void check_range_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_test_port_value_validity(EDIT_ELE_T* ele, uint32_t *value);
static void check_gr_output_cur_value_validity(EDIT_ELE_T* ele, uint32_t *value);

static uint8_t get_cur_step_mode(void);
static TEST_PORT *get_cur_step_test_port(void);
static void update_arc_mode_deit_inf(EDIT_ELE_T* ele);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  为设置而定义的中间的步骤变量
  */
static NODE_STEP tmp_step_par;
/**
  * @brief  设置端口的缓冲区
  */
static uint8_t set_port_buf[20];

/**
  * @brief  步骤编辑窗口的位置尺寸信息数组，根据不同的屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* step_edit_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_step_edit_windows,/*4.3寸屏*/
    &_7_step_edit_windows,/*5.6寸屏*/
    &_7_step_edit_windows,/*7寸屏*/
};

/**
  * @brief  步骤编辑窗口的位置尺寸信息数组，根据不同的屏幕尺寸进行初始化
  */
static EDIT_ELE_AUTO_LAYOUT_T *step_edit_win_edit_ele_auto_layout_pool[SCREEN_NUM]=
{
    &_7_step_par_edit_ele_auto_layout_inf,/*4.3寸屏*/
    &_7_step_par_edit_ele_auto_layout_inf,/*5.6寸屏*/
    &_7_step_par_edit_ele_auto_layout_inf,/*7寸屏*/
};

/**
  * @brief  步骤编辑窗口中要显示的编辑控件的索引表
  */
static CS_INDEX step_par_index[]=
{
    STEP_EDIT_WIN_STEP,
    STEP_EDIT_WIN_MODE,
    STEP_EDIT_WIN_VOL,
    STEP_EDIT_WIN_RANGE,
    STEP_EDIT_WIN_UPPER,///<电流上限
    STEP_EDIT_WIN_LOWER,///<电流下限
    
    STEP_EDIT_WIN_ARC,///<电弧侦测
    STEP_EDIT_WIN_REAL_C,///<真实电流
    STEP_EDIT_WIN_FREQ,///<输出频率
    STEP_EDIT_WIN_RAISE_T,///<上升时间
    STEP_EDIT_WIN_TEST_T,///<测试时间
    STEP_EDIT_WIN_FALL_T,///<下降时间
    STEP_EDIT_WIN_INTER_T,///<间隔时间
    STEP_EDIT_WIN_CONT,///<步间连续
    STEP_EDIT_WIN_PASS,///<步间PASS
    STEP_EDIT_WIN_PORT,///<输出端口
};
/**
  * @brief  ACW 步骤编辑窗口中要显示的编辑控件的索引表
  */
static CS_INDEX acw_par_index[]=
{
    STEP_EDIT_WIN_STEP,
    STEP_EDIT_WIN_MODE,
    STEP_EDIT_WIN_VOL,
    STEP_EDIT_WIN_RANGE,
    STEP_EDIT_WIN_UPPER,///<电流上限
    STEP_EDIT_WIN_LOWER,///<电流下限
    
    STEP_EDIT_WIN_ARC,///<电弧侦测
    STEP_EDIT_WIN_REAL_C,///<真实电流
    STEP_EDIT_WIN_FREQ,///<输出频率
    STEP_EDIT_WIN_RAISE_T,///<上升时间
    STEP_EDIT_WIN_TEST_T,///<测试时间
    STEP_EDIT_WIN_FALL_T,///<下降时间
    STEP_EDIT_WIN_INTER_T,///<间隔时间
    STEP_EDIT_WIN_CONT,///<步间连续
    STEP_EDIT_WIN_PASS,///<步间PASS
    STEP_EDIT_WIN_PORT,///<输出端口
    STEP_EDIT_WIN_DELAY_T,///<延时时间
};
/**
  * @brief  DCW 步骤编辑窗口中要显示的编辑控件的索引表
  */
static CS_INDEX dcw_par_index[]=
{
    STEP_EDIT_WIN_STEP,
    STEP_EDIT_WIN_MODE,
    STEP_EDIT_WIN_VOL,
    STEP_EDIT_WIN_RANGE,
    STEP_EDIT_WIN_UPPER,///<电流上限
    STEP_EDIT_WIN_LOWER,///<电流下限
    
    STEP_EDIT_WIN_ARC,///<电弧侦测
    STEP_EDIT_WIN_RAISE_T,///<上升时间
    STEP_EDIT_WIN_TEST_T,///<测试时间
    STEP_EDIT_WIN_FALL_T,///<下降时间
    STEP_EDIT_WIN_INTER_T,///<间隔时间
    STEP_EDIT_WIN_DELAY_T,///<延时时间
    STEP_EDIT_WIN_CONT,///<步间连续
    STEP_EDIT_WIN_PASS,///<步间PASS
    STEP_EDIT_WIN_PORT,///<输出端口
};
/**
  * @brief  IR 步骤编辑窗口中要显示的编辑控件的索引表
  */
static CS_INDEX ir_par_index[]=
{
    STEP_EDIT_WIN_STEP,
    STEP_EDIT_WIN_MODE,
    STEP_EDIT_WIN_VOL,
    STEP_EDIT_WIN_UPPER_IR,///<电流上限
    STEP_EDIT_WIN_LOWER_IR,///<电流下限
    
    STEP_EDIT_WIN_AUTO_IR,///<自动换档
    STEP_EDIT_WIN_DELAY_T,///<延时时间
    STEP_EDIT_WIN_RAISE_T,///<上升时间
    STEP_EDIT_WIN_TEST_T,///<测试时间
    STEP_EDIT_WIN_INTER_T,///<间隔时间
    STEP_EDIT_WIN_CONT,///<步间连续
    STEP_EDIT_WIN_PASS,///<步间PASS
    STEP_EDIT_WIN_PORT,///<输出端口
};
/**
  * @brief  GR 步骤编辑窗口中要显示的编辑控件的索引表
  */
static CS_INDEX gr_par_index[]=
{
    STEP_EDIT_WIN_STEP,
    STEP_EDIT_WIN_MODE,
    STEP_EDIT_WIN_CUR,
    STEP_EDIT_WIN_UPPER_GR,///<电流上限
    STEP_EDIT_WIN_LOWER_GR,///<电流下限
    
    STEP_EDIT_WIN_TEST_T,///<测试时间
    STEP_EDIT_WIN_INTER_T,///<间隔时间
    STEP_EDIT_WIN_CONT,///<步间连续
    STEP_EDIT_WIN_PASS,///<步间PASS
};
/**
  * @brief  编辑步骤编号时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_step_num_menu_key_init_info[] =
{
    {"", F_KEY_DEL		, KEY_F1 & _KEY_UP, edit_step_num_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, edit_step_num_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, edit_step_num_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, edit_step_num_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, edit_step_num_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_step_num_f6_cb },//f6
};
/**
  * @brief  编辑测试模式时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_mode_menu_key_init_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP, edit_mode_f1_cb },//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP, edit_mode_f2_cb },//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP, edit_mode_f3_cb },//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP, edit_mode_f4_cb },//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP, edit_mode_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_mode_f6_cb },//f6
};
/**
  * @brief  编辑测试模式时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_range_menu_key_init_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP, edit_range_f1_cb },//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP, edit_range_f2_cb },//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP, edit_range_f3_cb },//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP, edit_range_f4_cb },//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP, edit_range_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_range_f6_cb },//f6
};
/**
  * @brief  编辑开关变量时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_sw_menu_key_init_info[] =
{
    {"", F_KEY_ON   , KEY_F1 & _KEY_UP, edit_sw_f1_cb },//f1
    {"", F_KEY_OFF  , KEY_F2 & _KEY_UP, edit_sw_f2_cb },//f2
    {"", F_KEY_NULL , KEY_F3 & _KEY_UP, edit_sw_f3_cb },//f3
    {"", F_KEY_NULL , KEY_F4 & _KEY_UP, edit_sw_f4_cb },//f4
    {"", F_KEY_NULL , KEY_F5 & _KEY_UP, edit_sw_f5_cb },//f5
    {"", F_KEY_BACK , KEY_F6 & _KEY_UP, edit_sw_f6_cb },//f6
};
/**
  * @brief  步骤编辑窗口系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	step_edit_win_sys_key_init_pool[]=
{
	{KEY_UP		, step_edit_win_direct_key_up_cb      },
	{KEY_DOWN	, step_edit_win_direct_key_down_cb 	  },
	{KEY_LEFT	, step_edit_win_direct_key_left_cb    },
	{KEY_RIGHT	, step_edit_win_direct_key_right_cb   },
	{CODE_LEFT	, step_edit_win_direct_key_down_cb    },
	{CODE_RIGH	, step_edit_win_direct_key_up_cb      },
	{KEY_ENTER	, step_edit_win_sys_key_enter_cb },
};
/**
  * @brief  步骤编辑窗口系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	edit_test_port_sys_key_init_pool[]=
{
	{KEY_UP		, step_edit_win_direct_key_up_cb      },
	{KEY_DOWN	, step_edit_win_direct_key_down_cb    },
    
	{KEY_LEFT	, edit_test_port_direct_key_left_cb    },
	{KEY_RIGHT	, edit_test_port_direct_key_right_cb   },
    
	{CODE_LEFT	, step_edit_win_direct_key_down_cb    },
	{CODE_RIGH	, step_edit_win_direct_key_up_cb      },
	{KEY_ENTER	, step_edit_win_sys_key_enter_cb },
};
/**
  * @brief  编辑步骤编号时使用的系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	edit_step_num_sys_key_init_pool[]=
{
	{KEY_UP		, edit_step_num_direct_key_up_cb    },
	{KEY_DOWN	, edit_step_num_direct_key_down_cb  },
	{KEY_LEFT	, step_edit_win_direct_key_left_cb    },
	{KEY_RIGHT	, step_edit_win_direct_key_right_cb   },
	{CODE_LEFT	, edit_step_num_direct_key_down_cb  },
	{CODE_RIGH	, edit_step_num_direct_key_up_cb    },
	{KEY_ENTER	, edit_step_num_direct_key_enter_down_cb },
};
/**
  * @brief  编辑步骤编号时使用的系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	edit_mode_sys_key_init_pool[]=
{
	{KEY_UP		, edit_mode_direct_key_up_cb    },
	{KEY_DOWN	, edit_mode_direct_key_down_cb  },
	{KEY_LEFT	, step_edit_win_direct_key_left_cb    },
	{KEY_RIGHT	, step_edit_win_direct_key_right_cb   },
	{CODE_LEFT	, edit_mode_direct_key_down_cb  },
	{CODE_RIGH	, edit_mode_direct_key_up_cb    },
	{KEY_ENTER	, edit_mode_direct_key_enter_cb },
};
/**
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF mode_inf_pool[]=
{
    {ACW_STR , ACW, select_test_mode_key_cb},
    {DCW_STR , DCW, select_test_mode_key_cb},
    {IR_STR  , IR , select_test_mode_key_cb},
    {GR_STR  , GR , select_test_mode_key_cb},
    {BBD_STR , BBD, select_test_mode_key_cb},
    {CC_STR  , CC , select_test_mode_key_cb},
};
/**
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF acw_range_inf_pool[]=
{
    { CUR_2uA_STR   , AC_2uA	, select_test_mode_key_cb},
    { CUR_20uA_STR  , AC_20uA	, select_test_mode_key_cb},
    { CUR_200uA_STR , AC_200uA  , select_test_mode_key_cb},
    { CUR_2mA_STR   , AC_2mA	, select_test_mode_key_cb},
    { CUR_10mA_STR  , AC_10mA	, select_test_mode_key_cb},
    { CUR_20mA_STR  , AC_20mA	, select_test_mode_key_cb},
    { CUR_50mA_STR  , AC_50mA	, select_test_mode_key_cb},
    { CUR_100mA_STR , AC_100mA  , select_test_mode_key_cb},
    { CUR_200mA_STR , AC_200mA  , select_test_mode_key_cb},
    { CUR_2A_STR    , AC_2A     , select_test_mode_key_cb},
};

/**
  * @brief  步骤编辑窗口中所有的编辑控件初始化池
  */
static EDIT_ELE_T step_par_ele_pool[]=
{
    {
        {"测试步骤:","TestStep:"}, /* 名称 */
        STEP_EDIT_WIN_STEP,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, sizeof(STEP_NUM)/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*dec*/,2/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {99/*heigh*/,1/*low*/,{"",""}/*notice*/},/*range*/
        {edit_step_num_sys_key, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试模式:","TestMode:"}, /* 名称 */
        STEP_EDIT_WIN_MODE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {MODE_END/*heigh*/,ACW/*low*/,{"",""}/*notice*/},/*range*/
        {edit_mode_win_sys_key_init, edit_mode_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出电压:","Voltage:"}, /* 名称 */
        STEP_EDIT_WIN_VOL,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*dec*/,5/*lon*/,VOL_U_kV/*unit*/,},/*format*/
        {5000/*heigh*/,50/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"电流档位:","Cur.Range:"}, /* 名称 */
        STEP_EDIT_WIN_RANGE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {
            MODE_END/*heigh*/,ACW/*low*/,{"",""}/*notice*/,
            check_range_value_validity
        },/*range*/
        {step_edit_win_sys_key_init, edit_range_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* ACW DCW 上限 */
    {
        {"电流上限:","Up Limt:"}, /* 名称 */
        STEP_EDIT_WIN_UPPER,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*dec*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"",""}/*notice*/,
            check_acw_dcw_upper_value_validity,
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* ACW DCW 下限 */
    {
        {"电流下限:","LowLimt:"}, /* 名称 */
        STEP_EDIT_WIN_LOWER,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*dec*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"",""}/*notice*/,
            check_acw_dcw_lower_value_validity,
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* IR 自动换档 */
    {
        {"自动换档:","Auto:"}, /* 名称 */
        STEP_EDIT_WIN_AUTO_IR,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* IR上限 */
    {
        {"电阻上限:","Up Limt:"}, /* 名称 */
        STEP_EDIT_WIN_UPPER_IR,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {0/*dec*/,5/*lon*/,RES_U_MOHM/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"",""}/*notice*/,
            check_ir_upper_value_validity
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* IR下限 */
    {
        {"电阻下限:","LowLimt:"}, /* 名称 */
        STEP_EDIT_WIN_LOWER_IR,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 4/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {0/*dec*/,5/*lon*/,RES_U_MOHM/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"",""}/*notice*/,
            check_ir_lower_value_validity
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* GR上限 */
    {
        {"电阻上限:","Up Limt:"}, /* 名称 */
        STEP_EDIT_WIN_UPPER_GR,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,RES_U_mOHM/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"",""}/*notice*/,
            check_gr_upper_value_validity
            
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    /* GR下限 */
    {
        {"电阻下限:","LowLimt:"}, /* 名称 */
        STEP_EDIT_WIN_LOWER_GR,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,RES_U_mOHM/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"",""}/*notice*/,
            check_gr_lower_value_validity
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"电弧侦测:","ARC:"}, /* 名称 */
        STEP_EDIT_WIN_ARC,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {2/*dec*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"(电流模式)",""}/*notice*/},/*range*/
//        {step_edit_win_sys_key_init, edit_arc_menu_key_init, keyboard_fun_num,},/*key_inf*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"真实电流:","Real Cur.:"}, /* 名称 */
        STEP_EDIT_WIN_REAL_C,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*dec*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出频率:","Frequence:"}, /* 名称 */
        STEP_EDIT_WIN_FREQ,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,FREQ_U_Hz/*unit*/,},/*format*/
        {4000/*heigh*/,400/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"延时时间:","Delay:"}, /* 名称 */
        STEP_EDIT_WIN_DELAY_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"上升时间:","Raise:"}, /* 名称 */
        STEP_EDIT_WIN_RAISE_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {
            9999/*heigh*/,3/*low*/,{"0,0.3-999.9s","0,0.3-999.9s"}/*notice*/,
            check_test_time_value_validity, provided_dis_test_time_range_fun
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试时间:","TestTime:"}, /* 名称 */
        STEP_EDIT_WIN_TEST_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,TIM_U_s/*unit*/},/*format*/
        {
            9999/*heigh*/,3/*low*/,{"0,0.3-999.9s","0,0.3-999.9s"}/*notice*/,
            check_test_time_value_validity, provided_dis_test_time_range_fun
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"下降时间:","FallTime:"}, /* 名称 */
        STEP_EDIT_WIN_FALL_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {
            9999/*heigh*/,3/*low*/,{"0,0.3-999.9s","0,0.3-999.9s"}/*notice*/,
            check_test_time_value_validity, provided_dis_test_time_range_fun
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"间隔时间:","Inter.Time:"}, /* 名称 */
        STEP_EDIT_WIN_INTER_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*dec*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"步间连续:","Step Con.:"}, /* 名称 */
        STEP_EDIT_WIN_CONT,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_FLOAT_T},/*类型*/
        {3/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"步间PASS:","Step Pass:"}, /* 名称 */
        STEP_EDIT_WIN_PASS,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_FLOAT_T},/*类型*/
        {3/*dec*/,5/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_sw_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出端口:","Port:"}, /* 名称 */
        STEP_EDIT_WIN_PORT,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_STR, E_STRING_T},/*类型*/
        {3/*dec*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {
            2000/*heigh*/,0/*low*/,{"0-X 1-L 2-H","0-X 1-L 2-H"}/*notice*/,
            check_test_port_value_validity
        },/*range*/
        {edit_test_port_sys_key_init, edit_step_num_menu_key_init, keyboard_test_port},/*key_inf*/
    },
    {
        {"输出电流:","Current:"}, /* 名称 */
        STEP_EDIT_WIN_CUR,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {2/*dec*/,5/*lon*/,CUR_U_A/*unit*/,},/*format*/
        {
            4000/*heigh*/,300/*low*/,{"",""}/*notice*/,
            check_gr_output_cur_value_validity
        },/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num},/*key_inf*/
    },
};

/**
  * @brief  步骤编辑窗口结构初始化定义
  */
static MYUSER_WINDOW_T step_edit_windows=
{
    {"步骤编辑","step_edit_windows"},
    step_edit_windows_cb, update_key_inf,
	{0},/*text*/
    {
        step_par_ele_pool,ARRAY_SIZE(step_par_ele_pool),
        (CS_INDEX*)step_par_index,ARRAY_SIZE(step_par_index),
        init_create_step_edit_win_edit_ele,
    },/*edit*/
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_page_group_com_ele_table,ARRAY_SIZE(range_page_group_com_ele_table),
        init_create_step_edit_win_com_ele,
    },/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        step_edit_win_edit_ele_auto_layout_pool,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    step_edit_win_pos_size_pool/*pos_size_pool*/
};
static MYUSER_WINDOW_T *this_win = &step_edit_windows;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  编辑步骤编号使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  编辑步骤编号使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  编辑步骤编号使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑步骤编号使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑步骤编号使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑步骤编号使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}
/**
  * @brief  编辑测试模式使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  编辑电流档位使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  编辑开关变量使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_sw_f1_cb(KEY_MESSAGE *key_msg)
{
    set_sw_status_on(key_msg->user_data);
}
/**
  * @brief  编辑开关变量使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_sw_f2_cb(KEY_MESSAGE *key_msg)
{
    set_sw_status_off(key_msg->user_data);
}
/**
  * @brief  编辑开关变量使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_sw_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑开关变量使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_sw_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑开关变量使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_sw_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑开关变量使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_sw_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  更新电弧侦测模式的编辑信息
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void update_arc_mode_deit_inf(EDIT_ELE_T* ele)
{
    uint8_t *buf[][2]=
    {
        {"(电流模式)", "(等级模式)"},
        {"(Current Mode)", "(Grade Mode)"},
    };
    uint8_t arc_mode = g_cur_file->arc_mode;
    
    /* 电流模式 */
    if(arc_mode == ARC_CUR_MODE)
    {
        ele->range.notice[CHINESE] = buf[CHINESE][ARC_CUR_MODE];
        ele->range.notice[ENGLISH] = buf[ENGLISH][ARC_CUR_MODE];
        ele->format.unit = CUR_U_mA;
        ele->range.high = 2000;
        ele->range.low = 0;
        ele->format.dec = 2;
        ele->format.lon = 5;
    }
    /* 等级模式 */
    else
    {
        ele->range.notice[CHINESE] = buf[CHINESE][ARC_GRADE_MODE];
        ele->range.notice[ENGLISH] = buf[ENGLISH][ARC_GRADE_MODE];
        ele->format.unit = NULL_U_NULL;
        ele->range.high = 9;
        ele->range.low = 0;
        ele->format.dec = 0;
        ele->format.lon = 1;
    }
}
/**
  * @brief  用于检查测试时间设置值是否合法
  * @param  [in] ele 当前编辑对象
  * @param  [in/out] value 要检查的数值
  * @retval 无
  */
static void check_test_time_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    
    if(val < 3)
    {
        *value = 0;
    }
    else if(val > ele->range.high)
    {
        *value = ele->range.high;
    }
}

static TEST_PORT *get_cur_step_test_port(void)
{
    void *p = NULL;
    uint8_t mode = get_cur_step_mode();
    
    switch(mode)
    {
        case ACW:
            p = &g_cur_step->one_step.acw.port;
            break;
        case DCW:
            p = &g_cur_step->one_step.dcw.port;
            break;
        case IR:
            p = &g_cur_step->one_step.ir.port;
            break;
        default:
            break;
    }
    
    return p;
}
/**
  * @brief  用于检查测试端口设置值是否合法
  * @param  [in] ele 当前编辑对象
  * @param  [in/out] value 要检查的数值
  * @retval 无
  */
static void check_test_port_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint8_t* str = (void*)value;
    TEST_PORT * port;
    
    port = get_cur_step_test_port();
    
    if(port !=  NULL)
    {
        transform_str_to_test_port(port, str);
    }
}
/**
  * @brief  用于检查GR输出电流设置值是否合法
  * @param  [in] ele 当前编辑对象
  * @param  [in/out] value 要检查的数值
  * @retval 无
  */
static void check_gr_output_cur_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    uint32_t res_high = 0;
    uint32_t high;
    uint32_t low;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
    }
    else if(val < ele->range.low)
    {
        *value = ele->range.low;
    }
    
    val = *value;
    
    res_high = GR_RES_H(val);
    high = g_cur_step->one_step.gr.upper_limit;
    low = g_cur_step->one_step.gr.lower_limit;
    
    if(high > res_high)
    {
        g_cur_step->one_step.gr.upper_limit = res_high;
        high = res_high;
    }
    
    if(low > high)
    {
        g_cur_step->one_step.gr.lower_limit = high;
    }
}
/**
  * @brief  用于显示测试时间范围信息的专用函数
  * @param  [in] ele 当前编辑对象
  * @retval 无
  */
static void provided_dis_test_time_range_fun(EDIT_ELE_T* ele)
{
    uint8_t*str[2] = {0};
    
    str[CHINESE] = ele->range.notice[CHINESE];
    str[ENGLISH] = ele->range.notice[CHINESE];
    
    set_com_text_ele_inf((CS_INDEX)COM_RANGE_NOTICE, g_cur_win, str);
    update_com_text_ele((CS_INDEX)COM_RANGE_NOTICE, g_cur_win, str[SYS_LANGUAGE]);
}
/**
  * @brief  获取当前步的步号数据
  * @param  无
  * @retval 当前步的步号
  */
static STEP_NUM get_cur_step(void)
{
    return *(STEP_NUM*)g_cur_edit_ele->data.data;
}
/**
  * @brief  重新初始化并创建窗口中的对象
  * @param  [in] step 要加载的步骤编号
  * @retval 无
  */
static void re_init_create_win_all_ele(STEP_NUM step)
{
    NODE_STEP *node;
    delete_win_all_ele(g_cur_win);
    load_steps_to_list(step, 1);
    
    node = get_g_cur_step();
    
    if(NULL != node)
    {
        memcpy(g_cur_step, node, sizeof(NODE_STEP));
        set_step_par_window_ele_data(&g_cur_step->one_step);//初始化编辑对象的参数
        init_create_win_all_ele(g_cur_win);
        select_edit_ele(g_cur_edit_ele);//重新选重当前编辑控件
        update_group_inf(g_cur_win);
        dis_one_page_win_edit_eles(g_cur_win, g_cur_edit_ele->page);//显示一页的编辑对象
    }
}
/**
  * @brief  编辑步骤编号使用的确认键回调函数,确认并将光标移动到下一个编辑控件
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_enter_cb(KEY_MESSAGE *key_msg)
{
    uint32_t new_step = get_edit_ele_value(g_cur_edit_ele,NULL);
    uint32_t old_step = get_cur_step();
    
    if(new_step != old_step && new_step != 0 && new_step <= g_cur_file->total)
    {
        re_init_create_win_all_ele(new_step);
    }
    else
    {
        set_edit_num_value(g_cur_edit_ele, old_step);
    }
}
/**
  * @brief  编辑步骤编号使用的确认键回调函数,确认并将光标移动到下一个编辑控件
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_enter_down_cb(KEY_MESSAGE *key_msg)
{
    edit_step_num_direct_key_enter_cb(key_msg);
    com_edit_win_direct_key_down_cb(key_msg);//调用通用的向下键回调
}

/**
  * @brief  编辑步骤编号使用的确认键回调函数,确认并将光标移动到上一个编辑控件
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_enter_up_cb(KEY_MESSAGE *key_msg)
{
    edit_step_num_direct_key_enter_cb(key_msg);
    com_edit_win_direct_key_up_cb(key_msg);//调用通用的向下键回调
}

/**
  * @brief  编辑步骤编号使用的向上键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    edit_step_num_direct_key_enter_up_cb(key_msg);
}

/**
  * @brief  编辑步骤编号使用的向下键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    edit_step_num_direct_key_enter_down_cb(key_msg);
}

/**
  * @brief  编辑测试模式使用的确认键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_direct_key_enter_cb(KEY_MESSAGE *key_msg)
{
    update_and_init_mode();
    com_edit_win_direct_key_down_cb(key_msg);//调用通用的向下键回调
}

/**
  * @brief  更新并初始化测试模式
  * @param  无
  * @retval 无
  */
static void update_and_init_mode(void)
{
    uint8_t mode = get_edit_ele_value(g_cur_edit_ele, NULL);//从控件中读出测试模式的值
    
    /* 测试模式改变了 */
    if(g_cur_step->one_step.com.mode != mode)
    {
        if(g_cur_file->work_mode == G_MODE)
        {
            clear_cur_group_all_test_step();
            insert_step(0, mode);//插入一步
            save_group_info(g_cur_file->num);//保存新建文件的记忆组信息
        }
        
        g_cur_step->one_step.com.mode = mode;//更新当前步的测试模式
        init_mode(g_cur_step);//初始化参数
        save_setting_step();//保存正在设置的步骤参数
        re_init_create_win_all_ele(g_cur_step->one_step.com.step);//重新初始化并创建所有的对象
    }
}
/**
  * @brief  编辑测试模式使用的向上键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    update_and_init_mode();//更新并初始化模式
    step_edit_win_direct_key_up_cb(key_msg);//调用向下键的回调函数
}

/**
  * @brief  编辑测试模式使用的向下键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    update_and_init_mode();
    step_edit_win_direct_key_down_cb(key_msg);
}
/**
  * @brief  编辑步骤编号使用的系统功能按键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_step_num_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(edit_step_num_sys_key_init_pool, ARRAY_SIZE(edit_step_num_sys_key_init_pool), hWin);
}
/**
  * @brief  编辑测试模式使用的系统功能按键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_mode_win_sys_key_init(WM_HMEM hWin)
{
    register_system_key_fun(edit_mode_sys_key_init_pool, ARRAY_SIZE(edit_mode_sys_key_init_pool), hWin);
}
/**
  * @brief  步骤编辑窗口系统功能键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void step_edit_win_sys_key_init(WM_HMEM hWin)
{
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    register_system_key_fun(step_edit_win_sys_key_init_pool, ARRAY_SIZE(step_edit_win_sys_key_init_pool), data);
}
/**
  * @brief  测试端口编辑使用的系统功能键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_test_port_sys_key_init(WM_HMEM hWin)
{
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
    register_system_key_fun(edit_test_port_sys_key_init_pool, ARRAY_SIZE(edit_test_port_sys_key_init_pool), data);
}
/**
  * @brief  编辑步骤编号使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_step_num_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_step_num_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_step_num_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}

/**
  * @brief  编辑开关变量使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_sw_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_sw_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_sw_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  选择测试模式时使用的按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void select_test_mode_key_cb(KEY_MESSAGE *key_msg)
{
    int data = key_msg->custom_data;
    int hWin = key_msg->user_data;
    uint32_t size = g_cur_edit_ele->data.bytes;
    CS_INDEX index;
    
    index = get_data_in_resource_table_index(g_cur_edit_ele, size, &data);
    
    DROPDOWN_SetSel(hWin, index);
    DROPDOWN_SetUserData(hWin, &data, size);
}
/**
  * @brief  编辑测试模式时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_mode_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_mode_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_mode_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    CUSTOM_MENU_KEY_INF *cus_inf = mode_inf_pool;
    uint16_t cus_size = ARRAY_SIZE(mode_inf_pool);
    
    init_menu_key_custom_inf(cus_inf, cus_size, g_cur_edit_ele, info, size);
	init_menu_key_info(info, size, data);
}
/**
  * @brief  编辑电流档位时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_range_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_range_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_range_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    CUSTOM_MENU_KEY_INF *cus_inf = acw_range_inf_pool;
    uint16_t cus_size = sizeof(acw_range_inf_pool);
    
    init_menu_key_custom_inf(cus_inf, cus_size, g_cur_edit_ele, info, size);
	init_menu_key_info(info, size, data);
}

/**
  * @brief  获取当前编辑步骤的测试模式
  * @param  [in] ele 编辑对象
  * @param  [in] range 档位数据的地址
  * @retval 无
  */
static uint8_t get_cur_step_mode(void)
{
    return g_cur_step->one_step.com.mode;
}

/**
  * @brief  检查ACW档位值的合法性
  * @param  [in] ele 编辑对象
  * @param  [in] range 档位数据的地址
  * @retval 无
  */
static void check_acw_range_value(EDIT_ELE_T* ele, uint32_t *range)
{
    uint8_t tmp = *range;
    ACW_STRUCT *acw = &g_cur_step->one_step.acw;
    EDIT_ELE_T* tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    if(tmp >= AC_GEAR_END)
    {
        *range = get_acw_max_gear();
        tmp = *range;
    }
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_UPPER, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->format.unit = ac_gear[tmp].unit;
        tmp_ele->format.lon = ac_gear[tmp].lon;
        tmp_ele->format.dec = ac_gear[tmp].dec;
        tmp_ele->range.high = ac_gear[tmp].high_max;
        
        if(acw->upper_limit > tmp_ele->range.high)
        {
            acw->upper_limit = tmp_ele->range.high;
        }
        
        set_edit_num_value(tmp_ele, acw->upper_limit);
    }
    
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->format.unit = ac_gear[tmp].unit;
        tmp_ele->format.lon = ac_gear[tmp].lon;
        tmp_ele->format.dec = ac_gear[tmp].dec;
        tmp_ele->range.high = acw->upper_limit;
        
        if(acw->lower_limit > acw->upper_limit)
        {
            acw->lower_limit = acw->upper_limit;
        }
        
        set_edit_num_value(tmp_ele, acw->lower_limit);
    }
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_REAL_C, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->format.unit = ac_gear[tmp].unit;
        tmp_ele->format.lon = ac_gear[tmp].lon;
        tmp_ele->format.dec = ac_gear[tmp].dec;
        tmp_ele->range.high = acw->upper_limit;
        
        if(acw->real_cur > acw->upper_limit)
        {
            acw->real_cur = acw->upper_limit;
        }
        
        set_edit_num_value(tmp_ele, acw->real_cur);
    }
}
/**
  * @brief  检查DCW档位值的合法性
  * @param  [in] ele 编辑对象
  * @param  [in] range 档位数据的地址
  * @retval 无
  */
static void check_dcw_range_value(EDIT_ELE_T* ele, uint32_t *range)
{
    uint8_t tmp = *range;
    DCW_STRUCT *dcw = &g_cur_step->one_step.dcw;
    EDIT_ELE_T* tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    if(tmp >= DC_GEAR_END)
    {
        *range = get_dcw_max_gear();
        tmp = *range;
    }
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_UPPER, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->format.unit = dc_gear[tmp].unit;
        tmp_ele->format.lon = dc_gear[tmp].lon;
        tmp_ele->format.dec = dc_gear[tmp].dec;
        tmp_ele->range.high = dc_gear[tmp].high_max;
        
        if(dcw->upper_limit > tmp_ele->range.high)
        {
            dcw->upper_limit = tmp_ele->range.high;
        }
        
        set_edit_num_value(tmp_ele, dcw->upper_limit);
    }
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->format.unit = dc_gear[tmp].unit;
        tmp_ele->format.lon = dc_gear[tmp].lon;
        tmp_ele->format.dec = dc_gear[tmp].dec;
        tmp_ele->range.high = dcw->upper_limit;
        
        if(dcw->lower_limit > dcw->upper_limit)
        {
            dcw->lower_limit = dcw->upper_limit;
        }
        
        set_edit_num_value(tmp_ele, dcw->lower_limit);
    }
}

/**
  * @brief  检查交直流上限值的合法性
  * @param  [in] ele 编辑对象
  * @param  [in] value 数据的地址
  * @retval 无
  */
static void check_acw_dcw_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint8_t mode = get_cur_step_mode();
    
    switch(mode)
    {
        case ACW:
            check_acw_upper_value_validity(ele, value);
            break;
        case DCW:
            check_dcw_upper_value_validity(ele, value);
            break;
    }
}
/**
  * @brief  检查交直流上限值的合法性
  * @param  [in] ele 编辑对象
  * @param  [in] value 数据的地址
  * @retval 无
  */
static void check_acw_dcw_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint8_t mode = get_cur_step_mode();
    
    switch(mode)
    {
        case ACW:
            check_acw_lower_value_validity(ele, value);
            break;
        case DCW:
            check_dcw_lower_value_validity(ele, value);
            break;
    }
}
/**
  * @brief  检查档位值的合法性
  * @param  [in] ele 编辑对象
  * @param  [in] value 数据的地址
  * @retval 无
  */
static void check_range_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint8_t mode;
    
    mode = get_cur_step_mode();
    
    switch(mode)
    {
        case ACW:
            check_acw_range_value(ele, value);
            break;
        case DCW:
            check_dcw_range_value(ele, value);
            break;
    }
}
/**
  * @brief  更新 ACW 电流档位影响的信息
  * @param  [in] ele 编辑对象
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void update_acw_range_affect_inf(EDIT_ELE_T* ele, UN_STRUCT *step)
{
    uint32_t range = step->acw.range;
    uint8_t mode = ACW;
    
    ele->resource.table = get_defined_range_table(mode);
    ele->resource.size = get_defined_range_num(mode);
    ele->resource.user_data = get_defined_range_flag(mode);
    ele->resource.user_data_size = get_defined_range_num(mode);
    
    check_range_value_validity(ele, &range);
}
/**
  * @brief  更新 DCW 电流档位影响的信息
  * @param  [in] ele 编辑对象
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void update_dcw_range_affect_inf(EDIT_ELE_T* ele, UN_STRUCT *step)
{
    uint32_t range = step->dcw.range;
    uint8_t mode = DCW;
    
    ele->resource.table = get_defined_range_table(mode);
    ele->resource.size = get_defined_range_num(mode);
    ele->resource.user_data = get_defined_range_flag(mode);
    ele->resource.user_data_size = get_defined_range_num(mode);
    
    check_range_value_validity(ele, &range);
}

/**
  * @brief  检查 ACW 电流上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_acw_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    EDIT_ELE_T* tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
        val = *value;
    }
    
    /* 电流下限 */
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->range.high = val;
        
        if(g_cur_step->one_step.acw.lower_limit > val)
        {
            g_cur_step->one_step.acw.lower_limit = val;
        }
        
        set_edit_num_value(tmp_ele, g_cur_step->one_step.acw.lower_limit);
    }
    
    /* 真实电流 */
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_REAL_C, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->range.high = val;
        
        if(g_cur_step->one_step.acw.real_cur > val)
        {
            g_cur_step->one_step.acw.real_cur = val;
        }
        
        set_edit_num_value(tmp_ele, g_cur_step->one_step.acw.real_cur);
    }
}
/**
  * @brief  检查 ACW 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_acw_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
    }
}
/**
  * @brief  设置ACW参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_acw_par_win_ele_data(UN_STRUCT *step)
{
    EDIT_ELE_T* ele;
    ACW_STRUCT *acw = &step->acw;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    set_g_cur_win_edit_index_inf(acw_par_index, ARRAY_SIZE(acw_par_index));//设置编辑对象索引表信息
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_VOL, &acw->output_vol, sizeof(acw->output_vol));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_VOL, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->range.high = ACW_VOL_H;
        ele->range.low = ACW_VOL_L;
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_RANGE, &acw->range, sizeof(acw->range));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_RANGE, &err);
    
    if(err == CS_ERR_NONE)
    {
        update_acw_range_affect_inf(ele, step);
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_UPPER, &acw->upper_limit, sizeof(acw->upper_limit));//电流上限
    reg_edit_ele_data_inf(STEP_EDIT_WIN_LOWER, &acw->lower_limit, sizeof(acw->lower_limit));//电流下限
    reg_edit_ele_data_inf(STEP_EDIT_WIN_ARC, &acw->arc_sur, sizeof(acw->arc_sur));//<电弧侦测
    reg_edit_ele_data_inf(STEP_EDIT_WIN_REAL_C, &acw->real_cur, sizeof(acw->real_cur));//<真实电流
    reg_edit_ele_data_inf(STEP_EDIT_WIN_FREQ, &acw->output_freq,   sizeof(acw->output_freq));//<输出频率
    reg_edit_ele_data_inf(STEP_EDIT_WIN_RAISE_T, &acw->rise_time,  sizeof(acw->rise_time));//上升时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_TEST_T, &acw->test_time,  sizeof(acw->test_time));//测试时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_FALL_T, &acw->fall_time,  sizeof(acw->fall_time));//下降时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_INTER_T, &acw->inter_time,  sizeof(acw->inter_time));//间隔时间
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_CONT, &acw->step_con,  sizeof(acw->step_con));//步间连续
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_CONT, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PASS, &acw->step_pass,  sizeof(acw->step_pass));//步间PASS
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PASS, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 输出端口 */
    transform_test_port_to_str(&acw->port, set_port_buf);
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PORT, set_port_buf, sizeof(set_port_buf));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PORT, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->format.lon = type_spe.port_num;
    }
    
}
/**
  * @brief  检查 DCW 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_dcw_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    EDIT_ELE_T* tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
        val = *value;
    }
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->range.high = val;
        
        if(g_cur_step->one_step.dcw.lower_limit > val)
        {
            g_cur_step->one_step.dcw.lower_limit = val;
        }
        
        set_edit_num_value(tmp_ele, g_cur_step->one_step.dcw.lower_limit);
    }
    
}
/**
  * @brief  检查 DCW 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_dcw_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
    }
}
/**
  * @brief  设置DCW参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_dcw_par_win_ele_data(UN_STRUCT *step)
{
    EDIT_ELE_T* ele;
    DCW_STRUCT *dcw = &step->dcw;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    set_g_cur_win_edit_index_inf(dcw_par_index, ARRAY_SIZE(dcw_par_index));//设置编辑对象索引表信息
    
    /* 输出电压 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_VOL, &dcw->output_vol, sizeof(dcw->output_vol));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_VOL, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->range.high = DCW_VOL_H;
        ele->range.low = DCW_VOL_L;
    }
    
    /* 电流档位 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_RANGE, &dcw->range, sizeof(dcw->range));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_RANGE, &err);
    
    if(err == CS_ERR_NONE)
    {
        update_dcw_range_affect_inf(ele, step);
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_UPPER, &dcw->upper_limit, sizeof(dcw->upper_limit));//电流上限
    reg_edit_ele_data_inf(STEP_EDIT_WIN_LOWER, &dcw->lower_limit, sizeof(dcw->lower_limit));//电流下限
    reg_edit_ele_data_inf(STEP_EDIT_WIN_ARC, &dcw->arc_sur, sizeof(dcw->arc_sur));//<电弧侦测
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_ARC, &err);
    
    if(err == CS_ERR_NONE)
    {
        update_arc_mode_deit_inf(ele);
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_DELAY_T, &dcw->delay_time,  sizeof(dcw->delay_time));//延时时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_RAISE_T, &dcw->rise_time,  sizeof(dcw->rise_time));//上升时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_TEST_T, &dcw->test_time,  sizeof(dcw->test_time));//测试时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_FALL_T, &dcw->fall_time,  sizeof(dcw->fall_time));//下降时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_INTER_T, &dcw->inter_time,  sizeof(dcw->inter_time));//间隔时间
    
    /* 步间连续 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_CONT, &dcw->step_con,  sizeof(dcw->step_con));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_CONT, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 步间PASS */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PASS, &dcw->step_pass,  sizeof(dcw->step_pass));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PASS, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 输出端口 */
    transform_test_port_to_str(&dcw->port, set_port_buf);
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PORT, set_port_buf, sizeof(set_port_buf));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PORT, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->format.lon = type_spe.port_num;
    }
}

/**
  * @brief  检查IR 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_ir_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    EDIT_ELE_T* tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
        val = *value;
    }
    
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER_IR, &err);
    
    if(val > 0)
    {
        tmp_ele->range.high = val;
    }
    else
    {
        tmp_ele->range.high = IR_RES_H;
    }
    
    if(g_cur_step->one_step.ir.lower_limit > tmp_ele->range.high)
    {
        g_cur_step->one_step.ir.lower_limit = tmp_ele->range.high;
    }
    
    set_edit_num_value(tmp_ele, g_cur_step->one_step.ir.lower_limit);
}
/**
  * @brief  检查IR 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_ir_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    
    if(g_cur_step->one_step.ir.upper_limit > 0)
    {
        if(val > g_cur_step->one_step.ir.upper_limit)
        {
            *value = g_cur_step->one_step.ir.upper_limit - 1;
        }
    }
    else
    {
        if(val > ele->range.high)
        {
            *value = ele->range.high;
        }
        else if(val < ele->range.low)
        {
            *value = ele->range.low;
        }
    }
}
/**
  * @brief  检查 GR 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_gr_upper_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    EDIT_ELE_T* tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
        val = *value;
    }
    
    /* 输出电压 */
    tmp_ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER_GR, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele->range.high = val;
        
        if(g_cur_step->one_step.gr.lower_limit > val)
        {
            g_cur_step->one_step.gr.lower_limit = val;
        }
        
        set_edit_num_value(tmp_ele, g_cur_step->one_step.gr.lower_limit);
    }
}
/**
  * @brief  检查 GR 上限值
  * @param  [in] ele 编辑对象
  * @param  [in] value 上限值
  * @retval 无
  */
static void check_gr_lower_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
    }
}
/**
  * @brief  设置 IR 参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_ir_par_win_ele_data(UN_STRUCT *step)
{
    EDIT_ELE_T* ele;
    IR_STRUCT *ir = &step->ir;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    set_g_cur_win_edit_index_inf(ir_par_index, ARRAY_SIZE(ir_par_index));//设置编辑对象索引表信息
    
    /* 输出电压 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_VOL, &ir->output_vol, sizeof(ir->output_vol));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_VOL, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->range.high = IR_VOL_H;
        ele->range.low = IR_VOL_L;
    }
    
    /* 电阻上限 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_UPPER_IR, &ir->upper_limit, sizeof(ir->upper_limit));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_UPPER_IR, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->range.high = IR_RES_H;
        check_ir_upper_value_validity(ele, &ir->upper_limit);
    }
    
    /* 电阻下限 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_LOWER_IR, &ir->lower_limit, sizeof(ir->lower_limit));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER_IR, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->range.low = IR_RES_L;
        check_ir_lower_value_validity(ele, &ir->lower_limit);
    }
    
    /* 自动换档 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_AUTO_IR, &ir->auto_shift,  sizeof(ir->auto_shift));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_AUTO_IR, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_DELAY_T, &ir->delay_time,  sizeof(ir->delay_time));//延时时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_RAISE_T, &ir->rise_time,  sizeof(ir->rise_time));//上升时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_TEST_T, &ir->test_time,  sizeof(ir->test_time));//测试时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_INTER_T, &ir->inter_time,  sizeof(ir->inter_time));//间隔时间
    
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_CONT, &ir->step_con,  sizeof(ir->step_con));//步间连续
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_CONT, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PASS, &ir->step_pass,  sizeof(ir->step_pass));//步间PASS
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PASS, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 输出端口 */
    transform_test_port_to_str(&ir->port, set_port_buf);
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PORT, set_port_buf, sizeof(set_port_buf));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PORT, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->format.lon = type_spe.port_num;
    }
    
}
/**
  * @brief  设置 IR 参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_gr_par_win_ele_data(UN_STRUCT *step)
{
    EDIT_ELE_T* ele;
    GR_STRUCT *gr = &step->gr;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint32_t value;
    
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    set_g_cur_win_edit_index_inf(gr_par_index, ARRAY_SIZE(gr_par_index));//设置编辑对象索引表信息
    
    /* 输出电流 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_CUR, &gr->output_cur, sizeof(gr->output_cur));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_CUR, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->range.high = GR_CUR_H;
        ele->range.low = GR_CUR_L;
    }
    
    /* 电阻上限 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_UPPER_GR, &gr->upper_limit, sizeof(gr->upper_limit));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_UPPER_GR, &err);
    
    if(err == CS_ERR_NONE)
    {
        value = gr->upper_limit;
        ele->range.high = GR_RES_H(gr->output_cur);
        check_gr_upper_value_validity(ele, &value);
        gr->upper_limit = value;
    }
    
    /* 电阻下限 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_LOWER_GR, &gr->lower_limit, sizeof(gr->lower_limit));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_LOWER_GR, &err);
    
    if(err == CS_ERR_NONE)
    {
        value = gr->lower_limit;
        ele->range.low = GR_RES_L;
        check_gr_lower_value_validity(ele, &value);
        gr->lower_limit = value;
    }
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_TEST_T, &gr->test_time,  sizeof(gr->test_time));//测试时间
    reg_edit_ele_data_inf(STEP_EDIT_WIN_INTER_T, &gr->inter_time,  sizeof(gr->inter_time));//间隔时间
    
    /* 步间连续 */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_CONT, &gr->step_con,  sizeof(gr->step_con));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_CONT, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    /* 步间PASS */
    reg_edit_ele_data_inf(STEP_EDIT_WIN_PASS, &gr->step_pass,  sizeof(gr->step_pass));
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_PASS, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
}

static void init_test_mode_edit_ele_resource_inf(EDIT_ELE_T* ele, UN_STRUCT *step)
{
    if(g_cur_file->work_mode == G_MODE)
    {
        if(step->com.step == 1)
        {
            ele->resource.table = get_defined_g_mode_table();//初始化资源表为已定义的测试模式
            ele->resource.size = get_defined_g_mode_num();//初始化资源表size为已定义的测试模式个数
            ele->resource.user_data = get_defined_g_mode_flag();//初始化用户数据为测试模式对应的数值数组
            ele->resource.user_data_size = get_defined_g_mode_num();//初始化用户数据个数
        }
        else
        {
            ele->resource.table = get_defined_g_mode_no_first_step_table();//初始化资源表为已定义的测试模式
            ele->resource.size = get_defined_g_mode_first_step_num();//初始化资源表size为已定义的测试模式个数
            ele->resource.user_data = get_defined_g_mode_first_step_flag();//初始化用户数据为测试模式对应的数值数组
            ele->resource.user_data_size = get_defined_g_mode_first_step_num();//初始化用户数据个数
        }
    }
    else
    {
        ele->resource.table = get_defined_mode_table();//初始化资源表为已定义的测试模式
        ele->resource.size = get_defined_mode_num();//初始化资源表size为已定义的测试模式个数
        ele->resource.user_data = get_defined_mode_flag();//初始化用户数据为测试模式对应的数值数组
        ele->resource.user_data_size = get_defined_mode_num();//初始化用户数据个数
    }
}

/**
  * @brief  根据测试模式来设置参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_step_par_window_ele_data(UN_STRUCT *step)
{
    EDIT_ELE_T* ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    
    reg_edit_ele_data_inf(STEP_EDIT_WIN_STEP, &step->com.step,  sizeof(step->com.step));//测试步骤
    reg_edit_ele_data_inf(STEP_EDIT_WIN_MODE, &step->com.mode,  sizeof(step->com.mode));//测试模式
    
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_MODE, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_test_mode_edit_ele_resource_inf(ele, step);
    }
    
    /* 根据不同的测试模式进行数据初始化 */
    switch(step->com.mode)
    {
        case ACW:
            set_acw_par_win_ele_data(step);
            break;
        case DCW:
            set_dcw_par_win_ele_data(step);
            break;
        case IR:
            set_ir_par_win_ele_data(step);
            break;
        case GR:
            set_gr_par_win_ele_data(step);
            break;
    }
}
/**
  * @brief  初始化并创建步骤编辑窗口中的编辑控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_step_edit_win_edit_ele(MYUSER_WINDOW_T* win)
{
    memcpy(&tmp_step_par, g_cur_step, sizeof(tmp_step_par));//将当前步信息拷贝到临时存放步骤信息的变量
    g_cur_step = &tmp_step_par;//当前步指向临时存放步骤信息的变量
    set_step_par_window_ele_data(&g_cur_step->one_step);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    auto_layout_win_edit_ele(win);//自动布局窗口中的编辑对象
    init_window_edit_ele(win);//初始化创建编辑对象
}
/**
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_page_num_com_text_ele_dis_inf(win);//初始化页码公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    update_group_inf(win);
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

/**
  * @brief  窗口重绘
  * @param  无
  * @retval 无
  */
static void step_edit_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_WHITE);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  向上键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    com_edit_win_direct_key_up_cb(key_msg);//调用通用的向上键回调
    save_setting_step();//保存数据
}
/**
  * @brief  向上键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_sys_key_enter_cb(KEY_MESSAGE *key_msg)
{
    com_edit_win_direct_key_down_cb(key_msg);//调用通用的向下键回调
    save_setting_step();//保存数据
}
/**
  * @brief  向下键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    com_edit_win_direct_key_down_cb(key_msg);//调用通用的向下键回调
    save_setting_step();//保存数据
}

/**
  * @brief  向左键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
    EDIT_ELE_T *node;
	CS_LIST *t_index = NULL;
	CS_LIST *t_list = &g_cur_win->edit.list_head;//链表头
    uint8_t flag = 0;
    uint8_t c = 0;
    uint8_t rows = 0;
    uint8_t this_page = 0;
    
    rows = g_cur_win->auto_layout.edit_ele_auto_layout_inf[SCREEM_SIZE]->rows;
    
    /* 初始化并创建编辑对象链表中的所有对象 */
	list_for_each_reverse(t_index, t_list)
	{
		node = list_entry(t_index, EDIT_ELE_T, e_list);
        
        if(flag && node->page == this_page)
        {
            if(++c == rows)
            {
                dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
                g_cur_edit_ele = node;
                select_edit_ele(g_cur_edit_ele);
                break;
            }
        }
        
        if(g_cur_edit_ele == node)
        {
            flag = 1;
            this_page = node->page;
        }
	}
}
/**
  * @brief  向右键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
    EDIT_ELE_T *node;
	CS_LIST *t_index = NULL;
	CS_LIST *t_list = &g_cur_win->edit.list_head;//链表
    uint8_t flag = 0;
    uint8_t c = 0;
    uint8_t rows = 0;
    uint8_t this_page = 0;
    
    rows = g_cur_win->auto_layout.edit_ele_auto_layout_inf[SCREEM_SIZE]->rows;
    
    /* 初始化并创建编辑对象链表中的所有对象 */
	list_for_each(t_index, t_list)
	{
		node = list_entry(t_index, EDIT_ELE_T, e_list);
        
        if(flag && node->page == this_page)
        {
            if(++c == rows)
            {
                dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
                g_cur_edit_ele = node;
                select_edit_ele(g_cur_edit_ele);
                break;
            }
        }
        
        if(g_cur_edit_ele == node)
        {
            flag = 1;
            this_page = node->page;
        }
	}
}
/**
  * @brief  编辑测试端口使用的向左键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void edit_test_port_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
    uint8_t cursor;
    WM_HWIN handle;
    
    handle = get_cur_edit_handle();
    
    if(handle == 0)
    {
        return;
    }
    
    cursor = EDIT_GetCursorCharPos(handle);//获取光标位置
    
    if(cursor == 0)
    {
        step_edit_win_direct_key_left_cb(key_msg);
    }
    else
    {
        GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
    }
}


/**
  * @brief  编辑测试端口使用的向右键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void edit_test_port_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
    uint8_t cursor;
    WM_HWIN handle;
    TEST_PORT *port;
    
    handle = get_cur_edit_handle();
    
    if(handle == 0)
    {
        return;
    }
    
    cursor = EDIT_GetCursorCharPos(handle);//获取光标位置
    
    port = get_cur_step_test_port();
    
    if(cursor == port->num)
    {
        step_edit_win_direct_key_left_cb(key_msg);
    }
    else
    {
        GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
    }
}
 
/**
  * @brief  更新系统按键信息
  * @param  [in] hWin窗口句柄
  * @retval 无
  */
static void update_sys_key_inf(WM_HWIN hWin)
{
    FUNCTION_KEY_INFO_T *inf;
    uint16_t size;
    
    inf = step_edit_win_sys_key_init_pool;
    size = ARRAY_SIZE(step_edit_win_sys_key_init_pool);
    
    register_system_key_fun(inf, size, hWin);
}
/**
  * @brief  更新按键信息
  * @param  [in] hWin窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_sys_key_inf(hWin);
}
/**
  * @brief  步骤参数设置窗口回调函数
  * @param  [in] pMsg窗口消息
  * @retval 无
  */
static void step_edit_windows_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case CM_CHILD_W_MSG:
        {
            update_key_inf(hWin);
            break;
        }
		case WM_CREATE:
        {
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
            
            init_create_win_all_ele(win);//创建窗口人所有的对象
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
            dis_one_page_win_edit_eles(win, g_cur_edit_ele->page);//显示一页的编辑对象
            update_page_num(win, g_cur_edit_ele);//更新页码显示
            update_default_range_name();//更新默认的范围显示
            break;
        }
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
		case WM_PAINT:
        {
			step_edit_win_paint_frame();//重绘窗口
            draw_group_inf_area();//绘出记忆组信息区域
			break;
        }
		default:
        {
			WM_DefaultProc(pMsg);
			break;
        }
	}
}

/* Public functions ---------------------------------------------------------*/


EDIT_ELE_T *get_mode_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    reg_edit_ele_data(&step_edit_windows, STEP_EDIT_WIN_MODE, &step->com.mode,  sizeof(step->com.mode));//测试模式
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_MODE, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_test_mode_edit_ele_resource_inf(ele, step);
//        ele->resource.table = get_defined_mode_table();//初始化资源表为已定义的测试模式
//        ele->resource.size = get_defined_mode_num();//初始化资源表size为已定义的测试模式个数
//        ele->resource.user_data = get_defined_mode_flag();//初始化用户数据为测试模式对应的数值数组
//        ele->resource.user_data_size = get_defined_mode_num();//初始化用户数据个数
    }
    
    return ele;
}
EDIT_ELE_T *get_range_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint8_t mode = 0;
    void *p_data;
    uint8_t n = 0;
    
    mode = get_cur_step_mode();
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    //电流档位
    switch(mode)
    {
        case ACW:
            p_data = &step->acw.range;
            n = sizeof(step->acw.range);
            break;
        case DCW:
            p_data = &step->dcw.range;
            n = sizeof(step->dcw.range);
            break;
        default:
            return ele;
    }
    
    reg_edit_ele_data(&step_edit_windows, STEP_EDIT_WIN_RANGE, p_data,  n);
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_RANGE, &err);
    
    if(err == CS_ERR_NONE)
    {
        ele->resource.table = get_defined_range_table(mode);
        ele->resource.size = get_defined_range_num(mode);
        ele->resource.user_data = get_defined_range_flag(mode);
        ele->resource.user_data_size = get_defined_range_num(mode);
    }
    
    return ele;
}

EDIT_ELE_T *get_auto_shift_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint8_t mode = 0;
    void *p_data;
    uint8_t n = 0;
    
    mode = get_cur_step_mode();
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    if(mode != IR)
    {
        return ele;
    }
    
    p_data = &step->ir.auto_shift;
    n = sizeof(step->ir.auto_shift);
    
    reg_edit_ele_data(this_win, STEP_EDIT_WIN_AUTO_IR, p_data,  n);
    ele = get_edit_ele_inf(pool, size, STEP_EDIT_WIN_AUTO_IR, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_sw_type_edit_ele_resource_inf(ele);
    }
    
    return ele;
}

EDIT_ELE_T *get_vol_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint8_t mode = 0;
    void *p_data;
    uint8_t n = 0;
    CS_INDEX index;
    uint32_t high = 0;
    uint32_t low = 0;
    
    mode = get_cur_step_mode();
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    //电流档位
    switch(mode)
    {
        case ACW:
            p_data = &step->acw.output_vol;
            n = sizeof(step->acw.output_vol);
            index = STEP_EDIT_WIN_VOL;
            high = ACW_VOL_H;
            low = ACW_VOL_L;
            break;
        case DCW:
            p_data = &step->dcw.output_vol;
            n = sizeof(step->dcw.output_vol);
            index = STEP_EDIT_WIN_VOL;
            high = DCW_VOL_H;
            low = DCW_VOL_L;
            break;
        case IR:
            p_data = &step->dcw.output_vol;
            n = sizeof(step->dcw.output_vol);
            index = STEP_EDIT_WIN_VOL;
            high = IR_VOL_H;
            low = IR_VOL_L;
            break;
        case GR:
            p_data = &step->gr.output_cur;
            n = sizeof(step->gr.output_cur);
            index = STEP_EDIT_WIN_CUR;
            high = GR_CUR_H;
            low = GR_CUR_L;
            break;
        default:
            return ele;
    }
    
    reg_edit_ele_data(&step_edit_windows, index, p_data,  n);
    ele = get_edit_ele_inf(pool, size, index, &err);
    ele->dis.edit.max_len = 5;
    ele->range.high = high;
    ele->range.low = low;
    
    return ele;
}
EDIT_ELE_T *get_test_time_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint8_t mode = 0;
    void *p_data;
    uint8_t n = 0;
    CS_INDEX index;
    
    mode = get_cur_step_mode();
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    index = STEP_EDIT_WIN_TEST_T;
    
    //电流档位
    switch(mode)
    {
        case ACW:
            p_data = &step->acw.test_time;
            n = sizeof(step->acw.test_time);
            break;
        case DCW:
            p_data = &step->dcw.test_time;
            n = sizeof(step->dcw.test_time);
            break;
        case IR:
            p_data = &step->dcw.test_time;
            n = sizeof(step->dcw.test_time);
            break;
        case GR:
            p_data = &step->gr.test_time;
            n = sizeof(step->gr.test_time);
            break;
        default:
            return ele;
    }
    
    reg_edit_ele_data(&step_edit_windows, index, p_data,  n);
    ele = get_edit_ele_inf(pool, size, index, &err);
    ele->dis.edit.max_len = 5;
    
    return ele;
}
EDIT_ELE_T *get_upper_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint8_t mode = 0;
    void *p_data;
    uint8_t n = 0;
    CS_INDEX index;
    uint32_t high = 0;
    uint32_t low = 0;
    uint8_t range = 0;
    UNIT_T unit = NULL_U_NULL;
    uint8_t dec = 0;
    
    mode = get_cur_step_mode();
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    //电流档位
    switch(mode)
    {
        case ACW:
            index = STEP_EDIT_WIN_UPPER;
            p_data = &step->acw.upper_limit;
            n = sizeof(step->acw.upper_limit);
            range = step->acw.range;
            unit = ac_gear[range].unit;
            dec = ac_gear[range].dec;
            high = ac_gear[range].high_max;
            low = 0;
            break;
        case DCW:
            index = STEP_EDIT_WIN_UPPER;
            p_data = &step->dcw.upper_limit;
            n = sizeof(step->dcw.upper_limit);
            range = step->dcw.range;
            unit = dc_gear[range].unit;
            dec = dc_gear[range].dec;
            high = dc_gear[range].high_max;
            low = 0;
            break;
        case IR:
            index = STEP_EDIT_WIN_UPPER_IR;
            p_data = &step->ir.upper_limit;
            n = sizeof(step->ir.upper_limit);
            range = step->dcw.range;
            unit = RES_U_MOHM;
            dec = 0;
            high = IR_RES_H;
            low = 0;
            break;
        case GR:
            index = STEP_EDIT_WIN_UPPER_GR;
            p_data = &step->gr.upper_limit;
            n = sizeof(step->gr.upper_limit);
            unit = RES_U_mOHM;
            dec = 1;
            high = GR_RES_H(step->gr.output_cur);
            low = GR_RES_L;
            break;
        default:
            return ele;
    }
    
    reg_edit_ele_data(this_win, index, p_data,  n);
    ele = get_edit_ele_inf(pool, size, index, &err);
    ele->dis.edit.max_len = 5;
    ele->range.high = high;
    ele->range.low = low;
    ele->format.dec = dec;
    ele->format.unit = unit;
    
    return ele;
}
EDIT_ELE_T *get_lower_edit_ele_inf(UN_STRUCT *step)
{
    EDIT_ELE_T* ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t size;
    uint8_t mode = 0;
    void *p_data;
    uint8_t n = 0;
    CS_INDEX index;
    uint32_t low = 0;
    uint32_t high = 0;
    UNIT_T unit = NULL_U_NULL;
    uint8_t dec = 0;
    uint8_t range = 0;
    
    mode = get_cur_step_mode();
    pool = this_win->edit.pool;
    size = this_win->edit.pool_size;
    
    //电流档位
    switch(mode)
    {
        case ACW:
            index = STEP_EDIT_WIN_LOWER;
            p_data = &step->acw.lower_limit;
            n = sizeof(step->acw.lower_limit);
            range = step->acw.range;
            unit = ac_gear[range].unit;
            dec = ac_gear[range].dec;
            high = step->acw.upper_limit;
            low = 0;
            break;
        case DCW:
            index = STEP_EDIT_WIN_LOWER;
            p_data = &step->dcw.lower_limit;
            n = sizeof(step->dcw.lower_limit);
            range = step->dcw.range;
            unit = dc_gear[range].unit;
            dec = dc_gear[range].dec;
            high = step->dcw.upper_limit;
            low = 0;
            break;
        case IR:
            index = STEP_EDIT_WIN_LOWER_IR;
            p_data = &step->ir.lower_limit;
            n = sizeof(step->ir.lower_limit);
            unit = RES_U_MOHM;
            dec = 0;
            
            if(step->ir.upper_limit != 0)
            {
                high = step->ir.upper_limit;
            }
            else
            {
                high = IR_RES_H;
            }
            
            low = IR_RES_L;
            break;
        case GR:
            index = STEP_EDIT_WIN_LOWER_GR;
            p_data = &step->gr.lower_limit;
            n = sizeof(step->gr.lower_limit);
            unit = RES_U_mOHM;
            dec = 1;
            high = step->gr.upper_limit;
            low = 0;
            break;
        default:
            return ele;
    }
    
    reg_edit_ele_data(&step_edit_windows, index, p_data,  n);
    ele = get_edit_ele_inf(pool, size, index, &err);
    ele->dis.edit.max_len = 5;
    ele->range.high = high;
    ele->range.low = low;
    ele->format.dec = dec;
    ele->format.unit = unit;
    
    return ele;
}
/**
  * @brief  创建步骤编辑窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_step_edit_win(int hWin)
{
    create_user_window(&step_edit_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
