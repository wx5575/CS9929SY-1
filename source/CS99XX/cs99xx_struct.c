/**
  ******************************************************************************
  * @file    cs99xx_struct.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   仪器相关结构体定义初始化
  ******************************************************************************
  */

#define STRUCT_GLOBALS
#include "cs99xx_struct.h"
#include "string.h"
#include "rtc_config.h"
#include "cs99xx_type.h"


//去除870# warning 中文遇到这个问题？因为ruby是采用US-ASCII，所以才会出现这种问题
#pragma  diag_suppress 870 

/**
  * @brief  单位信息池
  */
const uint8_t *unit_pool[]=
{
	"","s","mV","V","kV","μA","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","μF","Hz","%",
};
/**
  * @brief  工作模式
  */
const uint8_t *work_mode_pool[2]={"N","G"};
/**
  * @brief  语言
  */
const uint8_t *language_pool[2]={CHINESE_STR, ENGLISH_STR};

/**
  * @brief  开关
  */
const uint8_t *sw_pool[2][2]={
    {"关闭","开启"},
    {"OFF","ON"},
};
const SW_STATUS sw_status_buf[2] = 
{
    SW_OFF,
    SW_ON,
};
/**
  * @brief  电弧侦测模式
  */
const uint8_t *arc_mode_pool[2][2]={
    {"电流","等级"},
    {"Current","Grade"},
};
/**
  * @brief  测试端口
  */
const uint8_t *test_port_pool[2][2]={
    {"接地","浮地"},
    {"GND","Float"},
};
const uint8_t test_port_flag_pool[2]=
{
    TEST_PORT_GND,///< 测试端口接地
    TEST_PORT_FLOAT,///< 测试端口浮地
};
/**
  * @brief  测试模式
  */
const uint8_t * mode_pool[10]=
{"", ACW_STR, DCW_STR, IR_STR, GR_STR, BBD_STR, CC_STR};
/**
  * @brief  测试状态
  */
const char* status_str[][2] = 
{
	{"等待测试", "TestWait"},
	{"电压上升", "  Rise  "},
	{"正在测试", "Testing "},
	{"电压下降", "  Fall  "},
	{"电压缓变", "Change  "},
	{"间隔等待", "Interval"},
	{"测试合格", "  PASS  "},
	{"测试失败", "TestFail"},
	{"正在充电", " Charge "},
	{"稳压时间", "Sta.Time"},
	{"正在放电", "Discharg"},
	{"放电结束", "Disch.OK"},
	{"输出延时", "Out.Dly"},
};
/**
  * @brief  测试异常状态
  */
const char* except_buf[][3]=
{
	{"","",""},
	{"上限报警", "High Fail", "High"   },
	{"下限报警", "Low Fail" , "Low"    },
	{"真实报警", "Real Fail", "Real"   },
	{"充电报警", "Char Fail", "Charge" },
	{"开路报警", "Open Fail", "Open"   },
	{"短路报警", "ShortFail", "Short"  },
	{"ARC报警" , "ARC Fail" , "ARC"    },
	{"GFI报警" , "GFI Fail" , "GFI"    },
	{"功放报警", "AMP Fail" , "AMP"    },
	{"档位报警", "GEAR Fail", "GEAR"   },
	{"电压异常", "VOL. Fail", "VOL."   },
	{"未定义"  , "Undefine ", "Undef." },
};
/**
  * @brief  AC档位信息
  */
GEAR_STR ac_gear[]=
{
    {0},//0
    {CUR_2uA_STR   , CUR_U_uA, 2000, 1, 2000, 0, 5, 3},//2uA
    {CUR_20uA_STR  , CUR_U_uA, 2000, 1, 2000, 0, 5, 2},//20uA
    {CUR_200uA_STR , CUR_U_uA, 2000, 1, 2000, 0, 5, 1},//200uA
    {CUR_2mA_STR   , CUR_U_mA, 2000, 1, 2000, 0, 5, 3},
    {CUR_10mA_STR  , CUR_U_mA, 1000, 1, 1000, 0, 5, 2},
    {CUR_20mA_STR  , CUR_U_mA, 2000, 1, 2000, 0, 5, 2},
    {CUR_50mA_STR  , CUR_U_mA, 5000, 1, 5000, 0, 5, 2},
    {CUR_100mA_STR , CUR_U_mA, 1000, 1, 1000, 0, 5, 1},
    {CUR_200mA_STR , CUR_U_mA, 2000, 1, 2000, 0, 5, 1},
    {CUR_2A_STR    , CUR_U_A , 1250, 1, 1250, 0, 5, 3},//2A
};
/**
  * @brief  DC档位信息
  */
GEAR_STR dc_gear[]=
{
    {0},//0
    {CUR_2uA_STR   , CUR_U_uA, 2000, 1, 2000, 0, 5, 3},//2uA
    {CUR_20uA_STR  , CUR_U_uA, 2000, 1, 2000, 0, 5, 2},//20uA
    {CUR_200uA_STR , CUR_U_uA, 2000, 1, 2000, 0, 5, 1},//200uA
    {CUR_2mA_STR   , CUR_U_mA, 2000, 1, 2000, 0, 5, 3},
    {CUR_10mA_STR  , CUR_U_mA, 1000, 1, 1000, 0, 5, 2},
    {CUR_20mA_STR  , CUR_U_mA, 2000, 1, 2000, 0, 5, 2},
    {CUR_50mA_STR  , CUR_U_mA, 5000, 1, 5000, 0, 5, 2},
    {CUR_100mA_STR , CUR_U_mA, 1000, 1, 1000, 0, 5, 1},
};
/**
  * @brief  IR档位信息
  */
GEAR_STR ir_gear[]=
{
    {0},//0
    {"" , RES_U_MOHM, 0, 0, 0, 0, 5, 3},//IR_10MOHM
    {"" , RES_U_MOHM, 0, 0, 0, 0, 5, 2},//IR_100MOHM
    {"" , RES_U_MOHM, 0, 0, 0, 0, 5, 1},//IR_1GOHM
    {"" , RES_U_GOHM, 0, 0, 0, 0, 5, 3},//IR_10GOHM
    {"" , RES_U_GOHM, 0, 0, 0, 0, 5, 2},//IR_100GOHM
};
/**
  * @brief  默认文件定义
  */
TEST_FILE default_file = 
{
	0,///<文件编号
	"DEFAULT",///<文件名
	N_MODE,	///<工作模式 N模式 G模式
	0,///< 总测试步
	10,///<蜂鸣时间
	0,///<PASS时间
	ARC_CUR_MODE,///<电弧侦测模式
  0,///<存放日期时间 xxxx.xx.xx xx:xx:xx
};

/************************ (C) COPYRIGHT Nanjing Changsheng 2017 *****END OF FILE****/
