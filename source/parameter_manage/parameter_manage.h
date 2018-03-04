/**
  ******************************************************************************
  * @file    parameter_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.5
  * @brief   参数管理模块
  ******************************************************************************
  */

#ifndef __PARAMETER_MANAGE_H__
#define __PARAMETER_MANAGE_H__

#include "cs99xx_struct.h"

extern CS_ERR check_step_data(NODE_STEP*step);
extern CS_BOOL is_file_exist(FILE_NUM file_num);
extern CS_ERR check_file_data(TEST_FILE*file);
extern void init_file_data(TEST_FILE *file, FILE_NUM file_num);
extern void clear_file_data(TEST_FILE *file, FILE_NUM file_num);
extern void clear_all_file_data(void);
extern void init_all_file(void);
extern void del_all_file(void);
extern TEST_FILE* get_file_inf(FILE_NUM file_num, CS_ERR* err);
extern void init_sys_par(void);
extern void check_sys_par(CS_ERR *err);
extern uint8_t pwd_max_len(void);
extern void set_cur_file(FILE_NUM file_num);
extern void init_instrument_data(void);
extern void swap_step(const STEP_NUM one, const STEP_NUM two);
extern void insert_step(uint16_t pos, uint8_t mode);
extern void del_step(STEP_NUM step);
extern void load_steps_to_list(const int16_t step, uint8_t step_num);
extern NODE_STEP *get_g_cur_step(void);
extern void init_mode(NODE_STEP *p);
extern void transform_test_port_to_str(WORK_PORT *port, uint8_t *buf);
extern void transform_str_to_test_port(WORK_PORT *port, uint8_t *buf);
extern uint16_t transform_arc_cur_to_grade(uint16_t arc_cur_val);
extern uint16_t transform_arc_grade_to_cur(uint16_t gear);
extern void init_new_group_inf(TEST_FILE *file);
extern void del_one_group_inf(FILE_NUM file_num);
extern void clear_cur_group_all_test_step(void);
extern void save_setting_step(void);
extern void clear_group_table(void);
extern void clear_step_used_flag(void);
extern void copy_cur_file_to_new_pos(const FILE_NUM file_num);



#endif //__PARAMETER_MANAGE_H__


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
