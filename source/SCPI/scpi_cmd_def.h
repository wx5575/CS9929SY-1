/**
  ******************************************************************************
  * @file    scpi_cmd_def.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   SCPI服务器解析器对应的指令列表
  ******************************************************************************
  */
#ifndef __SCPI_CFG_H
#define __SCPI_CFG_H

#include <stm32f4xx.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif



typedef struct{
    struct{
        uint8_t _1;///<第1级指令
        uint8_t _2;///<第2级指令
        uint8_t _3;///<第3级指令
        uint8_t _4;///<第4级指令
    }cmd;
    void(*fun)(void);
}SCPI_CMD;

typedef enum{
    IDN             ,
    RST             ,
    COMM            ,
    COMMUNICATION   ,
    SADD            ,
    SADDRESS        ,
    REMOT           ,
    REMOTE          ,
    LOC             ,
    LOCAL           ,
    CONT            ,
    CONTROL         ,
    FILE            ,
    NEW             ,
    EDIT            ,
    DEL             ,
    DELETE          ,
    SING            ,
    SINGLE          ,
    ALL             ,
    SAVE            ,
    READ            ,
    CAT             ,
    CATALOG         ,  
    SOUR            ,
    SOURCE          ,    
    TEST            ,
    STAR            ,
    START           ,
    STOP            ,
    STAT            ,
    STATUS          ,
    FETC            ,
    FETCH           ,
    
    GETCap          ,/* 获取BBD标准电容值 */
    GETC            ,
    
    GETOffset       ,/* 获取偏移 */
    GETO            ,
    
    LOAD            ,
    STEP            ,
    SSTEP           ,
    LIST            ,
    FIND            ,
    FINDEX          ,
    FMES            ,
    FMESSAGE        ,
    SIND            ,
    SINDEX          ,
    SMES            ,
    SMESSAGE        ,
    MOVE            ,
    INS             ,
    INSERT          ,
    ACW             ,
    DCW             ,
    IR              ,
    GR              ,
    BBD             ,
    FRON            ,
    FRONT           ,
    BEH             ,
    BEHIND          ,
    INT             ,
    INTERCHANGE     ,
    MODE            ,
    VOLT            ,
    VOLTAGE         ,
    RANG            ,
    RANGE           ,
    HIGH            ,
    LOW             ,
    
    BBDOpen         ,
    BBDO            ,
    BBDShort        ,
    BBDS            ,
    
    RCUR            ,
    RCURRENT        ,
    ARC             ,
    FREQ            ,
    FREQUENCY       ,
    RTIM            ,
    RTIME           ,
    TTIM            ,
    TTIME           ,
    FTIM            ,
    FTIME           ,
    ITIM            ,
    ITIME           ,
    CTIM            ,
    CTIME           ,
    PSIG            ,
    PSIGNAL         ,
    CNEX            ,
    CNEXT           ,
    PORT            ,
    CCUR            ,
    CCURRENT        ,
    DTIM            ,
    DTIME           ,
    ARAN            ,
    ARANGE          ,
    CURR            ,
    CURRENT         ,
    RES             ,
    RESULT          ,
    CAP             ,
    CAPACITY        ,
    USED            ,
    FREE            ,
    PASS            ,
    FAIL            ,
    CLE             ,
    CLEAR           ,  
    DUT             ,
    NAME            ,
    SYST            ,
    SYSTEM          ,
    SYSTERM         ,
    SCR             ,
    SCREEN          ,
    CONTRAST        ,
    BEEP            ,
    BEEPER          ,
    VOL             ,
    VOLUME          ,
    RHIN            ,
    RHINT           ,
    RSAV            ,
    RSAVE           ,
    OCOV            ,
    OCOVER          ,
    GFI             ,
    PHV             ,
    TSIG            ,
    TSIGNAL         ,
    SCH             ,
    SCHECK          ,
    CHIN            ,
    CHINT           ,
    TPOR            ,
    TPORT           ,
    LANG            ,
    LANGUAGE        ,
    FCON            ,
    FCONTINUE       ,
    KEY             ,
    KLOC            ,
    KLOCK           ,
    PASSWORD        ,
    NOW             ,
    TIME            ,
    NRUL            ,
    NRULE           , 
    RHAL            ,
    RHALT           , 
    ZORE            ,
    ONE             ,
    TWO             ,
    THREE           , 
    TOTS            , 
	
	OFFSET			,
	OFFS			,
	
	CALIBRATION,
	CAL,
	ENTER,
	ENT,
	EXIT,
	MEASURE,
	MEAS,
	POINTS,
	POIN,
	
    END_OF_CMD_LIST ,
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

#define     FIRST_CMD       ( cmd[0] )
#define     SECOND_CMD      ( cmd[1] )
#define     THIRD_CMD       ( cmd[2] )
#define     FOURTH_CMD      ( cmd[3] )


#endif
