/**
  ******************************************************************************
  * @file    scpi_com_conf.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   SCPIͨ��ʹ�ô��ڵĽṹ��Ϣ��ʼ������
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
  * @brief  ���ڽṹ��Ϣ
  */
SCPI_COM_STRUCT scpi_com={
    MODULE_COMM_RECEIVE,///<����״̬
    get_scpi_com_send_frame,///<���ͺ���
    get_scpi_com_resend_frame,///<�ط�����
    scpi_receive_over_dispose,///<������ɺ�Ĵ�����
    get_scpi_com_receive_over_flag,///<��ȡ������ɱ�־
    get_scpi_com_frame,///<��ȡ��������֡
    get_scpi_com_frame_len,///<��ȡ��������֡����
    get_scpi_com_send_status,///<��ȡ����ͨ�ŵķ���״̬
};
/**
  * @brief  ��1·���ڷ������ݺ���,ֱ�ӵ��ô���������ķ��ͺ���
  * @param  [in] com ������ָ��
  * @param  [in] data ����
  * @param  [in] len ���ݳ���
  * @retval None
  */
static void get_scpi_com_send_frame(SCPI_COM_STRUCT *com, uint8_t *data, uint32_t len)
{
    usart6_send_data(data, len);
}

/**
  * @brief  ��1·�����ط�����,ֱ�ӵ��ô����������ṩ���ط�����
  * @param  [in] com ������ָ��
  * @retval None
  */
static void get_scpi_com_resend_frame(SCPI_COM_STRUCT *com)
{
    usart6_resend_data();
}
/**
  * @brief  ��ȡ��1·���ڽ��յ�������
  * @param  [in] com ������ָ��
  * @retval ���յ�������֡
  */
static uint8_t *get_scpi_com_frame(SCPI_COM_STRUCT *com)
{
    return usart6_get_receive_data();
}

/**
  * @brief  ��ȡ��1·���ڽ��յ������ݳ���
  * @param  [in] com ������ָ��
  * @retval ���յ�������֡����
  */
static uint32_t get_scpi_com_frame_len(SCPI_COM_STRUCT *com)
{
    uint32_t len = 0;
    
    len = usart6_get_receive_data_count();
    usart6_clear_receive_data_count();
    
    return len;
}

/**
  * @brief  ��ȡ���ڽ�����ɱ�־
  * @param  [in] com ������ָ��
  * @retval ���ڽ������״̬ 0 δ��� 1 �������
  */
static uint8_t get_scpi_com_receive_over_flag(SCPI_COM_STRUCT *com)
{
    uint8_t flag = get_usart6_receive_over_flag();
    
    if(flag)
    {
        clear_usart6_receive_over_flag();//���������ɱ�־
    }
    
    return flag;
}
/**
  * @brief  ��ȡ��1·���ڷ���״̬
  * @param  [in] com ������ָ��
  * @retval ���ڷ���״̬ 0 δ��� 1 �������
  */
static uint8_t get_scpi_com_send_status(SCPI_COM_STRUCT *com)
{
    return usart6_get_send_status();
}
/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
