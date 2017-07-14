/**
  ******************************************************************************
  * @file    com2_comm.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   第2路串口的结构信息初始化定义
  ******************************************************************************
  */

#include "module_manage.h"
#include "USART2.H"
#include "com_comm.h"


static uint8_t *get_com2_frame(COM_STRUCT *com);
static uint32_t get_com2_frame_len(COM_STRUCT *com);
static void get_com2_send_frame(COM_STRUCT *com, uint8_t *data, uint32_t len);
static void get_com2_resend_frame(COM_STRUCT *com);
static uint8_t get_com2_send_status(COM_STRUCT *com);
static void com2_wait_ack_timeout(void);
static uint8_t get_com2_receive_over_flag(COM_STRUCT *com);

/**
  * @brief  第1路串口结构信息
  */
COM_STRUCT com2={
    ROAD2_COM,///<串口编号
    MODULE_COMM_IDLE,///<空闲状态
    get_com2_send_frame,///<发送函数
    get_com2_resend_frame,///<重发函数
    com_set_wait_ack_timeout,///<设置等待从机响应的超时时间函数
    com2_wait_ack_timeout,///<等待从机应答超时函数
    receive_over_dispose,///<接收完成后的处理函数
    get_com2_receive_over_flag,///<获取接收完成标志
    get_com2_frame,///<获取串口数据帧
    get_com2_frame_len,///<获取串口数据帧长度
    get_com2_send_status,///<获取串口通信的发送状态
};
/**
  * @brief  获取串口接收完成标志
  * @param  [in] com 串口类指针
  * @retval 串口接收完成状态 0 未完成 1 接收完成
  */
static uint8_t get_com2_receive_over_flag(COM_STRUCT *com)
{
    uint8_t flag = get_usart2_receive_over_flag();
    
    if(flag)
    {
        clear_usart2_receive_over_flag();//清除发送完成标志
    }
    
    return flag;
}
/**
  * @brief  第2路串口发送数据函数,直接调用串口驱动层的发送函数
  * @param  [in] com 串口类指针
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval None
  */
static void get_com2_send_frame(COM_STRUCT *com, uint8_t *data, uint32_t len)
{
    usart2_send_data(data, len);
}

/**
  * @brief  第2路串口重发函数,直接调用串口驱动层提供的重发函数
  * @param  [in] com 串口类指针
  * @retval None
  */
static void get_com2_resend_frame(COM_STRUCT *com)
{
    usart2_resend_data();
}
/**
  * @brief  获取第2路串口接收到的数据
  * @param  [in] com 串口类指针
  * @retval 接收到的数据帧
  */
static uint8_t *get_com2_frame(COM_STRUCT *com)
{
    return usart2_get_receive_data();
}

/**
  * @brief  获取第2路串口接收到的数据长度
  * @param  [in] com 串口类指针
  * @retval 接收到的数据帧长度
  */
static uint32_t get_com2_frame_len(COM_STRUCT *com)
{
    uint32_t len = 0;
    
    len = usart2_get_receive_data_count();
    usart2_clear_receive_data_count();
    
    return len;
}

/**
  * @brief  获取第2路串口发送状态
  * @param  [in] com 串口类指针
  * @retval 串口发送状态 0 未完成 1 发送完成
  */
static uint8_t get_com2_send_status(COM_STRUCT *com)
{
    return usart2_get_send_status();
}
/**
  * @brief  提供给外部定时器调用的超时计时器
  * @param  无
  * @retval 无
  */
static void com2_wait_ack_timeout(void)
{
    if(com2.ack_timeout != 0)
    {
        if(--com2.ack_timeout == 0)
        {
            com2.status = MODULE_COMM_TIMEOUT;
        }
    }
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
