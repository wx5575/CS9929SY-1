/**
  ******************************************************************************
  * @file    scpi_cmd_def.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.13
  * @brief   SCPI服务器解析器对应的指令列表
  ******************************************************************************
  */
#include "scpi_cmd_def.h"
#include "string.h"
#include "cs99xx_scpi_dispose.h"

typedef struct{
    const char*cmd;
    CMD_NUM     num;
}CMD_SEGMENT_T;
/**
  * @brief  SCPI段与对应的数字编号，其中SCPI段支持重码，例如：
  * "CONT"          , C_CONTRAST,
  * "CONT"          , C_CONTROL,
  * "PASS"          , C_PASS,
  * "PASS"          , C_PASSWORD,
  */
static const CMD_SEGMENT_T scpi_segment_pool[]=
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
    "CAL"           , C_CALIBRATION,
    "CALibration"   , C_CALIBRATION,
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
    "ENT"           , C_ENTER,
    "ENTer"         , C_ENTER,
    "EXIT"          , C_EXIT,
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
    "MEAS"          , C_MEASURE,
    "MEASure"       , C_MEASURE,
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
    "PASS"          , C_PASSWORD,
    "PASSword"      , C_PASSWORD,
    "PHV"           , C_PHV,
    "POIN"          , C_POINTS,
    "POINts"        , C_POINTS,
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
  * @brief  错误信息指令列表
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

/**
  * @brief  SCPI命令池
  */
static const SCPI_CMD scpi_cmd_pool[]=
{
    /* 公用指令集 */
    {{C_IDN}                , __R, 0, idn_scpi_dispose_fun},
    {{C_RST}                , E__, 0, rst_scpi_dispose_fun},
    /* 通信指令集 */
    {{C_COMM,C_SADD}        , _WR, 1, comm_sadd_scpi_dispose_fun},
    {{C_COMM,C_REMOTE}      , E__, 0, comm_remote_scpi_dispose_fun},
    {{C_COMM,C_LOCAL}       , E__, 0, comm_local_scpi_dispose_fun},
    {{C_COMM,C_CONTROL}     , __R, 0, comm_control_scpi_dispose_fun},
    /* 文件指令集 */
    {{C_FILE,C_NEW}             , _W_, IGNORE_PAR_NUM   , file_new_scpi_dispose_fun},
    {{C_FILE,C_EDIT}            , _W_, IGNORE_PAR_NUM   , file_edit_scpi_dispose_fun},
    {{C_FILE,C_DELETE,C_SINGLE} , E__, 1                , file_del_single_scpi_dispose_fun},
    {{C_FILE,C_DELETE,C_ALL}    , E__, 0                , file_del_all_scpi_dispose_fun},
    {{C_FILE,C_SAVE}            , _W_, 2                , file_save_scpi_dispose_fun},
    {{C_FILE,C_READ}            , E__, 1                , file_read_scpi_dispose_fun},
    {{C_FILE,C_CATALOG,C_SINGLE}, __R, 1                , file_catalog_single_scpi_dispose_fun},
    /* 源指令集 */
    {{C_SOURCE,C_TEST,C_START}      , E__, 0, source_test_start_scpi_dispose_fun},
    {{C_SOURCE,C_TEST,C_STOP}       , E__, 0, source_test_stop_scpi_dispose_fun},
    {{C_SOURCE,C_TEST,C_STATUS}     , __R, 0, source_test_status_scpi_dispose_fun},
    {{C_SOURCE,C_TEST,C_FETCH}      , __R, 1, source_test_fetch_scpi_dispose_fun},
    {{C_SOURCE,C_LOAD,C_STEP}       , E__, 1, source_load_step_scpi_dispose_fun},
    {{C_SOURCE,C_LOAD,C_FILE}       , E__, 1, source_load_file_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_FINDEX}     , __R, 0, source_list_findex_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_FMESSAGE}   , __R, 0, source_list_fmessage_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_SINDEX}     , __R, 0, source_list_sindex_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_SMESSAGE}   , __R, 0, source_list_smessage_scpi_dispose_fun},
    {{C_SOURCE,C_LIST,C_MODE}       , __R, 0, source_list_mode_scpi_dispose_fun},
    /* 步骤指令集 */
    {{C_STEP,C_INSERT,C_ACW}    , E__, 0, step_insert_acw_scpi_dispose_fun},
    {{C_STEP,C_INSERT,C_DCW}    , E__, 0, step_insert_dcw_scpi_dispose_fun},
    {{C_STEP,C_INSERT,C_IR}     , E__, 0, step_insert_ir_scpi_dispose_fun},
    {{C_STEP,C_INSERT,C_GR}     , E__, 0, step_insert_gr_scpi_dispose_fun},
    {{C_STEP,C_DELETE}          , E__, 0, step_delete_scpi_dispose_fun},
    {{C_STEP,C_MOVE,C_FRONT}    , E__, 0, step_move_front_scpi_dispose_fun},
    {{C_STEP,C_MOVE,C_BEHIND}   , E__, 0, step_move_behind_scpi_dispose_fun},
    {{C_STEP,C_INTERCHANGE}     , E__, 1, step_interchange_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_ACW}      , E__, 0, step_mode_acw_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_DCW}      , E__, 0, step_mode_dcw_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_IR}       , E__, 0, step_mode_ir_scpi_dispose_fun},
    {{C_STEP,C_MODE,C_GR}       , E__, 0, step_mode_gr_scpi_dispose_fun},
    /* ACW */
    {{C_STEP,C_ACW,C_VOLTAGE}   , _WR, 1, step_acw_voltage_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_RANGE}     , _WR, 1, step_acw_range_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_HIGH}      , _WR, 1, step_acw_high_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_LOW}       , _WR, 1, step_acw_low_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_RCURRENT}  , _WR, 1, step_acw_real_current_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_ARC}       , _WR, 1, step_acw_arc_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_FREQUENCY} , _WR, 1, step_acw_frequency_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_RTIME}     , _WR, 1, step_acw_rtime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_TTIME}     , _WR, 1, step_acw_ttime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_FTIME}     , _WR, 1, step_acw_ftime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_ITIME}     , _WR, 1, step_acw_itime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_CTIME}     , _WR, 1, step_acw_ctime_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_PSIGNAL}   , _WR, 1, step_acw_psignal_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_CNEXT}     , _WR, 1, step_acw_cnext_scpi_dispose_fun},
    {{C_STEP,C_ACW,C_PORT}      , _WR, IGNORE_PAR_NUM, step_acw_port_scpi_dispose_fun},
    /* DCW */
    {{C_STEP,C_DCW,C_VOLTAGE}   , _WR, 1, step_dcw_voltage_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_RANGE}     , _WR, 1, step_dcw_range_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_HIGH}      , _WR, 1, step_dcw_high_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_LOW}       , _WR, 1, step_dcw_low_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_CCURRENT}  , _WR, 1, step_dcw_ccurrent_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_ARC}       , _WR, 1, step_dcw_arc_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_DTIME}     , _WR, 1, step_dcw_dtime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_RTIME}     , _WR, 1, step_dcw_rtime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_TTIME}     , _WR, 1, step_dcw_ttime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_FTIME}     , _WR, 1, step_dcw_ftime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_ITIME}     , _WR, 1, step_dcw_itime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_CTIME}     , _WR, 1, step_dcw_ctime_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_PSIGNAL}   , _WR, 1, step_dcw_psignal_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_CNEXT}     , _WR, 1, step_dcw_cnext_scpi_dispose_fun},
    {{C_STEP,C_DCW,C_PORT}      , _WR, IGNORE_PAR_NUM, step_dcw_port_scpi_dispose_fun},
    /* IR */
    {{C_STEP,C_IR,C_VOLTAGE}    , _WR, 1, step_ir_voltage_scpi_dispose_fun},
    {{C_STEP,C_IR,C_ARANGE}     , _WR, 1, step_ir_arange_scpi_dispose_fun},
    {{C_STEP,C_IR,C_HIGH}       , _WR, 1, step_ir_high_scpi_dispose_fun},
    {{C_STEP,C_IR,C_LOW}        , _WR, 1, step_ir_low_scpi_dispose_fun},
    {{C_STEP,C_IR,C_RTIME}      , _WR, 1, step_ir_rtime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_TTIME}      , _WR, 1, step_ir_ttime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_DTIME}      , _WR, 1, step_ir_dtime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_ITIME}      , _WR, 1, step_ir_itime_scpi_dispose_fun},
    {{C_STEP,C_IR,C_PSIGNAL}    , _WR, 1, step_ir_psignal_scpi_dispose_fun},
    {{C_STEP,C_IR,C_CNEXT}      , _WR, 1, step_ir_cnext_scpi_dispose_fun},
    {{C_STEP,C_IR,C_PORT}       , _WR, IGNORE_PAR_NUM, step_ir_port_scpi_dispose_fun},
    /* GR */
    {{C_STEP,C_GR,C_CURRENT}    , _WR, 1, step_gr_current_scpi_dispose_fun},
    {{C_STEP,C_GR,C_HIGH}       , _WR, 1, step_gr_high_scpi_dispose_fun},
    {{C_STEP,C_GR,C_LOW}        , _WR, 1, step_gr_low_scpi_dispose_fun},
    {{C_STEP,C_GR,C_TTIME}      , _WR, 1, step_gr_ttime_scpi_dispose_fun},
    {{C_STEP,C_GR,C_ITIME}      , _WR, 1, step_gr_itime_scpi_dispose_fun},
    {{C_STEP,C_GR,C_PSIGNAL}    , _WR, 1, step_gr_psignal_scpi_dispose_fun},
    {{C_STEP,C_GR,C_CNEXT}      , _WR, 1, step_gr_cnext_scpi_dispose_fun},
    /* 结果指令集 */
    {{C_RESULT,C_CAPACITY,C_USED}   , __R, 0, result_cap_used_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_FREE}   , __R, 0, result_cap_free_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_ALL}    , __R, 0, result_cap_all_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_PASS}   , __R, 0, result_cap_pass_scpi_dispose_fun},
    {{C_RESULT,C_CAPACITY,C_FAIL}   , __R, 0, result_cap_fail_scpi_dispose_fun},
    {{C_RESULT,C_CLEAR,C_ALL}       , E__, 0, result_clear_all_scpi_dispose_fun},
    {{C_RESULT,C_FETCH,C_SINGLE}    , __R, 1, result_fetch_single_scpi_dispose_fun},
    {{C_RESULT,C_DUT,C_NAME}        , _WR, 1, result_dut_name_scpi_dispose_fun},
    /* 系统指令集 */
    {{C_SYSTEM,C_SCREEN,C_CONTRAST} , _WR, 1, sys_screen_contrast_scpi_dispose_fun},
    {{C_SYSTEM,C_BEEPER,C_VOLUME}   , _WR, 1, sys_beeper_volume_scpi_dispose_fun},
    {{C_SYSTEM,C_RHINT}             , _WR, 1, sys_rhint_scpi_dispose_fun},
    {{C_SYSTEM,C_RSAVE}             , _WR, 1, sys_rsave_scpi_dispose_fun},
    {{C_SYSTEM,C_OCOVER}            , _WR, 1, sys_ocover_scpi_dispose_fun},
    {{C_SYSTEM,C_GFI}               , _WR, 1, sys_gfi_scpi_dispose_fun},
    {{C_SYSTEM,C_PHV}               , _WR, 1, sys_phv_scpi_dispose_fun},
    {{C_SYSTEM,C_TSIGNAL}           , _WR, 1, sys_tsignal_scpi_dispose_fun},
    {{C_SYSTEM,C_SCHECK}            , _WR, 1, sys_scheck_scpi_dispose_fun},
    {{C_SYSTEM,C_CHINT}             , _WR, 1, sys_chint_scpi_dispose_fun},
    {{C_SYSTEM,C_TPORT}             , _WR, 1, sys_tport_scpi_dispose_fun},
    {{C_SYSTEM,C_LANGUAGE}          , _WR, 1, sys_language_scpi_dispose_fun},
    {{C_SYSTEM,C_FCONTINUE}         , _WR, 1, sys_fcontinue_scpi_dispose_fun},
    {{C_SYSTEM,C_KEY,C_KLOCK}       , _WR, 1, sys_klock_scpi_dispose_fun},
    {{C_SYSTEM,C_KEY,C_PASSWORD,C_NEW}, _W_, 2, sys_password_new_scpi_dispose_fun},
    {{C_SYSTEM,C_KEY,C_PASSWORD,C_NOW}, __R, 0, sys_password_now_scpi_dispose_fun},
    {{C_SYSTEM,C_TIME}              , _WR, 6, sys_time_scpi_dispose_fun},
    {{C_SYSTEM,C_NRULE}             , _WR, 1, sys_nrule_scpi_dispose_fun},
    /* 校准指令集 */
    {{C_CALIBRATION,C_ENTER}  , E__, 0, cal_enter_scpi_dispose_fun},
    {{C_CALIBRATION,C_EXIT}   , E__, 0, cal_exit_scpi_dispose_fun},
    {{C_CALIBRATION,C_STATUS} , __R, 0, cal_status_scpi_dispose_fun},
    {{C_CALIBRATION,C_MODE}   , __R, 0, cal_mode_scpi_dispose_fun},
    {{C_CALIBRATION,C_RANGE}  , __R, 0, cal_range_scpi_dispose_fun},
    {{C_CALIBRATION,C_START}  , E__, 0, cal_start_scpi_dispose_fun},
    {{C_CALIBRATION,C_STOP}   , E__, 0, cal_stop_scpi_dispose_fun},
    {{C_CALIBRATION,C_MEASURE}, _W_, 1, cal_measure_scpi_dispose_fun},
    {{C_CALIBRATION,C_POINTS} , __R, 0, cal_points_scpi_dispose_fun},
    {{C_CALIBRATION,C_LOAD}   , E__, 1, cal_load_scpi_dispose_fun},
};
/**
  * @brief  查找命令码
  * @param  [in] cmd_num SCPI指令属性
  * @param  [in] err 错误码
  *         @arg SCPI_NO_ERROR
  *         @arg SCPI_UNDEFINED_HEADER
  * @retval 查找结果,SCPI命令结构信息
  */
const SCPI_CMD * find_cmd_num(CMD_NUM_T cmd_num, SCPI_ERR_T *err)
{
    uint8_t n = 0;
    int32_t i = 0;
    CMD_NUM_T *cmd_t;
    const SCPI_CMD *scpi_cmd = scpi_cmd_pool;
    
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
/**
  * @brief  查找SCPI一段命令对应的命令码
  * @param  [in] section 单段命令字符串
  * @param  [in] cmd_count 命令查找计数器，用于查找重码时从上次结束的位置开始查找
  * @param  [in] err 错误码
  *         @arg SCPI_NO_ERROR
  *         @arg SCPI_UNDEFINED_HEADER
  * @retval 查找结果,SCPI命令对应的命令码
  */
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
            *cmd_count = i + 1;//记录下结束位置以备下次断续查询重码
            return scpi_segment_pool[i].num;
        }
    }
    
    *err = SCPI_UNDEFINED_HEADER;
    
    return 0;
}


