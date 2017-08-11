/**
  ******************************************************************************
  * @file    com_comm.h
  * @author  ����
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   ���ڵĽṹ��Ϣ��ʼ������
  ******************************************************************************
  */

#ifndef __COM_COMM_H__
#define __COM_COMM_H__

#include "cs99xx_struct.h"

/* ���ﶨ��Ĺ㲥��ַ����������ʹ�ã�����ͳһ��·���ڵĲ������ڴӻ��˵Ĺ㲥��ַֻ��Ϊ0 */
#define COM1_BROADCAST_ADDR     (16 * 0)    ///<��1·���ڵĹ㲥��ַ
#define COM2_BROADCAST_ADDR     (16 * 1)    ///<��2·���ڵĹ㲥��ַ
#define COM3_BROADCAST_ADDR     (16 * 2)    ///<��3·���ڵĹ㲥��ַ
#define COM4_BROADCAST_ADDR     (16 * 3)    ///<��4·���ڵĹ㲥��ַ

#define SLAVE_BROADCAST_ADDR    0 ///<�ӻ��˹㲥��ַΪ0
#define SLAVE_ADDR_MAX          15  ///<�ӻ���ID���Ϊ15
#define MASTER_ADDR_RANGE       (16 * SYN_MAX_COM_NUM)  ///<�����˵�ַ��Χ ���ڸ��� * ÿ�����ڵĵ�ַ����

#define RESEND_NUM              3 ///<����ط�����
#define WAIT_TIMEOUT_MS         500 ///<�ȴ��ӻ���Ӧ�ĳ�ʱʱ�� ms

/**
  * @brief  ͨ��״̬����״̬����
  */
typedef enum{
    MODULE_COMM_IDLE,///<ģ��ͨ�ſ���
    MODULE_COMM_RECEIVE,///<ģ��ͨ�Ŵ��ڽ���״̬
    MODULE_COMM_SEND,///<ģ��ͨ�Ŵ��ڷ���״̬
    MODULE_COMM_TIMEOUT,///<ģ��ͨ�Ŵ��ڳ�ʱ״̬
    MODULE_COMM_ANOMALY,///<ģ��ͨ�Ŵ����쳣״̬,ͨ�Ž������ʱ�������쳣
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

/**
  * @brief  ���ڽṹ����
  */
typedef struct COM_STRUCT_ COM_STRUCT;
/**
  * @brief  �������ݽṹ�޷������ݳ�Ա�������Ͷ���
  */
typedef void (*COM_SERVE_FUN)(COM_STRUCT *);
/**
  * @brief  �������ݽṹ����U8���ݳ�Ա�������Ͷ���
  */
typedef uint8_t (*COM_SERVE_R_U8_FUN)(COM_STRUCT *);
/**
  * @brief  �������ݽṹ����U32���ݳ�Ա�������Ͷ���
  */
typedef uint32_t (*COM_SERVE_R_U32_FUN)(COM_STRUCT *);
/**
  * @brief  �������ݽṹ����U8*���ݳ�Ա�������Ͷ���
  */
typedef uint8_t* (*COM_SERVE_R_PU8_FUN)(COM_STRUCT *);

/**
  * @brief  ���ڽṹ����
  */
struct COM_STRUCT_{
    COM_NUM com_num;///<���ڱ��
    MODULE_COMM_STATUS status;///<����״̬ ����״̬ ����״̬ ����״̬
    void (*send_fun)(COM_STRUCT *,uint8_t*,uint32_t);///<���ͺ���
    COM_SERVE_FUN resend_fun;///<�ط�����
    COM_SERVE_FUN set_wait_ack_timeout;///<���õȴ�ָ��صĳ�ʱʱ��
    
    COM_SERVE_FUN receive_dispose_fun;///<������ɺ�Ĵ�����
    COM_SERVE_R_U8_FUN get_receive_over_flag;///<��ȡ������ɱ�־
    
    COM_SERVE_R_PU8_FUN get_com_frame;///<��ȡ��������֡
    COM_SERVE_R_U32_FUN get_com_frame_len;///<��ȡ��������֡����
    COM_SERVE_R_U8_FUN get_com_send_status;///<��ȡ����ͨ�ŵķ���״̬
    COM_SERVE_FUN init_com_env;///<��ʼ���������л���
    
    uint8_t resend_count;///<�ط�����
    uint32_t ack_timeout;///<ͨ��Ӧ��ʱ��ʱ��
    CS_ERR comm_cannot_connect;///<ͨ���޷����ӱ�ǡ������Ǵӻ������ڻ�ͨ��Ӳ������
};


extern void com_set_wait_ack_timeout(COM_STRUCT *com);
extern void receive_over_dispose(COM_STRUCT *com);
extern void com_set_wait_ack_timeout(COM_STRUCT *com);
extern void com_wait_ack_timeout(COM_STRUCT *com);
extern void com_receive_dispose(uint8_t com_num, uint8_t *frame, uint32_t len);


#endif //__COM_COMM_H__

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
