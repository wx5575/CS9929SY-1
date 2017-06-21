/**
  ******************************************************************************
  * @file    test_ui_com.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "string.h"
#include "keyboard.h"
#include "UI_COM/com_ui_info.h"
#include "test_win.h"
#include "7_test_ui_layout_1.h"
#include "step_par_win/step_edit_win.h"
#include "ui_com/com_edit_api.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void update_test_win_menu_key_inf(WM_HMEM hWin);
static void test_win_cb(WM_MESSAGE * pMsg);
static void select_set_acw_par_menu_1(int hWin);
static void select_set_acw_par_menu_2(int hWin);

static void change_key_menu(int id);
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
    
static void test_win_edit_mode_f6_cb(KEY_MESSAGE *key_msg);
static void test_win_edit_range_f6_cb(KEY_MESSAGE *key_msg);

static void test_win_edit_mode_menu_key_init(WM_HMEM hWin);
static void test_win_edit_range_menu_key_init(WM_HMEM hWin);
static void test_win_edit_vol_menu_key_init(WM_HMEM hWin);
static void test_win_edit_test_time_menu_key_init(WM_HMEM hWin);

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

static void update_test_win_text_ele_text(MYUSER_WINDOW_T* win);
static void init_create_test_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_test_win_text_ele(MYUSER_WINDOW_T* win);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  定时器句柄
  */
static	WM_HWIN timer_handle;
/**
  * @brief  测试界面的位置尺寸信息，根据不同屏幕进行初始化
  */
static WIDGET_POS_SIZE_T* test_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_test_windows,/*4.3寸屏*/
    &_7_test_windows,/*5.6寸屏*/
    &_7_test_windows,/*7寸屏*/
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
    {"", F_KEY_NULL	    , KEY_F4 & _KEY_UP,	test_win_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP,	test_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	test_win_f6_cb },//f6
};
/**
  * @brief  测试界面下的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_set_menu_key_pool[]=
{
    {"", F_KEY_DISPLAY	, KEY_F0 & _KEY_UP,	test_win_f0_cb },//f0
    {"", F_KEY_MODE		, KEY_F1 & _KEY_UP,	test_win_f1_cb },//f1
    {"", F_KEY_VOL	    , KEY_F2 & _KEY_UP,	test_win_f2_cb },//f2
    {"", F_KEY_RANGE    , KEY_F3 & _KEY_UP,	test_win_f3_cb },//f3
    {"", F_KEY_TIME	    , KEY_F4 & _KEY_UP,	test_win_f4_cb },//f4
    {"", F_KEY_MORE     , KEY_F5 & _KEY_UP,	test_win_f5_cb },//f5
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
        {"", F_KEY_LOWER , KEY_F2 & _KEY_UP, set_acw_par_f2_2_cb},//f2
        {"", F_KEY_NULL  , KEY_F3 & _KEY_UP, set_acw_par_f3_2_cb},//f3
        {"", F_KEY_NULL  , KEY_F4 & _KEY_UP, set_acw_par_f4_2_cb},//f4
        {"", F_KEY_NULL  , KEY_F5 & _KEY_UP, set_acw_par_f5_2_cb},//f5
        {"", F_KEY_BACK  , KEY_F6 & _KEY_UP, set_acw_par_f6_2_cb},//f6
    },
};


/**
  * @brief  测试界面显示的文本索引表
  */
static CS_INDEX test_ui_ele_buf[] =
{
//	TEST_UI_FILE_NAME,
//	TEST_UI_CUR_FILE_NAME,
//	TEST_UI_STEP,
//	TEST_UI_CUR_STEP,
//	TEST_UI_WORK_MODE,
//	TEST_UI_CUR_WORK_MODE,
	
	TEST_UI_ROAD01_NUM,
	TEST_UI_ROAD01_MODE,
	TEST_UI_ROAD01_STATUS,
	TEST_UI_ROAD01_VOLTAGE,
	TEST_UI_ROAD01_UPPER,
	TEST_UI_ROAD01_REAL,
	TEST_UI_ROAD01_TIME,
	TEST_UI_ROAD02_NUM,
	TEST_UI_ROAD02_MODE,
	TEST_UI_ROAD02_STATUS,
	TEST_UI_ROAD02_VOLTAGE,
	TEST_UI_ROAD02_UPPER,
	TEST_UI_ROAD02_REAL,
	TEST_UI_ROAD02_TIME,
	TEST_UI_ROAD03_NUM,
	TEST_UI_ROAD03_MODE,
	TEST_UI_ROAD03_STATUS,
	TEST_UI_ROAD03_VOLTAGE,
	TEST_UI_ROAD03_UPPER,
	TEST_UI_ROAD03_REAL,
	TEST_UI_ROAD03_TIME,
	TEST_UI_ROAD04_NUM,
	TEST_UI_ROAD04_MODE,
	TEST_UI_ROAD04_STATUS,
	TEST_UI_ROAD04_VOLTAGE,
	TEST_UI_ROAD04_UPPER,
	TEST_UI_ROAD04_REAL,
	TEST_UI_ROAD04_TIME,
};

/**
  * @brief 界面文本对象数组
  */
static TEXT_ELE_T test_ui_ele_pool[]=
{
	{{"文件名:", "FileName:"}, TEST_UI_FILE_NAME    },
	{{"DEFAULT", "DEFAULT"  }, TEST_UI_CUR_FILE_NAME},
	{{"步骤:"  , "Step:"    }, TEST_UI_STEP         },
	{{"01/01"  , "01/01"    }, TEST_UI_CUR_STEP     },
	{{"工作模式:","WorkMode:"}, TEST_UI_WORK_MODE   },
	{{"N"      , "N"        }, TEST_UI_CUR_WORK_MODE},
	
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
	
	{{"01"     ,"01"        }, TEST_UI_ROAD01_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD01_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD01_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD01_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD01_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD01_REAL   },
	{{"823.4s" ,"823.4s"    }, TEST_UI_ROAD01_TIME   },
	{{"02"     ,"02"        }, TEST_UI_ROAD02_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD02_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD02_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD02_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD02_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD02_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD02_TIME   },
	{{"03"     ,"03"        }, TEST_UI_ROAD03_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD03_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD03_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD03_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD03_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD03_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD03_TIME   },
	{{"04"     ,"04"        }, TEST_UI_ROAD04_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD04_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD04_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD04_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD04_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD04_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD04_TIME   },
	{{"05"     ,"05"        }, TEST_UI_ROAD05_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD05_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD05_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD05_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD05_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD05_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD05_TIME   },
	{{"06"     ,"06"        }, TEST_UI_ROAD06_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD06_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD06_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD06_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD06_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD06_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD06_TIME   },
	{{"07"     ,"07"        }, TEST_UI_ROAD07_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD07_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD07_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD07_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD07_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD07_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD07_TIME   },
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
enum{
    TEST_WIN_EDIT_INDEX,///<测试窗口中的编辑对象索引枚举
};
CS_INDEX test_win_edit_index_table[]=
{
    TEST_WIN_EDIT_INDEX,
};
static EDIT_ELE_T test_win_edit_ele_pool[]=
{
    {
        {"",""}, /* 名称 */
        TEST_WIN_EDIT_INDEX,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 0/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*decs*/,2/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {99/*heigh*/,1/*low*/,{"",""}/*notice*/},/*range*/
        {NULL, NULL, NULL,},/*key_inf*/
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
        (CS_INDEX*)group_com_ele_table,ARRAY_SIZE(group_com_ele_table),
        init_create_win_com_ele,
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
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF test_win_mode_inf_pool[]=
{
    {ACW_STR , ACW},
    {DCW_STR , DCW},
    {IR_STR  , IR },
    {GR_STR  , GR },
    {BBD_STR , BBD},
    {CC_STR  , CC },
};

/**
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF test_win_cur_range_inf_pool[]=
{
    { CUR_2uA_STR   , AC_2uA	, 0},
    { CUR_20uA_STR  , AC_20uA	, 0},
    { CUR_200uA_STR , AC_200uA  , 0},
    { CUR_2mA_STR   , AC_2mA	, 0},
    { CUR_10mA_STR  , AC_10mA	, 0},
    { CUR_20mA_STR  , AC_20mA	, 0},
    { CUR_50mA_STR  , AC_50mA	, 0},
    { CUR_100mA_STR , AC_100mA  , 0},
    { CUR_200mA_STR , AC_200mA  , 0},
    { CUR_2A_STR    , AC_2A     , 0},
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
  * @brief  编辑测试模式时使用的菜单键初始化信息数组
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
  * @brief  编辑电压时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	test_win_edit_vol_menu_key_init_info[] =
{
    {"", F_KEY_DEL		, KEY_F1 & _KEY_UP, edit_test_win_vol_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, edit_test_win_vol_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, edit_test_win_vol_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, edit_test_win_vol_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, edit_test_win_vol_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_test_win_vol_f6_cb },//f6
};

/**
  * @brief  编辑电压时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	test_win_edit_upper_menu_key_init_info[] =
{
    {"", F_KEY_DEL		, KEY_F1 & _KEY_UP, edit_test_win_upper_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, edit_test_win_upper_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, edit_test_win_upper_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, edit_test_win_upper_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, edit_test_win_upper_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_test_win_upper_f6_cb },//f6
};

/**
  * @brief  测试窗口编辑参数时使用的编辑控件功能键的初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	test_win_edit_par_sys_key_init_pool[]=
{
	{KEY_UP		, test_win_direct_key_up_cb      },
	{KEY_DOWN	, test_win_direct_key_down_cb 	  },
	{KEY_LEFT	, test_win_direct_key_left_cb    },
	{KEY_RIGHT	, test_win_direct_key_right_cb   },
//	{CODE_LEFT	, test_win_direct_key_down_cb    },
//	{CODE_RIGH	, test_win_direct_key_up_cb      },
	{KEY_ENTER	, test_win_sys_key_enter_cb      },
};
/* Private functions ---------------------------------------------------------*/

static void test_win_win_sys_key_init(WM_HMEM hWin)
{
    FUNCTION_KEY_INFO_T *pool = test_win_edit_par_sys_key_init_pool;
    uint32_t size = ARRAY_SIZE(test_win_edit_par_sys_key_init_pool);
    
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
    EDIT_ELE_T *ele;
    
    ele = get_mode_edit_ele_inf(&g_cur_step->one_step);
    
    if(ele != NULL)
    {
        init_menu_key_custom_inf(cus_inf, cus_size, ele, info, size);
        init_menu_key_info(info, size, hWin);
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
    EDIT_ELE_T *ele;
    
    ele = get_range_edit_ele_inf(&g_cur_step->one_step);
    
    if(ele != NULL)
    {
        init_menu_key_custom_inf(cus_inf, cus_size, ele, info, size);
        init_menu_key_info(info, size, hWin);
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
    EDIT_ELE_T *ele = NULL;
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    ele = get_vol_edit_ele_inf(&g_cur_step->one_step);
    
    if(ele != NULL)
    {
        memcpy(edit_ele, ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_vol_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
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
    EDIT_ELE_T *ele = NULL;
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    ele = get_test_time_edit_ele_inf(&g_cur_step->one_step);
    
    if(ele != NULL)
    {
        memcpy(edit_ele, ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_test_time_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
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
    EDIT_ELE_T *ele = NULL;
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    ele = get_cur_upper_edit_ele_inf(&g_cur_step->one_step);
    
    if(ele != NULL)
    {
        memcpy(edit_ele, ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_cur_upper_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
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
    EDIT_ELE_T *ele = NULL;
    EDIT_ELE_T *edit_ele = NULL;
    CS_ERR err;
    EDIT_ELE_T *pool;
    uint32_t n;
    
    pool = g_cur_win->edit.pool;
    n = g_cur_win->edit.pool_size;
    
    edit_ele = get_edit_ele_inf(pool, n, TEST_WIN_EDIT_INDEX, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    ele = get_cur_lower_edit_ele_inf(&g_cur_step->one_step);
    
    if(ele != NULL)
    {
        memcpy(edit_ele, ele, sizeof(EDIT_ELE_T));
        edit_ele->index = TEST_WIN_EDIT_INDEX;//恢复索引值
        init_cur_lower_edit_ele_pos_size(g_cur_win);
        init_create_test_win_edit_ele(g_cur_win);
        init_menu_key_info(info, size, hWin);
    }
}
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
    edit_ele->dis.edit.font = SEL_FONT(text_ele->dis_info.font);
}
static void init_vol_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *ele = NULL;
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
static void init_test_time_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *ele = NULL;
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
static void init_cur_upper_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *ele = NULL;
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
static void init_cur_lower_edit_ele_pos_size(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T *ele = NULL;
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
static void init_create_test_win_edit_ele(MYUSER_WINDOW_T *win)
{
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    init_window_edit_ele(win);//初始化创建编辑对象
    
    g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
    select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
    test_win_win_sys_key_init(g_cur_edit_ele->dis.edit.handle);
}

static void test_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_sys_key_enter_cb(KEY_MESSAGE *key_msg)
{
    upload_par_to_ram(g_cur_edit_ele);//将数据上载到内存中
    save_setting_step();
    update_test_win_text_ele_text(g_cur_win);
    delete_win_edit_ele(g_cur_win);
    change_key_menu(key_msg->user_data);
}
/**
  * @brief  设置测试界面功能键F0回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_edit_mode_f6_cb(KEY_MESSAGE *key_msg)
{
    change_key_menu(key_msg->user_data);
}
/**
  * @brief  设置测试界面功能键F0回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_edit_range_f6_cb(KEY_MESSAGE *key_msg)
{
    change_key_menu(key_msg->user_data);
}

static void edit_test_win_upper_f1_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_upper_f2_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_upper_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_upper_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_upper_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_upper_f6_cb(KEY_MESSAGE *key_msg)
{
    change_key_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
}

static void edit_test_win_vol_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
static void edit_test_win_vol_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(g_cur_edit_ele->dis.edit.handle);
}
static void edit_test_win_vol_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_vol_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_vol_f5_cb(KEY_MESSAGE *key_msg)
{
}
static void edit_test_win_vol_f6_cb(KEY_MESSAGE *key_msg)
{
    change_key_menu(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
}
/**
  * @brief  设置测试界面功能键F0回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f0_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f2_cb(KEY_MESSAGE *key_msg)
{
    change_key_menu(key_msg->user_data);
}
/**
  * @brief  设置测试界面功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

static void set_acw_par_f1_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_mode_menu_key_init(key_msg->user_data);
}
static void set_acw_par_f2_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_vol_menu_key_init(key_msg->user_data);
}
static void set_acw_par_f3_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_range_menu_key_init(key_msg->user_data);
}
static void set_acw_par_f4_1_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_test_time_menu_key_init(key_msg->user_data);
}
static void set_acw_par_f5_1_cb(KEY_MESSAGE *key_msg)
{
    select_set_acw_par_menu_2(key_msg->user_data);
}
static void set_acw_par_f6_1_cb(KEY_MESSAGE *key_msg)
{
    update_test_win_menu_key_inf(key_msg->user_data);
}

static void set_acw_par_f1_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_upper_menu_key_init(key_msg->user_data);
}
static void set_acw_par_f2_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_lower_menu_key_init(key_msg->user_data);
}
static void set_acw_par_f3_2_cb(KEY_MESSAGE *key_msg)
{
}
static void set_acw_par_f4_2_cb(KEY_MESSAGE *key_msg)
{
}
static void set_acw_par_f5_2_cb(KEY_MESSAGE *key_msg)
{
}
static void set_acw_par_f6_2_cb(KEY_MESSAGE *key_msg)
{
    select_set_acw_par_menu_1(key_msg->user_data);
    delete_win_edit_ele(g_cur_win);
}
static int8_t cur_at_menu_page_flag;
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
static void select_set_acw_par_menu_1(int hWin)
{
    cur_at_menu_page_flag = 0;
    init_menu_key_info(test_ui_acw_menu_pool[0], ARRAY_SIZE(test_ui_acw_menu_pool[0]), hWin);
}
static void select_set_acw_par_menu_2(int hWin)
{
    cur_at_menu_page_flag = 1;
    init_menu_key_info(test_ui_acw_menu_pool[1], ARRAY_SIZE(test_ui_acw_menu_pool[1]), hWin);
}
/**
  * @brief  改变菜单按键信息
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void change_key_menu(int hWin)
{
    switch(g_cur_step->one_step.com.mode)
    {
        case ACW:
            change_key_set_acw_par_menu(hWin);
            break;
        case DCW:
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
    switch(sys_par.screem_size)
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
static void update_test_win_text_ele_text(MYUSER_WINDOW_T* win)
{
    uint8_t buf[10];
    
    strcpy((char*)buf, (const char*)mode_pool[g_cur_step->one_step.com.mode]);
    update_text_ele((CS_INDEX)TEST_UI_ROAD01_MODE, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD02_MODE, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD03_MODE, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD04_MODE, win, buf);
    
    mysprintf(buf, unit_pool[VOL_U_kV], 153, g_cur_step->one_step.acw.output_vol);
    update_text_ele((CS_INDEX)TEST_UI_ROAD01_VOLTAGE, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD02_VOLTAGE, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD03_VOLTAGE, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD04_VOLTAGE, win, buf);
    
    mysprintf(buf, unit_pool[CUR_U_mA], 153, g_cur_step->one_step.acw.upper_limit);
    update_text_ele((CS_INDEX)TEST_UI_ROAD01_UPPER, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD02_UPPER, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD03_UPPER, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD04_UPPER, win, buf);
    
    mysprintf(buf, unit_pool[CUR_U_mA], 153, g_cur_step->one_step.acw.real_cur);
    update_text_ele((CS_INDEX)TEST_UI_ROAD01_REAL, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD02_REAL, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD03_REAL, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD04_REAL, win, buf);
    
    mysprintf(buf, unit_pool[TIM_U_s], 151, g_cur_step->one_step.acw.test_time);
    update_text_ele((CS_INDEX)TEST_UI_ROAD01_TIME, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD02_TIME, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD03_TIME, win, buf);
    update_text_ele((CS_INDEX)TEST_UI_ROAD04_TIME, win, buf);
    
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
  * @brief  初始化并创建步骤编辑窗口中的编辑控件
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
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
static void init_create_test_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    update_group_inf(win);
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
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
    
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
		{
            set_test_windows_handle(hWin);
            win = get_user_window_info(hWin);
            
			WM_SetFocus(hWin);/* 设置聚焦 */
			update_test_win_menu_key_inf(hWin);
			WM_SetCreateFlags(WM_CF_MEMDEV);//如果不开启显示效果非常差, 开启后刷屏很慢
			
			if(win != NULL)
			{
                NODE_STEP *node;
                
                load_steps_to_list(1, 1);
                node = get_g_cur_step();
                
                if(NULL != node)
                {
                    g_cur_step = node;
                }
                
                init_create_win_all_ele(win);//创建窗口人所有的对象
                
                update_group_inf(g_cur_win);
			}
//			timer_handle = WM_CreateTimer(hWin, id_base++, 100, 0);
			break;
		}
		case WM_TIMER:
		{
//            static int time = 0;
//            uint8_t buf[10];
//            if(++time > 9999)
//            {
//                time = 0;
//            }
//            mysprintf(buf, unit_pool[TIM_U_s], 151, time);
//			win = get_user_window_info(hWin);
//			update_text_ele((CS_INDEX)TEST_UI_ROAD01_TIME, win, buf);
//			update_text_ele((CS_INDEX)TEST_UI_ROAD02_TIME, win, buf);
//			update_text_ele((CS_INDEX)TEST_UI_ROAD03_TIME, win, buf);
//			update_text_ele((CS_INDEX)TEST_UI_ROAD04_TIME, win, buf);
//			WM_RestartTimer(timer_handle, 100);
			break;
        }
        case WM_KEY:
            break;
		case WM_PAINT:
			test_win_paint_frame();
            draw_group_inf_area();
			draw_composition_7_1();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
/* Public functions ---------------------------------------------------------*/

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
