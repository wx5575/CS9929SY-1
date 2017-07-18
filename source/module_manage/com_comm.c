/**
  ******************************************************************************
  * @file    com_comm.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块通信使用的公共接口
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "string.h"
#include "crc.h"
#include "USART1.H"
#include "mem_alloc.h"
#include "tim3.h"
#include "com_comm.h"

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  接收完成的处理函数
  * @param  [in] com 串口类型指针
  * @retval 无
  */
void receive_over_dispose(COM_STRUCT *com)
{
    uint8_t *frame;
    uint32_t len;
    
    frame = com->get_com_frame(com);
    len = com->get_com_frame_len(com);
    
    com_receive_dispose(com->com_num, frame, len);
}

/**
  * @brief  设置等待从机应答的超时时间 WAIT_TIMEOUT_MS 单位ms
  * @param  [in] com 串口类型指针
  * @retval 无
  */
void com_set_wait_ack_timeout(COM_STRUCT *com)
{
    com->ack_timeout = WAIT_TIMEOUT_MS;
}
/**
  * @brief  获取通信故障标志
  * @param  [in] com 串口类型指针
  * @retval 通信故障标志
  */
uint8_t get_com_comm_cannot_connect_flag(COM_STRUCT *com)
{
    return com->comm_cannot_connect;
}

/**
  * @brief  等待从机响应超时计时器
  * @param  无
  * @retval 无
  */
void com_wait_ack_timeout(COM_STRUCT *com)
{
    if(com->ack_timeout != 0)
    {
        if(--com->ack_timeout == 0)
        {
            com->status = MODULE_COMM_TIMEOUT;
        }
    }
}
/**
  * @brief  串口通信状态机
  * @param  None
  * @retval None
  */
void com_comm_status_machine(COM_STRUCT *com)
{
    switch(com->status)
    {
        case MODULE_COMM_IDLE:
            break;
        case MODULE_COMM_RECEIVE:
            if(com->get_receive_over_flag(com))
            {
                com->comm_cannot_connect = CS_ERR_COMM_NORMAL;//标记通信正常
                com->receive_dispose_fun(com);//数据解析
                
                /* 解析出现异常 */
                if(com->comm_cannot_connect != CS_ERR_COMM_NORMAL)
                {
                    com->status = MODULE_COMM_ANOMALY;//解析出现异常
                }
                /* 解析正常结束 */
                else
                {
                    com->status = MODULE_COMM_IDLE;//接收完成进入空闲状态
                }
            }
            break;
        case MODULE_COMM_SEND:
            if(com->get_com_send_status(com))
            {
                com->status = MODULE_COMM_RECEIVE;//进入接收状态
            }
            break;
            /* 通信超时 或 通信解析异常时需要进行重发 */
        case MODULE_COMM_TIMEOUT:
        case MODULE_COMM_ANOMALY:
            /* 超时如果重发次数少于3次就进行重发 */
            if(com->resend_count < RESEND_NUM)
            {
                com->resend_fun(com);//重发
                com->set_wait_ack_timeout(com);//设置超时计时器
                com->status = MODULE_COMM_SEND;//进入发送状态
                com->resend_count++;
            }
            else
            {
                com->resend_count = 0;
                com->status = MODULE_COMM_IDLE;//状态机进入空闲状态
                
                /* 如果是超时异常就标记通信异常从机不存在或线路异常 否则就保持异常状态以备查询 */
                if(com->status == MODULE_COMM_TIMEOUT)
                {
                    com->comm_cannot_connect = CS_ERR_COMM_TROUBLE;//标记通信异常
                }
            }
            break;
    }
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
