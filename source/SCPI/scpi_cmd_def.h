/**
  ******************************************************************************
  * @file    scpi_cmd_def.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   SCPI��������������Ӧ��ָ���б�
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
        uint8_t _1;///<��1��ָ��
        uint8_t _2;///<��2��ָ��
        uint8_t _3;///<��3��ָ��
        uint8_t _4;///<��4��ָ��
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
    
    GETCap          ,/* ��ȡBBD��׼����ֵ */
    GETC            ,
    
    GETOffset       ,/* ��ȡƫ�� */
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
    SCPI_NO_ERROR                 ,   /*ָ��ִ�гɹ�*/
    SCPI_SYNTAX_ERROR             ,   /*�﷨����ָ��а������Ϸ����ַ�����ָ���ʽ����ȷ*/
    SCPI_EXECUTE_NOT_ALLOWED       ,   /*ָ��ִ�в������ڵ�ǰ״̬�£�������ִ��ָ��*/
    SCPI_PARAMETER_NOT_ALLOWED     ,   /*���յ�ָ�����Ĳ���*/
    SCPI_MISSING_PARAMETER        ,   /*ָ������©�˲���*/
    SCPI_UNDEFINED_HEADER         ,   /*δ�����ָ��Ͷ*/
    SCPI_PARAMETER_TYPE_ERROR      ,   /*�������ʹ���*/
    SCPI_INVALID_STRING_DATA       ,   /*��������ַ�������*/
    SCPI_EXECUTE_TIME_OUT          ,   /*ִ�г�ʱ*/
    SCPI_DATA_OUT_OF_RANGE          ,   /*����ֵ������Χ*/
}SCPI_ERR_T;

#define     FIRST_CMD       ( cmd[0] )
#define     SECOND_CMD      ( cmd[1] )
#define     THIRD_CMD       ( cmd[2] )
#define     FOURTH_CMD      ( cmd[3] )


#endif
