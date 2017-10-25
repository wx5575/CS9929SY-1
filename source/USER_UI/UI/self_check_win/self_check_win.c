/**
  ******************************************************************************
  * @file    self_check_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   自检窗口
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "FILE_SYS.H"
#include "image/user_image.h"
#include "image/logo.h"
#include "UI_COM/com_ui_info.h"
#include "rtc_config.h"
#include "7_self_check_win.h"
#include "self_check_win.h"
#include "PROGBAR.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void self_check_win_cb(WM_MESSAGE * pMsg);

static void init_create_self_check_win_text_ele(MYUSER_WINDOW_T* win);
/* Private variables ---------------------------------------------------------*/


static	WM_HWIN timer_handle;///<定时器句柄
static uint8_t self_check_count = 0;
/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* self_check_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_self_check_windows,/*4.3寸屏*/
    &_7_self_check_windows,/*5.6寸屏*/
    &_7_self_check_windows,/*7寸屏*/
};
/**
  * @brief  界面文本对象索引表
  */
CS_INDEX self_check_win_test_ele_index_pool[]=
{
    SELF_CHECK_UI_POWER,///<电源自检
    SELF_CHECK_UI_KEY ,///<键盘自检
    SELF_CHECK_UI_MEM ,///<存储器自检
    SELF_CHECK_UI_VOL ,///<电压自检
    SELF_CHECK_UI_CUR ,///<电流自检
    SELF_CHECK_UI_SYS ,///<系统参数自检
};
/**
  * @brief  界面的文本对象池
  */
static TEXT_ELE_T self_check_ui_text_ele_pool[]=
{
	{{"1.电源检查", "1.Power check"}, SELF_CHECK_UI_POWER },
	{{"2.键盘检查", "2.Key check"}, SELF_CHECK_UI_KEY },
	{{"3.存储检查", "3.Memory check"}, SELF_CHECK_UI_MEM },
	{{"4.电压检查", "4.Voltage check"}, SELF_CHECK_UI_VOL },
	{{"5.电流检查", "5.Current check"}, SELF_CHECK_UI_CUR },
	{{"6.系统参数检查", "6.Sys.Par.check"}, SELF_CHECK_UI_SYS },
};
/**
  * @brief  自检窗口结构体初始化
  */
MYUSER_WINDOW_T self_check_windows=
{
    {"自检窗口", "Self check Window"},
    self_check_win_cb, NULL,
	{
        self_check_ui_text_ele_pool,COUNT_ARRAY_SIZE(self_check_ui_text_ele_pool),
        (CS_INDEX*)self_check_win_test_ele_index_pool,
        COUNT_ARRAY_SIZE(self_check_win_test_ele_index_pool),
        init_create_self_check_win_text_ele
    },/* text */
    {0},
    {0},
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    self_check_win_pos_size_pool,/*pos_size_pool */
};

/**
  * @brief  指向自检窗口的this指针
  */
static MYUSER_WINDOW_T *this_win = &self_check_windows;

/* Private functions ---------------------------------------------------------*/

static void init_self_check_win_text_ele_pos_inf(void)
{
    switch(SCREEM_SIZE)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_init_self_check_win_layout1_text_ele_pos(self_check_ui_text_ele_pool);
            break;
    }
}
/**
  * @brief  初始化并创建步骤编辑窗口中的文本控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_self_check_win_text_ele(MYUSER_WINDOW_T* win)
{
    init_self_check_win_text_ele_pos_inf();
    init_window_text_ele_list(win);//初始化窗口文本对象链表
    init_window_text_ele(win);
//    update_result_win_text_ele_text(win);
}

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void self_check_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  自检服务函数
  * @param  无
  * @retval 无
  */
void self_check_server(void)
{
    uint8_t num = 0;
    CS_INDEX index;
    uint8_t buf[201] = {0};
    static uint8_t flag = 0;
    uint8_t len = 0;
    int32_t i = 0;
    
    num = ARRAY_SIZE(self_check_win_test_ele_index_pool);
    
    if(self_check_count < num)
    {
        index = self_check_win_test_ele_index_pool[self_check_count];
        get_text_ele_text(index, this_win, buf, 200);
        
        if(flag == 0)
        {
            set_text_ele_visible(index, this_win, 1);
            flag = 1;
        }
        else if(flag == 1)
        {
            len = strlen((const char*)buf);
            
            if(len < 18)
            {
                for(i = 0; i < 18 - len; i++)
                {
                    strcat((char*)buf, ".");
                }
            }
            
            strcat((char*)buf, "............................");
            flag = 2;
        }
        else
        {
            get_text_ele_text(index, this_win, buf, 200);
            strcat((char*)buf, " OK");
            flag = 0;
            self_check_count++;
        }
        
        update_text_ele(index, this_win, buf);
    }
    else
    {
        self_check_ok = 1;
    }
}

/**
  * @brief  隐藏窗口
  * @param  无
  * @retval 无
  */
void hide_all_text_ele(void)
{
    uint8_t num = 0;
    CS_INDEX index;
    int32_t i = 0;
    
    num = ARRAY_SIZE(self_check_win_test_ele_index_pool);
    
    for(i = 0; i < num; i++)
    {
        index = self_check_win_test_ele_index_pool[i];
        set_text_ele_visible(index, this_win, 0);
    }
}
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void self_check_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	static CS_IMAGE_T cs_image;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
            init_create_win_all_ele(win);
			WM_SetFocus(hWin);/* 设置聚焦 */
            create_logo_imagex(hWin, &cs_image);
			timer_handle = WM_CreateTimer(hWin, 0, 300, 0);
            self_check_count = 0;
            self_check_ok = 0;
            
            hide_all_text_ele();
			break;
		case WM_PAINT:
			self_check_win_paint_frame();
			break;
		case WM_KEY:
			break;
		case WM_TIMER:
		{
            self_check_server();
            WM_RestartTimer(timer_handle, 300);
			break;
        }
		case WM_DELETE:
            delete_image(&cs_image);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  删除自检窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void delete_self_check_win(void)
{
    if(self_check_windows.handle != 0)
    {
        del_user_window(self_check_windows.handle);
    }
}
/**
  * @brief  创建自检窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_self_check_win(int hWin)
{
    create_user_window(&self_check_windows, &windows_list, hWin);//创建文件管理界面
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
