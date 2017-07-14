

#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART1.H"
#include "mem_alloc.h"
#include "tim3.h"

/**
  * @brief  ������ɵĴ�����
  * @param  [in] com ��������ָ��
  * @retval ��
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
  * @brief  ���õȴ��ӻ�Ӧ��ĳ�ʱʱ�� WAIT_TIMEOUT_MS ��λms
  * @param  [in] com ��������ָ��
  * @retval ��
  */
void com_set_wait_ack_timeout(COM_STRUCT *com)
{
    com->ack_timeout = WAIT_TIMEOUT_MS;
}
/**
  * @brief  ��ȡͨ�Ź��ϱ�־
  * @param  [in] com ��������ָ��
  * @retval ͨ�Ź��ϱ�־
  */
uint8_t get_com_comm_cannot_connect_flag(COM_STRUCT *com)
{
    return com->comm_cannot_connect;
}
/**
  * @brief  ����ͨ��״̬��
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
                com->status = MODULE_COMM_IDLE;//������ɽ������״̬
            }
            break;
        case MODULE_COMM_SEND:
            if(com->get_com_send_status(com))
            {
                com->status = MODULE_COMM_RECEIVE;//�������״̬
            }
            break;
        case MODULE_COMM_TIMEOUT:
            /* ��ʱ����ط���������3�ξͽ����ط� */
            if(com->resend_count < RESEND_NUM)
            {
                com->resend_fun(com);//�ط�
                com->set_wait_ack_timeout(com);//���ó�ʱ��ʱ��
                com->status = MODULE_COMM_SEND;//���뷢��״̬
                com->resend_count++;
            }
            else
            {
                com->resend_count = 0;
                com->status = MODULE_COMM_IDLE;//״̬���������״̬
                com->comm_cannot_connect = CS_ERR_COMM_TROUBLE;
            }
            break;
    }
}
