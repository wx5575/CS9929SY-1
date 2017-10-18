/**
  ******************************************************************************
  * @file    scpi_cmd_def.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   SCPI��������������Ӧ��ָ���б�
  ******************************************************************************
  */
#include "scpi_cmd_def.h"
#include "string.h"
#include "cs99xx_scpi_dispose.h"

typedef struct{
    const char*cmd;
    CMD_NUM     num;
}CMD_SEGMENT_T;
CMD_SEGMENT_T scpi_segment_pool[]=
{
    "*IDN"          , C_IDN,
    "*RST"          , C_RST,
    "ACW"           , C_ACW,
    "ALL"           , C_ALL,
    "ARAN"          , C_ARANGE,
    "ARANge"        , C_ARANGE,
    "ARC"           , C_ARC,
    "BBD"           , C_BBD,
    "BBDO"          , C_BBDOpen,
    "BBDOpen"       , C_BBDOpen,
    "BBDS"          , C_BBDShort,
    "BBDShort"      , C_BBDShort,
    "BEEP"          , C_BEEPER,
    "BEEPer"        , C_BEEPER,
    "BEH"           , C_BEHIND,
    "BEHind"        , C_BEHIND,
    "CAP"           , C_CAPACITY,
    "CAPacity"      , C_CAPACITY,
    "CAT"           , C_CATALOG,
    "CATalog"       , C_CATALOG,
    "CCUR"          , C_CCURRENT,
    "CCURrent"      , C_CCURRENT,
    "CHIN"          , C_CHINT,
    "CHINt"         , C_CHINT,
    "CLE"           , C_CLEAR,
    "CLEar"         , C_CLEAR,
    "CNEX"          , C_CNEXT,
    "CNEXt"         , C_CNEXT,
    "COMM"          , C_COMM,
    "COMMunication" , C_COMM,
    "CONT"          , C_CONTRAST,
    "CONT"          , C_CONTROL,
    "CONTrast"      , C_CONTRAST,
    "CONTrol"       , C_CONTROL,
    "CTIM"          , C_CTIME,
    "CTIMe"         , C_CTIME,
    "CURR"          , C_CURRENT,
    "CURRent"       , C_CURRENT,
    "DCW"           , C_DCW,
    "DEL"           , C_DELETE,
    "DELete"        , C_DELETE,
    "DTIM"          , C_DTIME,
    "DTIMe"         , C_DTIME,
    "DUT"           , C_DUT,
    "EDIT"          , C_EDIT,
    "FAIL"          , C_FAIL,
    "FCON"          , C_FCONTINUE,
    "FCONtinue"     , C_FCONTINUE,
    "FETC"          , C_FETCH,
    "FETCh"         , C_FETCH,
    "FILE"          , C_FILE,
    "FIND"          , C_FINDEX,
    "FINDex"        , C_FINDEX,
    "FMES"          , C_FMESSAGE,
    "FMESsage"      , C_FMESSAGE,
    "FREE"          , C_FREE,
    "FREQ"          , C_FREQUENCY,
    "FREQuency"     , C_FREQUENCY,
    "FRON"          , C_FRONT,
    "FRONt"         , C_FRONT,
    "FTIM"          , C_FTIME,
    "FTIMe"         , C_FTIME,
    "GETC"          , C_GETCap,
    "GETCap"        , C_GETCap,
    "GETO"          , C_GETOffset,
    "GETOffset"     , C_GETOffset,
    "GFI"           , C_GFI,
    "GR"            , C_GR,
    "HIGH"          , C_HIGH,
    "INS"           , C_INSERT,
    "INSert"        , C_INSERT,
    "INT"           , C_INTERCHANGE,
    "INTerchange"   , C_INTERCHANGE,
    "IR"            , C_IR,
    "ITIM"          , C_ITIME,
    "ITIMe"         , C_ITIME,
    "KEY"           , C_KEY,
    "KLOC"          , C_KLOCK,
    "KLOCk"         , C_KLOCK,
    "LANG"          , C_LANGUAGE,
    "LANGuage"      , C_LANGUAGE,
    "LIST"          , C_LIST,
    "LOAD"          , C_LOAD,
    "LOC"           , C_LOCAL,
    "LOCal"         , C_LOCAL,
    "LOW"           , C_LOW,
    "MODE"          , C_MODE,
    "MOVE"          , C_MOVE,
    "NAME"          , C_NAME,
    "NEW"           , C_NEW,
    "NOW"           , C_NOW,
    "NRUL"          , C_NRULE,
    "NRULe"         , C_NRULE,
    "OCOV"          , C_OCOVER,
    "OCOVer"        , C_OCOVER,
    "PASS"          , C_PASS,
    "PASSword"      , C_PASSWORD,
    "PHV"           , C_PHV,
    "PORT"          , C_PORT,
    "PSIG"          , C_PSIGNAL,
    "PSIGnal"       , C_PSIGNAL,
    "RANG"          , C_RANGE,
    "RANGe"         , C_RANGE,
    "RCUR"          , C_RCURRENT,
    "RCURrent"      , C_RCURRENT,
    "READ"          , C_READ,
    "REM"           , C_REMOTE,
    "REMote"        , C_REMOTE,
    "RES"           , C_RESULT,
    "RESult"        , C_RESULT,
    "RHAL"          , C_RHALT,
    "RHALt"         , C_RHALT,
    "RHIN"          , C_RHINT,
    "RHINt"         , C_RHINT,
    "RSAV"          , C_RSAVE,
    "RSAVe"         , C_RSAVE,
    "RTIM"          , C_RTIME,
    "RTIMe"         , C_RTIME,
    "SADD"          , C_SADD,
    "SADDress"      , C_SADD,
    "SAVE"          , C_SAVE,
    "SCH"           , C_SCHECK,
    "SCHeck"        , C_SCHECK,
    "SCR"           , C_SCREEN,
    "SCReen"        , C_SCREEN,
    "SIND"          , C_SINDEX,
    "SINDex"        , C_SINDEX,
    "SING"          , C_SINGLE,
    "SINGle"        , C_SINGLE,
    "SMES"          , C_SMESSAGE,
    "SMESsage"      , C_SMESSAGE,
    "SOUR"          , C_SOURCE,
    "SOURce"        , C_SOURCE,
    "STAR"          , C_START,
    "STARt"         , C_START,
    "STAT"          , C_STATUS,
    "STATus"        , C_STATUS,
    "STEP"          , C_STEP,
    "STOP"          , C_STOP,
    "SYST"          , C_SYSTEM,
    "SYSTem"        , C_SYSTEM,
    "TEST"          , C_TEST,
    "TIME"          , C_TIME,
    "TOTS"          , C_TOTS,
    "TPOR"          , C_TPORT,
    "TPORt"         , C_TPORT,
    "TSIG"          , C_TSIGNAL,
    "TSIGnal"       , C_TSIGNAL,
    "TTIM"          , C_TTIME,
    "TTIMe"         , C_TTIME,
    "USED"          , C_USED,
    "VOL"           , C_VOLUME,
    "VOLT"          , C_VOLTAGE,
    "VOLTage"       , C_VOLTAGE,
    "VOLume"        , C_VOLUME,
};

/**
  * @brief  ������Ϣָ���б�
  */
const char * comm_error_msg[] =
{
    "+0,\"No error\""                   ,
    "-102,\"Syntax error\""             ,
    "-105,\"Execute not allowed\""      ,
    "-108,\"Parameter not allowed\""    ,
    "-109,\"Missing parameter\""        ,
    "-113,\"Undefined header\""         ,
    "-120,\"Parameter type error\""     ,
    "-151,\"Invalid string data\""      ,
    "-152,\"Execute time out\""         ,
    "-222,\"Data out of range\""        ,
};


SCPI_CMD scpi_cmd_pool[]=
{
    /* ����ָ� */
    {{C_IDN}                , __R, 0, idn_scpi_dispose_fun},
    {{C_RST}                , E__, 0, rst_scpi_dispose_fun},
    /* ͨ��ָ� */
    {{C_COMM,C_SADD}        , _WR, 1, comm_sadd_scpi_dispose_fun},
    {{C_COMM,C_REMOTE}      , E__, 0, comm_remote_scpi_dispose_fun},
    {{C_COMM,C_LOCAL}       , E__, 0, comm_local_scpi_dispose_fun},
    {{C_COMM,C_CONTROL}     , __R, 0, comm_control_scpi_dispose_fun},
    /* �ļ�ָ� */
    {{C_FILE,C_NEW}             , _W_, IGNORE_PAR_NUM   , file_new_scpi_dispose_fun},
    {{C_FILE,C_EDIT}            , _W_, IGNORE_PAR_NUM   , file_edit_scpi_dispose_fun},
    {{C_FILE,C_DELETE,C_SINGLE} , E__, 1                , file_del_single_scpi_dispose_fun},
    {{C_FILE,C_DELETE,C_ALL}    , E__, 0                , file_del_all_scpi_dispose_fun},
    {{C_FILE,C_SAVE}            , _W_, 2                , file_save_scpi_dispose_fun},
    {{C_FILE,C_READ}            , E__, 1                , file_read_scpi_dispose_fun},
    {{C_FILE,C_CATALOG,C_SINGLE}, __R, 1                , file_catalog_single_scpi_dispose_fun},
    /* Դָ� */
    {{C_SOURCE,C_TEST,C_START}      , source_test_start_scpi_dispose_fun},
    {{C_SOURCE,C_TEST,C_STOP}       , source_test_stop_scpi_dispose_fun},
    {{C_SOURCE,C_TEST,C_STATUS}     , source_test_status_scpi_dispose_fun},
    {{C_SOURCE,C_TEST,C_FETCH}      , source_test_fetch_scpi_dispose_fun},
    {{C_SOURCE,C_LOAD,C_STEP}       , source_load_step_scpi_dispose_fun},
    {{C_SOURCE,C_LOAD,C_FILE}       , source_load_file_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_FINDEX}     , source_list_findex_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_FMESSAGE}   , source_list_fmessage_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_SINDEX}     , source_list_sindex_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_SMESSAGE}   , source_list_smessage_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_MODE}       , source_list_mode_scpi_dispose_fun},
    /* ����ָ� */
    {{C_STEP,C_INSERT,C_ACW}    , step_insert_acw_scpi_dispose_fun},
    {{C_STEP,C_INSERT,C_DCW}    , step_insert_dcw_scpi_dispose_fun},
    {{C_STEP,C_INSERT,C_IR}     , step_insert_ir_scpi_dispose_fun},
    {{C_STEP,C_INSERT,C_GR}     , step_insert_gr_scpi_dispose_fun},
    {{C_STEP,C_DELETE}          , step_delete_scpi_dispose_fun},
    {{C_STEP,C_MOVE,C_FRONT}    , step_move_front_scpi_dispose_fun},
    {{C_STEP,C_MOVE,C_BEHIND}   , step_move_behind_scpi_dispose_fun},
    {{C_STEP,C_INTERCHANGE}     , step_interchange_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_ACW}      , step_mode_acw_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_DCW}      , step_mode_dcw_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_IR}       , step_mode_ir_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_GR}       , step_mode_gr_scpi_dispose_fun},
    /* ACW */
    {{C_STEP,C_ACW,C_VOLTAGE}   , step_acw_voltage_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_RANGE}     , step_acw_range_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_HIGH}      , step_acw_high_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_LOW}       , step_acw_low_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_RCURRENT}  , step_acw_real_current_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_ARC}       , step_acw_arc_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_FREQUENCY} , step_acw_frequency_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_RTIME}     , step_acw_rtime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_TTIME}     , step_acw_ttime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_FTIME}     , step_acw_ftime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_ITIME}     , step_acw_itime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_CTIME}     , step_acw_ctime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_PSIGNAL}   , step_acw_psignal_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_CNEXT}     , step_acw_cnext_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_PORT}      , step_acw_port_scpi_dispose_fun},
    /* DCW */
    {{C_STEP,C_DCW,C_VOLTAGE}   , step_dcw_voltage_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_RANGE}     , step_dcw_range_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_HIGH}      , step_dcw_high_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_LOW}       , step_dcw_low_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_CCURRENT}  , step_dcw_ccurrent_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_ARC}       , step_dcw_arc_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_DTIME}     , step_dcw_dtime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_RTIME}     , step_dcw_rtime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_TTIME}     , step_dcw_ttime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_FTIME}     , step_dcw_ftime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_ITIME}     , step_dcw_itime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_CTIME}     , step_dcw_ctime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_PSIGNAL}   , step_dcw_psignal_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_CNEXT}     , step_dcw_cnext_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_PORT}      , step_dcw_port_scpi_dispose_fun},
    /* IR */
    {{C_STEP,C_IR,C_VOLTAGE}    , step_ir_voltage_scpi_dispose_fun},
    {{C_STEP,C_IR,C_ARANGE}     , step_ir_range_scpi_dispose_fun},
    {{C_STEP,C_IR,C_HIGH}       , step_ir_high_scpi_dispose_fun},
    {{C_STEP,C_IR,C_LOW}        , step_ir_low_scpi_dispose_fun},
    {{C_STEP,C_IR,C_RTIME}      , step_ir_rtime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_TTIME}      , step_ir_ttime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_DTIME}      , step_ir_dtime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_ITIME}      , step_ir_itime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_PSIGNAL}    , step_ir_psignal_scpi_dispose_fun},
    {{C_STEP,C_IR,C_CNEXT}      , step_ir_cnext_scpi_dispose_fun},
    {{C_STEP,C_IR,C_PORT}       , step_ir_port_scpi_dispose_fun},
    /* GR */
    {{C_STEP,C_GR,C_CURRENT}    , step_gr_current_scpi_dispose_fun},
    {{C_STEP,C_GR,C_HIGH}       , step_gr_high_scpi_dispose_fun},
    {{C_STEP,C_GR,C_LOW}        , step_gr_low_scpi_dispose_fun},
    {{C_STEP,C_GR,C_TTIME}      , step_gr_ttime_scpi_dispose_fun},
    {{C_STEP,C_GR,C_ITIME}      , step_gr_itime_scpi_dispose_fun},
    {{C_STEP,C_GR,C_PSIGNAL}    , step_gr_psignal_scpi_dispose_fun},
    {{C_STEP,C_GR,C_CNEXT}      , step_gr_cnext_scpi_dispose_fun},
    /* ���ָ� */
    {{C_RESULT,C_CAPACITY,C_USED}   , result_cap_used_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_FREE}   , result_cap_free_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_ALL}    , result_cap_all_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_PASS}   , result_cap_pass_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_FAIL}   , result_cap_fail_scpi_dispose_fun},
    {{C_RESULT,C_CLEAR,C_ALL}       , result_clear_all_scpi_dispose_fun},
    {{C_RESULT,C_FETCH,C_SINGLE}    , result_fetch_single_scpi_dispose_fun},
    {{C_RESULT,C_DUT,C_NAME}        , result_dut_name_scpi_dispose_fun},
    /* ϵͳָ� */
    {{C_SYSTEM,C_SCREEN,C_CONTRAST} , sys_screen_contrast_scpi_dispose_fun},
    {{C_SYSTEM,C_BEEPER,C_VOLUME}   , sys_beeper_volume_scpi_dispose_fun},
    {{C_SYSTEM,C_RHINT}             , sys_rhint_scpi_dispose_fun},
    {{C_SYSTEM,C_RSAVE}             , sys_rsave_scpi_dispose_fun},
    {{C_SYSTEM,C_OCOVER}            , sys_ocover_scpi_dispose_fun},
    {{C_SYSTEM,C_GFI}               , sys_gfi_scpi_dispose_fun},
    {{C_SYSTEM,C_PHV}               , sys_phv_scpi_dispose_fun},
    {{C_SYSTEM,C_TSIGNAL}           , sys_tsignal_scpi_dispose_fun},
    {{C_SYSTEM,C_SCHECK}            , sys_scheck_scpi_dispose_fun},
    {{C_SYSTEM,C_CHINT}             , sys_chint_scpi_dispose_fun},
    {{C_SYSTEM,C_TPORT}             , sys_tport_scpi_dispose_fun},
    {{C_SYSTEM,C_LANGUAGE}          , sys_language_scpi_dispose_fun},
    {{C_SYSTEM,C_FCONTINUE}         , sys_fcontinue_scpi_dispose_fun},
    {{C_SYSTEM,C_KEY,C_KLOCK}       , sys_klock_scpi_dispose_fun},
    {{C_SYSTEM,C_KEY,C_PASSWORD,C_NEW}, sys_password_new_scpi_dispose_fun},
    {{C_SYSTEM,C_KEY,C_PASSWORD,C_NOW}, sys_password_now_scpi_dispose_fun},
    {{C_SYSTEM,C_TIME}              , sys_time_scpi_dispose_fun},
    {{C_SYSTEM,C_NRULE}             , sys_nrule_scpi_dispose_fun},
};

SCPI_CMD * find_cmd_num(CMD_NUM_T cmd_num, SCPI_ERR_T *err)
{
    uint8_t n = 0;
    int32_t i = 0;
    CMD_NUM_T *cmd_t;
    SCPI_CMD *scpi_cmd = scpi_cmd_pool;
    
    n = ARRAY_SIZE(scpi_cmd_pool);
    
    for(i = 0; i < n; i++, scpi_cmd++)
    {
        cmd_t = (CMD_NUM_T *)scpi_cmd;
        
        if(*cmd_t == cmd_num)
        {
            *err = SCPI_NO_ERROR;
            return scpi_cmd;
        }
    }
    
    *err = SCPI_UNDEFINED_HEADER;
    return NULL;
}

uint8_t find_one_section_cmd(uint8_t *section, uint32_t *cmd_count, SCPI_ERR_T *err)
{
    uint8_t num = 0;
    int32_t i = 0;
    
    num = ARRAY_SIZE(scpi_segment_pool);
    
    for(i = *cmd_count; i < num; i++)
    {
        if( 0 == strcmp((const char*)section, scpi_segment_pool[i].cmd))
        {
            *err = SCPI_NO_ERROR;
            *cmd_count = i + 1;
            return scpi_segment_pool[i].num;
        }
    }
    
    *err = SCPI_UNDEFINED_HEADER;
    
    return 0;
}

