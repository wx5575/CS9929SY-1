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
    MODULE_INF module_inf;///<模块信息
    uint8_t buf[512];///<通信缓冲区
}ROAD_INF;
/**
  * @brief  同步测试端口信息
  */
typedef struct{
    COM_STRUCT *com;
}SYN_TEST_PORT_INF;

#define FRAME_HEAD_SIZE     4  ///<帧头的字节个数
#define CRC_LEN             2  ///<CRC校验字节个数

#pragma pack(1)
typedef struct{
    uint8_t addr;
    uint16_t fun_code;
    uint8_t st;
    uint8_t data[512];
}FRAME_T;
#pragma pack()

#ifdef   MODULE_GLOBALS
#define  MODULE_EXT
#else
#define  MODULE_EXT  extern
#endif

MODULE_EXT ROAD_INF    *road_inf_pool;
MODULE_EXT SYN_TEST_PORT_INF   syn_test_port[8];///最多8路同步测试

extern COM_STRUCT com1;
extern COM_STRUCT com2;
extern COM_STRUCT com3;
extern COM_STRUCT com4;
extern void com_wait_ack_timeout(COM_STRUCT *com);
extern void com_comm_status_machine(COM_STRUCT *com);

extern CS_ERR com_module_connect(uint8_t addr);

extern void init_module_manage_env(void);
extern void module_comm_task(void);
extern void clear_module_inf(void);
extern uint8_t com_comm_is_idle(uint8_t addr);

#endif //__MODULE_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
