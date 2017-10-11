/**
  ******************************************************************************
  * @file    result_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   结果管理窗口
  ******************************************************************************
  */

#include "string.h"
#include "stdio.h"
#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"
#include "7_result_win.h"
#include "result_win.h"
#include "ff.h"
#include "FATFS_MANAGE.H"
#include "running_test.h"
#include "rtc_config.h"
#include "set_goto_result_num_win.h"
#include "image/logo.h"
#include "mem_alloc.h"


/* Private typedef -----------------------------------------------------------*/

enum{
    RES_WIN_IDLE,///<空闲状态
    RES_WIN_UPDATE,///<更新显示
};
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void result_win_cb(WM_MESSAGE* pMsg);
static void update_result_win_menu_key_inf(WM_HMEM hWin);

static void result_win_f1_cb(KEY_MESSAGE *key_msg);
static void result_win_f2_cb(KEY_MESSAGE *key_msg);
static void result_win_f3_cb(KEY_MESSAGE *key_msg);
static void result_win_f4_cb(KEY_MESSAGE *key_msg);
static void result_win_f5_cb(KEY_MESSAGE *key_msg);
static void result_win_f6_cb(KEY_MESSAGE *key_msg);

static void result_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void result_win_direct_key_down_cb(KEY_MESSAGE *key_msg);

static void init_create_result_win_text_ele(MYUSER_WINDOW_T* win);
static void init_create_result_win_com_ele(MYUSER_WINDOW_T* win);
static void update_result_win_key_inf(WM_HWIN hWin);
static void update_selete_result_opt(void);
static void update_result_inf_dis(uint32_t result_count);
static void update_result_inf_dis(uint32_t result_count);
static void read_result_to_buf(uint32_t result_count);
static void update_result_win_title(RESULT_INF *res);
static void result_win_task(void);
/* Private variables ---------------------------------------------------------*/
static uint8_t win_task;///<窗口任务号
static WM_HWIN result_win_timer_handle;
static uint8_t l_cur_result;///<当前结果项
static uint32_t l_result_count;///<当前结果项计数
/**
  * @brief  结果管理窗口位置尺寸信息,根据不同屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* result_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_result_windows,///<4.3寸屏
    &_7_result_windows,///<5.6寸屏
    &_7_result_windows,///<7寸屏
};
/**
  * @brief 界面文本对象池数组
  */
static TEXT_ELE_T result_win_ele_pool[]=
{
	{{"测试结果信息","Test Result Information"}, RESULT_WIN_TEST_RES_INF},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_1},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_2},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_3},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_4},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_5},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_6},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_7},
	{{"0001", "0001"}, RESULT_WIN_TEST_RES_8},
//	{{"0001 第3路 ACW 5.000kV 2.000mA   3.0s 合格",
//      "0001 ROAD1 ACW 5.000kV 2.000mA   3.0s PASS"}, RESULT_WIN_TEST_RES_8},
      
	{{"设置参数", "SETTING PAR."}, RESULT_WIN_SETTING_PAR},
	{{"测试模式:ACW\n"
       "输出电压:5.000kV\n"
       "电流上限:2.000mA\n"
       "电流下限:2.000mA\n"
       "测试时间:003.0s\n"
       "上升时间:003.0s\n",
       "TEST MODE:ACW\n"
       "Output Vol.:5.000kV\n"
       "Cur High:2.000mA\n"
       "Cur Low:2.000mA\n"
       "TEST TIME:003.0s\n"
       "RISE TIME:003.0s\n",}, RESULT_WIN_SETTING_PAR_C},
       
	{{"测试数据", "TEST DATA"}, RESULT_WIN_TEST_DATA},
	{{
       "产品编号:123456789\n"
       "输出电压:5.000kV\n"
       "测试电流:2.000mA\n"
       "测试时间:003.0s\n"
       "测试结果:合格\n"
       "记录时间:2017-9-15 12:12:12\n",
       "Product NO.:123456789\n"
       "Output Vol.:5.000kV\n"
       "TEST CUR:2.000mA\n"
       "TEST TIME:3.0s\n"
       "TEST RESULT:PASS\n"
       "RECORD:2017-9-15 12:12:12\n",}, RESULT_WIN_TEST_DATA_C},
};
static CS_INDEX result_inf_index[]=
{
    RESULT_WIN_TEST_RES_1,
    RESULT_WIN_TEST_RES_2,
    RESULT_WIN_TEST_RES_3,
    RESULT_WIN_TEST_RES_4,
};
static CS_INDEX result_win_text_ele_index_pool[]=
{
    RESULT_WIN_TEST_RES_1,
    RESULT_WIN_TEST_RES_2,
    RESULT_WIN_TEST_RES_3,
    RESULT_WIN_TEST_RES_4,
//    RESULT_WIN_TEST_RES_5,
//    RESULT_WIN_TEST_RES_6,
//    RESULT_WIN_TEST_RES_7,
//    RESULT_WIN_TEST_RES_8,
    
    RESULT_WIN_SETTING_PAR,
    RESULT_WIN_SETTING_PAR_C,
    RESULT_WIN_TEST_DATA,
    RESULT_WIN_TEST_DATA_C,
};

/**
  * @brief 结果管理界面文本对象池数组
  */
MYUSER_WINDOW_T result_windows =
{
    {"结果管理窗口", "Result window"},
    result_win_cb,update_result_win_key_inf,
	{
        result_win_ele_pool,COUNT_ARRAY_SIZE(result_win_ele_pool),
        (CS_INDEX*)result_win_text_ele_index_pool,
        COUNT_ARRAY_SIZE(result_win_text_ele_index_pool),
        init_create_result_win_text_ele
    },/*text*/
    {0},/*edit*/
    {
        com_text_ele_pool, ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)group_com_ele_table,ARRAY_SIZE(group_com_ele_table),
        init_create_result_win_com_ele,
    },/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    result_win_pos_size_pool/*pos_size_pool*/
};
/**
  * @brief 定义指向结果窗口的this指针
  */
static MYUSER_WINDOW_T *this_win = &result_windows;
/**
  * @brief 菜单键初始化信息
  */
static MENU_KEY_INFO_T 	result_win_menu_key_info[] =
{
    {"", F_KEY_STATIS   , KEY_F1 & _KEY_UP, result_win_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, result_win_f2_cb },//f2
    {"", F_KEY_LEAD_OUT , KEY_F3 & _KEY_UP, result_win_f3_cb },//f3
    {"", F_KEY_GOTO		, KEY_F4 & _KEY_UP, result_win_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, result_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, result_win_f6_cb },//f6
};
/**
  * @brief 系统功能键初始化信息
  */
static CONFIG_FUNCTION_KEY_INFO_T 	result_win_sys_key_pool[]={
	{KEY_UP		, result_win_direct_key_up_cb		},
	{KEY_DOWN	, result_win_direct_key_down_cb 	},
	{CODE_LEFT	, result_win_direct_key_down_cb	 },
	{CODE_RIGH	, result_win_direct_key_up_cb      },
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  结果存在菜单功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_f1_cb(KEY_MESSAGE *key_msg)
{
    create_result_statis_dialog(key_msg->user_data);
}
/**
  * @brief  结果存在菜单功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_f2_cb(KEY_MESSAGE *key_msg)
{
    my_deldir("\\ROOT\\RESULT");//删除结果目录
    read_result_to_buf(0);
    update_result_inf_dis(l_result_count - l_cur_result);//更新显示
    
    sys_par.used_res_num = 0;
    sys_par.pass_res_num = 0;
    sys_par.cover_res_num = 0;
    save_sys_par();
}
/**
  * @brief  结果存在菜单功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_f4_cb(KEY_MESSAGE *key_msg)
{
    create_set_goto_result_num_win(key_msg->user_data);
}
/**
  * @brief  结果存在菜单功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

static void read_result_to_buf(uint32_t result_count)
{
    uint8_t num = 0;
    int32_t i = 0;
    FRESULT fresult;
    FIL f;
    uint32_t one_res_size;
    uint32_t real_res_size;
    
    memset(&result_inf_pool, 0, sizeof(result_inf_pool));
    fresult = f_open (&f, (const char*)"\\ROOT\\RESULT\\RESULT.BIN", FA_READ | FA_OPEN_ALWAYS);
    
    if(fresult != FR_OK)
    {
        goto end;
    }
    
    one_res_size = sizeof(RESULT_INF);
    num = ARRAY_SIZE(result_inf_index);
    
    fresult = f_lseek (&f, (result_count - 1) * one_res_size);
    
    if(fresult != FR_OK)
    {
        goto end;
    }
    
    for(i = 0; i < num; i++)
    {
        fresult = f_read (&f, &result_inf_pool[i], one_res_size, &real_res_size);
        
        if(fresult != FR_OK)
        {
            goto end;
        }
    }
    
    end:
    f_close(&f);
}

void update_one_res_inf(RESULT_INF *res)
{
    uint8_t t_buf[201] = {0};
    uint8_t setting_buf[201] = {0};
    uint8_t testting_buf[201] = {0};
    uint8_t decs = 0;
    uint8_t unit = 0;
    
    /* 判断结果是有效数据 */
    if(res->par.step != 0)
    {
        sprintf((char*)setting_buf, "%s", SELE_STR("测试模式:","Mode:"));
        strcat((char*)setting_buf, (const char*)mode_pool[res->par.mode]);
        strcat((char*)setting_buf, "\n");
        
        strcat((char*)setting_buf, SELE_STR("输出电压:","Voltage:"));
        mysprintf(t_buf, unit_pool[VOL_U_kV], 53, res->par.un.acw.vol);
        strcat((char*)setting_buf, (const char*)t_buf);
        strcat((char*)setting_buf, "\n");
        strcat((char*)setting_buf, SELE_STR("电流上限:","High:"));
        decs = ac_gear[res->par.un.acw.range].dec;
        unit = ac_gear[res->par.un.acw.range].unit;
        mysprintf(t_buf, unit_pool[unit], 50 + decs, res->par.un.acw.hight);
        strcat((char*)setting_buf, (const char*)t_buf);
        strcat((char*)setting_buf, "\n");
        strcat((char*)setting_buf, SELE_STR("电流下限:","Low:"));
        mysprintf(t_buf, unit_pool[unit], 50 + decs, res->par.un.acw.lower);
        strcat((char*)setting_buf, (const char*)t_buf);
        strcat((char*)setting_buf, "\n");
        strcat((char*)setting_buf, SELE_STR("测试时间:","TestTime:"));
        mysprintf(t_buf, unit_pool[TIM_U_s], 50 + 1, res->par.un.acw.test_time);
        strcat((char*)setting_buf, (const char*)t_buf);
        strcat((char*)setting_buf, "\n");
        strcat((char*)setting_buf, SELE_STR("上升时间:","RiseTime:"));
        mysprintf(t_buf, unit_pool[TIM_U_s], 50 + 1, res->par.un.acw.rise_time);
        strcat((char*)setting_buf, (const char*)t_buf);
        strcat((char*)setting_buf, "\n");
        
        testting_buf[0] = 0;
        strcat((char*)testting_buf, SELE_STR("产品编号:","Product NO.:"));
        strcat((char*)testting_buf, (const char*)res->product_code);
        strcat((char*)testting_buf, "\n");
        
        strcat((char*)testting_buf, SELE_STR("输出电压:","Output Vol.:"));
        mysprintf(t_buf, unit_pool[VOL_U_kV], 53, res->test_data.un.acw.vol);
        strcat((char*)testting_buf, (const char*)t_buf);
        strcat((char*)testting_buf, "\n");
        strcat((char*)testting_buf, SELE_STR("测试电流:","Test CUR.:"));
        decs = ac_gear[res->test_data.un.acw.range].dec;
        unit = ac_gear[res->test_data.un.acw.range].unit;
        mysprintf(t_buf, unit_pool[unit], 50 + decs, res->test_data.un.acw.cur);
        strcat((char*)testting_buf, (const char*)t_buf);
        strcat((char*)testting_buf, "\n");
        strcat((char*)testting_buf, SELE_STR("测试时间:","TestTime:"));
        mysprintf(t_buf, unit_pool[TIM_U_s], 50 + 1, res->test_data.test_time);
        strcat((char*)testting_buf, (const char*)t_buf);
        strcat((char*)testting_buf, "\n");
        strcat((char*)testting_buf, SELE_STR("测试结果:","TestResult:"));
        strcat((char*)testting_buf, (const char*)get_test_status_str(res->test_data.test_result));
        strcat((char*)testting_buf, "\n");
        strcat((char*)testting_buf, SELE_STR("记录时间:","RECORD:"));
        turn_rtc_date_str(res->test_data.record_date, t_buf);
        strcat((char*)testting_buf, (const char*)t_buf);
        strcat((char*)testting_buf, " ");
        turn_rtc_time_str(res->test_data.record_time, t_buf);
        strcat((char*)testting_buf, (const char*)t_buf);
    }
    
    set_text_ele(RESULT_WIN_SETTING_PAR_C, this_win, setting_buf);
    set_text_ele(RESULT_WIN_TEST_DATA_C, this_win, testting_buf);
    update_result_win_title(res);
}

static void update_result_win_title(RESULT_INF *res)
{
    uint8_t buf[201] = {0};
    
    /* 判断结果是有效数据 */
    if(res->par.step != 0)
    {
        update_com_text_ele(COM_UI_CUR_FILE_NAME, this_win, res->par.file_name);
        sprintf((char*)buf, "%d/%d", res->par.step, res->par.total_step);
        update_com_text_ele(COM_UI_CUR_STEP, this_win, buf);
        sprintf((char*)buf, "%s", work_mode_pool[res->par.work_mode==N_MODE]);
        update_com_text_ele(COM_UI_CUR_WORK_MODE, this_win, buf);
    }
    /* 无效数据 */
    else
    {
        update_com_text_ele(COM_UI_CUR_FILE_NAME, this_win, "");
        update_com_text_ele(COM_UI_CUR_STEP, this_win, "");
        update_com_text_ele(COM_UI_CUR_WORK_MODE, this_win, "");
    }
}
static void update_result_inf_dis(uint32_t result_count)
{
    uint8_t num = 0;
    int32_t i = 0;
    uint8_t buf[201] = {0};
    uint8_t t_buf[201] = {0};
    CS_INDEX index;
    RESULT_INF *res;
    uint8_t decs = 0;
    uint8_t unit = 0;
    const uint8_t *str = NULL;
    
    num = ARRAY_SIZE(result_inf_index);
    
    for(i = 0; i < num; i++)
    {
        res = &result_inf_pool[i];
        
        /* 判断结果是有效数据 */
        if(res->par.step != 0)
        {
            sprintf((char*)buf, "%04d ", result_count + i);
            sprintf((char*)t_buf, SELE_STR("第%d路 ", "Road%d "), res->road_num);
            strcat((char*)buf, (const char*)t_buf);
            strcat((char*)buf, (const char*)mode_pool[res->par.mode]);
            
            switch(res->par.mode)
            {
                case ACW:
                    strcat((char*)buf, " ");
                    mysprintf(t_buf, unit_pool[VOL_U_kV], 53, res->test_data.un.acw.vol);
                    strcat((char*)buf, (const char*)t_buf);
                    strcat((char*)buf, " ");
                    strcat((char*)buf, (const char*)res->test_data.un.acw.cur);
                    strcat((char*)buf, (const char*)unit_pool[ac_gear[res->test_data.un.acw.range].dec]);
                    
                    decs = ac_gear[res->test_data.un.acw.range].dec;
                    unit = ac_gear[res->test_data.un.acw.range].unit;
                    mysprintf(t_buf, unit_pool[unit], 50 + decs, res->test_data.un.acw.cur);    
                    strcat((char*)buf, (const char*)t_buf);
                    strcat((char*)buf, " ");
                    mysprintf(t_buf, unit_pool[TIM_U_s], 50 + 1, res->test_data.test_time);
                    strcat((char*)buf, (const char*)t_buf);
                    strcat((char*)buf, " ");
                    str = get_test_status_str(res->test_data.test_result);
                    strcat((char*)buf, (const char*)str);
                    break;
                case DCW:
                    break;
                case IR:
                    break;
                case GR:
                    break;
            }
            
            index = result_inf_index[i];
            set_text_ele(index, this_win, buf);
        }
        /* 无效数据 */
        else
        {
            index = result_inf_index[i];
            get_text_ele_text(index, this_win, buf, 200);
            sprintf((char*)t_buf, "%04d", result_count + i);
            set_text_ele(index, this_win, t_buf);
            
            update_com_text_ele(COM_UI_CUR_FILE_NAME, this_win, "");
            update_com_text_ele(COM_UI_CUR_STEP, this_win, "");
            update_com_text_ele(COM_UI_CUR_WORK_MODE, this_win, "");
            
            set_text_ele(RESULT_WIN_SETTING_PAR_C, this_win, "");
            set_text_ele(RESULT_WIN_TEST_DATA_C, this_win, "");
        }
    }
}

void update_cur_result_inf(uint32_t result_num)
{
    uint8_t num = 0;
    uint32_t max_result = 0;
    
    num = ARRAY_SIZE(result_inf_index);
    max_result = get_result_max_num();
    
    if(result_num == 0)
    {
        return;
    }
    
    if(result_num > max_result)
    {
        result_num = max_result;
    }
    
    l_cur_result = (result_num - 1) % num;
    l_result_count = result_num;
    
    win_task = RES_WIN_UPDATE;
}

/**
  * @brief  向上功能键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    uint8_t num = 0;
    
    num = ARRAY_SIZE(result_inf_index);
    l_cur_result = (l_cur_result + num - 1) % num;
    update_selete_result_opt();
    
    if(l_result_count > 1)
    {
        l_result_count--;
    }
    else
    {
        l_result_count = MAX_RESULT_NUM;
    }
    
    if((l_result_count - 1) % num == 3)
    {
        read_result_to_buf(l_result_count - 3);
        update_result_inf_dis(l_result_count - 3);
    }
    
    update_one_res_inf(&result_inf_pool[l_cur_result]);
}
static void update_selete_result_opt(void)
{
    TEXT_ELE_T *ele;
    CS_ERR err;
    
    dis_select_text_ele(g_cur_text_ele);
    ele = get_text_ele_inf(g_cur_win->text.pool,
            g_cur_win->text.pool_size, result_inf_index[l_cur_result], &err);
    
    if(err == CS_ERR_NONE)
    {
        g_cur_text_ele = ele;
    }
    
    select_text_ele(g_cur_text_ele);
}
/**
  * @brief  向下功能键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    uint8_t num = 0;
    
    num = ARRAY_SIZE(result_inf_index);
    l_cur_result = (l_cur_result + 1) % num;
    update_selete_result_opt();
    
    if(l_result_count < MAX_RESULT_NUM)
    {
        l_result_count++;
    }
    else
    {
        l_result_count = 1;
    }
    
    if((l_result_count - 1) % num == 0)
    {
        read_result_to_buf(l_result_count);
        update_result_inf_dis(l_result_count);
    }
    
    update_one_res_inf(&result_inf_pool[l_cur_result]);
}

/**
  * @brief  更新功能键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_resule_win_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(result_win_sys_key_pool, ARRAY_SIZE(result_win_sys_key_pool), hWin);
}
/**
  * @brief  更新功能键和菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_result_win_key_inf(WM_HWIN hWin)
{
    update_result_win_menu_key_inf(hWin);
    update_resule_win_sys_key_inf(hWin);
}

/**
  * @brief  更新菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_result_win_menu_key_inf(WM_HWIN hWin)
{
    init_menu_key_info(result_win_menu_key_info,
                    ARRAY_SIZE(result_win_menu_key_info), hWin);
}
/**
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
static void init_create_result_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
    update_group_inf(win);//更新记忆组信息
}

/**
  * @brief  窗口重绘
  * @param  无
  * @retval 无
  */
static void result_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

static void init_result_win_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_result_win_layout1_text_ele_pos(result_win_ele_pool);
            break;
    }
}

/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_result_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_result_win_text_ele_pos_inf();
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
//    update_result_win_text_ele_text(win);
}

void draw_result_win_frame(void)
{
    const GUI_RECT area0 =
    {
        0, 35, 800 - 110 - 20, 200
    };
    const GUI_RECT area1 =
    {
        0, 210, 260, 420
    };
    const GUI_RECT area2 =
    {
        260, 210, 800 - 110 - 20, 420
    };
    GUI_RECT area1_t;
    GUI_RECT area2_t;
    
	GUI_SetColor(GUI_WHITE);
	GUI_SetPenSize(2);
	myGUI_DrawRectEx(&area0);
	myGUI_DrawRectEx(&area1);
	myGUI_DrawRectEx(&area2);
    
    area1_t.x0 = area1.x0 + 2;
    area1_t.y0 = area1.y0 + 2;
    area1_t.x1 = area1.x1 - 1;
    area1_t.y1 = area1.y0 + 40;
    
    area2_t.x0 = area2.x0 + 2;
    area2_t.y0 = area2.y0 + 2;
    area2_t.x1 = area2.x1 - 1;
    area2_t.y1 = area2.y0 + 40;
    
    GUI_SetColor(GUI_GRAY);//GUI_LIGHTGRAY
    GUI_FillRectEx(&area1_t);
    GUI_FillRectEx(&area2_t);
}
static void result_win_task(void)
{
    switch(win_task)
    {
        case RES_WIN_IDLE:
            break;
        case RES_WIN_UPDATE:
            read_result_to_buf(l_result_count - l_cur_result);
            update_result_inf_dis(l_result_count - l_cur_result);
            update_selete_result_opt();
            update_one_res_inf(&result_inf_pool[l_cur_result]);
            win_task = RES_WIN_IDLE;
            break;
    }
}
/**
  * @brief  结果管理窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void result_win_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
    CS_ERR err;
    static void *p_mem;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			WM_SetFocus(hWin);/* 设置聚焦 */
            set_user_window_handle(hWin);
            create_miclogo_image(hWin, &p_mem);
			win = get_user_window_info(hWin);
            init_create_win_all_ele(win);
            update_result_win_key_inf(hWin);
            win_task = RES_WIN_UPDATE;
            l_cur_result = 0;
            l_result_count = 1;
            g_cur_text_ele = get_text_ele_inf(g_cur_win->text.pool,
                    g_cur_win->text.pool_size, result_inf_index[l_cur_result], &err);
            select_text_ele(g_cur_text_ele);
            result_win_timer_handle = WM_CreateTimer(hWin, 0, 100, 0);
            break;
		case WM_TIMER:
        {
            result_win_task();
			WM_RestartTimer(result_win_timer_handle, 10);
			break;
        }
		 case WM_KEY:
            break;
		case WM_PAINT:
			result_win_paint_frame();
            draw_result_win_frame();
            draw_group_inf_area();
			break;
		case WM_NOTIFY_PARENT:
			break;
		case WM_DELETE:
            free_ex_mem(p_mem);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建结果管理窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_result_win(int hWin)
{
    create_user_window(&result_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
