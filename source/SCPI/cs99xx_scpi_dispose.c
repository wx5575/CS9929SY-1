/**
  ******************************************************************************
  * @file    cs99xx_scpi_dispose.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.23
  * @brief   SCPI指令处理函数定义
  ******************************************************************************
  */
#include "string.h"
#include "stdio.h"
#include "cs99xx_struct.h"
#include "cs99xx_type.h"
#include "scpi_cmd_def.h"
#include "cs99xx_scpi_dispose.h"
#include "comm_bridge.h"
#include "cs99xx_mem_api.h"
#include "app.h"
#include "module_manage.h"
#include "send_cmd.h"
#include "rtc_config.h"
#include "parameter_manage.h"
#include "string.h"
#include "stdlib.h"
#include "ui_comm_api.h"
#include "running_test.h"
#include "rtc_config.h"


SCPI_ERR_T idn_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "Allwin Technologies,%s,%s", g_cur_type->name, g_cur_type->ver);
    *par->ask_len = strlen((const char*)par->ask_data);
    return SCPI_NO_ERROR;
}

SCPI_ERR_T rst_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T comm_sadd_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t addr = 0;
    
    if(par->type == SCPI_EXE)
    {
        addr = atoi((const char*)par->argv[0]);
        if(addr > 0)
        {
            if(sys_par.local_addr != addr)
            {
                sys_par.local_addr = addr;
                save_sys_par();
            }
        }
        else
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.local_addr);
    }
    
    return err;
}

SCPI_ERR_T comm_remote_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    set_instrument_into_remote_bridge();
    app_flag.comm_status = COMM_REMOTE;
    return SCPI_NO_ERROR;
}

SCPI_ERR_T comm_local_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    set_instrument_exit_remote_bridge();
    app_flag.comm_status = COMM_LOCAL;
    return SCPI_NO_ERROR;
}

SCPI_ERR_T comm_control_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    get_instrument_comm_status_bridge();
    strcpy((char*)par->ask_data, app_flag.comm_status==COMM_REMOTE? "1":"0");
    
    return SCPI_NO_ERROR;
}

/* 文件指令集 */
/**
  * @brief  提取文件参数
  * @param  [out] file 文件信息
  * @param  [in] par SCPI指令携带的参数信息
  * @retval 错误码
  */
static SCPI_ERR_T extract_file_par(TEST_FILE *file, SCPI_DIS_FUN_PAR *par)
{
    uint32_t value = 0;
    uint8_t len = 0;
    
    if(par->argc < 6)
    {
        return SCPI_MISSING_PARAMETER;
    }
    else if(par->argc > 6)
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    file->num = atoi((const char*)par->argv[0]);
    
    if(file->num >= MAX_FILES)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    if(CS_TRUE == is_file_exist(file->num))
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    len = strlen((const char*)par->argv[1]);
    if(par->argv[1][0] != '"' || par->argv[1][len - 1] != '"' || len > NAME_LON)
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    strncpy((char*)file->name, (const char*)&par->argv[1][1], len - 2);
    
    if(0 == strcmp((const char*)par->argv[2], "N")
        || 0 == strcmp((const char*)par->argv[2], "1")
        )
    {
        file->work_mode = N_MODE;
    }
    else if(0 == strcmp((const char*)par->argv[2], "G")
        || 0 == strcmp((const char*)par->argv[2], "0")
        )
    {
        file->work_mode = G_MODE;
    }
    else
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    value = atof((const char*)par->argv[3]) * 10;
    
    if(value > 9999)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    file->pass_time = value;
    
    value = atof((const char*)par->argv[4]) * 10;
    
    if(value > 9999)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    file->buzzer_time = value;
    
    if(0 == strcmp((const char*)par->argv[5], "CURRent")
        || 0 == strcmp((const char*)par->argv[5], "CURR")
        || 0 == strcmp((const char*)par->argv[5], "1")
        )
    {
        file->arc_mode = ARC_CUR_MODE;
    }
    else if(0 == strcmp((const char*)par->argv[5], "SCALe")
        || 0 == strcmp((const char*)par->argv[5], "SCAL")
        || 0 == strcmp((const char*)par->argv[5], "0")
        )
    {
        file->arc_mode = ARC_GRADE_MODE;
    }
    else
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    return SCPI_NO_ERROR;
}

/* 文件指令集 */
SCPI_ERR_T file_new_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    TEST_FILE file;
    SCPI_ERR_T err;
    
    /* 提取文件参数 */
    err = extract_file_par(&file, par);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* 获取RTC时间 */
    file.create_date = get_rtc_data();
    file.create_time = get_rtc_time();
    
    file_pool[file.num] = file;
    save_file(file.num);//保存文件
    init_new_group_inf(&file_pool[file.num]);//初始化记忆组
    send_cmd_to_all_module((void*)&file, sizeof(TEST_FILE),
                send_slave_new_file);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_edit_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    TEST_FILE file;
    SCPI_ERR_T err;
    
    /* 提取文件参数 */
    err = extract_file_par(&file, par);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    file_pool[file.num] = file;
    
    /* 获取RTC时间 */
    file.create_date = get_rtc_data();
    file.create_time = get_rtc_time();
    save_file(file.num);//保存文件
    send_cmd_to_all_module((void*)&file, sizeof(TEST_FILE),
                send_slave_edit_file);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_del_single_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    FILE_NUM   file_num;
    TEST_FILE *file;
    CS_ERR err;
    
    file_num = atoi((const char*)par->argv[0]);
    
    if(file_num >= MAX_FILES)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    if(CS_TRUE != is_file_exist(file_num))
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    send_cmd_to_all_module((void*)&file_num,
        sizeof(file_num), send_slave_del_file);
    
    del_one_group_inf(file_num);//删除一个记组信息
    
    file = get_file_inf(0, &err);
    
    if(err == CS_ERR_NONE)
    {
        g_cur_file = file;
        read_group_info(g_cur_file->num);//保存新建文件的记忆组信息
        sys_flag.last_file_num = g_cur_file->num;//更新最近使用的文件编号
        save_sys_flag();//保存系统标记
        read_group_info(g_cur_file->num);//恢复最近使用的记忆组信息
    }
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_del_all_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    del_all_file();
    send_cmd_to_all_module(NULL, 0, send_slave_clear_all_files);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_save_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    FILE_NUM   file_num;
    TEST_FILE file;
    uint8_t len = 0;
    
    file_num = atoi((const char*)par->argv[0]);
    
    if(file_num >= MAX_FILES)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    if(CS_TRUE == is_file_exist(file_num))
    {
        return SCPI_PARAMETER_NOT_ALLOWED;
    }
    
    /* 与当前文件号相同不允许操作 */
    if(g_cur_file->num == file_num)
    {
        return SCPI_PARAMETER_NOT_ALLOWED;
    }
    
    len = strlen((const char*)par->argv[1]);
    if(par->argv[1][0] != '"' || par->argv[1][len - 1] != '"' || len > NAME_LON || len < 3)
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    strncpy((char*)file.name, (const char*)&par->argv[1][1], len - 2);
    
    file.num = file_num;
    
    file.create_date = get_rtc_data();
    file.create_time = get_rtc_time();
    file_pool[file_num] = file;
    copy_cur_file_to_new_pos(file_num);//拷贝当前文件到指定位置
    send_cmd_to_all_module((void*)&file_pool[file_num], sizeof(TEST_FILE),
                send_slave_save_file);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_read_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    FILE_NUM   file_num;
    CS_ERR err;
    TEST_FILE *f;
    
    file_num = atoi((const char*)par->argv[0]);
    
    if(file_num >= MAX_FILES)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    if(CS_TRUE != is_file_exist(file_num))
    {
        return SCPI_PARAMETER_NOT_ALLOWED;
    }
    
    f = get_file_inf(file_num, &err);
    
    if(err == CS_ERR_NONE)
    {
        g_cur_file = f;
        read_group_info(g_cur_file->num);//保存新建文件的记忆组信息
        sys_flag.last_file_num = g_cur_file->num;//更新最近使用的文件编号
        save_sys_flag();//保存系统标记
        read_group_info(g_cur_file->num);//恢复最近使用的记忆组信息
        send_cmd_to_all_module((void*)&g_cur_file->num,
            sizeof(g_cur_file->num), send_slave_load_file);
    }
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_catalog_single_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    FILE_NUM   file_num;
    CS_ERR err;
    TEST_FILE *f;
    
    file_num = atoi((const char*)par->argv[0]);
    
    if(file_num >= MAX_FILES)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    if(CS_TRUE != is_file_exist(file_num))
    {
        strcpy((char*)par->ask_data, "0");
    }
    else
    {
        f = get_file_inf(file_num, &err);
        
        sprintf((char*)par->ask_data, "%d,\"%s\",%d,%s,%05.1f,%05.1f,%s",
                f->num,
                f->name,
                f->total,
                f->work_mode==G_MODE? "G":"N",
                f->pass_time / 10.0,
                f->buzzer_time / 10.0,
                f->arc_mode==ARC_CUR_MODE? "1":"0");
    }
    
    return SCPI_NO_ERROR;
}
/* 源指令集 */
SCPI_ERR_T source_test_start_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    source_test_start_bridge();
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_test_stop_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    source_test_stop_bridge();
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_test_status_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint8_t status[4] = {0};
    
    status[0] = get_road_test_status(1);
    status[1] = get_road_test_status(2);
    status[2] = get_road_test_status(3);
    status[3] = get_road_test_status(4);
    
    sprintf((char*)par->ask_data, "%d,%d,%d,%d",status[0],status[1],status[2],status[3]);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_test_fetch_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    ROAD_NUM_T road = 0;
    uint8_t buf[20] = {0};
    uint8_t *str = NULL;
    uint8_t max_roads = 0;
    
    road = atoi((const char*)par->argv[0]);
    
    max_roads = get_max_roads();
    
    if(road < 1 || road > max_roads)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    /* 路号，步骤编号，测试模式，电压，电流档位，测试电流，真实电流，时间，测试状态 */
    sprintf((char*)par->ask_data, "%d,%d,%d,", road, cur_step, cur_mode - 1);
    
    switch(cur_mode)
    {
        case ACW:
            strcat((char*)par->ask_data, (const char*)get_road_test_voltage(road));
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d,", cur_gear);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, (const char*)get_road_test_current(road));
            strcat((char*)par->ask_data, ",");
            str = get_road_test_real_current(road);
            if(strlen((const char*)str) == 0)
            {
                strcat((char*)par->ask_data, (const char*)"-----");
            }
            else
            {
                strcat((char*)par->ask_data, (const char*)str);
            }
            break;
        case DCW:
            strcat((char*)par->ask_data, (const char*)get_road_test_voltage(road));
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d,", cur_gear);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, (const char*)get_road_test_current(road));
            break;
        case IR:
            strcat((char*)par->ask_data, (const char*)get_road_test_voltage(road));
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d,", cur_gear);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, (const char*)get_road_test_current(road));
            break;
        case GR:
            strcat((char*)par->ask_data, (const char*)get_road_test_voltage(road));
            strcat((char*)par->ask_data, ",");
            strcat((char*)par->ask_data, (const char*)get_road_test_current(road));
            break;
    }
    
    strcat((char*)par->ask_data, ",");
    strcat((char*)par->ask_data, (const char*)get_road_test_time(road));
    strcat((char*)par->ask_data, ",");
    sprintf((char*)buf, "%d", get_road_test_status(road));
    strcat((char*)par->ask_data, (const char*)buf);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_load_step_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    STEP_NUM step = 0;
    
    step = atoi((const char*)par->argv[0]);
    
    if(step > g_cur_file->total || step == 0)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    
    load_steps_to_list(step, 1);//加载新的当前步
    g_cur_step = get_g_cur_step();
    load_data();
    update_test_win_text_display();
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_load_file_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    FILE_NUM   file_num;
    CS_ERR err;
    TEST_FILE *f;
    
    file_num = atoi((const char*)par->argv[0]);
    
    if(file_num >= MAX_FILES)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    if(CS_TRUE != is_file_exist(file_num))
    {
        return SCPI_PARAMETER_NOT_ALLOWED;
    }
    
    f = get_file_inf(file_num, &err);
    
    if(err == CS_ERR_NONE)
    {
        g_cur_file = f;
        read_group_info(g_cur_file->num);//保存新建文件的记忆组信息
        sys_flag.last_file_num = g_cur_file->num;//更新最近使用的文件编号
        save_sys_flag();//保存系统标记
        read_group_info(g_cur_file->num);//恢复最近使用的记忆组信息
        send_cmd_to_all_module((void*)&g_cur_file->num,
            sizeof(g_cur_file->num), send_slave_load_file);
    }
    
    load_steps_to_list(1, 1);//加载新的当前步
    g_cur_step = get_g_cur_step();
    load_data();
    update_test_win_text_display();
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_findex_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "%02d", g_cur_file->num);
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_fmessage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "%d,\"%s\",%d,%s,%05.1f,%05.1f,%s",
            g_cur_file->num,
            g_cur_file->name,
            g_cur_file->total,
            g_cur_file->work_mode==G_MODE? "G":"N",
            g_cur_file->pass_time / 10.0,
            g_cur_file->buzzer_time / 10.0,
            g_cur_file->arc_mode==ARC_CUR_MODE? "1":"0");
        
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_sindex_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "%02d", cur_step);
    return SCPI_NO_ERROR;
}

static void transition_work_port(uint8_t *buf, WORK_PORT *port)
{
    uint16_t *p_16;
    int32_t i = 0;
    
    p_16 = (void*)port;
    buf[0] = 0;
    
    for(i = 0; i < port->num; i++)
    {
        strcat((char*)buf, (((*p_16)>>(i * 2)) & 3) == 0 ? "X" : "H");
//        strcat((char*)buf, ((i + 1) < port->num)? ",":"");
    }
}
SCPI_ERR_T source_list_smessage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint8_t buf[20] = {0};
    
    switch(cur_mode)
    {
        case ACW:
            sprintf((char*)par->ask_data,"%02d,%d,", cur_step, cur_mode);
            sprintf((char*)buf, "%05.3f,", cur_vol / 1000.0);
            strcat((char*)par->ask_data, (const char*)buf);
            sprintf((char*)buf, "%d,", cur_gear);
            strcat((char*)par->ask_data, (const char*)buf);
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_high);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_low);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_real_cur);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            if(g_cur_file->arc_mode == ARC_CUR_MODE)
            {
                mysprintf(buf, NULL, 152, cur_arc_gear);
            }
            else
            {
                sprintf((char*)buf, "%d", cur_arc_gear);
            }
            
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            mysprintf(buf, NULL, 151, cur_frequency);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            mysprintf(buf, NULL, 151, ris_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, tes_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, fal_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, int_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_pass);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_con);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            transition_work_port(buf, &cur_work_port);
            strcat((char*)par->ask_data, (const char*)buf);
            
            break;
        case DCW:
            sprintf((char*)par->ask_data,"%02d,%d,", cur_step, cur_mode);
            sprintf((char*)buf, "%05.3f,", cur_vol / 1000.0);
            strcat((char*)par->ask_data, (const char*)buf);
            sprintf((char*)buf, "%d,", cur_gear);
            strcat((char*)par->ask_data, (const char*)buf);
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_high);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_low);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_charge_cur);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            if(g_cur_file->arc_mode == ARC_CUR_MODE)
            {
                mysprintf(buf, NULL, 152, cur_arc_gear);
            }
            else
            {
                sprintf((char*)buf, "%d", cur_arc_gear);
            }
            
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            mysprintf(buf, NULL, 151, dly_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            mysprintf(buf, NULL, 151, ris_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, tes_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, fal_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, int_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_pass);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_con);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            transition_work_port(buf, &cur_work_port);
            strcat((char*)par->ask_data, (const char*)buf);
            
            break;
        case IR:
            sprintf((char*)par->ask_data,"%02d,%d,", cur_step, cur_mode);
            sprintf((char*)buf, "%05.3f,", cur_vol / 1000.0);
            strcat((char*)par->ask_data, (const char*)buf);
            sprintf((char*)buf, "%d,", cur_auto);
            strcat((char*)par->ask_data, (const char*)buf);
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_high);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_low);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            mysprintf(buf, NULL, 151, ris_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, tes_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, dly_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, int_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_pass);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_con);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            transition_work_port(buf, &cur_work_port);
            strcat((char*)par->ask_data, (const char*)buf);
            
            break;
        case GR:
            sprintf((char*)par->ask_data,"%02d,%d,", cur_step, cur_mode);
            sprintf((char*)buf, "%05.3f,", cur_vol / 1000.0);
            strcat((char*)par->ask_data, (const char*)buf);
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_high);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 150 + ac_gear[cur_gear].dec, cur_low);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, tes_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            mysprintf(buf, NULL, 151, int_t);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_pass);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            sprintf((char*)buf, "%d", steps_con);
            strcat((char*)par->ask_data, (const char*)buf);
            strcat((char*)par->ask_data, ",");
            
            transition_work_port(buf, &cur_work_port);
            strcat((char*)par->ask_data, (const char*)buf);
            
            break;
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_mode_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    strcpy((char*)par->ask_data, (const char*)mode_pool[cur_mode - 1]);
    return SCPI_NO_ERROR;
}
/* 步骤指令集 */
static SCPI_ERR_T step_insert_for_comm(uint8_t mode)
{
    STEP_NUM step;
    uint8_t t_mode;
    
    step = g_cur_step->one_step.com.step;
    
    if(step >= MAX_STEPS)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    if(CS_FALSE == check_mode_validity(mode))
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    /* G模式 */
    if(g_cur_file->work_mode == G_MODE)
    {
        t_mode = get_first_step_mode();
        
        if(t_mode != mode)
        {
            return SCPI_EXECUTE_NOT_ALLOWED;
        }
    }
    /* N模式 */
    else
    {
    }
    
    insert_step(step, mode);//插入一步
    save_group_info(g_cur_file->num);//保存当前步
    load_steps_to_list(step + 1, 1);//加载新的当前步
    g_cur_step = get_g_cur_step();
    load_data();
    update_test_win_text_display();
    
    send_cmd_to_all_module((void*)&mode, sizeof(mode), send_insert_step);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_insert_acw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_insert_for_comm(ACW);
}
SCPI_ERR_T step_insert_dcw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_insert_for_comm(DCW);
}
SCPI_ERR_T step_insert_ir_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_insert_for_comm(IR);
}
SCPI_ERR_T step_insert_gr_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_insert_for_comm(GR);
}
SCPI_ERR_T step_delete_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint16_t total = 0;
    STEP_NUM step = 0;
    
    total = g_cur_file->total;
    step = g_cur_step->one_step.com.step;
    
    del_step(step);//删除光标所在行的测试步
    send_cmd_to_all_module((void*)&step, sizeof(step), send_del_step);
    
    /* 删除的是最后一步 */
    if(total == step)
    {
        load_steps_to_list(step - 1, 1);
    }
    /* 删除的不是最后一步 */
    else
    {
        load_steps_to_list(step, 1);
    }
    
    g_cur_step = get_g_cur_step();//获取当前步
    load_data();
    update_test_win_text_display();
    
    return SCPI_NO_ERROR;
}
/**
  * @brief  移动步骤的方向枚举定义 向前移 或 向后移
  */
typedef enum{
    MOVE_FORWARD,///<向前移动当前步
    MOVE_BACKWARD,///<向后移动当前步
    MOVE_SWAP,///<交换
}MOVE_STEP_DIRECTION_T;

static SCPI_ERR_T step_move_for_comm(MOVE_STEP_DIRECTION_T dir, STEP_NUM sw_step)
{
    STEP_NUM one;
    STEP_NUM two;
    
    two = g_cur_step->one_step.com.step;
    
    /* 向前移动测试步 */
    if(dir == MOVE_FORWARD)
    {
        one = two - 1;
        /* 第一行是第一步它不能向前移动 */
        if(one == 0)
        {
            return SCPI_EXECUTE_NOT_ALLOWED;
        }
    }
    /* 向后移动测试步 */
    else if(dir == MOVE_BACKWARD)
    {
        /* 最后一步不能向后移动 */
        if(two >= g_cur_file->total)
        {
            return SCPI_EXECUTE_NOT_ALLOWED;
        }
        
        one = two + 1;
    }
    else if(dir == MOVE_SWAP)
    {
        one = sw_step;
    }
    /* 未知操作 */
    else
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    swap_step(one, two);//交换步骤
    send_cmd_to_all_module((void*)&one, sizeof(one), send_swap_step);
    save_group_info(g_cur_file->num);//保存记忆组信息
    
    load_steps_to_list(one, 1);
    g_cur_step = get_g_cur_step();//获取当前步
    load_data();
    update_test_win_text_display();
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_move_front_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_move_for_comm(MOVE_FORWARD, 0);
}
SCPI_ERR_T step_move_behind_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_move_for_comm(MOVE_BACKWARD, 0);
}
SCPI_ERR_T step_interchange_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    STEP_NUM sw_step;
    
    sw_step = atoi((const char*)par->argv[0]);
    
    if(sw_step > g_cur_file->total)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    if(sw_step == g_cur_step->one_step.com.step)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    return step_move_for_comm(MOVE_SWAP, sw_step);
}

static void set_step_par_for_comm(void)
{
    save_one_step(g_cur_step, g_cur_file->num, g_cur_step->one_step.com.step);
    load_data();
    update_test_win_text_display();
    
    send_cmd_to_all_module((void*)g_cur_step, sizeof(NODE_STEP), send_edit_step);
}
static SCPI_ERR_T step_mode_for_comm(uint8_t mode)
{
    /* 测试模式改变了 */
    if(g_cur_step->one_step.com.mode != mode)
    {
        if(g_cur_file->work_mode == G_MODE)
        {
            if(g_cur_step->one_step.com.step != 1)
            {
                return SCPI_EXECUTE_NOT_ALLOWED;
            }
            
            clear_cur_group_all_test_step();
            insert_step(0, mode);//插入一步
            save_group_info(g_cur_file->num);//保存新建文件的记忆组信息
            load_steps_to_list(1, 1);
            g_cur_step = get_g_cur_step();//获取当前步
        }
        
        g_cur_step->one_step.com.mode = mode;//更新当前步的测试模式
        init_mode(g_cur_step);//初始化参数
        set_step_par_for_comm();
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_mode_acw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_mode_for_comm(ACW);
}
SCPI_ERR_T step_mode_dcw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_mode_for_comm(DCW);
}
SCPI_ERR_T step_mode_ir_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_mode_for_comm(IR);
}
SCPI_ERR_T step_mode_gr_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return step_mode_for_comm(GR);
}

static SCPI_ERR_T count_par_dces(uint8_t *buf, uint8_t *dec)
{
    uint8_t len = 0;
    int32_t i = 0;
    uint8_t flag = 0;
    uint8_t tmp = 0;
    
    len = strlen((const char*)buf);
    
    for(i = 0; i < len; i++)
    {
        if(buf[i] >= '0' && buf[i] <= '9')
        {
            if(flag)
            {
                tmp++;
            }
        }
        else if(buf[i] == '.')
        {
            if(flag)
            {
                return SCPI_INVALID_STRING_DATA;
            }
            
            flag = 1;
        }
        else
        {
            return SCPI_INVALID_STRING_DATA;
        }
    }
    
    *dec = tmp;
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T check_mode_for_comm(uint8_t mode)
{
    if(mode != g_cur_step->one_step.com.mode)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T set_cur_step_voltage(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t value;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    value = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            if(value > ACW_VOL_H || value < ACW_VOL_L)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            g_cur_step->one_step.acw.output_vol = value;
            break;
        case DCW:
            if(value > DCW_VOL_H || value < DCW_VOL_L)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            g_cur_step->one_step.dcw.output_vol = value;
            break;
        case IR:
            if(value > IR_VOL_H || value < IR_VOL_L)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            g_cur_step->one_step.ir.output_vol = value;
            break;
        case GR:
            if(value > GR_CUR_H || value < GR_CUR_L)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            g_cur_step->one_step.gr.output_cur = value;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_voltage(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t value;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            value = g_cur_step->one_step.acw.output_vol;
            break;
        case DCW:
            value = g_cur_step->one_step.dcw.output_vol;
            break;
        case IR:
            value = g_cur_step->one_step.ir.output_vol;
            break;
        case GR:
            value = g_cur_step->one_step.gr.output_cur;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 153, value);
    
    return SCPI_NO_ERROR;
}

/* ACW */
SCPI_ERR_T step_acw_voltage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_voltage(par, ACW);
    }
    else
    {
        err = get_cur_step_voltage(par, ACW);
    }
    
    return err;
}
/**
  * @brief  检查整型参数的合法性
  * @param  [in] argv 参数字符串
  * @retval 合法性
  *         @arg SCPI_NO_ERROR
  *         @arg SCPI_INVALID_STRING_DATA
  */
static SCPI_ERR_T check_int_par(uint8_t *argv)
{
    uint8_t len = 0;
    int32_t i = 0;
    
    len = strlen((const char*)argv);
    
    for(i = 0; i < len; i++)
    {
        if(argv[i] < '0' || argv[i] > '9')
        {
            return SCPI_INVALID_STRING_DATA;
        }
    }
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T set_cur_step_range(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t range;
    
    err = check_int_par(par->argv[0]);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    range = atoi((const char*)par->argv[0]);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    if(CS_TRUE != check_range_validity(mode, range))
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.range != range)
            {
                g_cur_step->one_step.acw.range = range;
                
                /* 调整上限值 */
                g_cur_step->one_step.acw.upper_limit = 500;
                /* 调整下限值 */
                g_cur_step->one_step.acw.lower_limit = 0;
                /* 调整真实电流值 */
                g_cur_step->one_step.acw.real_cur = 0;
                /* 偏移值清零 */
                g_cur_step->one_step.acw.offset_cur = 0;
                g_cur_step->one_step.acw.offset_real = 0;
                g_cur_step->one_step.acw.offset_result = 0;
            }
            break;
        }
        case DCW:
            if(g_cur_step->one_step.dcw.range != range)
            {
                g_cur_step->one_step.dcw.range = range;
                
                /* 调整上限值 */
                g_cur_step->one_step.dcw.upper_limit = 500;
                /* 调整下限值 */
                g_cur_step->one_step.dcw.lower_limit = 0;
                /* 调整充电电流值 */
                g_cur_step->one_step.dcw.charge_cur = 0;
                /* 偏移值清零 */
                g_cur_step->one_step.dcw.offset_cur = 0;
                g_cur_step->one_step.dcw.offset_result = 0;
            }
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}


static SCPI_ERR_T get_cur_step_range(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t range;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            range = g_cur_step->one_step.acw.range;
            break;
        case DCW:
            range = g_cur_step->one_step.acw.range;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    sprintf((char*)par->ask_data, "%d", range);
    
    return SCPI_NO_ERROR;
}


SCPI_ERR_T step_acw_range_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_range(par, ACW);
    }
    else
    {
        err = get_cur_step_range(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_high(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t gear = 0;
    uint8_t dec = 0;
    uint16_t high = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    high = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(high == g_cur_step->one_step.acw.upper_limit)
            {
                break;
            }
            
            gear = g_cur_step->one_step.acw.range;
            
            if(high > ac_gear[gear].high_max || high < ac_gear[gear].high_min)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.acw.upper_limit = high;
            
            if(g_cur_step->one_step.acw.lower_limit > high)
            {
                g_cur_step->one_step.acw.lower_limit = high;
            }
            
            if(g_cur_step->one_step.acw.real_cur > high)
            {
                g_cur_step->one_step.acw.real_cur = high;
            }
            break;
        }
        case DCW:
        {
            if(high == g_cur_step->one_step.dcw.upper_limit)
            {
                break;
            }
            
            gear = g_cur_step->one_step.dcw.range;
            
            if(high > dc_gear[gear].high_max || high < dc_gear[gear].high_min)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.dcw.upper_limit = high;
            
            if(g_cur_step->one_step.dcw.lower_limit > high)
            {
                g_cur_step->one_step.dcw.lower_limit = high;
            }
            
            if(g_cur_step->one_step.dcw.charge_cur > high)
            {
                g_cur_step->one_step.dcw.charge_cur = high;
            }
            break;
        }
        case IR:
        {
            if(high == g_cur_step->one_step.ir.upper_limit)
            {
                break;
            }
            
            if(high > IR_RES_H || high < IR_RES_L)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.dcw.upper_limit = high;
            
            if(high > 0)
            {
                if(g_cur_step->one_step.dcw.lower_limit > high)
                {
                    g_cur_step->one_step.dcw.lower_limit = high - 1;
                }
            }
            break;
        }
        case GR:
        {
            if(high == g_cur_step->one_step.gr.upper_limit)
            {
                break;
            }
            
            if(high > IR_RES_H || high < IR_RES_L)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.gr.upper_limit = high;
            
            if(g_cur_step->one_step.gr.lower_limit > high)
            {
                g_cur_step->one_step.gr.lower_limit = high - 1;
            }
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_high(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t gear = 0;
    uint16_t high = 0;
    uint8_t format = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            gear = g_cur_step->one_step.acw.range;
            high = g_cur_step->one_step.acw.upper_limit;
            format = 100 + 50 + ac_gear[gear].dec;
            break;
        case DCW:
            gear = g_cur_step->one_step.dcw.range;
            high = g_cur_step->one_step.dcw.upper_limit;
            format = 100 + 50 + dc_gear[gear].dec;
            break;
        case IR:
            high = g_cur_step->one_step.ir.upper_limit;
            format = 150;
            break;
        case GR:
            high = g_cur_step->one_step.gr.upper_limit;
            format = 151;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, format, high);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_high(par, ACW);
    }
    else
    {
        err = get_cur_step_high(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_low(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t low = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    low = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(low == g_cur_step->one_step.acw.lower_limit)
            {
                break;
            }
            
            if(low > g_cur_step->one_step.acw.upper_limit)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.acw.lower_limit = low;
            break;
        }
        case DCW:
        {
            if(low == g_cur_step->one_step.dcw.lower_limit)
            {
                break;
            }
            
            if(low > g_cur_step->one_step.dcw.upper_limit)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.dcw.lower_limit = low;
            break;
        }
        case IR:
        {
            if(low == g_cur_step->one_step.ir.lower_limit)
            {
                break;
            }
            
            if(g_cur_step->one_step.ir.upper_limit > 0)
            {
                if(low > g_cur_step->one_step.ir.upper_limit)
                {
                    return SCPI_DATA_OUT_OF_RANGE;
                }
            }
            
            g_cur_step->one_step.ir.lower_limit = low;
            break;
        }
        case GR:
        {
            if(low == g_cur_step->one_step.gr.lower_limit)
            {
                break;
            }
            
            if(low > g_cur_step->one_step.gr.upper_limit)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.gr.lower_limit = low;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_low(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t low = 0;
    uint8_t gear = 0;
    uint8_t format = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            low = g_cur_step->one_step.acw.lower_limit;
            gear = g_cur_step->one_step.acw.range;
            format = 100 + 50 + ac_gear[gear].dec;
            break;
        case DCW:
            low = g_cur_step->one_step.dcw.lower_limit;
            gear = g_cur_step->one_step.dcw.range;
            format = 100 + 50 + dc_gear[gear].dec;
            break;
        case IR:
            low = g_cur_step->one_step.ir.lower_limit;
            format = 150;
            break;
        case GR:
            low = g_cur_step->one_step.gr.lower_limit;
            format = 151;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, format, low);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_low(par, ACW);
    }
    else
    {
        err = get_cur_step_low(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_real(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t real = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    real = atof((const char*)par->argv[0]) * ten_power(dec);
    
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(real == g_cur_step->one_step.acw.real_cur)
            {
                break;
            }
            
            if(real > g_cur_step->one_step.acw.upper_limit)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.acw.real_cur = real;
            break;
        }
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_real(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t real = 0;
    uint8_t gear = 0;
    uint8_t format = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            gear = g_cur_step->one_step.acw.range;
            real = g_cur_step->one_step.acw.real_cur;
            format = 100 + 50 + ac_gear[gear].dec;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, format, real);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_real_current_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_real(par, ACW);
    }
    else
    {
        err = get_cur_step_real(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_arc(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t arc = 0;
    uint8_t dec = 0;
    
    //电弧侦测电流模式
    if(g_cur_file->arc_mode == ARC_CUR_MODE)
    {
        err = count_par_dces(par->argv[0], &dec);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        arc = atof((const char*)par->argv[0]) * ten_power(dec);
    }
    //电弧侦测等级模式
    else
    {
        err = check_int_par(par->argv[0]);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        arc = atoi((const char*)par->argv[0]);
    }
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_file->arc_mode == ARC_CUR_MODE)
            {
                if(arc == g_cur_step->one_step.acw.arc_sur)
                {
                    break;
                }
                
                if(arc > 2000)
                {
                    return SCPI_DATA_OUT_OF_RANGE;
                }
                
                g_cur_step->one_step.acw.real_cur = arc;
            }
            else
            {
                if(arc == g_cur_step->one_step.acw.arc_sur)
                {
                    break;
                }
                
                if(arc > 9)
                {
                    return SCPI_DATA_OUT_OF_RANGE;
                }
                
                g_cur_step->one_step.acw.real_cur = arc;
            }
            break;
        }
        case DCW:
        {
            if(g_cur_file->arc_mode == ARC_CUR_MODE)
            {
                if(arc == g_cur_step->one_step.dcw.arc_sur)
                {
                    break;
                }
                
                if(arc > 2000)
                {
                    return SCPI_DATA_OUT_OF_RANGE;
                }
                
                g_cur_step->one_step.dcw.arc_sur = arc;
            }
            else
            {
                if(arc == g_cur_step->one_step.dcw.arc_sur)
                {
                    break;
                }
                
                if(arc > 9)
                {
                    return SCPI_DATA_OUT_OF_RANGE;
                }
                
                g_cur_step->one_step.dcw.arc_sur = arc;
            }
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_arc(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t arc = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            arc = g_cur_step->one_step.acw.real_cur;
            break;
        case DCW:
            arc = g_cur_step->one_step.acw.real_cur;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    //电弧侦测电流模式
    if(g_cur_file->arc_mode == ARC_CUR_MODE)
    {
        mysprintf(par->ask_data, NULL, 152, arc);
    }
    //电弧侦测等级模式
    else
    {
        sprintf((char*)par->ask_data, "%d", arc);
    }
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_arc_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_arc(par, ACW);
    }
    else
    {
        err = get_cur_step_arc(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_frequency(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t freq = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    freq = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.output_freq == freq)
            {
                return SCPI_NO_ERROR;
            }
            
            if(freq > 4000 || freq < 400)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.acw.output_freq = freq;
            break;
        }
        case GR:
        {
            if(g_cur_step->one_step.gr.output_freq == freq)
            {
                return SCPI_NO_ERROR;
            }
            
            if(freq > 4000 || freq < 400)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.gr.output_freq = freq;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_frequency(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t freq = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            freq = g_cur_step->one_step.acw.output_freq;
            break;
        case GR:
            freq = g_cur_step->one_step.gr.output_freq;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, freq);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_frequency_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_frequency(par, ACW);
    }
    else
    {
        err = get_cur_step_frequency(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_rtime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    time = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.rise_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            if(time > 9999 || (time > 0 && time < 3))
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.acw.rise_time = time;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.rise_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            if(time > 9999 || (time > 0 && time < 3))
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.dcw.rise_time = time;
            break;
        }
        case IR:
        {
            if(g_cur_step->one_step.ir.rise_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            if(time > 9999 || (time > 0 && time < 3))
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.ir.rise_time = time;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_rtime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            time = g_cur_step->one_step.acw.rise_time;
            break;
        case DCW:
            time = g_cur_step->one_step.dcw.rise_time;
            break;
        case IR:
            time = g_cur_step->one_step.ir.rise_time;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, time);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_rtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_rtime(par, ACW);
    }
    else
    {
        err = get_cur_step_rtime(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_ttime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    time = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    if(time > 9999 || (time > 0 && time < 3))
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.test_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.acw.test_time = time;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.test_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.dcw.test_time = time;
            break;
        }
        case IR:
        {
            if(g_cur_step->one_step.ir.test_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.ir.test_time = time;
            break;
        }
        case GR:
        {
            if(g_cur_step->one_step.gr.test_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.gr.test_time = time;
            break;
        }
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_ttime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            time = g_cur_step->one_step.acw.test_time;
            break;
        case DCW:
            time = g_cur_step->one_step.dcw.test_time;
            break;
        case IR:
            time = g_cur_step->one_step.ir.test_time;
            break;
        case GR:
            time = g_cur_step->one_step.gr.test_time;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, time);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ttime(par, ACW);
    }
    else
    {
        err = get_cur_step_ttime(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_ftime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint32_t time = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    time = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    if(time > 9999 || (time > 0 && time < 3))
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.fall_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.acw.fall_time = time;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.fall_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.dcw.fall_time = time;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_ftime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            time = g_cur_step->one_step.acw.fall_time;
            break;
        case DCW:
            time = g_cur_step->one_step.dcw.fall_time;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, time);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_ftime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ftime(par, ACW);
    }
    else
    {
        err = get_cur_step_ftime(par, ACW);
    }
    
    return err;
}
static SCPI_ERR_T set_cur_step_itime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    time = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    if(time > 9999)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.inter_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.acw.inter_time = time;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.inter_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.dcw.inter_time = time;
            break;
        }
        case IR:
        {
            if(g_cur_step->one_step.ir.inter_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.ir.inter_time = time;
            break;
        }
        case GR:
        {
            if(g_cur_step->one_step.gr.inter_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.gr.inter_time = time;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_itime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            time = g_cur_step->one_step.acw.inter_time;
            break;
        case DCW:
            time = g_cur_step->one_step.dcw.inter_time;
            break;
        case IR:
            time = g_cur_step->one_step.ir.inter_time;
            break;
        case GR:
            time = g_cur_step->one_step.gr.inter_time;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, time);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_itime(par, ACW);
    }
    else
    {
        err = get_cur_step_itime(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_ctime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    time = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* G模式才允许执行 */
    if(g_cur_file->work_mode != G_MODE)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    if(time > 9999)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    /* G模式的第一步缓变时间只能为0 */
    if(g_cur_step->one_step.com.step == 1)
    {
        time = 0;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.inter_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.acw.inter_time = time;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.inter_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.dcw.inter_time = time;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}


static SCPI_ERR_T get_cur_step_ctime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    /* G模式才允许执行 */
    if(g_cur_file->work_mode != G_MODE)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    switch(mode)
    {
        case ACW:
            time = g_cur_step->one_step.acw.inter_time;
            break;
        case DCW:
            time = g_cur_step->one_step.dcw.inter_time;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, time);
    
    return SCPI_NO_ERROR;
}


SCPI_ERR_T step_acw_ctime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ctime(par, ACW);
    }
    else
    {
        err = get_cur_step_ctime(par, ACW);
    }
    
    return err;
}



static SCPI_ERR_T check_sw_par(uint8_t *argv, uint8_t *value)
{
    if(0 == strcmp((const char*)argv, "ON") || 0 == strcmp((const char*)argv, "1"))
    {
        *value = 1;
    }
    else if(0 == strcmp((const char*)argv, "OFF") || 0 == strcmp((const char*)argv, "0"))
    {
        *value = 0;
    }
    else
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T set_cur_step_psignal(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t psignal = 0;
    
    err = check_sw_par(par->argv[0], &psignal);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.step_pass == psignal)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.acw.step_pass = psignal;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.step_pass == psignal)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.dcw.step_pass = psignal;
            break;
        }
        case IR:
        {
            if(g_cur_step->one_step.ir.step_pass == psignal)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.ir.step_pass = psignal;
            break;
        }
        case GR:
        {
            if(g_cur_step->one_step.gr.step_pass == psignal)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.gr.step_pass = psignal;
            break;
        }
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}


static SCPI_ERR_T get_cur_step_psignal(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t psignal = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            psignal = g_cur_step->one_step.acw.step_pass;
            break;
        case DCW:
            psignal = g_cur_step->one_step.dcw.step_pass;
            break;
        case IR:
            psignal = g_cur_step->one_step.ir.step_pass;
            break;
        case GR:
            psignal = g_cur_step->one_step.gr.step_pass;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    strcpy((char*)par->ask_data, psignal==0?"0":"1");
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_psignal(par, ACW);
    }
    else
    {
        err = get_cur_step_psignal(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T set_cur_step_cnext(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err;
    uint8_t cnext = 0;
    
    err = check_sw_par(par->argv[0], &cnext);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
        {
            if(g_cur_step->one_step.acw.step_con == cnext)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.acw.step_con = cnext;
            break;
        }
        case DCW:
        {
            if(g_cur_step->one_step.dcw.step_con == cnext)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.dcw.step_con = cnext;
            break;
        }
        case IR:
        {
            if(g_cur_step->one_step.ir.step_con == cnext)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.ir.step_con = cnext;
            break;
        }
        case GR:
        {
            if(g_cur_step->one_step.gr.step_con == cnext)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.gr.step_con = cnext;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_cnext(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t cnext = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            cnext = g_cur_step->one_step.acw.step_con;
            break;
        case DCW:
            cnext = g_cur_step->one_step.dcw.step_con;
            break;
        case IR:
            cnext = g_cur_step->one_step.ir.step_con;
            break;
        case GR:
            cnext = g_cur_step->one_step.gr.step_con;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    strcpy((char*)par->ask_data, cnext==0?"0":"1");
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_acw_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_cnext(par, ACW);
    }
    else
    {
        err = get_cur_step_cnext(par, ACW);
    }
    
    return err;
}

static SCPI_ERR_T check_port_par(uint8_t *argv, WORK_PORT *port)
{
    uint8_t len = 0;
    int32_t i = 0;
    uint16_t *p16 = NULL;
    uint8_t port_num = 0;
    
    memset(port, 0, sizeof(WORK_PORT));
    p16 = (uint16_t*)port;
    len = strlen((const char*)argv);
    
    for(i = 0; i < len; i++)
    {
        if(argv[i] == 'H')
        {
            *p16 |= (2 << (i * 2));
            port_num++;
        }
        else if(argv[i] == 'X')
        {
            *p16 &= ~(3 << (i * 2));
            port_num++;
        }
        else
        {
            return SCPI_INVALID_STRING_DATA;
        }
    }
    
    port->num = port_num;
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T set_cur_step_port(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    WORK_PORT port;
    WORK_PORT *tport;
    uint8_t buf[20] = {0};
    int32_t i = 0;
    
    for(i = 0; i < par->argc; i++)
    {
        if(strlen((const char*)par->argv[i]) > 1)
        {
            return SCPI_INVALID_STRING_DATA;
        }
        
        strcat((char*)buf, (const char*)par->argv[i]);
    }
    
    err = check_port_par(buf, &port);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            tport = &g_cur_step->one_step.acw.work_port;
            break;
        case DCW:
            tport = &g_cur_step->one_step.dcw.work_port;
            break;
        case IR:
            tport = &g_cur_step->one_step.ir.work_port;
            break;
        case GR:
            tport = &g_cur_step->one_step.gr.work_port;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    if(tport->num != port.num)
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    if(0 == memcmp(tport, &port, sizeof(tport->ports)))
    {
        return SCPI_NO_ERROR;
    }
    
    memcpy(tport, &port, sizeof(tport->ports));
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_port_par(uint8_t *argv, WORK_PORT *port)
{
    int32_t i = 0;
    uint16_t *p16 = NULL;
    uint8_t val = 0;
    
    p16 = (uint16_t*)port;
    argv[0] = 0;
    
    for(i = 0; i < port->num; i++)
    {
        val = ((*p16) >> (i * 2)) & 3;
        
        if(val == 2)
        {
            strcat((char*)argv, "H");
        }
        else if(val == 0)
        {
            strcat((char*)argv, "L");
        }
        
        if((i + 1) < port->num)
        {
            strcat((char*)argv, ",");
        }
    }
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_port(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    WORK_PORT *tport;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case ACW:
            tport = &g_cur_step->one_step.acw.work_port;
            break;
        case DCW:
            tport = &g_cur_step->one_step.dcw.work_port;
            break;
        case IR:
            tport = &g_cur_step->one_step.ir.work_port;
            break;
        case GR:
            tport = &g_cur_step->one_step.gr.work_port;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    get_port_par(par->ask_data, tport);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_port_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_port(par, ACW);
    }
    else
    {
        err = get_cur_step_port(par, ACW);
    }
    
    return err;
}
/* DCW */
SCPI_ERR_T step_dcw_voltage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_voltage(par, DCW);
    }
    else
    {
        err = get_cur_step_voltage(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_range_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_range(par, DCW);
    }
    else
    {
        err = get_cur_step_range(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_high(par, DCW);
    }
    else
    {
        err = get_cur_step_high(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_low(par, DCW);
    }
    else
    {
        err = get_cur_step_low(par, DCW);
    }
    
    return err;
}
static SCPI_ERR_T set_cur_step_ccurrent(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t charge = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    charge = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case DCW:
        {
            if(charge == g_cur_step->one_step.dcw.charge_cur)
            {
                return SCPI_NO_ERROR;
            }
            
            if(charge > g_cur_step->one_step.dcw.upper_limit)
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.dcw.charge_cur = charge;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T get_cur_step_ccurrent(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t charge = 0;
    uint8_t gear = 0;
    uint8_t format = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case DCW:
            charge = g_cur_step->one_step.dcw.lower_limit;
            gear = g_cur_step->one_step.dcw.range;
            format = 100 + 50 + dc_gear[gear].dec;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, format, charge);
    
    return SCPI_NO_ERROR;
}


SCPI_ERR_T step_dcw_ccurrent_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ccurrent(par, DCW);
    }
    else
    {
        err = get_cur_step_ccurrent(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_arc_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_arc(par, DCW);
    }
    else
    {
        err = get_cur_step_arc(par, DCW);
    }
    
    return err;
}
static SCPI_ERR_T set_cur_step_dtime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    uint8_t dec = 0;
    
    err = count_par_dces(par->argv[0], &dec);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    time = atof((const char*)par->argv[0]) * ten_power(dec);
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case DCW:
        {
            if(g_cur_step->one_step.dcw.delay_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            if(time > 9999 || (time > 0 && time < 3))
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.dcw.delay_time = time;
            break;
        }
        case IR:
        {
            if(g_cur_step->one_step.ir.delay_time == time)
            {
                return SCPI_NO_ERROR;
            }
            
            if(time > 9999 || (time > 0 && time < 3))
            {
                return SCPI_DATA_OUT_OF_RANGE;
            }
            
            g_cur_step->one_step.ir.delay_time = time;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T get_cur_step_dtime(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint32_t time = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case DCW:
            time = g_cur_step->one_step.dcw.delay_time;
            break;
        case IR:
            time = g_cur_step->one_step.ir.delay_time;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(par->ask_data, NULL, 151, time);
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T step_dcw_dtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_dtime(par, DCW);
    }
    else
    {
        err = get_cur_step_dtime(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_rtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_rtime(par, DCW);
    }
    else
    {
        err = get_cur_step_rtime(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ttime(par, DCW);
    }
    else
    {
        err = get_cur_step_ttime(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_ftime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ftime(par, DCW);
    }
    else
    {
        err = get_cur_step_ftime(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_itime(par, DCW);
    }
    else
    {
        err = get_cur_step_itime(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_ctime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ctime(par, DCW);
    }
    else
    {
        err = get_cur_step_ctime(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_psignal(par, DCW);
    }
    else
    {
        err = get_cur_step_psignal(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_cnext(par, DCW);
    }
    else
    {
        err = get_cur_step_cnext(par, DCW);
    }
    
    return err;
}
SCPI_ERR_T step_dcw_port_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_port(par, DCW);
    }
    else
    {
        err = get_cur_step_port(par, DCW);
    }
    
    return err;
}
/* IR */
SCPI_ERR_T step_ir_voltage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_voltage(par, IR);
    }
    else
    {
        err = get_cur_step_voltage(par, IR);
    }
    
    return err;
}
static SCPI_ERR_T set_cur_step_arange(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t arange = 0;
    
    err = check_sw_par(par->argv[0], &arange);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case IR:
        {
            if(g_cur_step->one_step.ir.auto_shift == arange)
            {
                return SCPI_NO_ERROR;
            }
            
            g_cur_step->one_step.ir.auto_shift = arange;
            break;
        }
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    set_step_par_for_comm();
    
    return SCPI_NO_ERROR;
}


static SCPI_ERR_T get_cur_step_arange(SCPI_DIS_FUN_PAR *par, uint8_t mode)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t arange = 0;
    
    err = check_mode_for_comm(mode);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    switch(mode)
    {
        case IR:
            arange = g_cur_step->one_step.ir.auto_shift;
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    strcpy((char*)par->ask_data, arange==0?"0":"1");
    
    return SCPI_NO_ERROR;
}


SCPI_ERR_T step_ir_arange_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_arange(par, IR);
    }
    else
    {
        err = get_cur_step_arange(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_high(par, IR);
    }
    else
    {
        err = get_cur_step_high(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_low(par, IR);
    }
    else
    {
        err = get_cur_step_low(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_rtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_rtime(par, IR);
    }
    else
    {
        err = get_cur_step_rtime(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ttime(par, IR);
    }
    else
    {
        err = get_cur_step_ttime(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_dtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_dtime(par, IR);
    }
    else
    {
        err = get_cur_step_dtime(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_itime(par, IR);
    }
    else
    {
        err = get_cur_step_itime(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_psignal(par, IR);
    }
    else
    {
        err = get_cur_step_psignal(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_cnext(par, IR);
    }
    else
    {
        err = get_cur_step_cnext(par, IR);
    }
    
    return err;
}
SCPI_ERR_T step_ir_port_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_port(par, IR);
    }
    else
    {
        err = get_cur_step_port(par, IR);
    }
    
    return err;
}
/* GR */
SCPI_ERR_T step_gr_current_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_voltage(par, GR);
    }
    else
    {
        err = get_cur_step_voltage(par, GR);
    }
    
    return err;
}
SCPI_ERR_T step_gr_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_high(par, GR);
    }
    else
    {
        err = get_cur_step_high(par, GR);
    }
    
    return err;
}
SCPI_ERR_T step_gr_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_low(par, GR);
    }
    else
    {
        err = get_cur_step_low(par, GR);
    }
    
    return err;
}
SCPI_ERR_T step_gr_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_ttime(par, GR);
    }
    else
    {
        err = get_cur_step_ttime(par, GR);
    }
    
    return err;
}
SCPI_ERR_T step_gr_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_itime(par, GR);
    }
    else
    {
        err = get_cur_step_itime(par, GR);
    }
    
    return err;
}
SCPI_ERR_T step_gr_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_psignal(par, GR);
    }
    else
    {
        err = get_cur_step_psignal(par, GR);
    }
    
    return err;
}
SCPI_ERR_T step_gr_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    
    if(par->type == SCPI_EXE)
    {
        err = set_cur_step_cnext(par, GR);
    }
    else
    {
        err = get_cur_step_cnext(par, GR);
    }
    
    return err;
}
/* 结果指令集 */
SCPI_ERR_T result_cap_used_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "%d", sys_par.used_res_num);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_free_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint32_t total_res = 0;
    
    total_res = get_result_max_num();
    sprintf((char*)par->ask_data, "%d", total_res - sys_par.used_res_num);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_all_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint32_t total_res = 0;
    
    total_res = get_result_max_num();
    sprintf((char*)par->ask_data, "%d", total_res);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_pass_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "%d", sys_par.pass_res_num);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_fail_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    sprintf((char*)par->ask_data, "%d", sys_par.used_res_num - sys_par.pass_res_num);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_clear_all_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    result_clear_all();
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T transform_one_res_to_str(uint8_t *res_str, RESULT_INF *res)
{
    uint8_t buf[20] = {0};
    uint8_t range = 0;
    uint8_t format = 0;
    
    sprintf((char*)res_str, "%s,%02d,%02d,%s,%d,%s,",
                res->product_code,
                res->par.step,
                res->par.total_step,
                res->par.work_mode == G_MODE?"G":"N",
                res->par.mode - 1,
                res->par.file_name);
                
    switch(res->par.mode)
    {
        case ACW:
            mysprintf(buf, NULL, 153, res->test_data.un.acw.vol);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            range = res->test_data.un.acw.range;
            sprintf((char*)buf, "%d,", ac_gear[range].comm);
            strcat((char*)res_str, (const char*)buf);
            format = 100 + 50 + ac_gear[range].dec;
            mysprintf(buf, NULL, format, res->test_data.un.acw.cur);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            if(res->test_data.un.acw.real > 0)
            {
                mysprintf(buf, NULL, format, res->test_data.un.acw.real);
            }
            else
            {
                strcpy((char*)buf, "-----");
            }
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            break;
        case DCW:
            mysprintf(buf, NULL, 153, res->test_data.un.dcw.vol);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            range = res->test_data.un.dcw.range;
            sprintf((char*)buf, "%d,", dc_gear[range].comm);
            strcat((char*)res_str, (const char*)buf);
            format = 100 + 50 + dc_gear[range].dec;
            mysprintf(buf, NULL, format, res->test_data.un.dcw.cur);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            strcat((char*)res_str, "-----");
            strcat((char*)res_str, ",");
            break;
        case IR:
            mysprintf(buf, NULL, 153, res->test_data.un.ir.vol);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            range = res->test_data.un.ir.range;
            sprintf((char*)buf, "%d,", ir_gear[range].comm);
            strcat((char*)res_str, (const char*)buf);
            format = 100 + 50 + ir_gear[range].dec;
            mysprintf(buf, NULL, format, res->test_data.un.ir.res);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            strcat((char*)res_str, "-----");
            strcat((char*)res_str, ",");
            break;
        case GR:
            mysprintf(buf, NULL, 153, res->test_data.un.gr.cur);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            format = 100 + 50 + 1;
            mysprintf(buf, NULL, format, res->test_data.un.gr.res);
            strcat((char*)res_str, (const char*)buf);
            strcat((char*)res_str, ",");
            strcat((char*)res_str, "-----");
            strcat((char*)res_str, ",");
            break;
        default:
            return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    mysprintf(buf, NULL, 151, res->test_data.test_time);
    strcat((char*)res_str, (const char*)buf);
    strcat((char*)res_str, ",");
    strcat((char*)res_str, res->test_data.test_result==ST_PASS? "P":"F");
    strcat((char*)res_str, ",");
    turn_rtc_date_str(res->test_data.record_date, buf);
    strcat((char*)res_str, (const char*)buf);
    strcat((char*)res_str, " ");
    turn_rtc_time_str(res->test_data.record_time, buf);
    strcat((char*)res_str, (const char*)buf);
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_fetch_single_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err;
    uint32_t result_count = 0;
    RESULT_INF res;
    CS_ERR cs_err;
    
    err = check_int_par(par->argv[0]);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    result_count = atoi((const char*)par->argv[0]);
    
    if(result_count > sys_par.used_res_num)
    {
        return SCPI_DATA_OUT_OF_RANGE;
    }
    
    read_one_result(result_count, &res, &cs_err);
    
    if(cs_err != CS_ERR_NONE)
    {
        return SCPI_EXECUTE_NOT_ALLOWED;
    }
    
    err = transform_one_res_to_str(par->ask_data, &res);
    
    return err;
}
SCPI_ERR_T result_dut_name_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint8_t len = 0;
    
    len = strlen((const char*)par->argv[0]);
    
    if(par->type == SCPI_EXE)
    {
        if(len > NAME_LON)
        {
            return SCPI_INVALID_STRING_DATA;
        }
        
        if(len <= 2 || par->argv[0][0] != '"' || par->argv[0][len - 1] != '"')
        {
            return SCPI_INVALID_STRING_DATA;
        }
        
        if(0 == memcmp(sys_par.dut_name, &par->argv[0][1], len - 2))
        {
            return SCPI_NO_ERROR;
        }
        
        memset(sys_par.dut_name, 0, sizeof(sys_par.dut_name));
        strncpy((char*)sys_par.dut_name, (const char*)&par->argv[0][1], len - 2);
        save_sys_par();
    }
    else
    {
        strcpy((char*)par->ask_data, (const char*)sys_par.dut_name);
    }
    
    return SCPI_NO_ERROR;
}
/* 系统指令集 */
SCPI_ERR_T sys_screen_contrast_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    
    if(par->type == SCPI_EXE)
    {
        err = check_int_par(par->argv[0]);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        value = atoi((const char*)par->argv[0]);
        
        if(value < 1 || value > 9)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        if(sys_par.contrast == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.contrast = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.contrast);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_beeper_volume_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_rhint_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_int_par(par->argv[0]);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        value = atoi((const char*)par->argv[0]);
        
        if(value > 9)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        if(sys_par.allowance == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.allowance = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.allowance);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_rsave_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_sw_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.is_save_res == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.is_save_res = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.is_save_res);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_ocover_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_sw_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.is_overflow_cover == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.is_overflow_cover = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.is_overflow_cover);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_gfi_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_sw_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.is_gfi_protect == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.is_gfi_protect = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.is_gfi_protect);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_phv_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
static SCPI_ERR_T check_tsignal_par(uint8_t *argv, uint8_t *value)
{
    if(0 == strcmp((const char*)argv, "TEVel") || 0 == strcmp((const char*)argv, "1"))
    {
        *value = 1;
    }
    else if(0 == strcmp((const char*)argv, "DOT") || 0 == strcmp((const char*)argv, "0"))
    {
        *value = 0;
    }
    else
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T sys_tsignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_tsignal_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.test_level == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.test_level = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.test_level);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_scheck_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_sw_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.is_self_check == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.is_self_check = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.is_self_check);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_chint_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

static SCPI_ERR_T check_tport_par(uint8_t *argv, uint8_t *value)
{
    if(0 == strcmp((const char*)argv, "GND") || 0 == strcmp((const char*)argv, "1"))
    {
        *value = 1;
    }
    else if(0 == strcmp((const char*)argv, "FLOat") || 0 == strcmp((const char*)argv, "0"))
    {
        *value = 0;
    }
    else
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    return SCPI_NO_ERROR;
}

SCPI_ERR_T sys_tport_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_tport_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.test_method == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.test_method = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.test_method);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_language_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint8_t value;
    
    if(par->type == SCPI_EXE)
    {
        if(0 == strcmp((const char*)par->argv[0], "CHINese")
            || 0 == strcmp((const char*)par->argv[0], "1"))
        {
            value = CHINESE;
        }
        else if(0 == strcmp((const char*)par->argv[0], "ENGLish")
            || 0 == strcmp((const char*)par->argv[0], "0"))
        {
            value = ENGLISH;
        }
        /* 不允许的字符串参数 */
        else
        {
            return SCPI_INVALID_STRING_DATA;
        }
        
        if(sys_par.language != value)
        {
            sys_par.language = value;
            save_sys_par();
            update_system_language_bridge();
        }
    }
    else
    {
        strcpy((char*)par->ask_data, sys_par.language==CHINESE? "1":"0");
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_fcontinue_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_sw_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.is_falt_continue == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.is_falt_continue = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.is_falt_continue);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_klock_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_sw_par(par->argv[0], &value);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        if(sys_par.key_lock == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.key_lock = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.key_lock);
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_password_new_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t len = 0;
    
    err = check_int_par(par->argv[1]);
    
    if(err != SCPI_NO_ERROR)
    {
        return err;
    }
    
    len = strlen((const char*)par->argv[1]);
    
    /* 检查新密码长度 */
    if(len > 8)
    {
        return SCPI_INVALID_STRING_DATA;
    }
    
    /* 验证密码 */
    if(0 != strcmp((const char*)sys_par.password, (const char*)par->argv[0]))
    {
        return SCPI_PARAMETER_NOT_ALLOWED;
    }
    
    if(0 == strcmp((const char*)sys_par.password, (const char*)par->argv[1]))
    {
        return SCPI_NO_ERROR;
    }
    
    /* 设置新密码 */
    strcpy((char*)sys_par.password, (const char*)par->argv[1]);
    save_sys_par();
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_password_now_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    strcpy((char*)par->ask_data, (const char*)sys_par.password);
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_time_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value[6] = {0};
    uint8_t flag = 0;
    uint16_t year = 0;
    uint8_t day_max;
    int32_t i = 0;
    
    if(par->type == SCPI_EXE)
    {
        for(i = 0; i < 6; i++)
        {
            err = check_int_par(par->argv[i]);
            
            if(err != SCPI_NO_ERROR)
            {
                return err;
            }
            
            value[i] = atoi((const char*)par->argv[i]);
        }
        
        /* 检查年 */
        if(value[0] > 99)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        /* 检查月 */
        if(value[1] < 1 || value[1] > 12)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        year = 2000 + value[0];
        
        if((((year % 4) == 0) && (year % 100 != 0)) || ((year % 400) == 0))
        {
            flag = 1;
        }
        
        if(value[1] == 4 || value[1] == 6 || value[1] == 9 || value[1] == 11)
        {
            day_max = 30;
        }
        else if(value[1] == 2)
        {
            day_max = 28 + flag;
        }
        else
        {
            day_max = 31;
        }
        
        /* 检查日期 */
        if(value[2] < 1 || value[2] > day_max)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        /* 检查时 */
        if(value[3] > 23)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        /* 检查分 */
        if(value[4] > 59)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        /* 检查秒 */
        if(value[5] > 59)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        rtc_set_time(year, value[1], value[2], value[3], value[4], value[5]);
    }
    else
    {
        sprintf((char*)par->ask_data, "%02d,%02d,%02d,%02d,%02d,%02d",
                get_rtc_year() - 2000,
                get_rtc_month(),
                get_rtc_day(),
                get_rtc_hour(),
                get_rtc_minute(),
                get_rtc_second());
    }
    
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_nrule_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    SCPI_ERR_T err = SCPI_NO_ERROR;
    uint8_t value = 0;
    
    if(par->type == SCPI_EXE)
    {
        err = check_int_par(par->argv[0]);
        
        if(err != SCPI_NO_ERROR)
        {
            return err;
        }
        
        value = atoi((const char*)par->argv[0]);
        
        if(value > 2)
        {
            return SCPI_DATA_OUT_OF_RANGE;
        }
        
        if(sys_par.num_rule == value)
        {
            return SCPI_NO_ERROR;
        }
        
        sys_par.num_rule = value;
        save_sys_par();
    }
    else
    {
        sprintf((char*)par->ask_data, "%d", sys_par.num_rule);
    }
    
    return SCPI_NO_ERROR;
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
