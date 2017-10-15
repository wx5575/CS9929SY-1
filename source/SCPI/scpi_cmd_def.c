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


static const char * cmd_list[] =
{
    "*IDN"          ,
    "*RST"          ,
    "COMM"          ,
    "COMMunication" ,
    "SADD"          ,
    "SADDress"      ,
    "REM"           ,
    "REMote"        ,
    "LOC"           ,
    "LOCal"         ,
    "CONT"          ,
    "CONTrol"       ,
    "FILE"          ,
    "NEW"           ,
    "EDIT"          ,
    "DEL"           ,
    "DELete"        ,
    "SING"          ,
    "SINGle"        ,
    "ALL"           ,
    "SAVE"          ,
    "READ"          ,
    "CAT"           ,
    "CATalog"       ,
    "SOUR"          ,
    "SOURce"        ,
    "TEST"          ,
    "STAR"          ,
    "STARt"         ,
    "STOP"          ,
    "STAT"          ,
    "STATus"        ,
    "FETC"          ,
    "FETCh"         ,
    
    "GETCap"        ,
    "GETC"          ,
    
    "GETOffset"     ,
    "GETO"          ,
    
    "LOAD"          ,
    "STEP"          ,
    "SSTEP"         ,
    "LIST"          ,
    "FIND"          ,
    "FINDex"        ,
    "FMES"          ,
    "FMESsage"      ,
    "SIND"          ,
    "SINDex"        ,
    "SMES"          ,
    "SMESsage"      ,
    "MOVE"          ,
    "INS"           ,
    "INSert"        ,
    "ACW"           ,
    "DCW"           ,
    "IR"            ,
    "GR"            ,
    "BBD"           ,
    "FRON"          ,
    "FRONt"         ,
    "BEH"           ,
    "BEHind"        ,
    "INT"           ,
    "INTerchange"   ,
    "MODE"          ,
    "VOLT"          ,
    "VOLTage"       ,
    "RANG"          ,
    "RANGe"         ,
    "HIGH"          ,
    "LOW"           ,
    
    "BBDOpen"       ,
    "BBDO"          ,
    "BBDShort"      ,
    "BBDS"          ,
    
    "RCUR"          ,
    "RCURrent"      ,
    "ARC"           ,
    "FREQ"          ,
    "FREQuency"     ,
    "RTIM"          ,
    "RTIMe"         ,
    "TTIM"          ,
    "TTIMe"         ,
    "FTIM"          ,
    "FTIMe"         ,
    "ITIM"          ,
    "ITIMe"         ,
    "CTIM"          ,
    "CTIMe"         ,
    "PSIG"          ,
    "PSIGnal"       ,
    "CNEX"          ,
    "CNEXt"         ,
    "PORT"          ,
    "CCUR"          ,
    "CCURrent"      ,
    "DTIM"          ,
    "DTIMe"         ,
    "ARAN"          ,
    "ARANge"        ,
    "CURR"          ,
    "CURRent"       ,
    "RES"           ,
    "RESult"        ,
    "CAP"           ,
    "CAPacity"      ,
    "USED"          ,
    "FREE"          ,
    "PASS"          ,   
    "FAIL"          ,
    "CLE"           ,
    "CLEar"         ,
    "DUT"           ,
    "NAME"          ,
    "SYST"          ,
    "SYSTem"        ,
    "SYSTerm"       ,
    "SCR"           ,
    "SCReen"        ,
    "CONTrast"      ,
    "BEEP"          ,
    "BEEPer"        ,
    "VOL"           ,
    "VOLume"        ,
    "RHIN"          ,
    "RHINt"         ,
    "RSAV"          ,
    "RSAVe"         ,
    "OCOV"          ,
    "OCOVer"        ,
    "GFI"           ,
    "PHV"           ,
    "TSIG"          ,
    "TSIGnal"       ,
    "SCH"           ,
    "SCHeck"        ,
    "CHIN"          ,
    "CHINt"         ,
    "TPOR"          ,
    "TPORt"         ,
    "LANG"          ,
    "LANGuage"      ,
    "FCON"          ,
    "FCONtinue"     ,
    "KEY"           ,
    "KLOC"          ,
    "KLOCk"         ,
    "PASSword"      ,
    "NOW"           ,
    "TIME"          ,
    "NRUL"          ,
    "NRULe"         ,
    "RHAL"          ,
    "RHALt"         ,
    "0"             ,
    "1"             ,
    "2"             ,
    "3"             ,
    "TOTS"          ,
	
	"OFFSet",
	"OFFS",
	
	"CALibration",
	"CAL",
	"ENTer",
	"ENT",
	"EXIT",
	"MEASure",
	"MEAS",
	"POINts",
	"POIN",
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


SCPI_CMD scpi_cmd_pool[]=
{
    {{IDN}              , NULL},
    {{STEP,GR,HIGH,}    , NULL},
    
};
typedef uint32_t CMD_NUM_T;

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

uint8_t find_one_section_cmd(uint8_t *section, SCPI_ERR_T *err)
{
    uint8_t num = 0;
    int32_t i = 0;
    
    num = ARRAY_SIZE(cmd_list);
    
    for(i = 0; i < num; i++)
    {
        if( 0 == strcmp((const char*)section, cmd_list[i]))
        {
            *err = SCPI_NO_ERROR;
            return i;
        }
    }
    
    *err = SCPI_UNDEFINED_HEADER;
    
    return 0;
}


