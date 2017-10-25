/**
  ******************************************************************************
  * @file    test_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   测试窗口
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "running_test.h"
#include "ui_com/com_edit_api.h"
#include "7_test_ui_layout_1.h"
#include "test_win.h"
#include "module_manage.h"
#include "send_cmd.h"
#include "start_stop_key.h"
#include "tim3.h"
#include "os.h"
#include "running_test.h"
#include "crc.h"
#include "key_led_buzzer.h"
#include "rtc_config.h"
#include "ff.h"
#include "FATFS_MANAGE.H"
#include "image/logo.h"
#include "mem_alloc.h"

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  测试窗口编辑控件索引枚举
  */
enum{
    TEST_WIN_EDIT_INDEX,///<测试窗口中的编辑对象索引枚举
};
/**
  * @brief  测试状态机的状态枚举定义
  */
typedef enum{
    TEST_IDLE,///<空闲状态
    TEST_START,///<启动测试
    TEST_RESET,///<复位测试
    TEST_TESTING,///<正在测试
    CHECK_TEST_OVER_SIGN,///<检查从机测试完成信号
}TEST_STATUS_ENUM;

typedef struct{
    uint8_t fail;///<测试失败标记
    uint8_t save_only_onece;///<只保存1次结果标记
}TEST_FLAG;
/**
  * @brief  每路的文本对象在文本对象池中的索引测试状态等
  */
typedef struct
{
    ROAD_INDEX road_num;
    CS_INDEX num;///<路编号
    CS_INDEX mode;///<测试模块
    CS_INDEX status;///<测试状态
    CS_INDEX vol;///<测试电压
    CS_INDEX cur;///<测试电流
    CS_INDEX real;///<真实电流
    CS_INDEX time;///<测试时间
    CS_INDEX bar;///<状态条背景颜色
    uint8_t test_st;///<测试路状态
    CS_BOOL (*road_test_over)(void);///<判断该路是否测试结束
    CS_BOOL (*road_test_alarm)(void);///<判断该路是否测试报警
    uint8_t output_buf[10];///<输出采集值
    uint8_t loop_buf[10];///<回路采集值
    uint8_t real_buf[10];///<真实电流采集值
    uint8_t time_buf[10];///<测试时间
}ROAD_DIS_ELE_INF;

/**
  * @brief  数据设置的倍率
  */
typedef struct{
    uint32_t x_n;///<n倍率
}DATA_MUL_POWER;
/* Private define ------------------------------------------------------------*/
#define CUR_VALUE_AT_KEY_COLOR      GUI_RED     ///<当前值所在的菜单键字体颜色
#define NO_CUR_VALUE_AT_KEY_COLOR   GUI_WHITE   ///<当前值不在的菜单键字体颜色

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void update_test_win_menu_key_inf(WM_HMEM hWin);
static void test_win_cb(WM_MESSAGE * pMsg);
static void select_set_acw_par_menu_1(int hWin);
static void select_set_acw_par_menu_2(int hWin);
static void select_set_ir_par_menu_1(int hWin);
static void select_set_ir_par_menu_2(int hWin);
static void select_set_gr_par_menu_1(int hWin);
static void select_set_gr_par_menu_2(int hWin);

static void update_key_menu(int id);
static void test_win_f0_cb(KEY_MESSAGE *key_msg);
static void test_win_f1_cb(KEY_MESSAGE *key_msg);
static void test_win_f2_cb(KEY_MESSAGE *key_msg);
static void test_win_f3_cb(KEY_MESSAGE *key_msg);
static void test_win_f4_cb(KEY_MESSAGE *key_msg);
static void test_win_f5_cb(KEY_MESSAGE *key_msg);
static void test_win_f6_cb(KEY_MESSAGE *key_msg);

static void set_acw_par_f1_1_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f2_1_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f3_1_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f4_1_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f5_1_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f6_1_cb(KEY_MESSAGE *key_msg);

static void set_acw_par_f1_2_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f2_2_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f3_2_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f4_2_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f5_2_cb(KEY_MESSAGE *key_msg);
static void set_acw_par_f6_2_cb(KEY_MESSAGE *key_msg);

static void set_ir_par_f1_1_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f2_1_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f3_1_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f4_1_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f5_1_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f6_1_cb(KEY_MESSAGE *key_msg);

static void set_ir_par_f1_2_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f2_2_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f3_2_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f4_2_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f5_2_cb(KEY_MESSAGE *key_msg);
static void set_ir_par_f6_2_cb(KEY_MESSAGE *key_msg);

static void set_gr_par_f1_1_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f2_1_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f3_1_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f4_1_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f5_1_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f6_1_cb(KEY_MESSAGE *key_msg);

static void set_gr_par_f1_2_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f2_2_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f3_2_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f4_2_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f5_2_cb(KEY_MESSAGE *key_msg);
static void set_gr_par_f6_2_cb(KEY_MESSAGE *key_msg);

static void edit_test_win_vol_f1_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_vol_f2_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_vol_f3_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_vol_f4_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_vol_f5_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_vol_f6_cb(KEY_MESSAGE *key_msg);

static void edit_test_win_upper_f1_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_upper_f2_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_upper_f3_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_upper_f4_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_upper_f5_cb(KEY_MESSAGE *key_msg);
static void edit_test_win_upper_f6_cb(KEY_MESSAGE *key_msg);

static void edit_auto_shift_f1_cb(KEY_MESSAGE *key_msg);
static void edit_auto_shift_f2_cb(KEY_MESSAGE *key_msg);
static void edit_auto_shift_f3_cb(KEY_MESSAGE *key_msg);
static void edit_auto_shift_f4_cb(KEY_MESSAGE *key_msg);
static void edit_auto_shift_f5_cb(KEY_MESSAGE *key_msg);
static void edit_auto_shift_f6_cb(KEY_MESSAGE *key_msg);

static void test_win_edit_mode_f6_cb(KEY_MESSAGE *key_msg);
static void test_win_edit_range_f6_cb(KEY_MESSAGE *key_msg);

static void test_win_edit_mode_menu_key_init(WM_HMEM hWin);
static void test_win_edit_range_menu_key_init(WM_HMEM hWin);
static void test_win_edit_vol_menu_key_init(WM_HMEM hWin);
static void test_win_edit_test_time_menu_key_init(WM_HMEM hWin);
static void test_win_edit_auto_shift_menu_key_init(WM_HMEM hWin);

static void init_create_test_win_edit_ele(MYUSER_WINDOW_T *win);
static void init_vol_edit_ele_pos_size(MYUSER_WINDOW_T *win);
static void init_test_time_edit_ele_pos_size(MYUSER_WINDOW_T *win);
static void init_cur_upper_edit_ele_pos_size(MYUSER_WINDOW_T *win);
static void init_cur_lower_edit_ele_pos_size(MYUSER_WINDOW_T *win);

static void test_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void test_win_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void test_win_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void test_win_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void test_win_sys_key_enter_cb(KEY_MESSAGE *key_msg);
static void test_win_sys_start_key_fun_cb(KEY_MESSAGE *key_msg);
static void test_win_sys_stop_key_fun_cb(KEY_MESSAGE *key_msg);

static void test_win_edit_num_direct_key_up_cb   (KEY_MESSAGE *key_msg);
static void test_win_edit_num_direct_key_down_cb (KEY_MESSAGE *key_msg);
static void test_win_edit_num_direct_key_left_cb (KEY_MESSAGE *key_msg);
static void test_win_edit_num_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void test_win_edit_num_sys_key_enter_cb   (KEY_MESSAGE *key_msg);

static void test_win_win_edit_num_sys_key_init(WM_HMEM hWin);


static void update_test_win_text_ele_text(MYUSER_WINDOW_T* win);
static void init_create_test_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_test_win_text_ele(MYUSER_WINDOW_T* win);
static void clear_range_text_ele(MYUSER_WINDOW_T* win);
static void test_win_select_test_mode_key_cb(KEY_MESSAGE *key_msg);
static void test_win_select_cur_range_key_cb(KEY_MESSAGE *key_msg);

static void change_key_set_ir_par_menu(int hWin);
static void update_key_set_ir_par_menu(int hWin);
static void change_key_set_gr_par_menu(int hWin);
static void update_key_set_gr_par_menu(int hWin);
static void change_key_set_acw_par_menu(int hWin);


static void update_cur_range_cur_value_menu_key_color(void);
static void update_data_mul_power(uint32_t key_value, uint32_t mul_power);


static void test_win_check_cur_step_changed_send_to_slave(void);
static void test_win_update_cur_step_crc(void);

static void set_roads_test_st(uint8_t test_st);
static void roads_into_test_st(void);
static void update_roads_bar(void);

static void test_reset_fun(void);
/* Private variables ---------------------------------------------------------*/
static TEST_FLAG test_flag;///<测试标记
static uint16_t test_step = 1;///测试步骤

/**
  * @brief  各路显示测试信息
  */
ROAD_DIS_ELE_INF road_test_dis_inf[]=
{
    {
        INDEX_ROAD_1,
        TEST_UI_ROAD01_NUM,///<第1路的编号
        TEST_UI_ROAD01_MODE,///<第1路的模式
        TEST_UI_ROAD01_STATUS,///<第1路的状态
        TEST_UI_ROAD01_VOLTAGE,///<第1路的输出电压
        TEST_UI_ROAD01_UPPER,///<第1路的测试电流 在等待测试时显示的是电流上限
        TEST_UI_ROAD01_REAL,///<第1路的真实电流
        TEST_UI_ROAD01_TIME,///<第1路的测试时间
        TEST_UI_ROAD01_BAR,///<第1路的BAR文本复用其改变背景颜色
        ST_WAIT,///<测试路状态
        road1_test_over,///<判断该路是否测试结束
        road1_test_alarm,///<判断该路是否测试报警
    },
    {
        INDEX_ROAD_2,
        TEST_UI_ROAD02_NUM,///<第2路的编号
        TEST_UI_ROAD02_MODE,///<第2路的模式
        TEST_UI_ROAD02_STATUS,///<第2路的状态
        TEST_UI_ROAD02_VOLTAGE,///<第2路的输出电压
        TEST_UI_ROAD02_UPPER,///<第2路的测试电流 在等待测试时显示的是电流上限
        TEST_UI_ROAD02_REAL,///<第2路的真实电流
        TEST_UI_ROAD02_TIME,///<第2路的测试时间
        TEST_UI_ROAD02_BAR,///<第2路的BAR文本复用其改变背景颜色
        ST_WAIT,///<测试路状态
        road2_test_over,///<判断该路是否测试结束
        road2_test_alarm,///<判断该路是否测试报警
    },
    {
        INDEX_ROAD_3,
        TEST_UI_ROAD03_NUM,///<第3路的编号
        TEST_UI_ROAD03_MODE,///<第3路的模式
        TEST_UI_ROAD03_STATUS,///<第3路的状态
        TEST_UI_ROAD03_VOLTAGE,///<第3路的输出电压
        TEST_UI_ROAD03_UPPER,///<第3路的测试电流 在等待测试时显示的是电流上限
        TEST_UI_ROAD03_REAL,///<第3路的真实电流
        TEST_UI_ROAD03_TIME,///<第3路的测试时间
        TEST_UI_ROAD03_BAR,///<第3路的BAR文本复用其改变背景颜色
        ST_WAIT,///<测试路状态
        road3_test_over,///<判断该路是否测试结束
        road3_test_alarm,///<判断该路是否测试报警
    },
    {
        INDEX_ROAD_4,
        TEST_UI_ROAD04_NUM,///<第4路的编号
        TEST_UI_ROAD04_MODE,///<第4路的模式
        TEST_UI_ROAD04_STATUS,///<第4路的状态
        TEST_UI_ROAD04_VOLTAGE,///<第4路的输出电压
        TEST_UI_ROAD04_UPPER,///<第4路的测试电流 在等待测试时显示的是电流上限
        TEST_UI_ROAD04_REAL,///<第4路的真实电流
        TEST_UI_ROAD04_TIME,///<第4路的测试时间
        TEST_UI_ROAD04_BAR,///<第4路的BAR文本复用其改变背景颜色
        ST_WAIT,///<测试路状态
        road4_test_over,///<判断该路是否测试结束
        road4_test_alarm,///<判断该路是否测试报警
    },
};

static uint32_t hold_start_sign_time;///<启动信号保持时间
static uint32_t hold_stop_sign_time;///<复位信号保持时间
/**
  * @brief  测试状态机的状态
  */
TEST_STATUS_ENUM test_status;
/**
  * @brief  定时器句柄
  */
static	WM_HWIN test_win_timer_handle;
/**
  * @brief  数据倍率
  */
static DATA_MUL_POWER data_mul_power;
/**
  * @brief  当前所在菜单页标记
  */
static int8_t cur_at_menu_page_flag;

/**
  * @brief  测试界面的位置尺寸信息，根据不同屏幕进行初始化
  */
static WIDGET_POS_SIZE_T* test_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_test_windows,///<4.3寸屏
    &_7_test_windows,///<5.6寸屏
    &_7_test_windows,///<7寸屏
};

/**
  * @brief  测试界面下的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_menu_key_pool[]=
{
    {"", F_KEY_DISPLAY	, KEY_F0 & _KEY_UP,	test_win_f0_cb },//f0
    {"", F_KEY_FILE		, KEY_F1 & _KEY_UP,	test_win_f1_cb },//f1
    {"", F_KEY_SETTING  , KEY_F2 & _KEY_UP,	test_win_f2_cb },//f2
    {"", F_KEY_RESULT   , KEY_F3 & _KEY_UP,	test_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP,	test_win_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP,	test_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	test_win_f6_cb },//f6
};
/**
  * @brief  测试界面下ACW设置参数的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_acw_menu_pool[][6]=
{
    {
        {"", F_KEY_MODE  , KEY_F1 & _KEY_UP, set_acw_par_f1_1_cb},//f1
        {"", F_KEY_VOL   , KEY_F2 & _KEY_UP, set_acw_par_f2_1_cb},//f2
        {"", F_KEY_RANGE , KEY_F3 & _KEY_UP, set_acw_par_f3_1_cb},//f3
        {"", F_KEY_TIME  , KEY_F4 & _KEY_UP, set_acw_par_f4_1_cb},//f4
        {"", F_KEY_MORE  , KEY_F5 & _KEY_UP, set_acw_par_f5_1_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_acw_par_f6_1_cb},//f6
    },
    {
        {"", F_KEY_UPPER , KEY_F1 & _KEY_UP, set_acw_par_f1_2_cb},//f1
        {"", F_KEY_NULL  , KEY_F2 & _KEY_UP, set_acw_par_f2_2_cb},//f2
        {"", F_KEY_NULL  , KEY_F3 & _KEY_UP, set_acw_par_f3_2_cb},//f3
        {"", F_KEY_NULL  , KEY_F4 & _KEY_UP, set_acw_par_f4_2_cb},//f4
        {"", F_KEY_NULL  , KEY_F5 & _KEY_UP, set_acw_par_f5_2_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_acw_par_f6_2_cb},//f6
    },
};
/**
  * @brief  测试界面下IR设置参数的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_ir_menu_pool[][6]=
{
    {
        {"", F_KEY_MODE  , KEY_F1 & _KEY_UP, set_ir_par_f1_1_cb},//f1
        {"", F_KEY_VOL   , KEY_F2 & _KEY_UP, set_ir_par_f2_1_cb},//f2
        {"", F_KEY_AUTO  , KEY_F3 & _KEY_UP, set_ir_par_f3_1_cb},//f3
        {"", F_KEY_TIME  , KEY_F4 & _KEY_UP, set_ir_par_f4_1_cb},//f4
        {"", F_KEY_MORE  , KEY_F5 & _KEY_UP, set_ir_par_f5_1_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_ir_par_f6_1_cb},//f6
    },
    {
        {"", F_KEY_LOWER , KEY_F1 & _KEY_UP, set_ir_par_f1_2_cb},//f1
        {"", F_KEY_NULL  , KEY_F2 & _KEY_UP, set_ir_par_f2_2_cb},//f2
        {"", F_KEY_NULL  , KEY_F3 & _KEY_UP, set_ir_par_f3_2_cb},//f3
        {"", F_KEY_NULL  , KEY_F4 & _KEY_UP, set_ir_par_f4_2_cb},//f4
        {"", F_KEY_NULL  , KEY_F5 & _KEY_UP, set_ir_par_f5_2_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_ir_par_f6_2_cb},//f6
    },
};
/**
  * @brief  测试界面下GR设置参数的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_gr_menu_pool[][6]=
{
    {
        {"", F_KEY_MODE  , KEY_F1 & _KEY_UP, set_gr_par_f1_1_cb},//f1
        {"", F_KEY_CUR   , KEY_F2 & _KEY_UP, set_gr_par_f2_1_cb},//f2
        {"", F_KEY_TIME  , KEY_F3 & _KEY_UP, set_gr_par_f3_1_cb},//f3
        {"", F_KEY_UPPER , KEY_F4 & _KEY_UP, set_gr_par_f4_1_cb},//f4
        {"", F_KEY_MORE  , KEY_F5 & _KEY_UP, set_gr_par_f5_1_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_gr_par_f6_1_cb},//f6
    },
    {
        {"", F_KEY_NULL  , KEY_F1 & _KEY_UP, set_gr_par_f1_2_cb},//f1
        {"", F_KEY_NULL  , KEY_F2 & _KEY_UP, set_gr_par_f2_2_cb},//f2
        {"", F_KEY_NULL  , KEY_F3 & _KEY_UP, set_gr_par_f3_2_cb},//f3
        {"", F_KEY_NULL  , KEY_F4 & _KEY_UP, set_gr_par_f4_2_cb},//f4
        {"", F_KEY_NULL  , KEY_F5 & _KEY_UP, set_gr_par_f5_2_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_gr_par_f6_2_cb},//f6
    },
};

/**
  * @brief  测试界面显示的文本索引表
  */
static CS_INDEX test_ui_ele_buf[] =
{
	TEST_UI_ROAD01_BAR,///<第一路的BAR文本复用其改变背景颜色
	TEST_UI_ROAD01_NUM,     ///<第一路编号
	TEST_UI_ROAD01_MODE,    ///<第一路模式
	TEST_UI_ROAD01_STATUS,  ///<第一路状态
	TEST_UI_ROAD01_VOLTAGE, ///<第一路电压
	TEST_UI_ROAD01_UPPER,   ///<第一路上限
	TEST_UI_ROAD01_REAL,    ///<第一路真实电流
	TEST_UI_ROAD01_TIME,    ///<第一路测试时间
	TEST_UI_ROAD02_BAR,///<第一路的BAR文本复用其改变背景颜色
	TEST_UI_ROAD02_NUM,     ///<第二路编号
	TEST_UI_ROAD02_MODE,    ///<第二路模式
	TEST_UI_ROAD02_STATUS,  ///<第二路状态
	TEST_UI_ROAD02_VOLTAGE, ///<第二路电压
	TEST_UI_ROAD02_UPPER,   ///<第二路上限
	TEST_UI_ROAD02_REAL,    ///<第二路真实电流
	TEST_UI_ROAD02_TIME,    ///<第二路测试时间
	TEST_UI_ROAD03_BAR,///<第一路的BAR文本复用其改变背景颜色
	TEST_UI_ROAD03_NUM,     ///<第三路编号
	TEST_UI_ROAD03_MODE,    ///<第三路模式
	TEST_UI_ROAD03_STATUS,  ///<第三路状态
	TEST_UI_ROAD03_VOLTAGE, ///<第三路电压
	TEST_UI_ROAD03_UPPER,   ///<第三路上限
	TEST_UI_ROAD03_REAL,    ///<第三路真实电流
	TEST_UI_ROAD03_TIME,    ///<第三路测试时间
	TEST_UI_ROAD04_BAR,///<第一路的BAR文本复用其改变背景颜色
	TEST_UI_ROAD04_NUM,     ///<第四路编号
	TEST_UI_ROAD04_MODE,    ///<第四路模式
	TEST_UI_ROAD04_STATUS,  ///<第四路状态
	TEST_UI_ROAD04_VOLTAGE, ///<第四路电压
	TEST_UI_ROAD04_UPPER,   ///<第四路上限
	TEST_UI_ROAD04_REAL,    ///<第四路真实电流
	TEST_UI_ROAD04_TIME,    ///<第四路测试时间
};

/**
  * @brief 界面文本对象池数组
  */
static TEXT_ELE_T test_ui_ele_pool[]=
{
	{{"输出电压:","Voltage:"}, TEST_UI_VOLTAGE      },
	{{"5.000kV" , "5.000kV" }, TEST_UI_CUR_VOLTAGE  },
	{{"电流档位:","CUR.Range:"}, TEST_UI_RANGE      },
	{{"2mA"    , "2mA"      }, TEST_UI_CUR_RANGE    },
	{{"电流上限:","CUR.Upper:"}, TEST_UI_UPPER      },
	{{"2.000mA", "2.000mA"  }, TEST_UI_CUR_UPPER    },
	{{"电流下限:","CUR.Lower:"}, TEST_UI_LOWER      },
	{{"1.000mA", "1.000mA"  }, TEST_UI_CUR_LOWER    },
	{{"电弧侦测:","Arc:"    }, TEST_UI_ARC          },
	{{"10.00mA", "10.00mA"  }, TEST_UI_CUR_ARC      },
	{{"步间连续:","Continue:"}, TEST_UI_CONTINUE    },
	{{"开"     , "ON"       }, TEST_UI_CUR_CONTINUE },
	
	{{"测试:"  ,"Test:"     }, TEST_UI_TEST         },
	{{"ACW"    ,"ACW"       }, TEST_UI_TEST_1       },
	{{"->"     ,"->"        }, TEST_UI_TEST_2       },
	{{"DCW"    ,"DCW"       }, TEST_UI_TEST_3       },
	{{"->"     ,"->"        }, TEST_UI_TEST_4       },
	{{"IR"     ,"IR"        }, TEST_UI_TEST_5       },
	{{"->"     ,"->"        }, TEST_UI_TEST_6       },
	{{"端口:"  ,"Port:"     }, TEST_UI_PORT         },
	{{"浮地"   ,"Float"     }, TEST_UI_CUR_PORT     },
	{{"多路:"  ,"Roads:"    }, TEST_UI_ROADS        },
	{{"HHXX"   ,"HHXX"      }, TEST_UI_CUR_ROADS    },
	
	{{"" ,""                }, TEST_UI_ROAD01_BAR    },
	{{"01"     ,"01"        }, TEST_UI_ROAD01_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD01_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD01_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD01_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD01_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD01_REAL   },
	{{"823.4s" ,"823.4s"    }, TEST_UI_ROAD01_TIME   },
	{{"" ,""                }, TEST_UI_ROAD02_BAR    },
	{{"02"     ,"02"        }, TEST_UI_ROAD02_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD02_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD02_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD02_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD02_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD02_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD02_TIME   },
	{{"" ,""                }, TEST_UI_ROAD03_BAR    },
	{{"03"     ,"03"        }, TEST_UI_ROAD03_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD03_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD03_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD03_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD03_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD03_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD03_TIME   },
	{{"" ,""                }, TEST_UI_ROAD04_BAR    },
	{{"04"     ,"04"        }, TEST_UI_ROAD04_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD04_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD04_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD04_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD04_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD04_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD04_TIME   },
	{{"" ,""                }, TEST_UI_ROAD05_BAR    },
	{{"05"     ,"05"        }, TEST_UI_ROAD05_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD05_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD05_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD05_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD05_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD05_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD05_TIME   },
	{{"" ,""                }, TEST_UI_ROAD06_BAR    },
	{{"06"     ,"06"        }, TEST_UI_ROAD06_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD06_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD06_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD06_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD06_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD06_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD06_TIME   },
	{{"" ,""                }, TEST_UI_ROAD07_BAR    },
	{{"07"     ,"07"        }, TEST_UI_ROAD07_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD07_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD07_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD07_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD07_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD07_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD07_TIME   },
	{{"" ,""                }, TEST_UI_ROAD08_BAR    },
	{{"08"     ,"08"        }, TEST_UI_ROAD08_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD08_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD08_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD08_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD08_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD08_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD08_TIME   },
	
	{{"测试结果:","TestResult:"}, TEST_UI_RESULT     },
	{{"0001"   ,"0001"      }, TEST_UI_RES1_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES1_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES1_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES1_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES1_RESULT   },
    
	{{"0001"   ,"0001"      }, TEST_UI_RES2_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES2_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES2_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES2_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES2_RESULT   },
	{{"0001"   ,"0001"      }, TEST_UI_RES3_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES3_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES3_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES3_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES3_RESULT   },
	{{"0001"   ,"0001"      }, TEST_UI_RES4_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES4_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES4_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES4_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES4_RESULT   },
};

/**
  * @brief 界面编辑对象索引表
  */
CS_INDEX test_win_edit_index_table[]=
{
    TEST_WIN_EDIT_INDEX,
};
/**
  * @brief 界面编辑对象池数组
  */
static EDIT_ELE_T test_win_edit_ele_pool[]=
{
    {
        {"",""}, /* 名称 */
        TEST_WIN_EDIT_INDEX,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 0/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, },/*类型*/
        {0/*dec*/,2/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {99/*heigh*/,1/*low*/,{"",""}/*notice*/},/*range*/
        {test_win_win_edit_num_sys_key_init, NULL, NULL,},/*key_inf*/
    },
};
/**
  * @brief  测试窗口结构体初始化
  */
static MYUSER_WINDOW_T test_windows=
{
    {"测试窗口","Test Window"},
    test_win_cb, update_test_win_menu_key_inf,
	{
        test_ui_ele_pool,COUNT_ARRAY_SIZE(test_ui_ele_pool),
        (CS_INDEX*)test_ui_ele_buf,COUNT_ARRAY_SIZE(test_ui_ele_buf),
        init_create_test_win_text_ele
    },/*text*/
    {
        test_win_edit_ele_pool, ARRAY_SIZE(test_win_edit_ele_pool),
        test_win_edit_index_table, ARRAY_SIZE(test_win_edit_index_table),
        NULL,
    },/*edit*/
    {
        com_text_ele_pool, ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_group_com_ele_table,ARRAY_SIZE(range_group_com_ele_table),
        init_create_test_win_com_ele,
    },/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    test_win_pos_size_pool,/*pos_size_pool */
};
/**
  * @brief  用this_win来代指向测试窗口
  */
static MYUSER_WINDOW_T *this_win = &test_windows;
/**
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF test_win_mode_inf_pool[]=
{
    {ACW_STR , ACW, test_win_select_test_mode_key_cb},
    {DCW_STR , DCW, test_win_select_test_mode_key_cb},
    {IR_STR  , IR , test_win_select_test_mode_key_cb},
    {GR_STR  , GR , test_win_select_test_mode_key_cb},
    {BBD_STR , BBD, test_win_select_test_mode_key_cb},
    {CC_STR  , CC , test_win_select_test_mode_key_cb},
};
/**
  * @brief  编辑电流档位时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF test_win_cur_range_inf_pool[]=
{
    { CUR_2uA_STR   , AC_2uA	, test_win_select_cur_range_key_cb},
    { CUR_20uA_STR  , AC_20uA	, test_win_select_cur_range_key_cb},
    { CUR_200uA_STR , AC_200uA  , test_win_select_cur_range_key_cb},
    { CUR_2mA_STR   , AC_2mA	, test_win_select_cur_range_key_cb},
    { CUR_10mA_STR  , AC_10mA	, test_win_select_cur_range_key_cb},
    { CUR_20mA_STR  , AC_20mA	, test_win_select_cur_range_key_cb},
    { CUR_50mA_STR  , AC_50mA	, test_win_select_cur_range_key_cb},
    { CUR_100mA_STR , AC_100mA  , test_win_select_cur_range_key_cb},
    { CUR_200mA_STR , AC_200mA  , test_win_select_cur_range_key_cb},
    { CUR_2A_STR    , AC_2A     , test_win_select_cur_range_key_cb},
};
/**
  * @brief  编辑测试模式时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	test_win_edit_mode_menu_key_init_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP, 0},//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP, 0},//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP, 0},//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP, 0},//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP, 0},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, test_win_edit_mode_f6_cb },//f6
};
/**
  * @brief  编辑电流档位时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	test_win_edit_range_menu_key_init_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP, 0},//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP, 0},//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP, 0},//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP, 0},//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP, 0},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, test_win_edit_range_f6_cb },//f6
};
/**
  * @brief  编辑IR自动换档时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_auto_shift_menu_key_init_info[] =
{
    {"", F_KEY_ON   , KEY_F1 & _KEY_UP, edit_auto_shift_f1_cb },//f1
    {"", F_KEY_OFF  , KEY_F2 & _KEY_UP, edit_auto_shift_f2_cb },//f2
    {"", F_KEY_NULL , KEY_F3 & _KEY_UP, edit_auto_shift_f3_cb },//f3
    {"", F_KEY_NULL , KEY_F4 & _KEY_UP, edit_auto_shift_f4_cb },//f4
    {"", F_KEY_NULL , KEY_F5 & _KEY_UP, edit_auto_shift_f5_cb },//f5
    {"", F_KEY_BACK , KEY_F6 & _KEY_UP, edit_auto_shift_f6_cb },//f6
};

/**
  * @brief  编辑电压时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	test_win_edit_vol_menu_key_init_info[] =
{
    {""     , F_KEY_DEL     , KEY_F1 & _KEY_UP, edit_test_win_vol_f1_cb },//f1
    {""     , F_KEY_CLEAR   , KEY_F2 & _KEY_UP, edit_test_win_vol_f2_cb },//f2
    {"X10"  , F_KEY_CUSTOM  , KEY_F3 & _KEY_UP, edit_test_win_vol_f3_cb },//f3
    {"X100" , F_KEY_CUSTOM  , KEY_F4 & _KEY_UP, edit_test_win_vol_f4_cb },//f4
    {"X1000", F_KEY_CUSTOM  , KEY_F5 & _KEY_UP, edit_test_win_vol_f5_cb },//f5
    {""     , F_KEY_BACK    , KEY_F6 & _KEY_UP, edit_test_win_vol_f6_cb },//f6
};

/**
  * @brief  编辑上下限时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	test_win_edit_upper_menu_key_init_info[] =
{
    {"", F_KEY_DEL		    , KEY_F1 & _KEY_UP, edit_test_win_upper_f1_cb },//f1
    {"", F_KEY_CLEAR        , KEY_F2 & _KEY_UP, edit_test_win_upper_f2_cb },//f2
    {"X10"  , F_KEY_CUSTOM  , KEY_F3 & _KEY_UP, edit_test_win_upper_f3_cb },//f3
    {"X100" , F_KEY_CUSTOM  , KEY_F4 & _KEY_UP, edit_test_win_upper_f4_cb },//f4
    {"X1000", F_KEY_CUSTOM  , KEY_F5 & _KEY_UP, edit_test_win_upper_f5_cb },//f5
    {"", F_KEY_BACK		    , KEY_F6 & _KEY_UP, edit_test_win_upper_f6_cb },//f6
};

/**
  * @brief  测试窗口编辑参数时使用的功能键的初始化信息数组
  */
static CONFIG_FUNCTION_KEY_INFO_T 	test_win_edit_par_sys_key_init_pool[]=
{
	{KEY_UP		, test_win_direct_key_up_cb      },
	{KEY_DOWN	, test_win_direct_key_down_cb    },
	{KEY_LEFT	, test_win_direct_key_left_cb    },
	{KEY_RIGHT	, test_win_direct_key_right_cb   },
	{KEY_ENTER	, test_win_sys_key_enter_cb      },
    
	{CODE_RIGH	, 0 },
	{CODE_LEFT	, 0 },
	{KEY_START	    , test_win_sys_start_key_fun_cb       },
	{KEY_STOP	    , test_win_sys_stop_key_fun_cb       },
};
/**
  * @brief  测试窗口编辑数值时使用的功能键的初始化信息数组
  */
static CONFIG_FUNCTION_KEY_INFO_T 	test_win_edit_num_sys_key_init_pool[]=
{
	{KEY_UP		, test_win_edit_num_direct_key_up_cb      },
	{KEY_DOWN	, test_win_edit_num_direct_key_down_cb    },
	{KEY_LEFT	, test_win_edit_num_direct_key_left_cb    },
	{KEY_RIGHT	, test_win_edit_num_direct_key_right_cb   },
	{KEY_ENTER	, test_win_edit_num_sys_key_enter_cb      },
    
	{CODE_RIGH	, test_win_edit_num_direct_key_up_cb      },
	{CODE_LEFT	, test_win_edit_num_direct_key_down_cb    },
};
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  更新当前编辑的电流档位对应的菜单键的颜色
  * @param  无
  * @retval 无
  */
static void update_cur_range_cur_value_menu_key_color(void)
{
    uint32_t key_value = 0;
    int32_t i = 0;
    GUI_COLOR color;
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    uint32_t size = 0;
    uint8_t **p_range;
    uint8_t *cur_range;
    uint8_t mode = 0;
    uint32_t key_value_buf[] ={
        KEY_F1 & _KEY_UP,
        KEY_F2 & _KEY_UP,
        KEY_F3 & _KEY_UP,
        KEY_F4 & _KEY_UP,
        KEY_F5 & _KEY_UP,
        KEY_F6 & _KEY_UP,
    };
    
    mode = g_cur_step->one_step.com.mode;
    get_range_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    p_range = tmp_ele.resource.table;
    size = tmp_ele.resource.size;
    
    switch(mode)
    {
        case ACW:
            cur_range = ac_gear[g_cur_step->one_step.acw.range].name;
            break;
        case DCW:
            cur_range = dc_gear[g_cur_step->one_step.dcw.range].name;
            break;
        default:
            return;
    }
    
    for(i = 0; i < size; i++)
    {
        key_value = key_value_buf[i];
        
        if(0 == strcmp((const char*)cur_range, (const char*)p_range[i]))
        {
            color = CUR_VALUE_AT_KEY_COLOR;
        }
        else
        {
            color = NO_CUR_VALUE_AT_KEY_COLOR;
        }
        
        change_menu_key_font_color(key_value, color);
    }
}
/**
  * @brief  更新当前编辑的测试模式对应的菜单键的颜色
  * @param  无
  * @retval 无
  */
static void update_test_mode_cur_value_menu_key_color(void)
{
    uint32_t key_value = 0;
    int32_t i = 0;
    GUI_COLOR color;
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    uint32_t size = 0;
    uint8_t **p_mode;
    const uint8_t *cur_mode;
    uint32_t key_value_buf[] ={
        KEY_F1 & _KEY_UP,
        KEY_F2 & _KEY_UP,
        KEY_F3 & _KEY_UP,
        KEY_F4 & _KEY_UP,
        KEY_F5 & _KEY_UP,
        KEY_F6 & _KEY_UP,
    };
    
    get_mode_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    cur_mode = mode_pool[g_cur_step->one_step.com.mode];
    p_mode = tmp_ele.resource.table;
    size = tmp_ele.resource.size;
    
    for(i = 0; i < size; i++)
    {
        key_value = key_value_buf[i];
        
        if(0 == strcmp((const char*)cur_mode, (const char*)p_mode[i]))
        {
            color = CUR_VALUE_AT_KEY_COLOR;
        }
        else
        {
            color = NO_CUR_VALUE_AT_KEY_COLOR;
        }
        
        change_menu_key_font_color(key_value, color);
    }
}
/**
  * @brief  选择测试模式时使用的按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_select_test_mode_key_cb(KEY_MESSAGE *key_msg)
{
    int data = key_msg->custom_data;
    uint8_t mode = 0;
    
    mode = g_cur_step->one_step.com.mode;
    
    if(data != mode)
    {
        if(g_cur_file->work_mode == G_MODE)
        {
            clear_cur_group_all_test_step();
            insert_step(0, data);//插入一步
            save_group_info(g_cur_file->num);//保存新建文件的记忆组信息
            
            load_steps_to_list(1, 1);
            g_cur_step = get_g_cur_step();
        }
        else
        {
            g_cur_step->one_step.com.mode = data;//更新当前步的测试模式
            init_mode(g_cur_step);//初始化参数
            save_setting_step();//保存正在设置的步骤参数
        }
        
        load_data();
        update_test_win_text_ele_text(g_cur_win);
        update_test_mode_cur_value_menu_key_color();
        update_group_inf(g_cur_win);
    }
}
/**
  * @brief  选择电流档位时使用的按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_select_cur_range_key_cb(KEY_MESSAGE *key_msg)
{
    int data = key_msg->custom_data;
    uint8_t mode = 0;
    uint8_t *range;
    uint8_t range_bk;
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    uint32_t value;
    
    
    mode = g_cur_step->one_step.com.mode;
   
    switch(mode)
    {
        case ACW:
            range = &g_cur_step->one_step.acw.range;
            break;
        case DCW:
            range = &g_cur_step->one_step.dcw.range;
            break;
        default:
            return;
    }
    
    range_bk = *range;
    
    if(data != range_bk)
    {
        *range = data;//更新当前步的电流档位
        
        get_range_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
        
        if(err != CS_ERR_NONE)
        {
            return;
        }
        
        value = data;
        tmp_ele.range.check_value_validity(&tmp_ele, &value);
        update_cur_range_cur_value_menu_key_color();
        save_setting_step();//保存正在设置的步骤参数
        update_test_win_text_ele_text(g_cur_win);
    }
}
/**
  * @brief  测试窗口系统功能键信息初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_win_sys_key_init(WM_HMEM hWin)
{
    CONFIG_FUNCTION_KEY_INFO_T *pool = test_win_edit_par_sys_key_init_pool;
    uint32_t size = ARRAY_SIZE(test_win_edit_par_sys_key_init_pool);
    
    register_system_key_fun(pool, size, hWin);
}
/**
  * @brief  测试窗口编辑数字使用的系统功能键信息初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_win_edit_num_sys_key_init(WM_HMEM hWin)
{
    CONFIG_FUNCTION_KEY_INFO_T *pool = test_win_edit_num_sys_key_init_pool;
    uint32_t size = ARRAY_SIZE(test_win_edit_num_sys_key_init_pool);
    
    register_system_key_fun(pool, size, hWin);
}
/**
  * @brief  编辑测试模式时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_mode_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = test_win_edit_mode_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(test_win_edit_mode_menu_key_init_info);
    CUSTOM_MENU_KEY_INF *cus_inf = test_win_mode_inf_pool;
    uint16_t cus_size = ARRAY_SIZE(test_win_mode_inf_pool);
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    
    get_mode_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_menu_key_custom_inf(cus_inf, cus_size, &tmp_ele, info, size);
        init_menu_key_info(info, size, hWin);
        update_test_mode_cur_value_menu_key_color();
        update_range_name(tmp_ele.name[SYS_LANGUAGE]);
        update_ele_range_text(&tmp_ele);
    }
}

/**
  * @brief  编辑电流档位时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_range_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = test_win_edit_range_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(test_win_edit_range_menu_key_init_info);
    CUSTOM_MENU_KEY_INF *cus_inf = test_win_cur_range_inf_pool;
    uint16_t cus_size = ARRAY_SIZE(test_win_cur_range_inf_pool);
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    
    get_range_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err == CS_ERR_NONE)
    {
        init_menu_key_custom_inf(cus_inf, cus_size, &tmp_ele, info, size);
        init_menu_key_info(info, size, hWin);
        update_cur_range_cur_value_menu_key_color();
        update_range_name(tmp_ele.name[SYS_LANGUAGE]);
        update_ele_range_text(&tmp_ele);
    }
}
/**
  * @brief  编辑IR自动换档时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_auto_shift_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_auto_shift_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_auto_shift_menu_key_init_info);
    EDIT_ELE_T *ele;
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    uint32_t key_value = 0;
    int32_t i = 0;
    
    get_auto_shift_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    for(i = 0; i < size; i++)
    {
        if(g_cur_step->one_step.ir.auto_shift)
        {
            if(info[i].index == F_KEY_ON)
            {
                key_value = info[i].fun_key.key_value;
            }
        }
        else
        {
            if(info[i].index == F_KEY_OFF)
            {
                key_value = info[i].fun_key.key_value;
            }
        }
    }
    
    if(err == CS_ERR_NONE)
    {
        init_menu_key_info(info, size, hWin);
        change_menu_key_font_color(key_value, CUR_VALUE_AT_KEY_COLOR);
        update_range_name(ele->name[SYS_LANGUAGE]);
        update_ele_range_text(&tmp_ele);
    }
}
/**
  * @brief  编辑电压时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_vol_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = test_win_edit_vol_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(test_win_edit_vol_menu_key_init_info);
    EDIT_ELE_T tmp_ele;
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    data_mul_power.x_n = 1;
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    get_vol_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele.key_inf.fun_sys_key = edit_ele->key_inf.fun_sys_key;
        memcpy(edit_ele, &tmp_ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_vol_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
        update_range_name(tmp_ele.name[SYS_LANGUAGE]);
    }
}
/**
  * @brief  编辑测试时间时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_test_time_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = test_win_edit_vol_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(test_win_edit_vol_menu_key_init_info);
    EDIT_ELE_T tmp_ele;
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    data_mul_power.x_n = 1;
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    get_test_time_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele.key_inf.fun_sys_key = edit_ele->key_inf.fun_sys_key;
        memcpy(edit_ele, &tmp_ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_test_time_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
        update_range_name(tmp_ele.name[SYS_LANGUAGE]);
    }
}
/**
  * @brief  编辑电流上限时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_cur_upper_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = test_win_edit_upper_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(test_win_edit_upper_menu_key_init_info);
    EDIT_ELE_T *edit_ele = NULL;
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    data_mul_power.x_n = 1;
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    get_upper_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele.key_inf.fun_sys_key = edit_ele->key_inf.fun_sys_key;
        memcpy(edit_ele, &tmp_ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_cur_upper_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
        update_range_name(tmp_ele.name[SYS_LANGUAGE]);
    }
}

/**
  * @brief  编辑电流下限时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void test_win_edit_cur_lower_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = test_win_edit_upper_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(test_win_edit_upper_menu_key_init_info);
    EDIT_ELE_T *edit_ele = NULL;
    EDIT_ELE_T tmp_ele;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    data_mul_power.x_n = 1;
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    get_lower_edit_ele_inf(&g_cur_step->one_step, &tmp_ele, &err);
    
    if(err == CS_ERR_NONE)
    {
        tmp_ele.key_inf.fun_sys_key = edit_ele->key_inf.fun_sys_key;
        memcpy(edit_ele, &tmp_ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_cur_lower_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
        update_range_name(tmp_ele.name[SYS_LANGUAGE]);
    }
}
/**
  * @brief  根据提供的文本对象信息来初始化编辑对象的位置尺寸信息
  * @param  [in] text_ele 文本对象
  * @param  [in] edit_ele 编辑对象
  * @retval 无
  */
static void init_test_win_edit_ele_pos_size(TEXT_ELE_T * text_ele, EDIT_ELE_T *edit_ele)
{
    
    edit_ele->dis.x = text_ele->dis_info.base_x + text_ele->dis_info.pos_size.x;
    edit_ele->dis.y = text_ele->dis_info.base_y + text_ele->dis_info.pos_size.y;
    edit_ele->dis.edit.width = text_ele->dis_info.pos_size.width;
    edit_ele->dis.edit.height = text_ele->dis_info.pos_size.height;
    edit_ele->dis.name.width = 1;
    edit_ele->dis.name.height = edit_ele->dis.edit.height;
    edit_ele->dis.unit.width = 1;
    edit_ele->dis.unit.height = edit_ele->dis.edit.height;
    edit_ele->dis.edit.back_color = text_ele->dis_info.back_color;
    edit_ele->dis.edit.font_color = GUI_BLACK;
    edit_ele->dis.edit.font = text_ele->dis_info.font;
}
/**
  * @brief  初始化输出电压编辑控件的位置尺寸信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_vol_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *edit_pool;
    uint32_t edit_pool_size;
    TEXT_ELE_T *text_pool;
    uint32_t text_pool_size;
    TEXT_ELE_T * text_ele = NULL;
    
    edit_pool = g_cur_win->edit.pool;
    edit_pool_size = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(edit_pool, edit_pool_size, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    text_pool = g_cur_win->text.pool;
    text_pool_size = g_cur_win->text.pool_size;
    
    text_ele = get_text_ele_inf(text_pool, text_pool_size, TEST_UI_ROAD01_VOLTAGE, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    init_test_win_edit_ele_pos_size(text_ele, edit_ele);
}
/**
  * @brief  初始化测试时间编辑控件的位置尺寸信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_test_time_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *edit_pool;
    uint32_t edit_pool_size;
    TEXT_ELE_T *text_pool;
    uint32_t text_pool_size;
    TEXT_ELE_T * text_ele = NULL;
    
    edit_pool = g_cur_win->edit.pool;
    edit_pool_size = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(edit_pool, edit_pool_size, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    text_pool = g_cur_win->text.pool;
    text_pool_size = g_cur_win->text.pool_size;
    
    text_ele = get_text_ele_inf(text_pool, text_pool_size, TEST_UI_ROAD01_TIME, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    init_test_win_edit_ele_pos_size(text_ele, edit_ele);
}
/**
  * @brief  初始化电流上限编辑控件的位置尺寸信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_cur_upper_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *edit_pool;
    uint32_t edit_pool_size;
    TEXT_ELE_T *text_pool;
    uint32_t text_pool_size;
    TEXT_ELE_T * text_ele = NULL;
    
    edit_pool = g_cur_win->edit.pool;
    edit_pool_size = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(edit_pool, edit_pool_size, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    text_pool = g_cur_win->text.pool;
    text_pool_size = g_cur_win->text.pool_size;
    
    text_ele = get_text_ele_inf(text_pool, text_pool_size, TEST_UI_ROAD01_UPPER, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    init_test_win_edit_ele_pos_size(text_ele, edit_ele);
}
/**
  * @brief  初始化电流下限编辑控件的位置尺寸信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_cur_lower_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *edit_pool;
    uint32_t edit_pool_size;
    TEXT_ELE_T *text_pool;
    uint32_t text_pool_size;
    TEXT_ELE_T * text_ele = NULL;
    
    edit_pool = g_cur_win->edit.pool;
    edit_pool_size = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(edit_pool, edit_pool_size, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    text_pool = g_cur_win->text.pool;
    text_pool_size = g_cur_win->text.pool_size;
    
    text_ele = get_text_ele_inf(text_pool, text_pool_size, TEST_UI_ROAD01_UPPER, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    init_test_win_edit_ele_pos_size(text_ele, edit_ele);
}
/**
  * @brief  初始化并创建测试窗口中的编辑对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_test_win_edit_ele(MYUSER_WINDOW_T *win)
{
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    init_window_edit_ele(win);//初始化创建编辑对象
    
    g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
    select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
}
/**
  * @brief  测试窗口向上键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    NODE_STEP *node;
    
    if(g_cur_step->one_step.com.step < g_cur_file->total)
    {
        load_steps_to_list(g_cur_step->one_step.com.step + 1, 1);
        node = get_g_cur_step();
        
        if(NULL != node)
        {
            g_cur_step = node;//更新当前步
            update_test_win_text_ele_text(g_cur_win);//更新界面显示文本对象
            update_group_inf(g_cur_win);//更新记忆组信息
        }
    }
}
/**
  * @brief  测试窗口向下键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    NODE_STEP *node;
    
    if(g_cur_step->one_step.com.step > 1)
    {
        load_steps_to_list(g_cur_step->one_step.com.step - 1, 1);
        node = get_g_cur_step();
        
        if(NULL != node)
        {
            g_cur_step = node;//更新当前步
            update_test_win_text_ele_text(g_cur_win);//更新界面显示文本对象
            update_group_inf(g_cur_win);//更新记忆组信息
        }
    }
}
/**
  * @brief  测试窗口向左键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  测试窗口向右键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  测试窗口ENTER键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_sys_key_enter_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口启动键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_sys_start_key_fun_cb(KEY_MESSAGE *key_msg)
{
    test_status = TEST_START;
}
/**
  * @brief  测试窗口复位键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_sys_stop_key_fun_cb(KEY_MESSAGE *key_msg)
{
    test_reset_fun();
}

static void test_win_edit_num_direct_key_up_cb   (KEY_MESSAGE *key_msg)
{
    uint32_t value = 0;
    
    if(data_mul_power.x_n == 0)
    {
        data_mul_power.x_n = 1;
    }
    
    value = get_edit_ele_value(g_cur_edit_ele, NULL);
    value += data_mul_power.x_n;
    
    if(value > g_cur_edit_ele->range.high)
    {
        value = g_cur_edit_ele->range.high;
    }
    else if(value < g_cur_edit_ele->range.low)
    {
        value = g_cur_edit_ele->range.low;
    }
    
    set_edit_num_value(g_cur_edit_ele, value);
}
static void test_win_edit_num_direct_key_down_cb (KEY_MESSAGE *key_msg)
{
    uint32_t value = 0;
    
    if(data_mul_power.x_n == 0)
    {
        data_mul_power.x_n = 1;
    }
    
    value = get_edit_ele_value(g_cur_edit_ele, NULL);
    
    if(value >= data_mul_power.x_n)
    {
        value -= data_mul_power.x_n;
    }
    else
    {
        value = 0;
    }
    
    if(value > g_cur_edit_ele->range.high)
    {
        value = g_cur_edit_ele->range.high;
    }
    else if(value < g_cur_edit_ele->range.low)
    {
        value = g_cur_edit_ele->range.low;
    }
    
    set_edit_num_value(g_cur_edit_ele, value);
}
static void test_win_edit_num_direct_key_left_cb (KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
static void test_win_edit_num_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}
static void test_win_edit_num_sys_key_enter_cb   (KEY_MESSAGE *key_msg)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    save_setting_step();
    update_test_win_text_ele_text(g_cur_win);
    delete_win_edit_ele(g_cur_win);
    set_cur_edit_ele(NULL);//将当前编辑对象置为空
    update_key_menu(key_msg->user_data);
    test_win_check_cur_step_changed_send_to_slave();
}

/**
  * @brief  更新当前步CRC值
  * @param  无
  * @retval 无
  */
static void test_win_update_cur_step_crc(void)
{
    g_cur_step_crc =  stm32_crc32_byte((uint8_t*)g_cur_step, sizeof(NODE_STEP));
    g_cur_step_crc_bk = g_cur_step_crc;
}
/**
  * @brief  检查当前的编辑步的参数是否发改变，如果改变就把当前步发送给从机模块
  * @param  无
  * @retval 无
  */
static void test_win_check_cur_step_changed_send_to_slave(void)
{
    g_cur_step_crc =  stm32_crc32_byte((uint8_t*)g_cur_step, sizeof(NODE_STEP));
    
    if(g_cur_step_crc != g_cur_step_crc_bk)
    {
        g_cur_step_crc_bk = g_cur_step_crc;
        send_cmd_to_all_module((void*)g_cur_step, sizeof(NODE_STEP), send_edit_step);
    }
}
/**
  * @brief  测试界面编辑测试模式时菜单键F6的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_edit_mode_f6_cb(KEY_MESSAGE *key_msg)
{
    update_key_menu(key_msg->user_data);
}
/**
  * @brief  设置测试窗口编辑电流档位时使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_edit_range_f6_cb(KEY_MESSAGE *key_msg)
{
    update_key_menu(key_msg->user_data);
}

/**
  * @brief  设置测试窗口编辑上下限时使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_upper_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  设置测试窗口编辑上下限时使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_upper_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(g_cur_edit_ele->dis.edit.handle);
}
/**
  * @brief  设置测试窗口编辑上下限时使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_upper_f3_cb(KEY_MESSAGE *key_msg)
{
    update_data_mul_power(key_msg->key_value, 10);
}
/**
  * @brief  设置测试窗口编辑上下限时使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_upper_f4_cb(KEY_MESSAGE *key_msg)
{
    update_data_mul_power(key_msg->key_value, 100);
}
/**
  * @brief  设置测试窗口编辑上下限时使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_upper_f5_cb(KEY_MESSAGE *key_msg)
{
    update_data_mul_power(key_msg->key_value, 1000);
}
/**
  * @brief  设置测试窗口编辑上下限时使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_upper_f6_cb(KEY_MESSAGE *key_msg)
{
    update_key_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
}

/**
  * @brief  设置测试窗口编辑输出电压时使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_vol_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  设置测试窗口编辑输出电压时使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_vol_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(g_cur_edit_ele->dis.edit.handle);
}
static void update_data_mul_power(uint32_t key_value, uint32_t mul_power)
{
    static uint32_t key_value_bk;
    
    change_menu_key_font_color(key_value_bk, NO_CUR_VALUE_AT_KEY_COLOR);
    key_value_bk = key_value;
    
    if(data_mul_power.x_n != mul_power)
    {
        data_mul_power.x_n = mul_power;
        change_menu_key_font_color(key_value, CUR_VALUE_AT_KEY_COLOR);
    }
    else
    {
        change_menu_key_font_color(key_value, NO_CUR_VALUE_AT_KEY_COLOR);
        data_mul_power.x_n = 1;
    }
}
/**
  * @brief  设置测试窗口编辑输出电压时使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_vol_f3_cb(KEY_MESSAGE *key_msg)
{
    update_data_mul_power(key_msg->key_value, 10);
}
/**
  * @brief  设置测试窗口编辑输出电压时使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_vol_f4_cb(KEY_MESSAGE *key_msg)
{
    update_data_mul_power(key_msg->key_value, 100);
}
/**
  * @brief  设置测试窗口编辑输出电压时使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_vol_f5_cb(KEY_MESSAGE *key_msg)
{
    update_data_mul_power(key_msg->key_value, 1000);
}
/**
  * @brief  设置测试窗口编辑输出电压时使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_test_win_vol_f6_cb(KEY_MESSAGE *key_msg)
{
    update_key_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
}

/**
  * @brief  更新当前编辑的IR自动换档对应的菜单键的文本颜色
  * @param  无
  * @retval 无
  */
static void update_auto_shift_cur_value_menu_key_color(void)
{
    MENU_KEY_INFO_T * info = edit_auto_shift_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_auto_shift_menu_key_init_info);
    uint32_t key_value = 0;
    int32_t i = 0;
    GUI_COLOR color_1;
    GUI_COLOR color_2;
    
    for(i = 0; i < size; i++)
    {
        if(info[i].index == F_KEY_ON)
        {
            key_value = info[i].fun_key.key_value;
            
            if(g_cur_step->one_step.ir.auto_shift)
            {
                color_1 = CUR_VALUE_AT_KEY_COLOR;
            }
            else
            {
                color_1 = NO_CUR_VALUE_AT_KEY_COLOR;
            }
            
            change_menu_key_font_color(key_value, color_1);
        }
        
        if(info[i].index == F_KEY_OFF)
        {
            key_value = info[i].fun_key.key_value;
            
            if(g_cur_step->one_step.ir.auto_shift)
            {
                color_2 = NO_CUR_VALUE_AT_KEY_COLOR;
            }
            else
            {
                color_2 = CUR_VALUE_AT_KEY_COLOR;
            }
            
            change_menu_key_font_color(key_value, color_2);
        }
    }
}
/**
  * @brief  更新当前编辑的IR自动换档时使用的菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_auto_shift_f1_cb(KEY_MESSAGE *key_msg)
{
    g_cur_step->one_step.ir.auto_shift = SW_ON;
    save_setting_step();
    update_auto_shift_cur_value_menu_key_color();
}
/**
  * @brief  更新当前编辑的IR自动换档时使用的菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_auto_shift_f2_cb(KEY_MESSAGE *key_msg)
{
    g_cur_step->one_step.ir.auto_shift = SW_OFF;
    save_setting_step();
    update_auto_shift_cur_value_menu_key_color();
}
/**
  * @brief  更新当前编辑的IR自动换档时使用的菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_auto_shift_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  更新当前编辑的IR自动换档时使用的菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_auto_shift_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  更新当前编辑的IR自动换档时使用的菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_auto_shift_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  更新当前编辑的IR自动换档时使用的菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_auto_shift_f6_cb(KEY_MESSAGE *key_msg)
{
    update_key_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
}
/**
  * @brief  测试窗口菜单键F0回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f0_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f1_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(create_file_win, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        create_file_win(key_msg->user_data);
    }
}
/**
  * @brief  测试窗口菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f2_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    test_win_update_cur_step_crc();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(update_key_menu, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        update_key_menu(g_cur_win->handle);
    }
}
/**
  * @brief  测试窗口菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f3_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = get_key_lock_flag();
    
    /* 加锁 */
    if(flag)
    {
        init_back_up_will_enter_win_inf(create_result_win, g_cur_win->handle);
    }
    /* 未加锁 */
    else
    {
        create_result_win(g_cur_win->handle);
    }
}
/**
  * @brief  测试窗口菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f4_cb(KEY_MESSAGE *key_msg)
{
    test_status = TEST_START;
}
/**
  * @brief  测试窗口菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第1页菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f1_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_mode_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第1页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f2_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_vol_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第1页菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f3_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_range_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第1页菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f4_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_test_time_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第1页菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f5_1_cb(KEY_MESSAGE *key_msg)
{
    select_set_acw_par_menu_2(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第1页菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f6_1_cb(KEY_MESSAGE *key_msg)
{
    update_test_win_menu_key_inf(key_msg->user_data);
    clear_range_text_ele(g_cur_win);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第2页菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f1_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_upper_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第2页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f2_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_lower_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第2页菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f3_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第2页菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f4_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第2页菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f5_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑ACW/DCW参数时使用的第2页菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_acw_par_f6_2_cb(KEY_MESSAGE *key_msg)
{
    select_set_acw_par_menu_1(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
    clear_range_text_ele(g_cur_win);
}

/**
  * @brief  测试窗口编辑IR参数时使用的第1页菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f1_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_mode_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第1页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f2_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_vol_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第1页菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f3_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_auto_shift_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第1页菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f4_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_test_time_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第1页菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f5_1_cb(KEY_MESSAGE *key_msg)
{
    change_key_set_ir_par_menu(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第1页菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f6_1_cb(KEY_MESSAGE *key_msg)
{
    update_test_win_menu_key_inf(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
    clear_range_text_ele(g_cur_win);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f1_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_lower_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f2_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_upper_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f3_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f4_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f5_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f6_2_cb(KEY_MESSAGE *key_msg)
{
    change_key_set_ir_par_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
    clear_range_text_ele(g_cur_win);
}

/**
  * @brief  测试窗口编辑GR参数时使用的第1页菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f1_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_mode_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第1页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f2_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_vol_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第1页菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f3_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_test_time_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第1页菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f4_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_upper_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第1页菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f5_1_cb(KEY_MESSAGE *key_msg)
{
    change_key_set_gr_par_menu(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第1页菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f6_1_cb(KEY_MESSAGE *key_msg)
{
    update_test_win_menu_key_inf(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
    clear_range_text_ele(g_cur_win);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第2页菜单键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f1_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_lower_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑GR参数时使用的第2页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f2_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑GR参数时使用的第2页菜单键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f3_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑GR参数时使用的第2页菜单键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f4_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑GR参数时使用的第2页菜单键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f5_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  测试窗口编辑GR参数时使用的第2页菜单键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_gr_par_f6_2_cb(KEY_MESSAGE *key_msg)
{
    change_key_set_gr_par_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
    clear_range_text_ele(g_cur_win);
}
/**
  * @brief  切换设置ACW参数时的菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void change_key_set_acw_par_menu(int hWin)
{
    if(cur_at_menu_page_flag == 0)
    {
        select_set_acw_par_menu_1(hWin);
    }
    else
    {
        select_set_acw_par_menu_2(hWin);
    }
}
/**
  * @brief  切换设置IR参数时的菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void change_key_set_ir_par_menu(int hWin)
{
    if(cur_at_menu_page_flag == 0)
    {
        select_set_ir_par_menu_2(hWin);
    }
    else
    {
        select_set_ir_par_menu_1(hWin);
    }
}
/**
  * @brief  更新设置IR参数时的菜单键信息，根据当前的菜单页来更新
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_set_ir_par_menu(int hWin)
{
    if(cur_at_menu_page_flag == 0)
    {
        select_set_ir_par_menu_1(hWin);
    }
    else
    {
        select_set_ir_par_menu_2(hWin);
    }
}
/**
  * @brief  切换设置GR参数时的菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void change_key_set_gr_par_menu(int hWin)
{
    if(cur_at_menu_page_flag == 0)
    {
        select_set_gr_par_menu_2(hWin);
    }
    else
    {
        select_set_gr_par_menu_1(hWin);
    }
}
/**
  * @brief  更新设置GR参数时的菜单键信息，根据当前的菜单页来更新
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_set_gr_par_menu(int hWin)
{
    if(cur_at_menu_page_flag == 0)
    {
        select_set_gr_par_menu_1(hWin);
    }
    else
    {
        select_set_gr_par_menu_2(hWin);
    }
}
/**
  * @brief  选择显示设置ACW参数的第一页菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void select_set_acw_par_menu_1(int hWin)
{
    cur_at_menu_page_flag = 0;
    init_menu_key_info(test_ui_acw_menu_pool[0], ARRAY_SIZE(test_ui_acw_menu_pool[0]), hWin);
}
/**
  * @brief  选择显示设置ACW参数的第二页菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void select_set_acw_par_menu_2(int hWin)
{
    cur_at_menu_page_flag = 1;
    init_menu_key_info(test_ui_acw_menu_pool[1], ARRAY_SIZE(test_ui_acw_menu_pool[1]), hWin);
}
/**
  * @brief  选择显示设置IR参数的第一页菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void select_set_ir_par_menu_1(int hWin)
{
    cur_at_menu_page_flag = 0;
    init_menu_key_info(test_ui_ir_menu_pool[0], ARRAY_SIZE(test_ui_ir_menu_pool[0]), hWin);
}
/**
  * @brief  选择显示设置IR参数的第二页菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void select_set_ir_par_menu_2(int hWin)
{
    cur_at_menu_page_flag = 1;
    init_menu_key_info(test_ui_ir_menu_pool[1], ARRAY_SIZE(test_ui_ir_menu_pool[1]), hWin);
}
/**
  * @brief  选择显示设置GR参数的第一页菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void select_set_gr_par_menu_1(int hWin)
{
    cur_at_menu_page_flag = 0;
    init_menu_key_info(test_ui_gr_menu_pool[0], ARRAY_SIZE(test_ui_gr_menu_pool[0]), hWin);
}
/**
  * @brief  选择显示设置GR参数的第二页菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void select_set_gr_par_menu_2(int hWin)
{
    cur_at_menu_page_flag = 1;
    init_menu_key_info(test_ui_gr_menu_pool[1], ARRAY_SIZE(test_ui_gr_menu_pool[1]), hWin);
}
/**
  * @brief  改变菜单按键信息
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void update_key_menu(int hWin)
{
    clear_range_text_ele(g_cur_win);
    
    switch(g_cur_step->one_step.com.mode)
    {
        case ACW:
            change_key_set_acw_par_menu(hWin);
            break;
        case DCW:
            change_key_set_acw_par_menu(hWin);
            break;
        case IR:
            update_key_set_ir_par_menu(hWin);
            break;
        case GR:
            update_key_set_gr_par_menu(hWin);
            break;
    }
}

/**
  * @brief  更新菜单键信息
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void update_test_win_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(test_ui_menu_key_pool, ARRAY_SIZE(test_ui_menu_key_pool), hWin);
    test_win_win_sys_key_init(hWin);
}

/**
  * @brief  设置测试界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_test_windows_handle(WM_HWIN hWin)
{
	test_windows.handle = hWin;
}

/**
  * @brief  根据屏幕尺寸初始化界面的文本对象位置尺寸信息
  * @param  无
  * @retval 无
  */
static void init_test_ui_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
    case SCREEN_4_3INCH:
        break;
    case SCREEN_6_5INCH:
        break;
    default:
    case SCREEN_7INCH:
        _7_init_test_ui_layout1_text_ele_pos(test_ui_ele_pool);
        break;
    }
}
/**
  * @brief  转换输出电压/电流为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_output_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t unit = 0;
    uint8_t lon = 0;
    uint8_t format = 0;
    
    mode = g_cur_step->one_step.com.mode;
    
    switch(mode)
    {
        /* 只有ACW有真实电流 */
        case ACW:
        {
            dec = 3;
            lon = 5;
            format = 100 + 10 * lon + dec;
            
            mysprintf(buf, NULL, format, g_cur_step->one_step.acw.output_vol);
            break;
        }
        case DCW:
        {
            dec = 3;
            lon = 5;
            format = 100 + 10 * lon + dec;
            
            mysprintf(buf, NULL, format, g_cur_step->one_step.dcw.output_vol);
            break;
        }
        case IR:
        {
            dec = 3;
            lon = 5;
            format = 100 + 10 * lon + dec;
            
            mysprintf(buf, NULL, format, g_cur_step->one_step.ir.output_vol);
            break;
        }
        case GR:
        {
            dec = 2;
            lon = 5;
            format = 100 + 10 * lon + dec;
            
            mysprintf(buf, unit_pool[unit], format, g_cur_step->one_step.gr.output_cur);
            break;
        }
        default:
        {
            buf[0] = 0;
            break;
        }
    }
}
/**
  * @brief  转换各模式上限值为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_upper_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t lon = 0;
    uint8_t format = 0;
    uint8_t range = 0;
    uint32_t value = 0;
    
    mode = g_cur_step->one_step.com.mode;
    
    switch(mode)
    {
        /* 只有ACW有真实电流 */
        case ACW:
        {
            range = g_cur_step->one_step.acw.range;
            dec = ac_gear[range].dec;
            lon = ac_gear[range].lon;
            format = 100 + 10 * lon + dec;
            value = g_cur_step->one_step.acw.upper_limit;
            break;
        }
        case DCW:
        {
            range = g_cur_step->one_step.dcw.range;
            dec = dc_gear[range].dec;
            lon = dc_gear[range].lon;
            format = 100 + 10 * lon + dec;
            
            value = g_cur_step->one_step.dcw.upper_limit;
            break;
        }
        case IR:
        {
            cur_range = IR_10MOHM;
            dec = ir_gear[cur_range].dec;
            lon = ir_gear[cur_range].lon;
            format = 100 + 10 * lon + dec;
            
            value = g_cur_step->one_step.ir.lower_limit * ten_power(dec);
            break;
        }
        case GR:
        {
            dec = 1;
            lon = 5;
            format = 100 + 10 * lon + dec;
            
            value = g_cur_step->one_step.gr.upper_limit;
            break;
        }
        default:
        {
            buf[0] = 0;
            return;
        }
    }
    
    mysprintf(buf, NULL, format, value);
}
/**
  * @brief  转换ACW真实电流值为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_real_cur_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t lon = 0;
    uint8_t format = 0;
    uint8_t range = 0;
    
    mode = g_cur_step->one_step.com.mode;
    
    switch(mode)
    {
        /* 只有ACW有真实电流 */
        case ACW:
        {
            range = g_cur_step->one_step.acw.range;
            dec = ac_gear[range].dec;
            lon = ac_gear[range].lon;
            format = 100 + 10 * lon + dec;
            
            if(g_cur_step->one_step.acw.real_cur == 0)
            {
                buf[0] = 0;
            }
            else
            {
                mysprintf(buf, NULL, format, g_cur_step->one_step.acw.real_cur);
            }
            break;
        }
        default:
        {
            buf[0] = 0;
            break;
        }
    }
}
/**
  * @brief  转换测试时间为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_test_time_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t lon = 0;
    uint8_t format = 0;
    uint16_t time = 0;
    
    mode = g_cur_step->one_step.com.mode;
    
    dec = 1;
    lon = 5;
    format = 100 + 10 * lon + dec;
    
    switch(mode)
    {
        /* 只有ACW有真实电流 */
        case ACW:
        {
            time = g_cur_step->one_step.acw.test_time;
            break;
        }
        case DCW:
        {
            time = g_cur_step->one_step.dcw.test_time;
            break;
        }
        case IR:
        {
            time = g_cur_step->one_step.ir.test_time;
            break;
        }
        case GR:
        {
            time = g_cur_step->one_step.gr.test_time;
            break;
        }
        default:
        {
            buf[0] = 0;
            return;
        }
    }
    
    mysprintf(buf, NULL, format, time);
}
void update_one_road_test_inf(MYUSER_WINDOW_T* win, ROAD_DIS_ELE_INF* road_ele_inf)
{
    const uint8_t *str = NULL;
    uint8_t buf[20] = {0};
    
    sprintf((char*)buf, "%s%s", road_ele_inf->time_buf, unit_pool[TIM_U_s]);
    str = div_str_pre_zero(buf);
    update_text_ele(road_ele_inf->time, win, str);
    update_text_ele(road_ele_inf->mode, win, mode_pool[cur_mode]);
    
    switch(cur_mode)
    {
        case ACW:
            sprintf((char*)buf, "%s%s", road_ele_inf->output_buf, unit_pool[VOL_U_kV]);
            update_text_ele(road_ele_inf->vol, win, div_str_pre_zero(buf));
            sprintf((char*)buf, "%s%s", road_ele_inf->loop_buf, unit_pool[ac_gear[cur_gear].unit]);
            update_text_ele(road_ele_inf->cur, win, div_str_pre_zero(buf));
            
            if(0 < strlen((const char*)road_ele_inf->real_buf))
            {   
                sprintf((char*)buf, "%s%s", road_ele_inf->real_buf, unit_pool[ac_gear[cur_gear].unit]);
            }
            else
            {
                buf[0] = 0;
            }
            
            update_text_ele(road_ele_inf->real, win, div_str_pre_zero(buf));
            break;
        case DCW:
            sprintf((char*)buf, "%s%s", road_ele_inf->output_buf, unit_pool[VOL_U_kV]);
            update_text_ele(road_ele_inf->vol, win, div_str_pre_zero(buf));
            sprintf((char*)buf, "%s%s", road_ele_inf->loop_buf, unit_pool[dc_gear[cur_gear].unit]);
            update_text_ele(road_ele_inf->cur, win, div_str_pre_zero(buf));
            buf[0] = 0;
            update_text_ele(road_ele_inf->real, win, div_str_pre_zero(buf));
            break;
        case IR:
            sprintf((char*)buf, "%s%s", road_ele_inf->output_buf, unit_pool[VOL_U_kV]);
            update_text_ele(road_ele_inf->vol, win, div_str_pre_zero(buf));
            sprintf((char*)buf, "%s%s", road_ele_inf->loop_buf, unit_pool[ir_gear[cur_gear].unit]);
            update_text_ele(road_ele_inf->cur, win, div_str_pre_zero(buf));
            buf[0] = 0;
            update_text_ele(road_ele_inf->real, win, div_str_pre_zero(buf));
            break;
        case GR:
            sprintf((char*)buf, "%s%s", road_ele_inf->output_buf, unit_pool[CUR_U_A]);
            update_text_ele(road_ele_inf->vol, win, div_str_pre_zero(buf));
            sprintf((char*)buf, "%s%s", road_ele_inf->loop_buf, unit_pool[RES_U_mOHM]);
            update_text_ele(road_ele_inf->cur, win, div_str_pre_zero(buf));
            buf[0] = 0;
            update_text_ele(road_ele_inf->real, win, div_str_pre_zero(buf));
            break;
    }
    
    str = get_test_status_str(road_ele_inf->test_st);
    update_text_ele(road_ele_inf->status, win, str);
}
void update_test_win_roads_text(MYUSER_WINDOW_T* win)
{
    int32_t i = 0;
    uint8_t n = 0;
    
    n = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < n; i++)
    {
        update_one_road_test_inf(win, &road_test_dis_inf[i]);
    }
}

void transform_test_win_roads_text(void)
{
    uint8_t buf[5][10] = {0};
    int32_t i = 0;
    uint8_t n = 0;
    
    n = ARRAY_SIZE(road_test_dis_inf);
    
    transform_output_to_str(buf[0]);
    transform_upper_to_str(buf[1]);
    transform_real_cur_to_str(buf[2]);
    transform_test_time_to_str(buf[3]);
    
    for(i = 0; i < n; i++)
    {
        strcpy((char*)road_test_dis_inf[i].output_buf, (const char*)buf[0]);
        strcpy((char*)road_test_dis_inf[i].loop_buf, (const char*)buf[1]);
        strcpy((char*)road_test_dis_inf[i].real_buf, (const char*)buf[2]);
        strcpy((char*)road_test_dis_inf[i].time_buf, (const char*)buf[3]);
    }
}
/**
  * @brief  更新测试窗口中的多路测试文本对象显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void update_test_win_text_ele_text(MYUSER_WINDOW_T* win)
{
    transform_test_win_roads_text();
    
    if(win->handle == 0)
    {
        return;
    }
    
    update_test_win_roads_text(win);
    update_group_inf(win);
}
/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void test_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_test_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_test_ui_text_ele_pos_inf();
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
    update_test_win_text_ele_text(win);
}
/**
  * @brief  初始化范围公共文本对象的显示信息
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_range_com_text_ele_dis_inf(MYUSER_WINDOW_T* win)
{
    UI_ELE_DISPLAY_INFO_T dis_info=
    {
        0/*base_x*/,0/*base_y*/,0/*x*/,200/*y*/,10/*width*/,30/*height*/,10,
        {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
    };
    
    dis_info.pos_size.x = 10;
    dis_info.pos_size.height = 25;
    dis_info.pos_size.y = win->pos_size.height - dis_info.pos_size.height;
    dis_info.pos_size.width = 130;
    dis_info.max_len = 100;
    dis_info.font = &GUI_Fonthz_20;
    dis_info.font_color = GUI_WHITE;
    dis_info.back_color = GUI_INVALID_COLOR;
    dis_info.align = GUI_TA_RIGHT;
    
    set_com_text_ele_dis_inf(&dis_info, COM_RANGE_NAME);//范围
    dis_info.pos_size.x += dis_info.pos_size.width;
    dis_info.pos_size.width = win->pos_size.width - 15 -  dis_info.pos_size.width;
    dis_info.align = GUI_TA_LEFT;
    set_com_text_ele_dis_inf(&dis_info, COM_RANGE_NOTICE);//提示信息
}
/**
  * @brief  清空范围公共文本对象的显示信息
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void clear_range_text_ele(MYUSER_WINDOW_T* win)
{
    uint8_t *buf[2]={"",""};
    set_com_text_ele_inf(COM_RANGE_NAME, win, buf);
    set_com_text_ele_inf(COM_RANGE_NOTICE, win, buf);
}
/**
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
static void init_create_test_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_range_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
    clear_range_text_ele(win);
    update_group_inf(win);//更新记忆组信息
}

/**
  * @brief  更新每路状态条背景色
  * @param  [in] inf 测试数据
  * @param  [in] road_ele_inf 每路的显示对象信息
  * @retval 无
  */
void update_road_bar_dis(UN_COMM_TEST_DATA *inf, ROAD_DIS_ELE_INF* road_ele_inf)
{
    GUI_COLOR backcolor;
    
    if(inf->status == ST_ERR_H
        || inf->status == ST_ERR_L
        || inf->status == ST_ERR_SHORT
        || inf->status == ST_ERR_VOL_ABNORMAL
        || inf->status == ST_ERR_ARC
        || inf->status == ST_ERR_GFI
        || inf->status == ST_ERR_FAIL
        || inf->status == ST_ERR_REAL
        || inf->status == ST_ERR_CHAR
        || inf->status == ST_ERR_GEAR
        || inf->status == ST_ERR_AMP
        || inf->status == ST_ERR_OPEN
    )
    {
        backcolor = ROAD_STATUS_BAR_WARNING_COLOR;
        set_text_ele_font_backcolor(road_ele_inf->bar, this_win, backcolor);
    }
}
void update_result_inf(UN_COMM_TEST_DATA *inf, ROAD_DIS_ELE_INF* road_ele_inf)
{
    RES_TEST_DATA *res_test_data;
    
    res_test_data = &result_inf_pool[road_ele_inf->road_num - 1].test_data;
    
    switch(cur_mode)
    {
        case ACW:
            res_test_data->un.acw.vol = inf->un.acw.vol;
            res_test_data->un.acw.vol = inf->un.acw.cur;
            res_test_data->test_time = inf->time;
            break;
        case DCW:
            break;
        case IR:
            break;
        case GR:
            break;
    }
}
/**
  * @brief  显示1路的测试信息
  * @param  [in] inf 测试数据
  * @param  [in] road_ele_inf 每路的文本对象信息
  * @param  [in] force 强制刷新标志
  * @retval 无
  */
void dis_one_road_test_inf(UN_COMM_TEST_DATA *inf, ROAD_DIS_ELE_INF* road_ele_inf, CS_BOOL force)
{
    const uint8_t *str = NULL;
    static uint8_t flag = 0;
    RES_TEST_DATA *res_test_data;
    uint8_t buf[20] = {0};
    
    res_test_data = &result_inf_pool[road_ele_inf->road_num - 1].test_data;
    
    mysprintf(road_ele_inf->time_buf, NULL, 151, inf->time);
    sprintf((char*)buf, "%s%s", road_ele_inf->time_buf, unit_pool[TIM_U_s]);
    update_text_ele(road_ele_inf->time, this_win, div_str_pre_zero(buf));
    update_text_ele(road_ele_inf->mode, this_win, mode_pool[cur_mode]);
    
    if(++flag % 5 == 0 || force == CS_TRUE)
    {
        switch(cur_mode)
        {
            case ACW:
                mysprintf(road_ele_inf->output_buf, NULL, 153, inf->un.acw.vol);
                sprintf((char*)buf, "%s%s", road_ele_inf->output_buf, unit_pool[VOL_U_kV]);
                update_text_ele(road_ele_inf->vol, this_win, div_str_pre_zero(buf));
                mysprintf(road_ele_inf->loop_buf, NULL, 150 + ac_gear[inf->un.acw.range].dec, inf->un.acw.cur);
                sprintf((char*)buf, "%s%s", road_ele_inf->loop_buf, unit_pool[ac_gear[inf->un.acw.range].unit]);
                update_text_ele(road_ele_inf->cur, this_win, div_str_pre_zero(buf));
                break;
            case DCW:
                break;
        }
        
        str = get_test_status_str(inf->status);
        res_test_data->test_result = inf->status;
        road_ele_inf->test_st = inf->status;
        
        update_text_ele(road_ele_inf->status, this_win, str);
        update_road_bar_dis(inf, road_ele_inf);//测试出现异常时使用
    }
}
void dis_step_num_inf(uint16_t step_num)
{
    uint8_t buf[10] = {0};
    
    sprintf((char*)buf, "%d/%d", step_num, g_cur_file->total);
    update_com_text_ele((CS_INDEX)COM_UI_CUR_STEP, this_win, buf);
}
/**
  * @brief  显示各路测试信息
  * @param  [in] force 强制刷新
  * @retval 无
  */
void dis_roads_inf(CS_BOOL force)
{
    UN_COMM_TEST_DATA test_data;
    UN_COMM_TEST_DATA *road_inf;
    uint32_t num = 0;
    uint8_t road_num;
    int32_t i = 0;
    
    num = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < num; i++)
    {
        road_num = road_test_dis_inf[i].road_num;
        road_inf = get_road_test_data(road_num, &test_data);
        
        if(road_inf == NULL)
        {
            continue;
        }
        
        if(road_inf->flag == 1)
        {
            road_inf->flag = 0;
            if(road_inf->status == ST_TESTING)
            {
                update_result_inf(&test_data, &road_test_dis_inf[i]);
            }
            dis_one_road_test_inf(&test_data, &road_test_dis_inf[i], force);
        }
    }
}
/**
  * @brief  显示各路测试信息
  * @param  [in] force 强制刷新
  * @retval 无
  */
CS_BOOL all_road_test_over(void)
{
    CS_BOOL res;
    uint8_t over_count = 0;
    static uint8_t timeout_count = 0;
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_1))
    {
        res = road1_test_over();
        
        if(res == CS_TRUE)
        {
            over_count++;
        }
    }
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_2))
    {
        res = road2_test_over();
        
        if(res == CS_TRUE)
        {
            over_count++;
        }
    }
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_3))
    {
        res = road3_test_over();
        
        if(res == CS_TRUE)
        {
            over_count++;
        }
    }
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_4))
    {
        res = road4_test_over();
        
        if(res == CS_TRUE)
        {
            over_count++;
        }
    }
    
    if(over_count == get_work_roads())
    {
        return CS_TRUE;
    }
    else if(over_count > 0)
    {
        if(++timeout_count > 5)
        {
//            return CS_TRUE;
        }
    }
    else if(over_count == 0)
    {
        timeout_count = 0;
    }
    
    return CS_FALSE;
}
/**
  * @brief  显示各路测试结束状态
  * @param  无
  * @retval 无
  */
void dis_test_over_status(void)
{
    CS_BOOL res;
//    const uint8_t *str;
    int32_t i = 0;
    uint32_t num = 0;
    ROAD_DIS_ELE_INF *inf = NULL;
    
    num = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < num; i++)
    {
        inf = &road_test_dis_inf[i];
        
        if(CS_TRUE == judge_road_work(inf->road_num))
        {
            res = inf->road_test_alarm();
            
            if(res == CS_FALSE)
            {
//                str = get_test_status_str(ST_PASS);
//                update_text_ele(inf->status, this_win, str);
//                inf->test_st = ST_PASS; //能够提前一致显示出测试合格
            }
            else
            {
                test_flag.fail = 1;
            }
        }
    }
    
    update_roads_bar();
}

/**
  * @brief  从机启动信号保持时提供给定时器调用的服务函数
  * @param  无
  * @retval 无
  */
void run_start_sign(void)
{
    if(hold_start_sign_time > 0)
    {
        if(--hold_start_sign_time == 0)
        {
            SYN_START_PIN = 1;
        }
    }
    
    if(hold_stop_sign_time > 0)
    {
        if(--hold_stop_sign_time == 0)
        {
            SYN_STOP_PIN = 1;
        }
    }
}

/**
  * @brief  设置启动信号保持时间
  * @param  [in] ms 保持时间单位 ms
  * @retval 无
  */
void hold_start_sign(uint32_t ms)
{
    hold_start_sign_time = ms;
}
/**
  * @brief  设置启动信号保持时间
  * @param  [in] ms 保持时间单位 ms
  * @retval 无
  */
void hold_stop_sign(uint32_t ms)
{
    hold_stop_sign_time = ms;
}

/**
  * @brief  向从机发送启动信号并保持100ms
  * @param  无
  * @retval 无
  */
void send_start_sign(void)
{
    SYN_START_PIN = 0;
    hold_start_sign(200);
}

/**
  * @brief  向从机发送启动信号并保持100ms
  * @param  无
  * @retval 无
  */
void send_stop_sign(void)
{
    SYN_STOP_PIN = 0;
    hold_stop_sign(200);
}
static void test_reset_fun(void)
{
    send_stop_sign();
    test_status = TEST_RESET;
}
/**
  * @brief  清空测试标记
  * @param  无
  * @retval 无
  */
void clear_test_flag(void)
{
    memset(&test_flag, 0, sizeof(test_flag));
}

void _record_setting_par_for_result(RESULT_INF *res, uint8_t road_num)
{
    strcpy((char*)res->par.file_name, (const char*)g_cur_file->name);
    res->par.mode = g_cur_step->one_step.com.mode;
    res->par.work_mode = g_cur_file->work_mode;
    res->par.step = g_cur_step->one_step.com.step;
    res->par.total_step = g_cur_file->total;
    sprintf((char*)res->product_code, "%04d", g_product_code);
    res->test_data.record_date = get_rtc_data();
    res->test_data.record_time = get_rtc_time();
    
    res->road_num = road_num;
    
    switch(cur_mode)
    {
        case ACW:
        {
            res->test_data.un.acw.range = cur_gear;
            res->par.un.acw.range = cur_gear;
            res->par.un.acw.vol = cur_vol;
            
            res->par.un.acw.hight = cur_high;
            res->par.un.acw.lower = cur_low;
            res->par.un.acw.test_time = tes_t;
            res->par.un.acw.rise_time = ris_t;
            break;
        }
        case DCW:
        {
            res->test_data.un.dcw.range = cur_gear;
            res->par.un.dcw.range = cur_gear;
            res->par.un.dcw.vol = cur_vol;
            
            res->par.un.dcw.hight = cur_high;
            res->par.un.dcw.lower = cur_low;
            res->par.un.dcw.test_time = tes_t;
            res->par.un.dcw.rise_time = ris_t;
            break;
        }
        case IR:
        {
            res->par.un.ir.vol = cur_vol;
            res->par.un.ir.hight = cur_high;
            res->par.un.ir.lower = cur_low;
            res->par.un.ir.test_time = tes_t;
            break;
        }
        case GR:
        {
            res->par.un.gr.cur = cur_vol;
            res->par.un.gr.hight = cur_high;
            res->par.un.gr.lower = cur_low;
            res->par.un.gr.test_time = tes_t;
            break;
        }
    }
}

void record_setting_par_for_result(void)
{
    int32_t i = 0;
    uint8_t num = 0;
    
    test_flag.save_only_onece = 0;
    num = ARRAY_SIZE(result_inf_pool);
    
    for(i = 0; i < num; i++)
    {
        _record_setting_par_for_result(&result_inf_pool[i], i + 1);
    }
}

static void save_roads_test_st(void)
{
    int32_t i = 0;
    uint32_t num = 0;
    ROAD_DIS_ELE_INF *inf = NULL;
    
    num = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < num; i++)
    {
        inf = &road_test_dis_inf[i];
        
        if(inf->test_st == ST_PASS)
        {
            result_inf_pool[i].test_data.test_result = ST_PASS;
        }
    }
}

void update_result_flag_inf(RESULT_INF *res)
{
    RESULT_INF old_res;
    CS_ERR err;
    
    if(sys_par.used_res_num < MAX_RESULT_NUM)
    {
        sys_par.used_res_num++;
        
        if(res->test_data.test_result == ST_PASS)
        {
            sys_par.pass_res_num++;
        }
    }
    else
    {
        if(sys_par.cover_res_num < MAX_RESULT_NUM)
        {
            sys_par.cover_res_num++;
        }
        else
        {
            sys_par.cover_res_num = 0;
        }
        
        read_one_result(sys_par.cover_res_num, &old_res, &err);
        
        if(err != CS_ERR_NONE)
        {
            return;
        }
        
        if(old_res.test_data.test_result != ST_PASS)
        {
            if(res->test_data.test_result == ST_PASS)
            {
                sys_par.pass_res_num++;
            }
        }
    }
}
void save_test_result(void)
{
    FRESULT fresult;
    FIL f;
    
    if(test_flag.save_only_onece)
    {
        return;
    }
    
    test_flag.save_only_onece = 1;
    save_roads_test_st();
    
    fresult = f_open (&f, (const char*)"\\ROOT\\RESULT\\RESULT.BIN", FA_WRITE | FA_OPEN_ALWAYS);
    
    if(fresult != FR_OK)
    {
        fresult = my_mkdir("\\ROOT\\RESULT");
        
        if(fresult != FR_OK)
        {
            return;
        }
        
        fresult = f_open(&f, (const char*)"\\ROOT\\RESULT\\RESULT.BIN", FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS);
        
        if(fresult != FR_OK)
        {
            return;
        }
    }
    
    fresult = f_lseek(&f, f.fsize);
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_1))
    {
        fresult = f_write (&f, &result_inf_pool[INDEX_ROAD_1 - 1], sizeof(RESULT_INF), NULL);
        
        if(fresult != FR_OK)
        {
            goto end;
        }
        
        update_result_flag_inf(&result_inf_pool[INDEX_ROAD_1 - 1]);
    }
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_2))
    {
        fresult = f_write (&f, &result_inf_pool[INDEX_ROAD_2 - 1], sizeof(RESULT_INF), NULL);
        
        if(fresult != FR_OK)
        {
            goto end;
        }
        
        update_result_flag_inf(&result_inf_pool[INDEX_ROAD_2 - 1]);
    }
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_3))
    {
        fresult = f_write (&f, &result_inf_pool[INDEX_ROAD_3 - 1], sizeof(RESULT_INF), NULL);
        
        if(fresult != FR_OK)
        {
            goto end;
        }
        
        update_result_flag_inf(&result_inf_pool[INDEX_ROAD_3 - 1]);
    }
    
    if(CS_TRUE == judge_road_work(INDEX_ROAD_4))
    {
        fresult = f_write (&f, &result_inf_pool[INDEX_ROAD_4 - 1], sizeof(RESULT_INF), NULL);
        
        if(fresult != FR_OK)
        {
            goto end;
        }
        
        update_result_flag_inf(&result_inf_pool[INDEX_ROAD_4 - 1]);
    }
    
end:
    f_close(&f);
    
    save_sys_par();//保存系统参数
}
/**
  * @brief  测试状态机
  * @param  无
  * @retval 无
  */
static void test_status_machine(void)
{
    static uint32_t count_dly;
    
    switch(test_status)
    {
        case TEST_IDLE:
            break;
        case CHECK_TEST_OVER_SIGN:
            break;
        case TEST_START:
            load_data();
            test_step = cur_step;
            dis_step_num_inf(test_step);
            send_cmd_to_all_module((uint8_t*)&test_step,
                                    2, send_load_step);
            send_cmd_to_all_module((uint8_t*)&test_step,
                                    2, send_load_step);
            
            record_setting_par_for_result();
            test_step = ((test_step) % (g_cur_file->total)) + 1;
            
            load_steps_to_list(test_step, 1);//加载新的当前步
            g_cur_step = get_g_cur_step();
            
            if(cur_step == 1)
            {
                clear_test_flag();
            }
            
            send_start_sign();//发出同步启动信号
            
            test_status = TEST_TESTING;//进入正在测试状态
            count_dly = 0;
            roads_into_test_st();
            update_roads_bar();
            key_scan_off();
            set_pass_led_state(LED_OFF);//关闭PASS灯
            register_tim3_server_fun(test_led_flicker);//测试灯闪烁
            break;
        case TEST_RESET:
            test_step = 1;
            load_steps_to_list(test_step, 1);//加载新的当前步
            g_cur_step = get_g_cur_step();
            load_data();
            test_status = TEST_IDLE;
            set_roads_test_st(ST_WAIT);
            update_test_win_text_ele_text(this_win);
            update_roads_bar();
            key_scan_on();
            set_pass_led_state(LED_OFF);
            set_fail_led_state(LED_OFF);
            set_test_led_state(LED_OFF);
            set_buzzer(BUZZER_OFF);
            dis_step_num_inf(test_step);
            clear_test_flag();
            
            break;
        case TEST_TESTING:
            send_cmd_to_all_module(NULL, 0, send_get_test_data);
            dis_roads_inf(CS_FALSE);
            
            /* 判断当前步是否测试结束 */
            if(CS_TRUE == all_road_test_over())
            {
                dis_roads_inf(CS_TRUE);
                dis_test_over_status();
                unregister_tim3_server_fun(test_led_flicker);
                set_test_led_state(LED_OFF);
                save_test_result();
                
                if(cur_step == g_cur_file->total)
                {
                    if(test_flag.fail)
                    {
                        set_fail_led_state(LED_ON);
                        set_buzzer(BUZZER_ON);
                        break;
                    }
                    else
                    {
                        set_pass_led_state(LED_ON);
                    }
                }
                
                /* 步间连续打开 */
                if(steps_con)
                {
                    /* 做延时是因为测试状态信息还没有传上来 */
                    if(++count_dly > 3)
                    {
                        count_dly = 0;
                        test_status = TEST_START;
                    }
                }
                /* 步间连续关闭 */
                else
                {
                    key_scan_on();
                    if(++count_dly > 10)
                    {
                        count_dly = 0;
                        test_status = TEST_IDLE;
                    }
                }
            }
            
            break;
    }
}

/**
  * @brief  设置各路的测试状态
  * @param  [in] test_st 测试状态
  * @retval 无
  */
static void set_roads_test_st(uint8_t test_st)
{
    int32_t i = 0;
    uint32_t num = 0;
    ROAD_DIS_ELE_INF *inf = NULL;
    
    num = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < num; i++)
    {
        inf = &road_test_dis_inf[i];
        
        inf->test_st = test_st;
    }
}

/**
  * @brief  设置各路进入测试状态,前面测试中发生异常的步骤不能再次进入测试状态，
  *         只能等仪器复位后再次参与测试
  * @param  无
  * @retval 无
  */
static void roads_into_test_st(void)
{
    int32_t i = 0;
    uint32_t num = 0;
    ROAD_DIS_ELE_INF *inf = NULL;
    
    num = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < num; i++)
    {
        inf = &road_test_dis_inf[i];
        
        if(!(inf->test_st == ST_ERR_H
            || inf->test_st == ST_ERR_L
            || inf->test_st == ST_ERR_SHORT
            || inf->test_st == ST_ERR_VOL_ABNORMAL
            || inf->test_st == ST_ERR_ARC
            || inf->test_st == ST_ERR_GFI
            || inf->test_st == ST_ERR_FAIL
            || inf->test_st == ST_ERR_REAL
            || inf->test_st == ST_ERR_CHAR
            || inf->test_st == ST_ERR_GEAR
            || inf->test_st == ST_ERR_AMP
            || inf->test_st == ST_ERR_OPEN
        ))
        {
            if(CS_TRUE == judge_road_work(inf->road_num))
            {
                inf->test_st = TEST_TESTING;
            }
        }
    }
}

/**
  * @brief  更新各路状态条的颜色显示
  * @param  [in] test_st 测试状态
  * @retval 无
  */
static void update_roads_bar(void)
{
    int32_t i = 0;
    uint32_t num = 0;
    ROAD_DIS_ELE_INF *inf = NULL;
    GUI_COLOR backcolor;
    
    num = ARRAY_SIZE(road_test_dis_inf);
    
    for(i = 0; i < num; i++)
    {
        inf = &road_test_dis_inf[i];
        
        /* 测试合格 */
        if(inf->test_st == ST_PASS)
        {
            backcolor = ROAD_STATUS_BAR_PASS_COLOR;
        }
        /* 常态 */
        else if(inf->test_st == ST_WAIT
            || inf->test_st == ST_VOL_RISE
            || inf->test_st == ST_TESTING
            || inf->test_st == ST_VOL_FALL
            || inf->test_st == ST_INTER_WAIT
            || inf->test_st == ST_STOP
        )
        {
            backcolor = ROAD_STATUS_BAR_NORMAL_COLOR;
        }
        /* 异常状态 */
        else if(inf->test_st == ST_ERR_H
            || inf->test_st == ST_ERR_L
            || inf->test_st == ST_ERR_SHORT
            || inf->test_st == ST_ERR_VOL_ABNORMAL
            || inf->test_st == ST_ERR_ARC
            || inf->test_st == ST_ERR_GFI
            || inf->test_st == ST_ERR_FAIL
            || inf->test_st == ST_ERR_REAL
            || inf->test_st == ST_ERR_CHAR
            || inf->test_st == ST_ERR_GEAR
            || inf->test_st == ST_ERR_AMP
            || inf->test_st == ST_ERR_OPEN
        )
        {
            backcolor = ROAD_STATUS_BAR_WARNING_COLOR;
        }
        /* 其他状态 */
        else
        {
            backcolor = ROAD_STATUS_BAR_NORMAL_COLOR;
        }
        
        set_text_ele_font_backcolor(inf->bar, this_win, backcolor);
    }
}
/**
  * @brief  测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void test_win_cb(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	MYUSER_WINDOW_T* win;
	static CS_IMAGE_T cs_image;
    
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
		{
            clear_key_buf();
            set_test_windows_handle(hWin);
            win = get_user_window_info(hWin);
            register_tim3_server_fun(run_start_sign);
            register_test_reset_server_fun(test_reset_fun);
//            create_test_image(hWin);
            create_miclogo_image(hWin, &cs_image);
			WM_SetFocus(hWin);/* 设置聚焦 */
			WM_SetCreateFlags(WM_CF_MEMDEV);//如果不开启显示效果非常差, 开启后刷屏很慢
			test_win_timer_handle = WM_CreateTimer(hWin, 0, 100, 0);
			
			if(win != NULL)
			{
                NODE_STEP *node;
                
                load_steps_to_list(1, 1);
                node = get_g_cur_step();
                
                if(NULL != node)
                {
                    g_cur_step = node;
                }
                
                set_roads_test_st(ST_WAIT);
                init_sys_function_key_inf(win);//初始化系统功能按键信息（包括菜单键和其他功能键）
                init_create_win_all_ele(win);//创建窗口内所有的对象
                
                update_group_inf(g_cur_win);//更新记忆组信息
                clear_range_text_ele(g_cur_win);//清空范围显示信息
			}
            
            test_status = TEST_IDLE;
			break;
		}
		case WM_TIMER:
		{
            test_status_machine();//测试状态机
			WM_RestartTimer(test_win_timer_handle, 10);
			break;
        }
        case WM_KEY:
            break;
		case WM_PAINT:
			test_win_paint_frame();
            draw_group_inf_area();
			draw_composition_7_1(this_win);
			break;
		case WM_NOTIFY_PARENT:
			break;
		case WM_DELETE:
            delete_image(&cs_image);
			break;
		default:
			WM_DefaultProc(pMsg);
            break;
	}
}
/* Public functions ---------------------------------------------------------*/
uint8_t get_road_test_status(ROAD_NUM_T road)
{
    return road_test_dis_inf[road - 1].test_st;
}
uint8_t* get_road_test_voltage(ROAD_NUM_T road)
{
    return road_test_dis_inf[road - 1].output_buf;
}
uint8_t* get_road_test_current(ROAD_NUM_T road)
{
    return road_test_dis_inf[road - 1].loop_buf;
}
uint8_t* get_road_test_real_current(ROAD_NUM_T road)
{
    return road_test_dis_inf[road - 1].real_buf;
}

uint8_t* get_road_test_time(ROAD_NUM_T road)
{
    return road_test_dis_inf[road - 1].time_buf;
}

void update_test_win_text_display(void)
{
    update_test_win_text_ele_text(this_win);
}
uint8_t get_max_roads(void)
{
    return ARRAY_SIZE(road_test_dis_inf);
}
/**
  * @brief  创建测试窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_test_win(int hWin)
{
	create_user_window(&test_windows, &windows_list, hWin);//创建测试界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
