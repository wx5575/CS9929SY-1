/**
  ******************************************************************************
  * @file    cs99xx_struct.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   仪器相关结构体定义
  ******************************************************************************
  */

#ifndef __CS99XX_STRUCT_H__
#define __CS99XX_STRUCT_H__

#include "tools.h"
#include "cs_list.h"

#define MAX_STEPS           (99)            ///<单个文件容纳最大步数
#define TEST_PORTS_MAX      (8 * 2)         ///<测试端口的最大数目 必须是8的倍数 
#define MAX_FILES			((uint16_t)31)  ///< 最大文件数目包括一个默认的文件 
#define NAME_LON			((uint16_t)14)  ///< 文件名的长度 



#define MAX_RESULT_NUM      ((uint16_t)8000) ///<最大的结果条数8000条
#define PWD_MAX_LEN         8               ///<密码的最大长度
#define DEFAULT_PWD         "888888"        ///<系统默认密码

/** 
  * @brief 用户数组对象索引,只提供类型，用来将用户的索引统一
  */
typedef   int32_t  CS_INDEX;
/**
  * @brief  模块地址
  */
typedef uint8_t MODULE_ADDR_T;
/**
  * @brief  多路端口的索引编号
  */
typedef uint8_t ROAD_INDEX_NUM_T;
/**
  * @brief  多路端口的编号
  */
typedef uint8_t ROAD_NUM_T;
/**
  * @brief  多路端口的索引
  */
typedef enum{
    INDEX_ROAD_1 = 1,///<第1路索引
    INDEX_ROAD_2,///<第2路索引
    INDEX_ROAD_3,///<第3路索引
    INDEX_ROAD_4,///<第4路索引
    INDEX_ROAD_5,///<第5路索引
    INDEX_ROAD_6,///<第6路索引
    INDEX_ROAD_7,///<第7路索引
    INDEX_ROAD_8,///<第8路索引
}ROAD_INDEX;
#define MAX_SYN_ROADS_NUM   8 ///<最大的同步测试路数
/**
  * @brief  bool常量定义
  */
typedef enum{
	CS_FALSE,
	CS_TRUE,
}CS_BOOL;
/**
  * @brief  系统错误码定义
  */
typedef enum{
    CS_ERR_NONE = 0x1000,///<正常
    CS_ERR_FILE_NO_EXIST,///<文件不存在
    CS_ERR_FILE_NUM_INVALID,///<文件编号无效
    CS_ERR_FILE_NAME_INVALID,///<文件名无效
    CS_ERR_STRING_TOO_LONG,///<字符串太长
    CS_ERR_DATE_STR_TOO_LONG,///<日期字符串太长
    CS_ERR_DATA_OUT_OF_RANGE,///<数据越界
    CS_ERR_INDEX_INVALID,///<索引值无效
    CS_ERR_KEY_VALUE_INVALID,///<键值无效
    CS_ERR_ELE_INDEX_INVALID,///<对象索引无效
    CS_ERR_TEST_MODE_INVALTD,///<测试模式非法
    
    CS_ERR_COM_BUSY,///<串口忙
    CS_ERR_SEND_SUCCESS,///<发送成功
    CS_ERR_SEND_FAIL,///<发送失败
    CS_ERR_COMM_TROUBLE,///<通信故障或从机不存在
    CS_ERR_COMM_ADDR_INVALTD,///<通信地址无效
    CS_ERR_COMM_NORMAL,///<通信正常
    CS_ERR_COMM_CRC_ERR,///<通信CRC校验错误
    CS_ERR_COMM_SLAVE_ERR,///<从机通信错误
    CS_ERR_ROAD_INVALTD,///<路号不存在
}CS_ERR;
/**
  * @brief  文件编号
  */
typedef uint16_t FILE_NUM;
/**
  * @brief  步骤编号
  */
typedef uint16_t STEP_NUM;

/**
  * @brief  定义8个测试端口数据
  */
typedef struct {
	uint16_t port1:2;
	uint16_t port2:2;
	uint16_t port3:2;
	uint16_t port4:2;
	uint16_t port5:2;
	uint16_t port6:2;
	uint16_t port7:2;
	uint16_t port8:2;
}_TEST_PORT;
/**
  * @brief  测试端口结构定义
  */
typedef struct{
    _TEST_PORT ports[TEST_PORTS_MAX / 8];
    uint8_t num;
    uint8_t mode;/* 通信时使用 */
}TEST_PORT;

/**
  * @brief  工作端口结构定义，针对多路同步测试设计，用来指定模块是否参与工作
  */
typedef struct{
    _TEST_PORT ports[TEST_PORTS_MAX / 8];
    uint8_t num;
    uint8_t mode;/* 通信时使用 */
}WORK_PORT;
/**
  * @brief  ACW数据结构定义
  */
typedef struct {
	STEP_NUM 	step;                       ///< 测试步 */
	uint8_t 	mode;                       ///< 模式 */
	uint16_t 	output_vol;            ///< 测试电压 */
	uint8_t 	range;                      ///< 电流档位  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				///< 电流上限 */
	uint16_t	lower_limit;				///< 电流下限 */
	uint16_t	real_cur;                ///< 真实电流上限值 */
	uint16_t	arc_sur;					///< 电弧侦测 可能是等级模式即整数 可能为电流模式即为浮点数 这个有电弧侦测模式决定 */
	uint16_t	output_freq;				///< 输出频率选择 可以连续也可以是等级 */
	uint16_t	rise_time;                  ///< 上升时间 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	fall_time;                  ///< 下降时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;					///< 步间pass */
	uint8_t 	step_con;					///< 步间连续 */
	TEST_PORT	port;                       ///< 端口 */
    WORK_PORT   work_port;                  ///< 工作端口，标示出参与工作的端口
	
	uint16_t	offset_cur;                 ///< 偏移电流 */
	uint16_t 	offset_real;                ///< 偏移真实电流 */
	uint8_t 	offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}ACW_STRUCT;

/**
  * @brief  CC数据结构定义
  */
typedef struct {
	STEP_NUM 	step;                       ///< 测试步 */
	uint8_t 	mode;                       ///< 模式 */
	uint16_t 	output_vol;            ///< 测试电压 */
	uint8_t 	range;                      ///< 电流档位  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				///< 电流上限 */
	uint16_t	lower_limit;				///< 电流下限 */
	uint16_t	real_cur;                ///< 真实电流上限值 */
	uint16_t	cur_intensity;              ///< 电流强度 高 中 低 */
	uint16_t	output_freq;      			///< 输出频率选择 可以连续也可以是等级 */
	uint16_t	rise_time;                  ///< 上升时间 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	fall_time;                  ///< 下降时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;         		///< 步间pass */
	uint8_t 	step_con; 				///< 步间连续 */
	TEST_PORT	port;                       ///< 端口 */
    WORK_PORT   work_port;                  ///< 工作端口，标示出参与工作的端口
	
	uint16_t	offset_cur;                 ///< 偏移电流 */
	uint16_t 	offset_real;                ///< 偏移真实电流 */
	uint8_t 	offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}CC_STRUCT;

/**
  * @brief  DCW数据结构定义
  */
typedef struct {
	STEP_NUM     step;                      ///< 测试步 */
	uint8_t     mode;                       ///< 测试步 */
	uint16_t 	output_vol;            ///< 测试电压 */
	uint8_t 	range;                      ///< 电流档位   2uA ,  20uA , 200uA ,  2mA,   10mA */
	uint16_t	upper_limit;				///< 电流上限 */
	uint16_t	lower_limit;				///< 电流下限 */
	uint16_t	charge_cur;         		///< 充电电流 */
	uint16_t	arc_sur;   					///< 电弧侦测 */
    uint8_t     output_impedance;           ///< 输出阻抗 */
	uint16_t	delay_time;                 ///< 延时时间 延时时间到了才判断 */
	uint16_t	rise_time;                  ///< 上升时间 */
	uint16_t	stab_time;                  ///< 稳压时间 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	fall_time;                  ///< 下降时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;         		///< 步间pass */
	uint8_t		step_con; 				///< 步间连续 */
	TEST_PORT	port;                       ///< 端口 */
    WORK_PORT   work_port;                  ///< 工作端口，标示出参与工作的端口
	
	uint16_t	offset_cur;                 ///< 偏移电流 */
	uint8_t 	offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}DCW_STRUCT;

/**
  * @brief  IR数据结构定义
  */
typedef struct {
	STEP_NUM 	step;                       ///< 测试步 */
	uint8_t 	mode;                       ///< 模式 */
	uint16_t 	output_vol;			///< 输出电压 */
	uint8_t 	auto_shift;           ///< 自动换挡 */
	uint32_t 	upper_limit;     			///< 电阻上限 */
	uint32_t 	lower_limit;	    		///< 电阻下限 */
	uint16_t	rise_time;	                ///< 上升时间 */
	uint16_t	test_time;	            ///< 测试时间 */
	uint16_t	delay_time;	                ///< 延时时间 */
	uint16_t	inter_time;	            ///< 间隔时间 */
	uint8_t 	step_pass;	        		///< 步间pass */
	uint8_t 	step_con;					///< 步间连续 */
	TEST_PORT	port;	                    ///< 端口 */
    WORK_PORT   work_port;                  ///< 工作端口，标示出参与工作的端口
}IR_STRUCT;

/**
  * @brief  GR数据结构定义
  */
typedef struct {
	STEP_NUM 	step;
	uint8_t 	mode;
	uint8_t		voltage_gear;               ///< 电压档位 1 20mv 2 200mv 3 2000mv */
	uint16_t	output_cur;                ///< 输出电流 */
	uint16_t 	upper_limit;				///< 电阻上限 */
	uint16_t 	lower_limit;				///< 电阻下限 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;					///< 步间pass */
	uint8_t 	step_con; 				///< 步间连续 */
	uint16_t	output_freq;				///< 输出频率 */
	uint16_t	test_method;                ///< 当脉冲测试模式使能了以后 0表示脉冲测试 1表示连续测试 */
	WORK_PORT   work_port;                  ///< 工作端口，标示出参与工作的端口
    
	uint16_t	offset_res;                 ///< 偏移电阻 */
	uint8_t	    offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}GR_STRUCT;

/**
  * @brief  BBD数据结构定义
  */
typedef struct {
	STEP_NUM 	step; ///< 设置步 */
	uint8_t 	mode; ///< 模式 */
	uint16_t 	output_vol; ///< 输出电压 */
	uint16_t	open_ratio; ///< 开路比例 0，10%-100% */
	uint16_t	short_ratio; ///< 短路比例 100% - 500% */
	uint16_t 	upper_limit; ///< 电容上限 */
	uint16_t 	lower_limit; ///< 电容下限 */
	uint16_t	output_freq; ///<输出频率 */
	uint16_t	test_time; ///< 测试时间 */
	uint16_t	inter_time; ///< 间隔时间 */
	uint8_t 	step_pass; ///< 步间pass */
	uint8_t 	step_con; ///< 步间连续 */
	float		cap_value; ///< 电容值 */
	uint8_t 	gear; ///< 保存获取电容值时的电流档位 默认 20mA */
	TEST_PORT	port; ///< 端口 */
	WORK_PORT   work_port;                  ///< 工作端口，标示出参与工作的端口
    
	uint8_t 	get_cs_ok; ///< 标记获取电容已经OK */
    
	float   	offset_cap; ///<偏移电阻 */
	uint8_t	    offset_result; ///<偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}BBD_STRUCT;

/**
  * @brief  公共数据结构定义
  */
typedef struct {
	STEP_NUM step; ///< 设置步
	uint8_t mode; ///< 模式
}STEP_COM_INF;

/**
  * @brief  将每一步都定义为一个联合体实现统一操作
  */
typedef union{
	STEP_COM_INF com;///<公共参数
	ACW_STRUCT 	 acw;///<ACW参数
	DCW_STRUCT 	 dcw;///<DCW参数
	IR_STRUCT 	 ir;///<IR参数
	GR_STRUCT 	 gr;///<GR参数
	BBD_STRUCT   bbd;///<BBD参数
    CC_STRUCT    cc;///<CC参数
}UN_STRUCT,*PUN_STRUCT,**PPUN_STRUCT;

/**
  * @brief  步骤节点结构定义
  */
typedef struct Node_Step{
	UN_STRUCT 	one_step;///<步骤结构数据
	CS_LIST     list;///<链表
}NODE_STEP;
/**
  * @brief  测试步骤缓冲区
  */
#define STEP_LIST_SIZE      ((uint16_t)5) /* 存放最近连续步的个数 */
typedef struct TestGroup{
	NODE_STEP test_steps[STEP_LIST_SIZE];
}TESTGROUP;

/**
  * @brief  测试模式的枚举定义
  */
typedef enum{
    NUL     = 0,///<空
    ACW     = 1,///<ACW 模式
    DCW     = 2,///<DCW 模式
    IR      = 3,///<IR 模式
    GR      = 4,///<GR 模式
    BBD     = 5,///<BBD 模式
    CC      = 6,///<CC 模式
    MODE_END,
}MODE_ENUM;

/**
  * @brief  工件模式的枚举定义
  */
typedef enum{
	G_MODE,///<G模式
	N_MODE,///<N模式
}WORK_MODE_T;

/**
  * @brief  ARC 侦测枚举定义
  */
enum{
	ARC_CUR_MODEL 	= 0,    ///<电流模式
	ARC_GRADE_MODEL = 1,    ///<等级模式
};
/**
  * @brief  测试连接方法
  */
enum{
	FLOAT_MODE	= 0,        ///<浮空
	GND_MODE	= 1,        ///<接地
};
/**
  * @brief  GFI 开关
  */
enum{
	GFI_OFF		= 0,        ///<GFI关闭
	GFI_ON		= 1,        ///<GFI打开
};
/**
  * @brief  PLC 开关
  */
enum{
	PLC_OFF		= 0,        ///<PLC关闭
	PLC_ON		= 1,        ///<PLC打开
};
/**
  * @brief  电弧侦测模式
  */
typedef enum{
	ARC_CUR_MODE,           ///<电弧侦测电流模式
	ARC_GRADE_MODE,         ///<电弧侦测档位模式
}ARC_MODE;
/**
  * @brief  直流GR的测试方法
  */
enum{
	DC_GR_PULSE = 0,        ///<直流接地脉冲测试
	DC_GR_CONT	= 1,        ///<直流接地连续测试
};
/**
  * @brief  AC档位枚举定义
  */
enum{
	AC_2uA		= 0X1,
	AC_20uA		= 0X2,
	AC_200uA	= 0X3,
	AC_2mA		= 0X4,
	AC_10mA		= 0X5,
	AC_20mA		= 0X6,
	AC_50mA		= 0x7,
	AC_100mA	= 0X8,
	AC_200mA	= 0X9,
	AC_2A	    = 0XA,
    AC_GEAR_END,
};
#define CUR_2uA_STR     "2uA"
#define CUR_20uA_STR    "20uA"
#define CUR_200uA_STR   "200uA"
#define CUR_2mA_STR     "2mA"
#define CUR_10mA_STR    "10mA"
#define CUR_20mA_STR    "20mA"
#define CUR_50mA_STR    "50mA"
#define CUR_100mA_STR   "100mA"
#define CUR_200mA_STR   "200mA"
#define CUR_2A_STR      "2A"



/**
  * @brief  DC档位枚举定义
  */
enum{
	DC_2uA		= 0X1,
	DC_20uA		= 0x2,
	DC_200uA	= 0X3,
	DC_2mA		= 0x4,
	DC_10mA		= 0X5,
	DC_20mA		= 0X6,
	DC_50mA		= 0X7,
	DC_100mA	= 0X8,
    DC_GEAR_END,
};
/**
  * @brief  IR档位枚举定义
  */
enum{
	IR_10MOHM	= 0X01,
	IR_100MOHM	= 0X02,
	IR_1GOHM	= 0x03,
	IR_10GOHM	= 0X04,
	IR_100GOHM	= 0X05,
};
/**
  * @brief  DC-GR电压档位枚举定义
  */
enum{
	DC_GR_20mV	= 0x2,
	DC_GR_200mV	= 0x3,
	DC_GR_2000mV= 0X4,
	DC_GR_12V	= 0X5,
	DC_GR_12V_100mA  = 0x6,
	DC_GR_12V_1000mA = 0x7,
};
/**
  * @brief  AC通信使用的档位枚举定义
  */
enum{
	AC_0_GEAR_COMM	= 0X0,
	AC_1_GEAR_COMM	= 0X1,
	AC_2_GEAR_COMM	= 0X2,
	AC_3_GEAR_COMM	= 0X3,
};
/**
  * @brief  DC通信使用的档位枚举定义
  */
enum{
	DC_0_GEAR_COMM	= 0X0,
	DC_1_GEAR_COMM	= 0X1,
	DC_2_GEAR_COMM	= 0X2,
	DC_3_GEAR_COMM	= 0X3,
	DC_4_GEAR_COMM	= 0X4,
	DC_5_GEAR_COMM	= 0X5,
};
/**
  * @brief  IR通信使用的档位枚举定义
  */
enum{
	IR_1_GEAR_COMM	= 0X1,
	IR_2_GEAR_COMM	= 0X2,
	IR_3_GEAR_COMM	= 0X3,
	IR_4_GEAR_COMM	= 0X4,
	IR_5_GEAR_COMM	= 0X5,
	IR_6_GEAR_COMM	= 0X6,
};
/**
  * @brief  大电容测试输出阻抗
  */
typedef enum{
    OUTPUT_IMPEDAN_OPT_1,///<第一组阻抗
    OUTPUT_IMPEDAN_OPT_2,///<第二组阻抗
}OUTPUT_IMPEDAN_OPTS;
/**
  * @brief  数据的单位枚举定义
  */
typedef enum{
    NULL_U_NULL,///<无单位
	TIM_U_s,///<秒
    VOL_U_mV,///<mV
    VOL_U_V,///< V
    VOL_U_kV,///< kV
    CUR_U_uA,///< uA
    CUR_U_mA,///< mA
    CUR_U_A,///< A
    RES_U_mOHM,///< mOhm
    RES_U_OHM,///< ohm
    RES_U_MOHM,///< Mohm
    RES_U_GOHM,///< Gohm
    CAP_U_pF,///< pF
    CAP_U_nF,///< nF
    CAP_U_uF,///< uF
    FREQ_U_Hz,///< Hz
    U_PER_CENT,///< (%) 
}UNIT_T, CS_UNIT_ENUM;

/**
  * @brief  文件结构定义
  */
typedef struct{
	FILE_NUM num;///<文件编号
	uint8_t name[NAME_LON + 1];///<文件名
	WORK_MODE_T work_mode;///<工作模式 N模式 G模式
	uint16_t total;///< 总测试步
	uint16_t buzzer_time;///<蜂鸣时间
	uint16_t pass_time;///<PASS时间
    ARC_MODE arc_mode;///<电弧侦测模式
	uint8_t date[22];///<存放日期时间 xxxx.xx.xx xx:xx:xx
}TEST_FILE;
/**
  * @brief  屏幕尺寸枚举定义
  */
typedef enum{
	SCREEN_4_3INCH,///<4.3寸屏
	SCREEN_6_5INCH,///<5.6寸屏
	SCREEN_7INCH,///<7寸屏
    SCREEN_NUM,///<支持的屏幕个数
}SCREEM_SIZE_T;
/**
  * @brief  显示布局枚举定义
  */
typedef enum{
	COMPOSITION_1,///<布局1
	COMPOSITION_2,///<布局2
	COMPOSITION_3,///<布局3
}UI_LAYOUT_T;
/**
  * @brief  系统支持的语言枚举定义
  */
typedef enum{
    CHINESE,///<中文
    ENGLISH,///<英文
    LANGUAGE_NUM,///<语言个数
}LANGUAGE_T;
/**
  * @brief  开关变量状态枚举定义
  */
typedef enum{
    SW_OFF,
    SW_ON,
}SW_STATUS;
/**
  * @brief  测试端口 接地 浮地
  */
typedef enum{
    TEST_PORT_GND,///< 测试端口接地
    TEST_PORT_FLOAT,///< 测试端口浮地
}TEST_PORT_GND_FLOAT;

/**
  * @brief  系统参数结构定义
  */
typedef struct Sys_Parameter{
	uint8_t contrast;			///< 液晶对比度(0-9)
	uint8_t allowance;			///< 余量提示 (0-9)
	uint8_t is_save_res;		///< 是否保存结果，0不保存，1保存
	uint8_t is_overflow_cover;	///< 是否溢出覆盖
	
	uint8_t is_gfi_protect;		///< 是否GFI保护
	uint8_t is_self_check;		///< 是否打开开机自检功能
	uint8_t language;			///< 语言切换，1中文，0英文
	uint8_t is_falt_continue;	///< 失败继续是否失败继续，0不继续，1继续
    uint8_t fail_mode;          ///< 失败模式
	uint8_t test_method;		///< 测试方法 1接地 0浮地 
	uint8_t test_level;			///< 测试电平 1电平 0触点 
	
	char 	data_of_survey[21];	///< 检验时间 
	uint8_t is_table_dis;		///< 是否打开列表显示功能 0，关闭，1打开 
	uint8_t num_rule;			///< 编号规则 (0-2) 
	uint8_t buzzer_sw;			///< 蜂鸣器开关 
	
	uint8_t commu_on_off;		///< 通信开关 1打开 0关闭
	uint8_t baud_rate_gear;		///< 波特率：1表示9600 2表示14400 3表示19200 
	uint8_t local_addr;			///< 本机地址 (1-255) 
    
	uint8_t plc_on_off;			///< PLC开关 1打开，0关闭 
	char password[PWD_MAX_LEN+1];	///< 密码最多8位，最少1位 
	
	uint16_t used_res_num;		///< 已经记录的结果总数 用于统计结果
	uint16_t pass_res_num;		///< 合格的数 用于统计结果 
	uint16_t cover_res_num;		///< 当used_res_num记满n条后就如果允许覆盖就启动该计数变量计数 当其记满4500条后清零 
	
	uint8_t key_lock;		    ///< 键盘锁 
	
    char dut_name[NAME_LON+1];	///< 被测件名 
	uint8_t silent_sw;			///< 静音开关 
    uint8_t plc_signal;         ///< plc 信号 当失败继续打开后如果plc_signal=each_step 表示每步都会发Plc信号 \
                                   最后一步也是，如果 plc_signal=all_steps 表示最后一步不单独发plc信号 \
                                   而是做一个综合判断 
    uint16_t output_delay;		///< 输出延时 用户要求启动后等待一段时间在输出高压 
    uint16_t ir_gear_hold;		///< IR切换档位延时 
	uint8_t ir_speed_sw;		///< IR测试速度 快 中 慢 通过设置滤波深度实现 
    uint8_t start_way;			///< 仪器的启动方式 组合启动与通信和PLC都发出信号才会启动仪器 
}SYS_PAR;

#define SYN_MAX_COM_NUM   4 ///<同步测试使用的串口个数

/**
  * @brief  管理所有路的信息为了加快通过路号对各模块地址的查找将所有的有效地址依次存入
  *         这个结构中
  */
typedef struct
{
    uint8_t road_buf[16];///<串口上挂的模块的地址
    uint8_t count;///<串口挂的模块总个数
}_ROADS_FLAG;
/**
  * @brief  所有串口上挂接的模块的信息结构定义
  */
typedef struct{
    _ROADS_FLAG     flag[SYN_MAX_COM_NUM];
}ROADS_FLAG;
/**
  * @brief  系统标志结构定义
  */
typedef struct{
	uint32_t power_on_count;///<开机次数计数
	uint32_t mem_init;///<存储器初始化标志 非零时要初始化
	FILE_NUM last_file_num;///< 最近使用的记忆组文件编号
}SYS_FLAG;
/**
  * @brief  档位信息结构定义
  */
typedef struct {
    uint8_t* name;///<档位名称
    UNIT_T unit;///<单位
    uint32_t high_max;///<档位的上限最大值
    uint32_t high_min;///<档位的上限最小值
    uint32_t low_max;///<档位的下限最大值
    uint32_t low_min;///<档位的下限最小值
    uint8_t lon;///<最大长度，包括小数点
    uint8_t dec;///<档位的小数点位数
}GEAR_STR;

#ifdef   STRUCT_GLOBALS
#define  STRUCT_EXT
#else
#define  STRUCT_EXT  extern
#endif

#define     TABLE_VALUE_NULL                ((uint16_t)0xffff)




//单位
extern const uint8_t *unit_pool[];
/******************************/
extern const uint8_t *work_mode_pool[2];

#define CHINESE_STR "中文"
#define ENGLISH_STR "English"
extern const uint8_t *language_pool[2];
extern const uint8_t *sw_pool[2][2];
extern const SW_STATUS sw_status_buf[2];
extern const uint8_t *arc_mode_pool[2][2];
extern const uint8_t arc_mode_buf[2];
extern const uint8_t *test_port_pool[2][2];
extern const uint8_t test_port_flag_pool[2];


enum{
	TEST_WAIT_INDEX,
	TEST_RISE_INDEX,
	TEST_TEST_INDEX,
	TEST_FALL_INDEX,
	TEST_CHANGE_INDEX,
	TEST_INTER_INDEX,
	TEST_PASS_INDEX,
	TEST_FAIL_INDEX,
	TEST_CHARGE_INDEX,
	TEST_STAB_INDEX,
	TEST_DISCHARGE_INDEX,
	TEST_DISCHARGE_OVER_INDEX,
    TEST_OUTPUT_DELAY,
    
    ERR_HIGH_INDEX  ,    /* 电流上限报警 */
    ERR_LOW_INDEX   ,    /* 电流下限报警 */
    ERR_REAL_INDEX  ,    /* 真实电流报警 */
    ERR_CHARGE_INDEX,    /* 充电报警 */
    ERR_OPEN_INDEX  ,    /* 开路报警 */
    ERR_SHORT_INDEX ,    /* 短路报警 */
    ERR_ARC_INDEX   ,    /* ARC 报警 */
    ERR_GFI_INDEX   ,    /* GFI 报警 */
    ERR_AMP_INDEX   ,    /* 功放报警 */
    ERR_GEAR_INDEX  ,   /* 档位报警 */
    ERR_VOL_INDEX   ,   /* 电压异常 */
};
extern const uint8_t* status_str[][2];

typedef enum {
	ST_VOL_RISE     = 0,
	ST_TESTING      = 1,
	ST_VOL_FALL     = 2,
	ST_INTER_WAIT   = 3,
	ST_WAIT         = 4,
	ST_PASS         = 5,
	ST_STOP         = 6,            /* 复位状态 */
	ST_ERR_H        = 7,
	ST_ERR_L        = 8,
	ST_ERR_SHORT    = 9,
	ST_ERR_VOL_ABNORMAL = 10,       /* 电压异常 */
	ST_ERR_ARC      = 11,
	ST_ERR_GFI      = 12,
	ST_ERR_FAIL     = 13,
	ST_ERR_REAL     = 14,           /* 真实电流报警 */
	ST_ERR_CHAR     = 15,           /* 充电报警 */
	ST_ERR_GEAR     = 16,           /* 量程报警 / 档位报警 */
	ST_ERR_AMP      = 17,
	ST_OUTPUT_DELAY = 18,
	ST_ERR_OPEN     = 19,           /* 开路报警 */
}TestingStatusTypedef;  /*运行时状态*/

/* 错误码 */
typedef enum err_num_enum{
    ERR_NONE   = 0,    /* 没有错误 */
    ERR_HIGH   = 1,    /* 电流上限报警 */
    ERR_LOW    = 2,    /* 电流下限报警 */
    ERR_REAL   = 3,    /* 真实电流报警 */
    ERR_CHARGE = 4,    /* 充电报警 */
    ERR_OPEN   = 5,    /* 开路报警 */
    ERR_SHORT  = 6,    /* 短路报警 */
    ERR_ARC    = 7,    /* ARC 报警 */
    ERR_GFI    = 8,    /* GFI 报警 */
    ERR_AMP    = 9,    /* 功放报警 */
    ERR_GEAR   = 10,   /* 档位报警 */
    ERR_VOL    = 11,   /* 电压异常 */
}ERR_NUM_ENUM;
extern const uint8_t* except_buf[][3];

#define ACW_STR     "ACW"
#define DCW_STR     "DCW"
#define IR_STR      "IR"
#define GR_STR      "GR"
#define BBD_STR     "BBD"
#define CC_STR      "CC"
extern const uint8_t *mode_pool[10];



STRUCT_EXT ROADS_FLAG  roads_flag;

STRUCT_EXT uint8_t step_used_flag[MAX_STEPS/8 + 1 + 1];///<步骤已使用的标记列表
STRUCT_EXT uint16_t cur_group_table[MAX_STEPS];///<记忆组步骤地址映射表
STRUCT_EXT TEST_FILE *g_cur_file;///<当前文件
STRUCT_EXT TEST_FILE file_pool[MAX_FILES];///< 文件池
STRUCT_EXT TEST_FILE global_file;///< 全局文件实体用于界面通信
STRUCT_EXT SYS_PAR sys_par;///<系统参数
STRUCT_EXT SYS_FLAG sys_flag;///<系统参数
STRUCT_EXT NODE_STEP * g_cur_step;///<当前步指针
STRUCT_EXT uint32_t g_cur_step_crc;///<当前步的CRC校验数据
STRUCT_EXT uint32_t g_cur_step_crc_bk;///<当前步CRC校验数据备份
STRUCT_EXT TESTGROUP test_step_buf;///<测试步缓冲区
STRUCT_EXT CS_LIST list_head_99xx;///<测试步链表头

extern GEAR_STR ac_gear[];
extern GEAR_STR dc_gear[];
extern GEAR_STR ir_gear[];
extern TEST_FILE default_file;


#endif //__CS99XX_STRUCT_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
