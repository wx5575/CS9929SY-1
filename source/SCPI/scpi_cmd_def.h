/**
  ******************************************************************************
  * @file    scpi_cmd_def.h
  * @author  ����
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   SCPI��������������Ӧ��ָ���б�
  ******************************************************************************
  */
#ifndef __SCPI_CMD_DEF_H__
#define __SCPI_CMD_DEF_H__

#include <stm32f4xx.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif


typedef uint32_t CMD_NUM_T;


typedef enum{
    C_ACW   ,
    C_ALL   ,
    C_ARANGE,
    C_ARC   ,
    C_BBD   ,
    C_BBDOpen,
    C_BBDShort,
    C_BEEPER,
    C_BEHIND,
    C_CALIBRATION,
    C_CAPACITY,
    C_CATALOG,  
    C_CCURRENT,
    C_CHINT ,
    C_CLEAR ,  
    C_CNEXT ,
    C_COMM   ,
    C_CONTRAST,
    C_CONTROL,
    C_CTIME ,
    C_CURRENT,
    C_DCW   ,
    C_DELETE ,
    C_DTIME ,
    C_DUT   ,
    C_EDIT   ,
    C_ENTER,
    C_EXIT,
    C_FAIL  ,
    C_FCONTINUE,
    C_FETCH  ,
    C_FILE   ,
    C_FINDEX,
    C_FMESSAGE,
    C_FREE  ,
    C_FREQUENCY,
    C_FRONT ,
    C_FTIME ,
    C_GETCap ,/* ��ȡBBD��׼����ֵ */
    C_GETOffset ,/* ��ȡƫ�� */
    C_GFI   ,
    C_GR    ,
    C_HIGH  ,
    C_IDN    ,
    C_INS   ,
    C_INSERT,
    C_INTERCHANGE,
    C_IR    ,
    C_ITIME ,
    C_KEY   ,
    C_KLOCK ,
    C_LANGUAGE,
    C_LIST  ,
    C_LOAD  ,
    C_LOCAL  ,
    C_LOW   ,
    C_MEASURE,
    C_MODE  ,
    C_MOVE  ,
    C_NAME  ,
    C_NEW    ,
    C_NOW   ,
    C_NRULE , 
    C_OCOVER,
    C_PASS  ,
    C_PASSWORD,
    C_PHV   ,
    C_POINTS,
    C_PORT  ,
    C_PSIGNAL,
    C_RANGE ,
    C_RCURRENT,
    C_READ   ,
    C_REMOTE ,
    C_RESULT,
    C_RHALT , 
    C_RHINT ,
    C_RSAVE ,
    C_RST    ,
    C_RTIME ,
    C_SADD   ,
    C_SAVE   ,
    C_SCHECK,
    C_SCREEN,
    C_SINDEX,
    C_SINGLE ,
    C_SMESSAGE,
    C_SOUR   ,
    C_SOURCE ,    
    C_START  ,
    C_STATUS ,
    C_STEP  ,
    C_STOP   ,
    C_SYSTEM,
    C_TEST   ,
    C_TIME  ,
    C_TOTS  , 
    C_TPORT ,
    C_TSIGNAL,
    C_TTIME ,
    C_USED  ,
    C_VOLTAGE,
    C_VOLUME,

}CMD_NUM;

typedef enum {
    SCPI_NO_ERROR               ,   /*ָ��ִ�гɹ�*/
    SCPI_SYNTAX_ERROR           ,   /*�﷨����ָ��а������Ϸ����ַ�����ָ���ʽ����ȷ*/
    SCPI_EXECUTE_NOT_ALLOWED    ,   /*ָ��ִ�в������ڵ�ǰ״̬�£�������ִ��ָ��*/
    SCPI_PARAMETER_NOT_ALLOWED  ,   /*���յ�ָ�����Ĳ���*/
    SCPI_MISSING_PARAMETER      ,   /*ָ������©�˲���*/
    SCPI_UNDEFINED_HEADER       ,   /*δ�����ָ��ͷ*/
    SCPI_PARAMETER_TYPE_ERROR   ,   /*�������ʹ���*/
    SCPI_INVALID_STRING_DATA    ,   /*��������ַ�������*/
    SCPI_EXECUTE_TIME_OUT       ,   /*ִ�г�ʱ*/
    SCPI_DATA_OUT_OF_RANGE      ,   /*����ֵ������Χ*/
}SCPI_ERR_T;
extern const char * comm_error_msg[];

typedef enum{
    SCPI_EXE,///<ִ����ָ��
    SCPI_QUERY,///<��ѯ��ָ��
}SCPI_CMD_TYPE;

#define MAX_SCPI_PAR_NUM    50 ///<SCPIָ�����Я��������������
#define MAX_SCPI_CMD_LAYER    4  ///<SCPIָ����԰��������ָ���
typedef struct{
    uint8_t **argv;///<SCPIָ�����Я���Ĳ���
    uint8_t argc;///<SCPIָ�����Я���Ĳ�������
    uint8_t *ask_data;///<SCPIָ��ص����ݻ�����
    uint8_t *ask_len;///<SCPIָ��ص����ݳ��ȣ��ֽ�����
    SCPI_CMD_TYPE type;///<���ָ��������SCPI_EXE SCPI_QUERY
}SCPI_DIS_FUN_PAR;
typedef enum{
    E__,///<ִֻ��
    _W_,///<ֻд
    __R,///<ֻ��
    _WR,///<��д
    EWR,///<��дִ��
}SCPI_ATTRIBUTE;

#define IGNORE_PAR_NUM 0xff //���Բ������������򲻻��Զ�������������ʵ��������ָ�\
                                �Ҳ���������ȷ����ʹ��

typedef struct{
    uint8_t cmd[MAX_SCPI_CMD_LAYER];///<4��ָ��
    SCPI_ATTRIBUTE att;///<ָ������
    uint8_t par_num;///<�������� ���Ϊ IGNORE_PAR_NUM ��Ϊ�Զ���������������򲻻��Զ������������ĺϷ���
    SCPI_ERR_T (*dispose_fun)(SCPI_DIS_FUN_PAR *par);///<������
}SCPI_CMD;


uint8_t find_one_section_cmd(uint8_t *section, uint32_t *cmd_count, SCPI_ERR_T *err);
const SCPI_CMD * find_cmd_num(CMD_NUM_T cmd_num, SCPI_ERR_T *err);

#endif //__SCPI_CMD_DEF_H__


/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
