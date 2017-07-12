

#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART3.H"
#include "mem_alloc.h"
#include "tim3.h"


/**
  * @brief  第1路串口
  */
COM_STRUCT com3={
    ROAD3_COM,///<串口编号
    MODULE_COMM_IDLE,///<空闲状态
    usart3_send_data,///<发送函数
    usart3_resend_data,///<重发函数
    com3_set_wait_ack_timeout,///<设置等待响应的超时时间函数
};

uint8_t *get_com3_frame(void)
{
    return usart3_get_receive_data();
}

uint32_t get_com3_frame_len(void)
{
    uint32_t len = 0;
    
    len = usart3_get_receive_data_count();
    usart3_clear_receive_data_count();
    
    return len;
}

void com3_receive_over(void)
{
    if(get_usart3_receive_over_flag())
    {
        com3.status = MODULE_COMM_IDLE;//接收完成进入空闲状态
        clear_usart3_receive_over_flag();
    }
}

void com3_receive_dis(void)
{
    uint8_t *frame;
    uint32_t len;
    
    frame = get_com3_frame();
    len = get_com3_frame_len();
    
    com_receive_dispose(ROAD3_COM, frame, len);
}

void com3_wait_ack_timeout(void)
{
    if(com3.ack_timeout != 0)
    {
        if(--com3.ack_timeout == 0)
        {
            com3.status = MODULE_COMM_TIMEOUT;
        }
    }
}

void com3_set_wait_ack_timeout(void)
{
    com3.ack_timeout = WAIT_TIMEOUT_MS;
}

uint8_t get_com3_comm_cannot_connect_flag(void)
{
    return com3.comm_cannot_connect;
}
/**
  * @brief  第1路串口通信状态机
  * @param  None
  * @retval None
  */
void com3_comm_status_machine(void)
{
    switch(com3.status)
    {
        case MODULE_COMM_IDLE:
            break;
        case MODULE_COMM_RECEIVE:
            com3_receive_over();
            break;
        case MODULE_COMM_SEND:
            if(usart3_get_send_status())
            {
                com3.status = MODULE_COMM_RECEIVE;//进入接收状态
            }
            break;
        case MODULE_COMM_TIMEOUT:
            /* 超时如果重发次数少于3次就进行重发 */
            if(com3.resend_count < RESEND_NUM)
            {
                com3.resend_fun();//重发
                com3.set_wait_ack_timeout();//设置超时计时器
                com3.status = MODULE_COMM_SEND;//进入发送状态
                com3.resend_count++;
            }
            else
            {
                com3.resend_count = 0;
                com3.status = MODULE_COMM_IDLE;//状态机进入空闲状态
                com3.comm_cannot_connect = CS_ERR_COMM_TROUBLE;
            }
            break;
    }
}
