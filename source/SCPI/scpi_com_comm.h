/**
  ******************************************************************************
  * @file    com_comm.h
  * @author  ����
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   ���ڵĽṹ��Ϣ
  ******************************************************************************
  */


#ifndef __COM_COMM_H__
#define __COM_COMM_H__



#include "cs99xx_struct.h"
typedef enum{
    MODULE_COMM_RECEIVE,///<ģ��ͨ�Ŵ��ڽ���״̬
    MODULE_COMM_SEND,///<ģ��ͨ�Ŵ��ڷ���״̬
}MODULE_COMM_STATUS;

/**
  * @brief  ���ڱ��
  */
typedef enum{
    ROAD1_COM,///<��һ·���ڱ��
    ROAD2_COM,///<�ڶ�·���ڱ��
    ROAD3_COM,///<����·���ڱ��
    ROAD4_COM,///<����·���ڱ��
}COM_NUM;

#define RESEND_NUM          3 ///<����ط�����
#define WAIT_TIMEOUT_MS     500 ///<�ȴ��ӻ���Ӧ�ĳ�ʱʱ�� ms
typedef struct SCPI_COMM_STRUCT_ SCPI_COM_STRUCT;
typedef void (*COM_SERVE_FUN)(SCPI_COM_STRUCT *);
typedef uint8_t (*COM_SERVE_R_U8_FUN)(SCPI_COM_STRUCT *);
typedef uint32_t (*COM_SERVE_R_U32_FUN)(SCPI_COM_STRUCT *);
typedef uint8_t* (*COM_SERVE_R_PU8_FUN)(SCPI_COM_STRUCT *);

struct SCPI_COMM_STRUCT_{
    MODULE_COMM_STATUS status;///<����״̬ ����״̬ ����״̬ ����״̬
    void (*send_fun)(SCPI_COM_STRUCT *,uint8_t*,uint32_t);///<���ͺ���
    COM_SERVE_FUN resend_fun;///<�ط�����
    
    COM_SERVE_FUN receive_dispose_fun;///<������ɺ�Ĵ�����
    COM_SERVE_R_U8_FUN get_receive_over_flag;///<��ȡ������ɱ�־
    
    COM_SERVE_R_PU8_FUN get_com_frame;///<��ȡ��������֡
    COM_SERVE_R_U32_FUN get_com_frame_len;///<��ȡ��������֡����
    COM_SERVE_R_U8_FUN get_com_send_status;///<��ȡ����ͨ�ŵķ���״̬
    uint8_t frame_buf[512];///<ͨ�Ż�����
};
extern SCPI_COM_STRUCT scpi_com;

void scpi_receive_dispose(SCPI_COM_STRUCT *com, uint8_t *frame, uint32_t len);

void scpi_comm_status_machine(SCPI_COM_STRUCT *com);
void com_set_wait_ack_timeout(SCPI_COM_STRUCT *com);
void receive_over_dispose(SCPI_COM_STRUCT *com);
void com_set_wait_ack_timeout(SCPI_COM_STRUCT *com);
void com_wait_ack_timeout(SCPI_COM_STRUCT *com);


#endif ///__COM_COMM_H__

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
