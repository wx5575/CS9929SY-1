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
#include "running_test.h"
#include "file_win/file_win.h"
#include "password_win/input_password_win.h"
#include "result_win/result_win.h"
#include "cs99xx_mem_api.h"
#include "UI/KEY_MENU_WIN/key_menu_win.h"


/* Private typedef -----------------------------------------------------------*/
enum{
    TEST_WIN_EDIT_INDEX,///<测试窗口中的编辑对象索引枚举
};

typedef struct{
    uint32_t x_n;///<n倍率
    uint8_t x10;///<10倍率
    uint8_t x100;///<100倍率
    uint8_t x1000;///<1000倍率
}DATA_MUL_POWER;///<数据设置的倍率
/* Private define ------------------------------------------------------------*/
#define CUR_VALUE_AT_KEY_COLOR      GUI_RED
#define NO_CUR_VALUE_AT_KEY_COLOR   GUI_WHITE
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
/* Private variables ---------------------------------------------------------*/

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
        {"", F_KEY_NULL , KEY_F2 & _KEY_UP, set_acw_par_f2_2_cb},//f2
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
        {"", F_KEY_UPPER , KEY_F1 & _KEY_UP, set_ir_par_f1_2_cb},//f1
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
        {"", F_KEY_NULL , KEY_F1 & _KEY_UP, set_gr_par_f1_2_cb},//f1
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
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
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
//    test_win_win_sys_key_init(g_cur_edit_ele->dis.edit.handle);
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
    update_key_menu(key_msg->user_data);
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
    test_win_edit_cur_upper_menu_key_init(key_msg->user_data);
}
/**
  * @brief  测试窗口编辑IR参数时使用的第2页菜单键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void set_ir_par_f2_2_cb(KEY_MESSAGE *key_msg)
{
    test_win_edit_cur_lower_menu_key_init(key_msg->user_data);
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
  * @brief  多路测试模式文本对象信息索引表
  */
static CS_INDEX roads_mode_index_table[]=
{
    TEST_UI_ROAD01_MODE,
    TEST_UI_ROAD02_MODE,
    TEST_UI_ROAD03_MODE,
    TEST_UI_ROAD04_MODE,
};
/**
  * @brief  更新测试窗口的测试模式文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void updata_test_win_test_mode(MYUSER_WINDOW_T* win)
{
    uint8_t n = 0;
    int32_t i = 0;
    uint8_t buf[10];
    CS_INDEX *index_pool = NULL;
    
    index_pool = roads_mode_index_table;
    n = ARRAY_SIZE(roads_mode_index_table);
    
    strcpy((char*)buf, (const char*)mode_pool[g_cur_step->one_step.com.mode]);
    
    for(i = 0; i < n; i++)
    {
        update_text_ele(index_pool[i], win, buf);
    }
}
/**
  * @brief  多路输出电压文本对象信息索引表
  */
static CS_INDEX roads_vol_index_table[]=
{
    TEST_UI_ROAD01_VOLTAGE,
    TEST_UI_ROAD02_VOLTAGE,    
    TEST_UI_ROAD03_VOLTAGE,    
    TEST_UI_ROAD04_VOLTAGE,    
};
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
            unit = VOL_U_kV;
            format = 100 + 10 * lon + dec;
            
            mysprintf_2(buf, unit_pool[unit], format, g_cur_step->one_step.acw.output_vol);
            break;
        }
        case DCW:
        {
            dec = 3;
            lon = 5;
            unit = VOL_U_kV;
            format = 100 + 10 * lon + dec;
            
            mysprintf_2(buf, unit_pool[unit], format, g_cur_step->one_step.dcw.output_vol);
            break;
        }
        case IR:
        {
            dec = 3;
            lon = 5;
            unit = VOL_U_kV;
            format = 100 + 10 * lon + dec;
            
            mysprintf_2(buf, unit_pool[unit], format, g_cur_step->one_step.ir.output_vol);
            break;
        }
        case GR:
        {
            dec = 2;
            lon = 5;
            unit = CUR_U_A;
            format = 100 + 10 * lon + dec;
            
            mysprintf_2(buf, unit_pool[unit], format, g_cur_step->one_step.gr.output_cur);
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
  * @brief  更新测试窗口的输出电压/电流文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void updata_test_win_test_vol(MYUSER_WINDOW_T* win)
{
    uint8_t n = 0;
    int32_t i = 0;
    uint8_t buf[10];
    CS_INDEX *index_pool = NULL;
    
    index_pool = roads_vol_index_table;
    n = ARRAY_SIZE(roads_vol_index_table);
    
    transform_output_to_str(buf);
    
    for(i = 0; i < n; i++)
    {
        update_text_ele(index_pool[i], win, buf);
    }
}
/**
  * @brief  多路上限文本对象信息索引表
  */
static CS_INDEX roads_upper_index_table[]=
{
    TEST_UI_ROAD01_UPPER,
    TEST_UI_ROAD02_UPPER,
    TEST_UI_ROAD03_UPPER,
    TEST_UI_ROAD04_UPPER,
};
/**
  * @brief  转换各模式上限值为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_upper_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t unit = 0;
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
            unit = ac_gear[range].unit;
            format = 100 + 10 * lon + dec;
            value = g_cur_step->one_step.acw.upper_limit;
            break;
        }
        case DCW:
        {
            range = g_cur_step->one_step.dcw.range;
            dec = dc_gear[range].dec;
            lon = dc_gear[range].lon;
            unit = dc_gear[range].unit;
            format = 100 + 10 * lon + dec;
            
            value = g_cur_step->one_step.dcw.upper_limit;
            break;
        }
        case IR:
        {
            cur_range = IR_10MOHM;
            dec = ir_gear[cur_range].dec;
            lon = ir_gear[cur_range].lon;
            unit = ir_gear[cur_range].unit;
            format = 100 + 10 * lon + dec;
            
            value = g_cur_step->one_step.ir.lower_limit * ten_power(dec);
            break;
        }
        case GR:
        {
            dec = 1;
            lon = 5;
            unit = RES_U_mOHM;
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
    
    mysprintf_2(buf, unit_pool[unit], format, value);
}
/**
  * @brief  更新测试窗口的上限值的文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void updata_test_win_test_upper(MYUSER_WINDOW_T* win)
{
    uint8_t n = 0;
    int32_t i = 0;
    uint8_t buf[10];
    CS_INDEX *index_pool = NULL;
    
    transform_upper_to_str(buf);
    index_pool = roads_upper_index_table;
    n = ARRAY_SIZE(roads_upper_index_table);
    
    for(i = 0; i < n; i++)
    {
        update_text_ele(index_pool[i], win, buf);
    }
}
/**
  * @brief  多路真实电流文本对象信息索引表
  */
static CS_INDEX roads_real_index_table[]=
{
    TEST_UI_ROAD01_REAL,
    TEST_UI_ROAD02_REAL,
    TEST_UI_ROAD03_REAL,
    TEST_UI_ROAD04_REAL,
};

/**
  * @brief  转换ACW真实电流值为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_real_cur_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t unit = 0;
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
            unit = ac_gear[range].unit;
            format = 100 + 10 * lon + dec;
            
            if(g_cur_step->one_step.acw.real_cur == 0)
            {
                buf[0] = 0;
            }
            else
            {
                mysprintf_2(buf, unit_pool[unit], format, g_cur_step->one_step.acw.real_cur);
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
  * @brief  更新测试窗口的真实电流值的文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void updata_test_win_test_real(MYUSER_WINDOW_T* win)
{
    uint8_t n = 0;
    int32_t i = 0;
    uint8_t buf[10];
    CS_INDEX *index_pool = NULL;
    
    transform_real_cur_to_str(buf);
    index_pool = roads_real_index_table;
    n = ARRAY_SIZE(roads_real_index_table);
    
    for(i = 0; i < n; i++)
    {
        update_text_ele(index_pool[i], win, buf);
    }
}
/**
  * @brief  多路测试时间文本对象信息索引表
  */
static CS_INDEX roads_time_index_table[]=
{
    TEST_UI_ROAD01_TIME,
    TEST_UI_ROAD02_TIME,
    TEST_UI_ROAD03_TIME,
    TEST_UI_ROAD04_TIME,
};
/**
  * @brief  转换测试时间为显示字符串
  * @param  [in] buf 字符串缓冲区
  * @retval 无
  */
static void transform_test_time_to_str(uint8_t *buf)
{
    uint8_t mode = 0;
    uint8_t dec = 0;
    uint8_t unit = 0;
    uint8_t lon = 0;
    uint8_t format = 0;
    uint16_t time = 0;
    
    mode = g_cur_step->one_step.com.mode;
    
    dec = 1;
    lon = 5;
    unit = TIM_U_s;
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
    
    mysprintf_2(buf, unit_pool[unit], format, time);
}
/**
  * @brief  更新测试窗口的测试时间的文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void updata_test_win_test_time(MYUSER_WINDOW_T* win)
{
    uint8_t n = 0;
    int32_t i = 0;
    uint8_t buf[10];
    CS_INDEX *index_pool = NULL;
    
    index_pool = roads_time_index_table;
    n = ARRAY_SIZE(roads_time_index_table);
    transform_test_time_to_str(buf);
    
    for(i = 0; i < n; i++)
    {
        update_text_ele(index_pool[i], win, buf);
    }
}
/**
  * @brief  多路状态显示文本对象信息索引表
  */
static CS_INDEX roads_status_index_table[]=
{
    TEST_UI_ROAD01_STATUS,
    TEST_UI_ROAD02_STATUS,
    TEST_UI_ROAD03_STATUS,
    TEST_UI_ROAD04_STATUS,
};
/**
  * @brief  更新测试窗口的测试状态的文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void updata_test_win_test_status(MYUSER_WINDOW_T* win)
{
    uint8_t n = 0;
    int32_t i = 0;
    uint8_t buf[10];
    CS_INDEX *index_pool = NULL;
    
    index_pool = roads_status_index_table;
    n = ARRAY_SIZE(roads_status_index_table);
    
    strcpy((char*)buf, (const char*)status_str[cur_status][SYS_LANGUAGE]);
    
    for(i = 0; i < n; i++)
    {
        update_text_ele(index_pool[i], win, buf);
    }
}
/**
  * @brief  更新测试窗口中的多路测试文本对象显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void update_test_win_text_ele_text(MYUSER_WINDOW_T* win)
{
    updata_test_win_test_mode(win);//更新测试模式
    updata_test_win_test_vol(win);//更新测试电流
    updata_test_win_test_upper(win);//更新上限
    updata_test_win_test_real(win);//更新真实电流
    updata_test_win_test_time(win);//更新测试时间
    updata_test_win_test_status(win);//更新测试状态
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
                clear_range_text_ele(g_cur_win);
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
