/**
  ******************************************************************************
  * @file    com_comm.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   串口的结构信息初始化定义
  ******************************************************************************
  */

#ifndef __COM_COMM_H__
#define __COM_COMM_H__

#include "cs99xx_struct.h"

/* 这里定义的广播地址仅在主机端使用，用来统一各路串口的操作，在从机端的广播地址只能为0 */
#define COM1_BROADCAST_ADDR     (16 * 0)    ///<第1路串口的广播地址
#define COM2_BROADCAST_ADDR     (16 * 1)    ///<第2路串口的广播地址
#define COM3_BROADCAST_ADDR     (16 * 2)    ///<第3路串口的广播地址
#define COM4_BROADCAST_ADDR     (16 * 3)    ///<第4路串口的广播地址

#define SLAVE_BROADCAST_ADDR    0 ///<从机端广播地址为0
#define SLAVE_ADDR_MAX          15  ///<从机端ID最大为15
#define MASTER_ADDR_RANGE       (16 * SYN_MAX_COM_NUM)  ///<主机端地址范围 串口个数 * 每个串口的地址个数

#define RESEND_NUM              3 ///<最大重发次数
#define WAIT_TIMEOUT_MS         500 ///<等待从机响应的超时时间 ms

/**
  * @brief  通信状态机的状态定义
  */
typedef enum{
    MODULE_COMM_IDLE,///<模块通信空闲
    MODULE_COMM_RECEIVE,///<模块通信处于接收状态
    MODULE_COMM_SEND,///<模块通信处于发送状态
    MODULE_COMM_TIMEOUT,///<模块通信处于超时状态
    MODULE_COMM_ANOMALY,///<模块通信处于异常状态,通信进入解析时出现了异常
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

/**
  * @brief  串口结构定义
  */
typedef struct COM_STRUCT_ COM_STRUCT;
/**
  * @brief  串口数据结构无返回数据成员函数类型定义
  */
typedef void (*COM_SERVE_FUN)(COM_STRUCT *);
/**
  * @brief  串口数据结构返回U8数据成员函数类型定义
  */
typedef uint8_t (*COM_SERVE_R_U8_FUN)(COM_STRUCT *);
/**
  * @brief  串口数据结构返回U32数据成员函数类型定义
  */
typedef uint32_t (*COM_SERVE_R_U32_FUN)(COM_STRUCT *);
/**
  * @brief  串口数据结构返回U8*数据成员函数类型定义
  */
typedef uint8_t* (*COM_SERVE_R_PU8_FUN)(COM_STRUCT *);

/**
  * @brief  串口结构定义
  */
struct COM_STRUCT_{
    COM_NUM com_num;///<串口编号
    MODULE_COMM_STATUS status;///<串口状态 空闲状态 发送状态 接收状态
    void (*send_fun)(COM_STRUCT *,uint8_t*,uint32_t);///<发送函数
    COM_SERVE_FUN resend_fun;///<重发函数
    COM_SERVE_FUN set_wait_ack_timeout;///<设置等待指令返回的超时时间
    
    COM_SERVE_FUN receive_dispose_fun;///<接收完成后的处理函数
    COM_SERVE_R_U8_FUN get_receive_over_flag;///<获取接收完成标志
    
    COM_SERVE_R_PU8_FUN get_com_frame;///<获取串口数据帧
    COM_SERVE_R_U32_FUN get_com_frame_len;///<获取串口数据帧长度
    COM_SERVE_R_U8_FUN get_com_send_status;///<获取串口通信的发送状态
    COM_SERVE_FUN init_com_env;///<初始化串口运行环境
    
    uint8_t resend_count;///<重发计数
    uint32_t ack_timeout;///<通信应答超时计时器
    CS_ERR comm_cannot_connect;///<通信无法连接标记。可能是从机不存在或通信硬件故障
};


extern void com_set_wait_ack_timeout(COM_STRUCT *com);
extern void receive_over_dispose(COM_STRUCT *com);
extern void com_set_wait_ack_timeout(COM_STRUCT *com);
extern void com_wait_ack_timeout(COM_STRUCT *com);
extern void com_receive_dispose(uint8_t com_num, uint8_t *frame, uint32_t len);


#endif //__COM_COMM_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
