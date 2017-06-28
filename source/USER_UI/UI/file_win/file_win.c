/**
  ******************************************************************************
  * @file    file_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   文件管理界面
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ff.h"
#include "OS.H"
#include "cs99xx_struct.h"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "file_edit_win.h"
#include "cs99xx_mem_api.h"
#include "UI_COM/com_ui_info.h"
#include "KEY_MENU_WIN/key_menu_win.h"
#include "WARNING_WIN/warning_win.h"
#include "7_file_win.h"
#include "file_win.h"
#include "type/cs99xx_type.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define WINDOWS_BAK_COLOR	GUI_BLUE	//窗口背景色

/* Private function prototypes -----------------------------------------------*/

static void into_save_file_dialog(WM_HMEM hWin);
static void file_win_cb(WM_MESSAGE* pMsg);
static void file_win_update_fun_key_inf(WM_HMEM hWin);

static void file_exist_f1_cb(KEY_MESSAGE *key_msg);
static void file_exist_f2_cb(KEY_MESSAGE *key_msg);
static void file_exist_f3_cb(KEY_MESSAGE *key_msg);
static void file_exist_f4_cb(KEY_MESSAGE *key_msg);
static void file_exist_f5_cb(KEY_MESSAGE *key_msg);
static void file_exist_f6_cb(KEY_MESSAGE *key_msg);

static void file_no_exist_f1_cb(KEY_MESSAGE *key_msg);
static void file_no_exist_f2_cb(KEY_MESSAGE *key_msg);
static void file_no_exist_f3_cb(KEY_MESSAGE *key_msg);
static void file_no_exist_f4_cb(KEY_MESSAGE *key_msg);
static void file_no_exist_f5_cb(KEY_MESSAGE *key_msg);
static void file_no_exist_f6_cb(KEY_MESSAGE *key_msg);

static void file_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void file_win_direct_key_down_cb(KEY_MESSAGE *key_msg);

static void update_cur_row_menu_key_st(WM_HWIN hWin);

static void pop_warning_dialog_for_read_file(WM_HWIN hWin);
static void pop_warning_dialog_for_del_file(WM_HWIN hWin);
static void pop_warning_dialog_for_clear_file(WM_HWIN hWin);

static void unprominent_dis_cur_file_in_listview(void);
static void prominent_dis_cur_file_in_listview(void);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  文件管理列表句柄
  */
static LISTVIEW_Handle   file_list_handle;
/**
  * @brief  文件管理窗口位置尺寸信息，根据屏幕尺寸来进行配置
  */
static WIDGET_POS_SIZE_T* file_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_file_windows,/*4.3寸屏*/
    &_7_file_windows,/*5.6寸屏*/
    &_7_file_windows,/*7寸屏*/
};
/**
  * @brief  文件管理窗口数据结构定义
  */
MYUSER_WINDOW_T file_window=
{
    {"File_window"},
    file_win_cb,file_win_update_fun_key_inf,
	{0},/*text*/
    {0},/*edit*/
    {0},/*com*/
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,//编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    file_win_pos_size_pool/*pos_size_pool*/
};
/**
  * @brief  文件存在的按键菜单
  */
static MENU_KEY_INFO_T 	file_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP, file_exist_f1_cb },//f1
    {"", F_KEY_READ		, KEY_F2 & _KEY_UP, file_exist_f2_cb },//f2
    {"", F_KEY_EDIT		, KEY_F3 & _KEY_UP, file_exist_f3_cb },//f3
    {"", F_KEY_DEL		, KEY_F4 & _KEY_UP, file_exist_f4_cb },//f4
    {"", F_KEY_CLEAR    , KEY_F5 & _KEY_UP, file_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, file_exist_f6_cb },//f6
};

/**
  * @brief  文件不存在的按键菜单
  */
static MENU_KEY_INFO_T 	file_no_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP, file_no_exist_f1_cb },//f1
    {"", F_KEY_NEW		, KEY_F2 & _KEY_UP, file_no_exist_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, file_no_exist_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, file_no_exist_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP, file_no_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, file_no_exist_f6_cb },//f6
};
/**
  * @brief  文件不存在的按键菜单
  */
static FUNCTION_KEY_INFO_T 	file_win_sys_key_pool[]={
	{KEY_UP		, file_win_direct_key_up_cb		},
	{KEY_DOWN	, file_win_direct_key_down_cb 	},
	{CODE_LEFT	, file_win_direct_key_down_cb   },
	{CODE_RIGH	, file_win_direct_key_up_cb	    },
};
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  文件存在菜单键f1的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_exist_f1_cb(KEY_MESSAGE *key_msg)
{
    memcpy(&global_file, g_cur_file, sizeof(global_file));
    into_save_file_dialog(key_msg->user_data);
}
/**
  * @brief  文件存在菜单键f2的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_exist_f2_cb(KEY_MESSAGE *key_msg)
{
    pop_warning_dialog_for_read_file(key_msg->user_data);
}
/**
  * @brief  文件存在菜单键f3的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_exist_f3_cb(KEY_MESSAGE *key_msg)
{
    TEST_FILE* f;
    int row = 0;
    CS_ERR err;
    
	row = LISTVIEW_GetSel(file_list_handle);
	
	/* 文件不存在 */
	if(CS_TRUE != is_file_exist(row + 1))
	{
        return;
    }
    
	/* 文件存在 */
    f = get_file_inf(row + 1, &err);
    
    if(err == CS_ERR_NONE)
    {
        memcpy(&global_file, f, sizeof(global_file));
    }
    else
    {
        return;
    }
    
    create_edit_file_dialog(key_msg->user_data);//创建编辑文件对话框
}
/**
  * @brief  文件存在菜单键f4的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_exist_f4_cb(KEY_MESSAGE *key_msg)
{
    //删除文件
    TEST_FILE* f;
    int row = 0;
    CS_ERR err;
    
	row = LISTVIEW_GetSel(file_list_handle);
	
	/* 文件不存在 */
	if(CS_TRUE != is_file_exist(row + 1))
	{
        return;
    }
    
	/* 文件存在 */
    f = get_file_inf(row + 1, &err);
    
    if(err == CS_ERR_NONE)
    {
        memcpy(&global_file, f, sizeof(global_file));
    }
    else
    {
        return;
    }
    
    pop_warning_dialog_for_del_file(key_msg->user_data);
}
/**
  * @brief  文件存在菜单键f5的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_exist_f5_cb(KEY_MESSAGE *key_msg)
{
    pop_warning_dialog_for_clear_file(key_msg->user_data);
}
/**
  * @brief  文件存在菜单键f6的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  文件不存在菜单键f1的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_no_exist_f1_cb(KEY_MESSAGE *key_msg)
{
    memcpy(&global_file, g_cur_file, sizeof(global_file));
    create_save_file_dialog(key_msg->user_data);
}
/**
  * @brief  文件不存在菜单键f2的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_no_exist_f2_cb(KEY_MESSAGE *key_msg)
{
    init_file_data(&global_file, 0);//初始化文件数据
    create_new_file_dialog(key_msg->user_data);//创建新建文件对话框
}
/**
  * @brief  文件不存在菜单键f3的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_no_exist_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  文件不存在菜单键f4的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_no_exist_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  文件不存在菜单键f5的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_no_exist_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  文件不存在菜单键f6的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_no_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  向上方向键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_DecSel(file_list_handle);
    update_cur_row_menu_key_st(key_msg->user_data);
}
/**
  * @brief  向下方向键回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void file_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_IncSel(file_list_handle);
    update_cur_row_menu_key_st(key_msg->user_data);
}

/**
  * @brief  读取文件时弹出的警告框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void pop_warning_dialog_for_read_file(WM_HWIN hWin)
{
    const uint16_t WAR_WIN_TX = 10;//警告文本的X坐标
    const uint16_t WAR_WIN_TY = 20;//警告文本的Y坐标
    const uint16_t WAR_WIN_W = 400;//警告对话框的宽度
    const uint16_t WAR_WIN_H = 200;//警告对话框的高度
    const uint16_t WAR_WIN_X = 150;//警告对话框在父窗口中的X坐标
    const uint16_t WAR_WIN_Y = 50;//警告对话框在父窗口中的Y坐标
    
    WARNING_INF w_inf =
    {
        /* 标题 */
        {{"警告","Warning"}, 0 },
        /* 内容 */
        {
            {"是否读取该文件?",
            "Do you want to read this file?"
            }, 2,
            0/*base_x*/,0/*base_y*/,
            {WAR_WIN_TX,WAR_WIN_TY,WAR_WIN_W - 20,WAR_WIN_H - (WAR_WIN_TY + 10)},/*pos_size*/
            100/*max_len*/,
            {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER
        },
        {WAR_WIN_X, WAR_WIN_Y, WAR_WIN_W, WAR_WIN_H},/*win_pos_size*/
        0,/*dly_auto_close xx ms后自动关闭 0表示不自动关闭*/
    };
    
    set_custom_msg_id(CM_FILE_UI_READ);
    set_warning_ui_inf(&w_inf);
    create_warning_dialog(hWin);
}
/**
  * @brief  在删除文件时弹出的警告框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void pop_warning_dialog_for_del_file(WM_HWIN hWin)
{
    const uint16_t WAR_WIN_TX = 10;//警告文本的X坐标
    const uint16_t WAR_WIN_TY = 20;//警告文本的Y坐标
    const uint16_t WAR_WIN_W = 400;//警告对话框的宽度
    const uint16_t WAR_WIN_H = 200;//警告对话框的高度
    const uint16_t WAR_WIN_X = 150;//警告对话框在父窗口中的X坐标
    const uint16_t WAR_WIN_Y = 50;//警告对话框在父窗口中的Y坐标
    
    WARNING_INF w_inf =
    {
        /* 标题 */
        {{"警告","Warning"}, 0 },
        /* 内容 */
        {
            {"该操作会删除文件数据.\n\n确定要继续吗?\n",
            "This will delete the file data.\n\n"
            "Do you want to continue?"}, 2,
            0/*base_x*/,0/*base_y*/,
            {WAR_WIN_TX,WAR_WIN_TY,WAR_WIN_W - 20,WAR_WIN_H - (WAR_WIN_TY + 10)},/*pos_size*/
            100/*max_len*/,
            {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER
        },
        {WAR_WIN_X, WAR_WIN_Y, WAR_WIN_W, WAR_WIN_H},/*win_pos_size*/
        0,/*dly_auto_close xx ms后自动关闭 0表示不自动关闭*/
    };
    
    set_custom_msg_id(CM_FILE_UI_DEL);
    set_warning_ui_inf(&w_inf);
    create_warning_dialog(hWin);
}
/**
  * @brief  在删除所有文件时弹出的警告框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void pop_warning_dialog_for_clear_file(WM_HWIN hWin)
{
    const uint16_t WAR_WIN_TX = 10;//警告文本的X坐标
    const uint16_t WAR_WIN_TY = 20;//警告文本的Y坐标
    const uint16_t WAR_WIN_W = 400;//警告对话框的宽度
    const uint16_t WAR_WIN_H = 200;//警告对话框的高度
    const uint16_t WAR_WIN_X = 150;//警告对话框在父窗口中的X坐标
    const uint16_t WAR_WIN_Y = 50;//警告对话框在父窗口中的Y坐标
    
    WARNING_INF w_inf =
    {
        /* 标题 */
        {{"警告","Warning"}, 0 },
        /* 内容 */
        {
            {"该操作会删除所有文件数据.\n\n确定要继续吗?\n",
            "This will delete all files.\n\n"
            "Do you want to continue?"}, 2,
            0/*base_x*/,0/*base_y*/,
            {WAR_WIN_TX,WAR_WIN_TY,WAR_WIN_W - 20,WAR_WIN_H - (WAR_WIN_TY + 10)},/*pos_size*/
            100/*max_len*/,
            {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER
        },
        {WAR_WIN_X, WAR_WIN_Y, WAR_WIN_W, WAR_WIN_H},/*win_pos_size*/
        0,/*dly_auto_close xx ms后自动关闭 0表示不自动关闭*/
    };
    
    set_custom_msg_id(CM_FILE_UI_CLEAR);
    set_warning_ui_inf(&w_inf);
    create_warning_dialog(hWin);
}
/**
  * @brief  在保存文件时弹出的警告框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void pop_warning_dialog_for_save_file(WM_HWIN hWin)
{
    const uint16_t WAR_WIN_TX = 10;//警告文本的X坐标
    const uint16_t WAR_WIN_TY = 20;//警告文本的Y坐标
    const uint16_t WAR_WIN_W = 400;//警告对话框的宽度
    const uint16_t WAR_WIN_H = 200;//警告对话框的高度
    const uint16_t WAR_WIN_X = 150;//警告对话框在父窗口中的X坐标
    const uint16_t WAR_WIN_Y = 50;//警告对话框在父窗口中的Y坐标
    
    WARNING_INF w_inf =
    {
        /* 标题 */
        {{"警告","Warning"}, 0 },
        /* 内容 */
        {
            {"该操作会覆盖文件数据.\n\n确定要继续吗?\n",
            "This will overwrite the file data.\n\n"
            "Do you want to continue?"}, 2,
            0/*base_x*/,0/*base_y*/,
            {WAR_WIN_TX,WAR_WIN_TY,WAR_WIN_W - 20,WAR_WIN_H - (WAR_WIN_TY + 10)},/*pos_size*/
            100/*max_len*/,
            {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER
        },
        {WAR_WIN_X, WAR_WIN_Y, WAR_WIN_W, WAR_WIN_H},/*win_pos_size*/
        0,/*dly_auto_close xx ms后自动关闭 0表示不自动关闭*/
    };
    
    set_custom_msg_id(CM_WARNING_INTO_SAVE_FILE);
    set_warning_ui_inf(&w_inf);
    create_warning_dialog(hWin);
}
/**
  * @brief  进入保存文件的对话框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void into_save_file_dialog(WM_HWIN hWin)
{
    int row = 0;
    
	row = LISTVIEW_GetSel(file_list_handle);
	
	/* 文件存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        pop_warning_dialog_for_save_file(hWin);
    }
	/* 文件不存在 */
	else
	{
        create_save_file_dialog(hWin);
    }
}
/**
  * @brief  更新当前行的菜单键，对于文件存在与否会有不同的菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_cur_row_menu_key_st(WM_HWIN hWin)
{
    int row = 0;
    uint8_t size = 0;
    MENU_KEY_INFO_T *info = NULL;
    
	row = LISTVIEW_GetSel(file_list_handle);
	
	/* 文件存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        size = ARRAY_SIZE(file_exist_menu_key_info);
        info = file_exist_menu_key_info;
	}
	/* 文件不存在 */
	else
	{
        size = ARRAY_SIZE(file_no_exist_menu_key_info);
        info = file_no_exist_menu_key_info;
	}
    
	init_menu_key_info(info, size, hWin);//刷新菜单键显示
}

/**
  * @brief  更新功能键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void file_win_update_fun_key_inf(WM_HMEM hWin)
{
    update_cur_row_menu_key_st(hWin);
}

/**
  * @brief  文件窗口重绘函数
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void file_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  显示出一个文件的信息到文件列表
  * @param  [in] file 文件信息
  * @retval 无
  */
static void dis_one_file_info(TEST_FILE *file)
{
	uint8_t list_buf[5][20] = {0};
	int32_t i = 0;
	uint16_t row = 0;
	
	/* 文件存在 */
	if(CS_TRUE == is_file_exist(file->num))
	{
        i = 1;
        sprintf((char *)list_buf[i++], "%s", file->name);
        sprintf((char *)list_buf[i++], "%s", work_mode_pool[file->work_mode%2]);
        sprintf((char *)list_buf[i++], "%d", file->total);
        sprintf((char *)list_buf[i++], "%s", file->date);//get_time_str(0));
    }
    /* 文件不存在 */
    else
    {
        for(i = 1; i < 5; i++)
        {
            strcpy((char *)list_buf[i], "");
        }
    }
    
	if(file->num >= 1)
	{
		row = file->num - 1;
	}
	
    /* 数据更新到listview控件上 */
	for(i = 1; i < 5; i++)
	{
		LISTVIEW_SetItemText(file_list_handle, i, row, (const char*)list_buf[i]);
	}
}

/**
  * @brief  更新文件列表的内容
  * @param  无
  * @retval 无
  */
static void update_file_dis(void)
{
	int32_t i = 0;
	
	for(i = 1; i < MAX_FILES; i++)
	{
		file_pool[i].num = i;
		dis_one_file_info(&file_pool[i]);
	}
}

/**
  * @brief  创建文件listview控件
  * @param  [in] hWin 窗口句柄
  * @retval 窗口句柄
  */
static WM_HWIN create_file_listview(WM_HWIN hWin)
{
    WM_HWIN handle = 0;
    
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            handle = _7_create_file_listview(hWin);
            break;
    }
    
    return handle;
}

/**
  * @brief  凸显当前文件
  * @param  无
  * @retval 无
  */
static void prominent_dis_cur_file_in_listview(void)
{
    uint8_t row = 0;
    int32_t i = 0;
    
    if(g_cur_file->num > 0)
    {
        row = g_cur_file->num - 1;
        
        for(i = 0; i < 5; i++)
        {
            LISTVIEW_SetItemTextColor(file_list_handle, i, row, LISTVIEW_CI_UNSEL, GUI_RED);
            LISTVIEW_SetItemTextColor(file_list_handle, i, row, LISTVIEW_CI_SEL, GUI_RED);
        }
    }
}

/**
  * @brief  反凸显当前文件
  * @param  无
  * @retval 无
  */
static void unprominent_dis_cur_file_in_listview(void)
{
    uint8_t row = 0;
    int32_t i = 0;
    GUI_COLOR col_1;
    GUI_COLOR col_2;
    
    if(g_cur_file->num > 0)
    {
        row = g_cur_file->num - 1;
        
        col_1 = LISTVIEW_GetTextColor(file_list_handle, LISTVIEW_CI_UNSEL);
        col_2 = LISTVIEW_GetTextColor(file_list_handle, LISTVIEW_CI_SEL);
        
        for(i = 0; i < 5; i++)
        {
            LISTVIEW_SetItemTextColor(file_list_handle, i, row, LISTVIEW_CI_UNSEL, col_1);
            LISTVIEW_SetItemTextColor(file_list_handle, i, row, LISTVIEW_CI_SEL, col_2);
        }
    }
}

/**
  * @brief  创建并初始化文件listview控件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void create_init_file_listview(WM_HWIN hWin)
{
    file_list_handle = create_file_listview(hWin);
	update_file_dis();
    prominent_dis_cur_file_in_listview();
}


/**
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void file_win_update_key_inf(WM_HWIN hWin)
{
    file_win_update_fun_key_inf(hWin);
    register_system_key_fun(file_win_sys_key_pool, ARRAY_SIZE(file_win_sys_key_pool), hWin);
}

static void init_group_inf(TEST_FILE *fs, TEST_FILE *fn)
{
    FILE_NUM file_num;
    TEST_FILE *file_bak;
    uint8_t mode = 0;
    
    file_bak = g_cur_file;
    
    g_cur_file = fs;
    file_num = g_cur_file->num;
    del_one_group_inf(file_num);
    
    if(fn->work_mode == N_MODE)
    {
        mode = get_first_mode();
    }
    else
    {
        mode = get_first_g_mode();
        if(mode == NUL)
        {
            return;
        }
    }
    
    insert_step(0, mode);
    save_group_info(file_num);//保存新建文件的记忆组信息
    g_cur_file = file_bak;
    read_group_info(g_cur_file->num);
}
/**
  * @brief  处理子窗口消息
  * @param  [in] msg 定制用户消息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void dispose_child_win_msg(CUSTOM_MSG_T * msg, WM_HWIN hWin)
{
    uint8_t flag = 0;
    
	switch(msg->id)
	{
        case CM_WARNING_INTO_SAVE_FILE:
        {
            /* 警告对话框点击了确定后就创建存贮文件对话框 */
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
                create_save_file_dialog(hWin);
                flag = 1;
            }
            /* 警告对话框点击了取消就返回到文件管理窗口，并更新一下按键信息 */
            
            break;
        }
		case CM_FILE_UI_SAVE://存贮文件界面
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
				TEST_FILE *f;
				
				f = (TEST_FILE*)msg->user_data;
				
				row = LISTVIEW_GetSel(file_list_handle);
				
				f->num = row + 1;
                
				if(f->num < MAX_FILES)
				{
					strcpy((char *)f->date, (const char*)get_time_str(0));
					file_pool[f->num] = *f;
                    copy_cur_file_to_new_pos(f->num);//拷贝当前文件到指定位置
				}
                
                update_file_dis();
			}
			break;
        }
		case CM_FILE_UI_NEW:
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
				TEST_FILE *f;
				
				f = (TEST_FILE*)msg->user_data;
				
				row = LISTVIEW_GetSel(file_list_handle);
				
				f->num = row + 1;
				if(f->num < MAX_FILES)
				{
					strcpy((char *)f->date, (const char*)get_time_str(0));
                    init_new_group_inf(f);//初始化记忆组
					file_pool[f->num] = *f;
                    save_file(f->num);
				}
                
                update_file_dis();
			}
			break;
        }
		case CM_FILE_UI_READ://文件读取
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
                CS_ERR err;
				TEST_FILE *f;
				
				row = LISTVIEW_GetSel(file_list_handle);
				
                f = get_file_inf(row + 1, &err);
                
                if(err == CS_ERR_NONE)
                {
                    unprominent_dis_cur_file_in_listview();//反凸显当前文件
                    g_cur_file = f;
                    prominent_dis_cur_file_in_listview();//凸显当前文件
                    read_group_info(g_cur_file->num);//保存新建文件的记忆组信息
                    sys_flag.last_file_num = g_cur_file->num;//更新最近使用的文件编号
                    save_sys_flag();//保存系统标记
                    read_group_info(g_cur_file->num);//恢复最近使用的记忆组信息
                }
			}
			break;
        }
		case CM_FILE_UI_EDIT://文件编辑
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
				TEST_FILE *fn;
                TEST_FILE *fs;
                CS_ERR err;
				
				fn = (TEST_FILE*)msg->user_data;
				
				row = LISTVIEW_GetSel(file_list_handle);
				
				fn->num = row + 1;
                fs = get_file_inf(fn->num, &err);
                
                if(fn->work_mode != fs->work_mode)
                {
                    init_group_inf(fs, fn);
                    fn->total = fs->total;
                }
                
                strcpy((char *)fn->date, (const char*)get_time_str(0));
                file_pool[fn->num] = *fn;
                save_file(fn->num);
                
                update_file_dis();
			}
			break;
        }
		case CM_FILE_UI_DEL://文件删除
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
				TEST_FILE *f;
                CS_ERR err;
                
				row = LISTVIEW_GetSel(file_list_handle);
				
                del_one_group_inf(row + 1);//删除一个记组信息
                update_file_dis();
                
                if(row + 1 == g_cur_file->num)
                {
                    f = get_file_inf(0, &err);
                    
                    if(err == CS_ERR_NONE)
                    {
                        g_cur_file = f;
                        read_group_info(g_cur_file->num);//保存新建文件的记忆组信息
                        sys_flag.last_file_num = g_cur_file->num;//更新最近使用的文件编号
                        save_sys_flag();//保存系统标记
                        read_group_info(g_cur_file->num);//恢复最近使用的记忆组信息
                    }
                }
			}
			break;
        }
		case CM_FILE_UI_CLEAR://文件删除
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
                del_all_file();
                update_file_dis();
			}
			break;
        }
	}
    
    if(0 == flag)
    {
        file_win_update_key_inf(hWin);
    }
}

/**
  * @brief  文件管理窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void file_win_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
    static CUSTOM_MSG_T msg;
	WM_HWIN hWin = pMsg->hWin;
    
	switch (pMsg->MsgId)
	{
		/*自定义消息 当子窗口被关闭后 收到由子窗口发给父窗口的消息 */
		case CM_CHILD_W_MSG:
		{
			msg = *(CUSTOM_MSG_T*)pMsg->Data.v;//拷贝消息
//             WM_SendMessageNoPara(hWin, CM_W_DIS_MSG);
            
			dispose_child_win_msg(&msg, hWin);
			break;
		}
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
			create_init_file_listview(hWin);//创建并初始化文件表控件
            init_create_win_all_ele(win);//初始化并创建窗口中所有控件
            file_win_update_key_inf(hWin);
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			file_win_paint_frame();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建文件管理窗口
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_file_win(int hWin)
{
    create_user_window(&file_window, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
