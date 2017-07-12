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

typedef enum{
    MODULE_COMM_IDLE,///<模块通信空闲
    MODULE_COMM_RECEIVE,///<模块通信处于接收状态
    MODULE_COMM_SEND,///<模块通信处于发送状态
    MODULE_COMM_TIMEOUT,///<模块通信处于超时状态
}MODULE_COMM_STATUS;

/**
  * @brief  串口编号
  */
typedef enum{
    ROAD1_COM,///<第一路串口编号
    ROAD2_COM,///<第二路串口编号
    ROAD3_COM,///<第三路串口编号
    ROAD4_COM,///<第四路串口编号
}COM_NUM;
typedef void (*COM_SEND_FUN)(uint8_t *, uint32_t);
typedef void (*COM_RESEND_FUN)(void);
typedef void (*COM_SET_ACK_TIMEOUT_FUN)(void);

#define RESEND_NUM          3 ///<最大重发次数
#define WAIT_TIMEOUT_MS     500 ///<等待从机响应的超时时间 ms

typedef struct{
    COM_NUM com_num;///<串口编号
    MODULE_COMM_STATUS status;///<串口状态 空闲状态 发送状态 接收状态
    COM_SEND_FUN send_fun;///<发送函数
    COM_RESEND_FUN resend_fun;///<重发函数
    COM_SET_ACK_TIMEOUT_FUN set_wait_ack_timeout;///<设置等待指令返回的超时时间
    uint8_t resend_count;///<重发计数
    uint32_t ack_timeout;///<通信应答超时计时器
    CS_ERR comm_cannot_connect;///<通信无法连接标记。可能是从机不存在或通信硬件故障
}COM_STRUCT;

typedef struct{
    uint8_t id;//ID号
    uint8_t name[20];//最长19个字符的名字
    uint8_t ver[15];//版本号
}MODULE_INF; 

typedef struct{
    COM_NUM com_num;///<串口编号
    MODULE_INF module_inf;///<模块信息
    uint8_t buf[512];///<通信缓冲区
}ROAD_INF;
/**
  * @brief  管理所有路的信息为了加快通过路号对各模块地址的查找将所有的有效地址依次存入
  *         这个结构中
  */
typedef struct{
    uint8_t road_buf[64];///< road地址缓冲 第0个元素放第1路的地址1-15 17-31 33-47 49-63
    uint8_t count;///< 当前的总路数
}ROADS_FLAG;
/**
  * @brief  同步测试端口信息
  */
typedef struct{
    COM_STRUCT *com;
}SYN_TEST_PORT_INF;

#pragma pack(1)
typedef struct{
    uint8_t addr;
    uint16_t fun_num;
    uint8_t st;
    void *data;
}FRAME_T;
#pragma pack()

#ifdef   MODULE_GLOBALS
#define  MODULE_EXT
#else
#define  MODULE_EXT  extern
#endif

MODULE_EXT ROAD_INF    *road_inf_pool;
MODULE_EXT ROADS_FLAG  roads_flag;
MODULE_EXT SYN_TEST_PORT_INF   syn_test_port[8];///最多8路同步测试

extern COM_STRUCT com1;
extern COM_STRUCT com2;
extern COM_STRUCT com3;
extern COM_STRUCT com4;

extern void init_module_manage_env(void);
extern void module_comm_task(void);
extern void com1_comm_status_machine(void);
extern uint8_t get_com1_comm_cannot_connect_flag(void);
extern void com1_set_wait_ack_timeout(void);
extern void com1_wait_ack_timeout(void);

extern void com2_comm_status_machine(void);
extern uint8_t get_com2_comm_cannot_connect_flag(void);
extern void com2_set_wait_ack_timeout(void);
extern void com2_wait_ack_timeout(void);

extern void com3_comm_status_machine(void);
extern uint8_t get_com3_comm_cannot_connect_flag(void);
extern void com3_set_wait_ack_timeout(void);
extern void com3_wait_ack_timeout(void);

extern void com4_comm_status_machine(void);
extern uint8_t get_com4_comm_cannot_connect_flag(void);
extern void com4_set_wait_ack_timeout(void);
extern void com4_wait_ack_timeout(void);

extern void com_receive_dispose(uint8_t com_num, uint8_t *frame, uint32_t len);
extern CS_ERR com_module_connect(uint8_t addr);

#endif //__MODULE_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
