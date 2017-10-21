
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
    return SCPI_NO_ERROR;
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

void transition_work_port(uint8_t *buf, WORK_PORT *port)
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
SCPI_ERR_T step_insert_acw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_insert_dcw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_insert_ir_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_insert_gr_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_delete_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_move_front_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_move_behind_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_interchange_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_mode_acw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_mode_dcw_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_mode_ir_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_mode_gr_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
    /* ACW */
SCPI_ERR_T step_acw_voltage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_range_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_real_current_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_arc_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_frequency_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_rtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_ftime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_ctime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_acw_port_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
    /* DCW */
SCPI_ERR_T step_dcw_voltage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_range_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_ccurrent_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_arc_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_dtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_rtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_ftime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_ctime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_dcw_port_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
    /* IR */
SCPI_ERR_T step_ir_voltage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_range_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_rtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_dtime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_ir_port_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
    /* GR */
SCPI_ERR_T step_gr_current_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_gr_high_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_gr_low_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_gr_ttime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_gr_itime_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_gr_psignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T step_gr_cnext_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
    /* 结果指令集 */
SCPI_ERR_T result_cap_used_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_free_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_all_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_pass_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_cap_fail_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_clear_all_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_fetch_single_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T result_dut_name_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
    /* 系统指令集 */
SCPI_ERR_T sys_screen_contrast_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_beeper_volume_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_rhint_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_rsave_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_ocover_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_gfi_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_phv_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_tsignal_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_scheck_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_chint_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_tport_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_language_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    uint8_t value;
    
    if(par->type == SCPI_EXE)
    {
        if(0 == strcmp((const char*)par->argv[0], "CHINese")
            || 0 == strcmp((const char*)par->argv[0], "CHIN")
            || 0 == strcmp((const char*)par->argv[0], "1"))
        {
            value = 0;
        }
        else if(0 == strcmp((const char*)par->argv[0], "ENGLish")
            || 0 == strcmp((const char*)par->argv[0], "ENGL")
            || 0 == strcmp((const char*)par->argv[0], "0"))
        {
            value = 1;
        }
        /* 参数不允许 */
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
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_klock_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_password_new_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_password_now_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_time_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T sys_nrule_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
