/**
  ******************************************************************************
  * @file    module_manage.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块管理，硬件上有四路串口，使用485通信就可实现每路串口最多挂接15个模块
  ******************************************************************************
  */
#ifndef __MODULE_MANAGE_H__
#define __MODULE_MANAGE_H__


#include "cs99xx_struct.h"
#include "com_comm.h"


#define MAX_MODULE_NUM  60

typedef enum{
    COMM_ST_NO_ERROR        = 0,    //<执行成功
    COMM_ST_SYNTAX_ERROR    = 102,  //<语法错误
    COMM_ST_NOT_EXE         = 105,  //<指令执行不允许
    COMM_ST_PAR_INVALID     = 108,  //<接收到指令不允许的参数
    COMM_ST_MISSING_PAR     = 109,  //<指令串中遗漏了参数
    COMM_ST_UNDEFINED       = 113,  //<未定义的指令
    COMM_ST_PAR_TYPE_ERROR  = 120,  //<参数类型错误
    COMM_ST_DATA_INVALID    = 151,  //<不允许的字符串参数
    COMM_ST_TIME_OUT        = 152,  //<指令执行超时
    COMM_ST_OUT_OF_RANGE    = 222,  //<参数值超出允许范围
    COMM_ST_SEND_TOO_FAST   = 110,  //<指令发送间隔时间短
    COMM_ST_CRC_ERROR       = 119,  //<指令CRC校验错误
}COMM_ST;

typedef struct{
    char name[20]; ///< 模块名称
    char version[10];///< 模块版本
    uint32_t id;///< 模块ID
}MODULE_INF;

typedef struct{
    COM_NUM com_num;///<串口编号
    ROAD_NUM_T road_num;///<路号
    MODULE_INF module_inf;///<模块信息
    uint8_t buf[512];///<通信缓冲区
}ROAD_INF;

typedef enum{
    ROAD_STOPPING,///<不在测试状态
    ROAD_TESTING,///<正在测试状态
}ROAD_TEST_ST;

typedef struct{
    uint8_t work_st;///<工作状态，0不参与工作，1参与工作
    uint8_t vol[10];///<电压值
    uint8_t cur[10];///<电流值
    uint8_t time[10];///<时间
    uint8_t status;///<测试状态
    uint8_t mode[5];///<测试模式
    uint8_t step[8];///<测试步骤
    CS_UNIT_ENUM vol_unit;///<电压单位
    CS_UNIT_ENUM cur_unit;///<电流/电阻单位
    CS_UNIT_ENUM real_unit;///<真实电流等单位
    uint8_t usable;///<数据可以使用标记
//    uint16_t g_dis_time;///<测试时间
}COMM_TEST_DATA;

/**
  * @brief  同步测试端口信息
  */
typedef struct{
    COM_STRUCT *com;///<串口信息
    CS_INDEX road_index;///<路号索引
    ROAD_NUM_T road_num;///<路号
    MODULE_ADDR_T addr;///<模块地址，方便查找使用
    MODULE_ADDR_T offset_addr;///<偏移地址
    ROAD_TEST_ST test_st;///<测试状态
    COMM_TEST_DATA test_data;///<实时测试数据
}SYN_TEST_PORT_INF;

#define FRAME_HEAD_SIZE     4  ///<帧头的字节个数
#define CRC_LEN             2  ///<CRC校验字节个数

#pragma pack(1)
typedef struct{
    MODULE_ADDR_T addr;///<模块地址
    uint16_t fun_code;///<功能码
    uint8_t st;///<状态码
    uint8_t data[512];///<数据缓冲区
}FRAME_T;
#pragma pack()

typedef struct{
    uint8_t index;///<索引值
    uint8_t mode[10];///<校准点的模式
    uint8_t point[10];///<校准点
    CS_UNIT_ENUM unit;///<单位
    uint8_t decs;///<小数点个数
    uint8_t lon;///<总长度包含小数点
    uint32_t mul_power;///<倍率 例如:电压单位0.1V 倍率就为10
}CAL_POINT_INF;

#ifdef   MODULE_GLOBALS
#define  MODULE_EXT
#else
#define  MODULE_EXT  extern
#endif

#define SYN_MAX_ROADS   8 ///<同步测试的最大路数
MODULE_EXT ROAD_INF    *module_inf_pool;
MODULE_EXT SYN_TEST_PORT_INF   syn_test_port[SYN_MAX_ROADS];///最多8路同步测试
MODULE_EXT uint8_t cur_module_cal_points;
MODULE_EXT uint8_t cur_module_cal_points_bk;
MODULE_EXT uint8_t comm_syn_sign;///<通信同步信号
MODULE_EXT CAL_POINT_INF cal_point_inf_pool[60];

extern COM_STRUCT com1;
extern COM_STRUCT com2;
extern COM_STRUCT com3;
extern COM_STRUCT com4;


extern void com_wait_ack_timeout(COM_STRUCT *com);
extern void com_comm_status_machine(COM_STRUCT *com);


extern void init_module_manage_env(void);
extern void module_comm_task(void);
extern void clear_module_inf(void);
extern CS_BOOL com_comm_is_idle(MODULE_ADDR_T addr);
extern void wait_com_comm_idle(MODULE_ADDR_T addr);
extern MODULE_ADDR_T get_module_addr(ROAD_INDEX road, CS_ERR *err);
extern MODULE_ADDR_T get_module_index_addr(ROAD_INDEX road, CS_ERR *err);
extern uint8_t get_total_roads_num(void);

extern CS_ERR com_module_connect(MODULE_ADDR_T addr);
extern CS_ERR com_module_set_road_num(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_module_connect(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_format_data(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_set_config_parameter(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);

extern CS_ERR send_query_cal_points(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_enter_cal_st(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_exit_cal_st(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_query_cal_point_inf(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);

extern CS_ERR send_slave_start_cal(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_stop_cal(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_set_measure_value(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_load_cur_point(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);

extern CS_ERR send_get_test_data(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_start_test(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_test_over_sign_h(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_test_over_sign_l(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_get_slave_test_time(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);

extern CS_ERR send_slave_new_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_read_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_edit_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_del_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_save_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_load_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_slave_clear_all_files(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);

extern CS_ERR send_insert_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_edit_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_load_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_del_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);
extern CS_ERR send_swap_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len);



extern ROAD_TEST_ST read_road_test_status(ROAD_NUM_T road);
extern COMM_TEST_DATA*  get_road_test_data(ROAD_NUM_T road, COMM_TEST_DATA *test_data);
extern SYN_TEST_PORT_INF* get_road_inf(MODULE_ADDR_T addr, CS_ERR *err);

extern ROAD_INDEX_NUM_T get_road_index_num(MODULE_ADDR_T addr);

extern CS_BOOL road1_test_over(void);
extern CS_BOOL road2_test_over(void);
extern CS_BOOL road3_test_over(void);
extern CS_BOOL road4_test_over(void);

extern CS_BOOL road1_test_alarm(void);
extern CS_BOOL road2_test_alarm(void);
extern CS_BOOL road3_test_alarm(void);
extern CS_BOOL road4_test_alarm(void);

#endif //__MODULE_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
