

#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "UART4.H"
#include "mem_alloc.h"
#include "tim3.h"


/**
  * @brief  第1路串口
  */
COM_STRUCT com4={
    ROAD4_COM,///<串口编号
    MODULE_COMM_IDLE,///<空闲状态
    uart4_send_data,///<发送函数
    uart4_resend_data,///<重发函数
    com4_set_wait_ack_timeout,///<设置等待响应的超时时间函数
};

uint8_t *get_com4_frame(void)
{
    return uart4_get_receive_data();
}

uint32_t get_com4_frame_len(void)
{
    uint32_t len = 0;
    
    len = uart4_get_receive_data_count();
    uart4_clear_receive_data_count();
    
    return len;
}

void com4_receive_over(void)
{
    if(get_uart4_receive_over_flag())
    {
        com4.status = MODULE_COMM_IDLE;//接收完成进入空闲状态
        clear_uart4_receive_over_flag();
    }
}

void com4_receive_dis(void)
{
    uint8_t *frame;
    uint32_t len;
    
    frame = get_com4_frame();
    len = get_com4_frame_len();
    
    com_receive_dispose(ROAD4_COM, frame, len);
}

void com4_wait_ack_timeout(void)
{
    if(com4.ack_timeout != 0)
    {
        if(--com4.ack_timeout == 0)
        {
            com4.status = MODULE_COMM_TIMEOUT;
        }
    }
}

void com4_set_wait_ack_timeout(void)
{
    com4.ack_timeout = WAIT_TIMEOUT_MS;
}

uint8_t get_com4_comm_cannot_connect_flag(void)
{
    return com4.comm_cannot_connect;
}
/**
  * @brief  第1路串口通信状态机
  * @param  None
  * @retval None
  */
void com4_comm_status_machine(void)
{
    switch(com4.status)
    {
        case MODULE_COMM_IDLE:
            break;
        case MODULE_COMM_RECEIVE:
            com4_receive_over();
            break;
        case MODULE_COMM_SEND:
            if(uart4_get_send_status())
            {
                com4.status = MODULE_COMM_RECEIVE;//进入接收状态
            }
            break;
        case MODULE_COMM_TIMEOUT:
            /* 超时如果重发次数少于3次就进行重发 */
            if(com4.resend_count < RESEND_NUM)
            {
                com4.resend_fun();//重发
                com4.set_wait_ack_timeout();//设置超时计时器
                com4.status = MODULE_COMM_SEND;//进入发送状态
                com4.resend_count++;
            }
            else
            {
                com4.resend_count = 0;
                com4.status = MODULE_COMM_IDLE;//状态机进入空闲状态
                com4.comm_cannot_connect = CS_ERR_COMM_TROUBLE;
            }
            break;
    }
}
