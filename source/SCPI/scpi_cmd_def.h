/**
  ******************************************************************************
  * @file    scpi_cmd_def.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   SCPI服务器解析器对应的指令列表
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
    C_GETCap ,/* 获取BBD标准电容值 */
    C_GETOffset ,/* 获取偏移 */
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
    SCPI_NO_ERROR                 ,   /*指令执行成功*/
    SCPI_SYNTAX_ERROR             ,   /*语法错误，指令串中包含不合法的字符或者指令串格式不正确*/
    SCPI_EXECUTE_NOT_ALLOWED       ,   /*指令执行不允许，在当前状态下，不允许执行指令*/
    SCPI_PARAMETER_NOT_ALLOWED     ,   /*接收到指令不允许的参数*/
    SCPI_MISSING_PARAMETER        ,   /*指令中遗漏了参数*/
    SCPI_UNDEFINED_HEADER         ,   /*未定义的指令投*/
    SCPI_PARAMETER_TYPE_ERROR      ,   /*参数类型错误*/
    SCPI_INVALID_STRING_DATA       ,   /*不允许的字符串参数*/
    SCPI_EXECUTE_TIME_OUT          ,   /*执行超时*/
    SCPI_DATA_OUT_OF_RANGE          ,   /*参数值超出范围*/
}SCPI_ERR_T;
extern const char * comm_error_msg[];

typedef enum{
    SCPI_EXE,///<执行类指令
    SCPI_QUERY,///<查询类指令
}SCPI_CMD_TYPE;

#define MAX_SCPI_PAR_NUM    50 ///<SCPI指令可以携带的最大参数个数
typedef struct{
    uint8_t **argv;///<SCPI指令处理函数携带的参数
    uint8_t argc;///<SCPI指令处理函数携带的参数个数
    uint8_t *ask_data;///<SCPI指令返回的数据缓冲区
    uint8_t *ask_len;///<SCPI指令返回的数据长度（字节数）
    SCPI_CMD_TYPE type;///<标记指令类型是SCPI_EXE SCPI_QUERY
}SCPI_DIS_FUN_PAR;

typedef struct{
    uint8_t cmd[4];///<4级指令
    SCPI_ERR_T (*dispose_fun)(SCPI_DIS_FUN_PAR *par);///<处理函数
}SCPI_CMD;


uint8_t find_one_section_cmd(uint8_t *section, SCPI_ERR_T *err);
SCPI_CMD * find_cmd_num(CMD_NUM_T cmd_num, SCPI_ERR_T *err);

#endif //__SCPI_CMD_DEF_H__


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
