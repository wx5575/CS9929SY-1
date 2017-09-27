/**
  ******************************************************************************
  * @file    start_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   启动窗口
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
#include "7_start_win.h"
#include "start_win.h"
#include "PROGBAR.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void start_win_cb(WM_MESSAGE * pMsg);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* start_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_start_windows,/*4.3寸屏*/
    &_7_start_windows,/*5.6寸屏*/
    &_7_start_windows,/*7寸屏*/
};

/**
  * @brief  启动窗口结构体初始化
  */
MYUSER_WINDOW_T start_windows=
{
    {"启动窗口", "Start Window"},
    start_win_cb,    NULL,
	{0},
    {0},
    {0},
    /* 自动布局 */
    {
        NULL,//文本自动布局信息池
        NULL,///<编辑对象自动布局信息池
        NULL,//文本对象调整布局信息池
        NULL,//编辑对象调整布局信息池
    },/* auto_layout */
    start_win_pos_size_pool,/*pos_size_pool */
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void start_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void start_win_cb(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
			set_user_window_handle(hWin);
            create_logo_image(hWin);
			break;
		case WM_PAINT:
			start_win_paint_frame();
			break;
		case WM_KEY:
			break;
		case WM_DELETE:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建启动窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_start_win(int hWin)
{
    create_user_window(&start_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
