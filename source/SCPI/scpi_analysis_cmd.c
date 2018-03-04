/**
  ******************************************************************************
  * @file    scpi_analysis_cmd.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.16
  * @brief   SCPI解析指令
  ******************************************************************************
  */
#include "string.h"
#include "stdio.h"
#include "crc.h"
#include "USART6.H"
#include "tim3.h"
#include "scpi_com_comm.h"
#include "scpi_cmd_def.h"


SCPI_CMD_TYPE cur_scpi_cmd_type;

typedef enum 
{
    SCPI_CMD_END_1,
    SCPI_CMD_END_2,
    SCPI_CMD_END_3
}SCPI_CMD_END_T;

SCPI_CMD_END_T end_type;

uint8_t end_data_1[] = {0X0D, 0X0A};
uint8_t end_data_2[] = {'#'};
uint8_t end_data_3[] = {0x0A};

/**
  * @brief  拆分SCPI指令携带的参数
  * @param  [in] scpi_cmd SCPI指令
  * @param  [out] par SCPI指令
  * @param  [out] layer SCPI指令携带的参数个数
  * @retval 拆分结果
  *         @arg SCPI_NO_ERROR 正常结束
  *         @arg SCPI_UNDEFINED_HEADER 未定义指令
  */
static SCPI_ERR_T split_scpi_par(uint8_t *scpi_cmd, uint8_t *par[], uint8_t *layer)
{
    uint8_t len = 0;
    int32_t i = 0;
    int32_t j = 0;
    
    len = strlen((const char*)scpi_cmd);
    
    /* 没有参数 */
    if(len < 1)
    {
        return SCPI_NO_ERROR;
    }
    
    par[j++] = scpi_cmd;
    
    for(i = 0; i < len; i++)
    {
        if(scpi_cmd[i] >= 'a' && scpi_cmd[i] <= 'z')
        {
        }
        else if(scpi_cmd[i] >= 'A' && scpi_cmd[i] <= 'Z')
        {
        }
        else if(scpi_cmd[i] >= '0' && scpi_cmd[i] <= '9')
        {
        }
        else if(scpi_cmd[i] == '"')
        {
        }
        else if(scpi_cmd[i] == '.')
        {
        }
        else if(scpi_cmd[i] == ',')
        {
            par[j++] = &scpi_cmd[i + 1];
            scpi_cmd[i] = 0;
        }
        else
        {
            return SCPI_INVALID_STRING_DATA;
        }
    }
    
    *layer = j;
    return SCPI_NO_ERROR;
}

/**
  * @brief  SCPI指令预处理，主要是去除":"后的空格
  * @param  [in/out] scpi_cmd SCPI指令
  * @retval 无
  */
static void pretreat_scpi_cmd(uint8_t *scpi_cmd)
{
    uint8_t len = 0;
    int32_t i = 0;
    int32_t j = 0;
    uint8_t buf[256] = {0};
    
    len = strlen((const char*)scpi_cmd);
    
    for(i = 0; i < len; i++)
    {
        if(i > 0 && scpi_cmd[i - 1] == ':' && scpi_cmd[i] == ' ')
        {
            for(;scpi_cmd[i] == ' '; i++);
        }
        
        buf[j++] = scpi_cmd[i];
    }
    
    strcpy((char*)scpi_cmd, (const char*)buf);
}

/**
  * @brief  拆分SCPI指令
  * @param  [in] scpi_cmd SCPI指令
  * @param  [out] cmd SCPI各级分解指令字符串地址缓冲区
  * @param  [out] layer SCPI指令级数
  * @param  [out] p_par SCPI指令携带参数的地址
  * @retval 拆分结果
  *         @arg SCPI_NO_ERROR 正常结束
  *         @arg SCPI_UNDEFINED_HEADER 未定义指令
  */
static SCPI_ERR_T split_scpi_cmd(uint8_t *scpi_cmd, uint8_t *cmd[], uint8_t *layer, uint8_t **p_par)
{
    uint8_t len = 0;
    int32_t i = 0;
    int32_t j = 0;
    
    /* 指令预处理 去删指令分级符':'后面的空格 */
    pretreat_scpi_cmd(scpi_cmd);
    
    len = strlen((const char*)scpi_cmd);
    
    cmd[j++] = scpi_cmd;
    
    for(i = 0; i < len; i++)
    {
        if(scpi_cmd[i] >= 'a' && scpi_cmd[i] <= 'z')
        {
        }
        else if(scpi_cmd[i] >= 'A' && scpi_cmd[i] <= 'Z')
        {
        }
        else if(scpi_cmd[i] == '?')
        {
            scpi_cmd[i] = 0;
            cur_scpi_cmd_type = SCPI_QUERY;//标记为查询类指令
        }
        /* 检查出指令与参数间的空格 */
        else if(scpi_cmd[i] == ' ')
        {
            scpi_cmd[i] = 0;
            i++;
            break;
        }
        else if(scpi_cmd[i] == ':')
        {
            if(j >= MAX_SCPI_CMD_LAYER)
            {
                return SCPI_UNDEFINED_HEADER;
            }
            
            cmd[j++] = &scpi_cmd[i + 1];
            scpi_cmd[i] = 0;
        }
        else if(scpi_cmd[i] == '*')
        {
        }
        else
        {
            return SCPI_UNDEFINED_HEADER;
        }
    }
    
    *layer = j;
    *p_par = scpi_cmd + i;
    
    return SCPI_NO_ERROR;
}

/**
  * @brief  接收到新的SCPI数据帧
  * @param  [in] scpi_cmd SCPI数据帧
  * @param  [out] len SCPI数据帧长度
  * @retval 
  *         @arg SCPI_NO_ERROR 接收到完整数据帧
  *         @arg SCPI_UNDEFINED_HEADER 接收到不完整数据帧
  */
static SCPI_ERR_T scpi_server_receive_new_frame(uint8_t *scpi_cmd, uint32_t len)
{
    uint8_t end_data[2];
    
    end_data[0] = scpi_cmd[len - 2];
    end_data[1] = scpi_cmd[len - 1];
    
    /*修改结束标志*/
    if(end_data[1] == end_data_2[0])
    {
        end_type = SCPI_CMD_END_2;
        scpi_cmd[len - 1] = 0;
    }
    /*修改结束标志*/
    else if(end_data[0] == end_data_1[0] && end_data[1] == end_data_1[1])
    {
        end_type = SCPI_CMD_END_1;
        scpi_cmd[len - 2] = 0;
    }
    else if(end_data[0] == end_data_3[0])
    {
        end_type = SCPI_CMD_END_3;
        scpi_cmd[len - 1] = 0;
    }
    else
    {
        return SCPI_SYNTAX_ERROR;
    }
    
    return SCPI_NO_ERROR;
}

/**
  * @brief  计算SCPI指令数据的校验和
  * @param  [in] data SCPI数据帧
  * @retval 
  *         @arg SCPI_NO_ERROR 接收到完整数据帧
  *         @arg SCPI_UNDEFINED_HEADER 接收到不完整数据帧
  */
static uint8_t count_check_num(uint8_t *data)
{
    uint8_t len = 0;
    int32_t i = 0;
    uint8_t res = 0;
    
    len = strlen((const char*)data);
    
    if(len <= 1)
    {
        return 0;
    }
    
    len--;
    
    for(i = 0; i < len; i++)
    {
        res += data[i];
    }
    
    res |= 0x80;
    
    return res;
}
/**
  * @brief  检查参数的合法性
  * @param  [in] cmd SCPI指令属性
  * @param  [in] par SCPI传入参数信息
  * @retval 检查结果
  *         @arg SCPI_NO_ERROR
  *         @arg SCPI_SYNTAX_ERROR
  *         @arg SCPI_MISSING_PARAMETER
  */
static SCPI_ERR_T check_scpi_cmd_validity(const SCPI_CMD *cmd, SCPI_DIS_FUN_PAR *par)
{
    /* 只执行，只写、只读类指令对参数个数进行检查。 */
    if(cmd->att == E__ || cmd->att == _W_ || cmd->att == __R)
    {
        if(cmd->par_num != IGNORE_PAR_NUM)
        {
            if(par->argc > cmd->par_num)
            {
                return SCPI_SYNTAX_ERROR;
            }
            else if(par->argc < cmd->par_num)
            {
                return SCPI_MISSING_PARAMETER;
            }
        }
    }
    
    if(cmd->att == E__ || cmd->att == _W_)
    {
        if(par->type != SCPI_EXE)
        {
            return SCPI_SYNTAX_ERROR;
        }
    }
    else if(cmd->att == __R)
    {
        if(par->type == SCPI_EXE)
        {
            return SCPI_SYNTAX_ERROR;
        }
    }
    
    /* 读写类指令只对写指令时的参数进行检查 对于读写属性的指令，当是读指令时是不允许带参数的 */
    if(cmd->att == _WR && cmd->par_num != IGNORE_PAR_NUM)
    {
        if(par->type == SCPI_EXE)
        {
            if(par->argc > cmd->par_num)
            {
                return SCPI_SYNTAX_ERROR;
            }
            else if(par->argc < cmd->par_num)
            {
                return SCPI_MISSING_PARAMETER;
            }
        }
        else
        {
            if(par->argc > 0)
            {
                return SCPI_SYNTAX_ERROR;
            }
        }
    }
    
    return SCPI_NO_ERROR;
}

/**
  * @brief  串口1的接收处理函数
  * @param  [in] com_num 串口号
  * @param  [in] frame 数据帧
  * @param  [in] len 数据帧长度
  * @retval 无
  */
static SCPI_ERR_T scpi_receive_dispose_(uint8_t *frame, uint32_t len,
                                uint8_t *ask_frame, uint8_t *ask_len)
{
    uint8_t *cmd[MAX_SCPI_CMD_LAYER] = {0};
    uint32_t cmd_count[MAX_SCPI_CMD_LAYER] = {0};
    uint8_t *par[MAX_SCPI_PAR_NUM] = {0};
    uint8_t *p_par = NULL;//参数字符串
    uint8_t layer = 0;
    uint8_t par_num = 0;//参数个数
    uint8_t tmp = 0;
    SCPI_ERR_T err;
    SCPI_CMD scpi_cmd;
    int32_t i = 0;
    CMD_NUM_T cmd_num;
    const SCPI_CMD *p_scpi_cmd = NULL;
    SCPI_DIS_FUN_PAR scpi_par;
    
    frame[len] = 0;
    
    /* 检查数据长度 */
    tmp = strlen((const char*)frame);
    
    if(tmp != len)
    {
        return SCPI_UNDEFINED_HEADER;
    }
    
    /* 检查结束符 */
    err = scpi_server_receive_new_frame(frame, len);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* 检查校验和 */
    if(end_type == SCPI_CMD_END_1 || end_type == SCPI_CMD_END_3)
    {
        len = strlen((const char*)frame);
        
        tmp = count_check_num(frame);
        
        if(tmp != frame[len - 1])
        {
            return SCPI_UNDEFINED_HEADER;
        }
        
        frame[len - 1] = 0;
    }
    
    cur_scpi_cmd_type = SCPI_EXE;///默认为执行类指令
    
    /* 拆分指令 */
    err = split_scpi_cmd(frame, cmd, &layer, &p_par);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* 拆分参数 */
    err = split_scpi_par(p_par, par, &par_num);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* 指令解析为对应的数字码 */
    memset(&scpi_cmd, 0, sizeof(scpi_cmd));
    
    for(i = 0; i < layer; i++)
    {
        scpi_cmd.cmd[i] = find_one_section_cmd(cmd[i], &cmd_count[i], &err);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
    }
    
    /* 指令解析匹配 */
    do{
        cmd_num = *(CMD_NUM_T*)&scpi_cmd;
        p_scpi_cmd = find_cmd_num(cmd_num, &err);
        
        /* 找到定义的合法指令 */
        if(err == SCPI_NO_ERROR)
        {
            break;
        }
        
        /* 没有找到合法指令，断续查找 */
        /* 断续查找下一个重码(多数是简写重码) */
        for(i = 0; i < layer; i++)
        {
            tmp = find_one_section_cmd(cmd[i], &cmd_count[i], &err);
            
            if(err == SCPI_NO_ERROR)
            {
                /* 有重码 */
                scpi_cmd.cmd[i] = tmp;
                break;
            }
        }
        
        /* 没找到重码 */
        if(i == layer)
        {
            return SCPI_UNDEFINED_HEADER;
        }
        
    }while(i < layer);
    
    scpi_par.argv = par;
    scpi_par.argc = par_num;
    scpi_par.ask_data = ask_frame;
    scpi_par.ask_len = ask_len;
    scpi_par.type = cur_scpi_cmd_type;
    
    /* 检查命令合法性 */
    err = check_scpi_cmd_validity(p_scpi_cmd, &scpi_par);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* 进入处理函数 */
    if(p_scpi_cmd->dispose_fun != NULL)
    {
        err = p_scpi_cmd->dispose_fun(&scpi_par);
    }
    else
    {
        err = SCPI_UNDEFINED_HEADER;
    }
    
    return err;
}

/**
  * @brief  给应答帧添加结束符
  * @param  [in/out] ask_frame 应答数据帧
  * @param  [in] ask_len 数据帧长度
  * @retval 无
  */
static void add_end_code(uint8_t *ask_frame, uint8_t *ask_len)
{
    uint8_t tmp = 0;
    uint8_t len = *ask_len;
    
    if(end_type == SCPI_CMD_END_1)
    {
        tmp = count_check_num(ask_frame);
        ask_frame[len++] = tmp;
        ask_frame[len++] = end_data_1[0];
        ask_frame[len++] = end_data_1[1];
    }
    else if(end_type == SCPI_CMD_END_2)
    {
        ask_frame[len++] = end_data_2[0];
    }
    else if(end_type == SCPI_CMD_END_3)
    {
        tmp = count_check_num(ask_frame);
        ask_frame[len++] = tmp;
        ask_frame[len++] = end_data_3[0];
    }
    
    *ask_len = len;
}

/**
  * @brief  SCPI接收处理函数
  * @param  [in/out] com 串口结构信息
  * @param  [in] frame 数据帧
  * @param  [in] len 数据帧长度
  * @retval 无
  */
void scpi_receive_dispose(SCPI_COM_STRUCT *com, uint8_t *frame, uint32_t len)
{
    SCPI_ERR_T err;
    uint8_t ask_len = 0;
    
    com->frame_buf[0] = 0;
    err = scpi_receive_dispose_(frame, len, com->frame_buf, &ask_len);
    
    /* 设置指令返回 或 查询指令执行失败 */
    if((cur_scpi_cmd_type == SCPI_EXE) 
        || ((cur_scpi_cmd_type == SCPI_QUERY) && (err != SCPI_NO_ERROR)))
    {
        ask_len = strlen((const char*)comm_error_msg[err]);
        strcpy((char*)com->frame_buf, (const char*)comm_error_msg[err]);
        add_end_code(com->frame_buf, &ask_len);
        com->send_fun(com, com->frame_buf, ask_len);
    }
    /* 查询指令执行成功 */
    else if((cur_scpi_cmd_type == SCPI_QUERY) && (err == SCPI_NO_ERROR))
    {
        ask_len = strlen((const char*)com->frame_buf);
        add_end_code(com->frame_buf, &ask_len);
        com->send_fun(com, com->frame_buf, ask_len);
    }
}
/**
  * @brief  初始化SCPI管理的环境 里使用到了串口和定时器
  * @param  无
  * @retval 无
  */
void init_scpi_manage_env(void)
{
    register_tim3_server_fun(usart6_judge_timout);
}
/**
  * @brief  SCPI通信任务
  * @param  无
  * @retval 无
  */
void scpi_comm_task(void)
{
    scpi_comm_status_machine(&scpi_com);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
