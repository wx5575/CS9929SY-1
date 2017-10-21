
#include "ui_comm_api.h"
#include "keyboard.h"
#include "os.h"

void set_instrument_exit_remote_bridge(void)
{
    com_st_exit_remote();
}

void set_instrument_into_remote_bridge(void)
{
    com_st_into_remote();
}

void get_instrument_comm_status_bridge(void)
{
//    com_st_into_remote();
}

void update_system_language_bridge(void)
{
    update_system_language();
    update_test_win_text_display();//更新测试窗口显示
}

void source_test_start_bridge(void)
{
    simulate_send_start_key();
}

void source_test_stop_bridge(void)
{
    simulate_send_stop_key();
}


