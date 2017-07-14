

#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART1.H"
#include "mem_alloc.h"
#include "tim3.h"

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
                com->receive_dispose_fun(com);
                com->status = MODULE_COMM_IDLE;//接收完成进入空闲状态
            }
            break;
        case MODULE_COMM_SEND:
            if(com->get_com_send_status(com))
            {
                com->status = MODULE_COMM_RECEIVE;//进入接收状态
            }
            break;
        case MODULE_COMM_TIMEOUT:
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
                com->comm_cannot_connect = CS_ERR_COMM_TROUBLE;
            }
            break;
    }
}
