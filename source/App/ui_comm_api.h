
#ifndef __UI_COMM_API_H__
#define __UI_COMM_API_H__

#include "cs99xx_struct.h"

void com_st_exit_remote(void);
void com_st_into_remote(void);
void update_system_language(void);
void send_start_sign(void);
uint8_t get_road_test_status(ROAD_NUM_T road);
uint8_t* get_road_test_voltage(ROAD_NUM_T road);
uint8_t* get_road_test_current(ROAD_NUM_T road);
uint8_t* get_road_test_real_current(ROAD_NUM_T road);
uint8_t* get_road_test_time(ROAD_NUM_T road);
uint8_t* get_road_test_st(ROAD_NUM_T road);
void update_test_win_text_display(void);
uint8_t get_max_roads(void);
void result_clear_all(void);


#endif //__UI_COMM_API_H__
