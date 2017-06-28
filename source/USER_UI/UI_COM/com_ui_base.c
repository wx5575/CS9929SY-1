/**
  ******************************************************************************
  * @file    com_ui_base.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   界面显示公共数据定义
  ******************************************************************************
  */
#include "com_ui_base.h"

//去除870# warning 中文遇到这个问题？因为ruby是采用US-ASCII，所以才会出现这种问题
#pragma  diag_suppress 870 

/**
  * @brief  单位信息池
  */
const uint8_t *unit_pool[]=
{
	"","s","mV","V","kV","uA","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","uF","Hz","%",
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
/**
  * @brief  电弧侦测模式
  */
const uint8_t *arc_mode_pool[2][2]={
    {"电流","等级"},
    {"Current","Grade"},
};
/**
  * @brief  测试模式
  */
const uint8_t * mode_pool[10]=
{"", ACW_STR, DCW_STR, IR_STR, GR_STR, BBD_STR, CC_STR};

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

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
