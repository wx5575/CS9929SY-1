/**
  ******************************************************************************
  * @file    scpi_com_conf.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   SCPI通信使用串口的结构信息初始化定义
  ******************************************************************************
  */


//#include "module_manage.h"
#include "USART6.H"
#include "scpi_com_comm.h"


static uint8_t *get_scpi_com_frame(SCPI_COM_STRUCT *com);
static uint32_t get_scpi_com_frame_len(SCPI_COM_STRUCT *com);
static void get_scpi_com_send_frame(SCPI_COM_STRUCT *com, uint8_t *data, uint32_t len);
static void get_scpi_com_resend_frame(SCPI_COM_STRUCT *com);
static uint8_t get_scpi_com_send_status(SCPI_COM_STRUCT *com);
//static void scpi_com_wait_ack_timeout(void);
static uint8_t get_scpi_com_receive_over_flag(SCPI_COM_STRUCT *com);

void scpi_receive_over_dispose(SCPI_COM_STRUCT *com);
/**
  * @brief  串口结构信息
  */
SCPI_COM_STRUCT scpi_com={
    MODULE_COMM_RECEIVE,///<接收状态
    get_scpi_com_send_frame,///<发送函数
    get_scpi_com_resend_frame,///<重发函数
    scpi_receive_over_dispose,///<接收完成后的处理函数
    get_scpi_com_receive_over_flag,///<获取接收完成标志
    get_scpi_com_frame,///<获取串口数据帧
    get_scpi_com_frame_len,///<获取串口数据帧长度
    get_scpi_com_send_status,///<获取串口通信的发送状态
};
/**
  * @brief  第1路串口发送数据函数,直接调用串口驱动层的发送函数
  * @param  [in] com 串口类指针
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval None
  */
static void get_scpi_com_send_frame(SCPI_COM_STRUCT *com, uint8_t *data, uint32_t len)
{
    usart6_send_data(data, len);
}

/**
  * @brief  第1路串口重发函数,直接调用串口驱动层提供的重发函数
  * @param  [in] com 串口类指针
  * @retval None
  */
static void get_scpi_com_resend_frame(SCPI_COM_STRUCT *com)
{
    usart6_resend_data();
}
/**
  * @brief  获取第1路串口接收到的数据
  * @param  [in] com 串口类指针
  * @retval 接收到的数据帧
  */
static uint8_t *get_scpi_com_frame(SCPI_COM_STRUCT *com)
{
    return usart6_get_receive_data();
}

/**
  * @brief  获取第1路串口接收到的数据长度
  * @param  [in] com 串口类指针
  * @retval 接收到的数据帧长度
  */
static uint32_t get_scpi_com_frame_len(SCPI_COM_STRUCT *com)
{
    uint32_t len = 0;
    
    len = usart6_get_receive_data_count();
    usart6_clear_receive_data_count();
    
    return len;
}

/**
  * @brief  获取串口接收完成标志
  * @param  [in] com 串口类指针
  * @retval 串口接收完成状态 0 未完成 1 接收完成
  */
static uint8_t get_scpi_com_receive_over_flag(SCPI_COM_STRUCT *com)
{
    uint8_t flag = get_usart6_receive_over_flag();
    
    if(flag)
    {
        clear_usart6_receive_over_flag();//清除发送完成标志
    }
    
    return flag;
}
/**
  * @brief  获取第1路串口发送状态
  * @param  [in] com 串口类指针
  * @retval 串口发送状态 0 未完成 1 发送完成
  */
static uint8_t get_scpi_com_send_status(SCPI_COM_STRUCT *com)
{
    return usart6_get_send_status();
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
