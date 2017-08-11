/**
  ******************************************************************************
  * @file    7_calibration_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   7寸屏启动窗口的布局
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "IMAGE.H"
#include "ui_com/com_ui_info.h"
#include "7_calibration_win.h"
#include "calibration_win.h"
#include "string.h"
#include "stdio.h"
#include "LISTVIEW.H"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SIZE_7INCH_WIDTH	800
#define SIZE_7INCH_HEIGH	480
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  初始化7寸屏中主界面位置尺寸信息
  */
WIDGET_POS_SIZE_T _7_calibration_windows=
{
	0/*x*/, 0/*y*/, SIZE_7INCH_WIDTH/*width*/, SIZE_7INCH_HEIGH/*height*/,
};

#define MODULE_BASE_T_X      5
#define MODULE_BASE_T_Y      0
#define MODULE_BASE_T_W      120
/** 
  * @brief 7寸屏校准管理界面文本对象自动布局结构
  */
TEXT_ELE_AUTO_LAYOUT_T _7_calibration_text_ele_auto_layout_inf=
{
   MODULE_BASE_T_X/*base_x*/,MODULE_BASE_T_Y/*base_y*/,MODULE_BASE_T_W/*width*/,30/*height*/,
    1/*rows*/,8/*column*/,50/*row_spacing*/,MODULE_BASE_T_W/*column_spacing*/,
    {&GUI_Fonthz_24}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_VCENTER,20
};

static ADJUST_TEXT_ELE_LAYOUT _7_calibration_win_text_ele_adjust_layout[]=
{
    {
        CAL_WIN_MODULE_ADDR, {GUI_TA_RIGHT | GUI_TA_VCENTER, CS_TRUE},
        {
            {0, CS_FALSE},/*base_x*/
            {40, CS_FALSE},/*offset_x*/
            {80, CS_FALSE},/*width*/
        },
    },
    {
        CAL_WIN_MODULE_ADDR_V, {GUI_TA_RIGHT | GUI_TA_VCENTER, CS_FALSE},
        {
            {0, CS_FALSE},/*base_x*/
            {40, CS_FALSE},/*offset_x*/
            {80, CS_TRUE},/*width*/
        },
    },
    {
        CAL_WIN_MODULE_NUM, {GUI_TA_RIGHT | GUI_TA_VCENTER, CS_TRUE},
        {
            {0, CS_FALSE},/*base_x*/
            {40, CS_FALSE},/*offset_x*/
            {80, CS_FALSE},/*width*/
        },
    },
    {
        CAL_WIN_MODULE_NUM_V, {GUI_TA_RIGHT | GUI_TA_VCENTER, CS_FALSE},
        {
            {0, CS_FALSE},/*base_x*/
            {40, CS_FALSE},/*offset_x*/
            {80, CS_TRUE},/*width*/
        },
    },
    {
        CAL_WIN_MODULE_PORT, {GUI_TA_RIGHT | GUI_TA_VCENTER, CS_TRUE},
        {
            {0, CS_FALSE},/*base_x*/
            {5, CS_FALSE},/*offset_x*/
            {0, CS_FALSE},/*width*/
        },
    },
};
/** 
  * @brief 7寸屏模块管理界面文本对象调整布局信息
  */
ADJUST_TEXT_ELE_LAYOUT_INF _7_calibration_win_adjust_text_ele_layout_inf=
{
    _7_calibration_win_text_ele_adjust_layout,
    ARRAY_SIZE(_7_calibration_win_text_ele_adjust_layout)
};
/** 
  * @brief 7寸屏创建模块管理列表
  */
WM_HWIN _7_create_calibration_listview(WM_HWIN hWin)
{
    LISTVIEW_Handle list_h;
	HEADER_Handle hHeader;
	SCROLLBAR_Handle hScrollbar;
	int32_t i = 0;
	uint8_t buf[20] = {0};
    
	list_h = LISTVIEW_CreateEx(0, 30, 690, 430, hWin, WM_CF_MEMDEV_ON_REDRAW | WM_CF_SHOW, 0, id_base++);
	hScrollbar = SCROLLBAR_CreateAttached(list_h, SCROLLBAR_CF_VERTICAL);
	
	LISTVIEW_SetFont(list_h, &GUI_Fonthz_24);
	
	hHeader = LISTVIEW_GetHeader(list_h);
	HEADER_SetFont(hHeader,&GUI_Fonthz_24);
	HEADER_SetHeight(hHeader,28);
	LISTVIEW_AddColumn(list_h, 60	, SELE_STR("编号","NO.") , GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 130	, SELE_STR("校准模式","mode") , GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 120	, SELE_STR("校准点","Cal.Point") , GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 100	, SELE_STR("DA值","DA") , GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 100	, SELE_STR("AD值","AD") , GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 170	, SELE_STR("测量值","Measured") , GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    SCROLLBAR_SetWidth(hScrollbar,18);
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_THUMB, GUI_BLACK);//缩略图0x00A0A0A0
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_SHAFT, GUI_WHITE);  //轴
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_ARROW, GUI_GRAY);  //箭头
	WIDGET_SetEffect(hScrollbar, &WIDGET_Effect_None);
	
	for(i = 0; i < 60; i++)
	{
		LISTVIEW_AddRow(list_h, 0);
		sprintf((char *)buf, "%02d", i + 1);
		LISTVIEW_SetItemText(list_h, 0,i, (const char*)buf);
	}
    
	LISTVIEW_SetSel(list_h, 0);
    LISTVIEW_SetGridVis(list_h, 1);
	LISTVIEW_SetBkColor(list_h, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
	LISTVIEW_SetRowHeight(list_h, 30);
    
    return list_h;
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
