
#include "string.h"
#include "stdio.h"
#include "cs99xx_struct.h"
#include "cs99xx_type.h"
#include "scpi_cmd_def.h"
#include "cs99xx_scpi_dispose.h"

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
    return SCPI_NO_ERROR;
}
SCPI_ERR_T comm_local_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T comm_control_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_new_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_edit_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_del_single_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_del_all_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_save_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_read_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T file_catalog_single_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}

SCPI_ERR_T source_test_start_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_test_stop_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_test_status_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_test_fetch_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_load_step_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_load_file_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_findex_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_fmessage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_sindex_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_smessage_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
    return SCPI_NO_ERROR;
}
SCPI_ERR_T source_list_mode_scpi_dispose_fun(SCPI_DIS_FUN_PAR *par)
{
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
