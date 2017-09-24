/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : 本实验主要学习图形控件的使用。
*              在对话框上面创建了一个图形控件，每50ms更新一次数据。
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-07-16   Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
//#include "MainTask.h"
//#include "bsp.h"
#include "GUI.H"
#include "WM.h"
#include "GRAPH.h"
#include "FRAMEWIN.h"
/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
static GRAPH_SCALE_Handle hScaleV;     
static GRAPH_DATA_Handle  ahData;

/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_GRAPH_0    (GUI_ID_USER + 0x01)


/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
	{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 10, 10, 400, 240, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 对话框回调函数		
*	形    参: pMsg  回调参数 
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			
			//
			// 初始化
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
			FRAMEWIN_SetText(hItem, "armfly");
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

			//
			// 初始化Graph控件
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

			/* 创建数据对象 *******************************************************/
			ahData = GRAPH_DATA_YT_Create(GUI_GREEN, 400, 0, 0);

			/* 数据对象添加到图形控件 */
			GRAPH_AttachData(hItem, ahData);

			/* 设置Y轴方向的栅格间距 */
			GRAPH_SetGridDistY(hItem, 20);

			/* 固定X轴方向的栅格 */
			GRAPH_SetGridFixedX(hItem, 1);

			/* 设置栅格可见 */
			GRAPH_SetGridVis(hItem, 1);

			/* 创建刻度对象  ***************************************************/
			hScaleV = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);

			/* 将垂直刻度对象添加到图形控件 */
			GRAPH_AttachScale(hItem, hScaleV);

			/* 用于设置比例刻度的因子 */
			GRAPH_SCALE_SetFactor(hScaleV, 0.5);

			/* 设置标签字体颜色 */
			GRAPH_SCALE_SetTextColor(hScaleV, GUI_RED);

			/* 设置上下左右边界的大小 */
			GRAPH_SetBorder(hItem, 20, 10, 10, 10);
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateFramewin
*	功能说明: 创建对话框		
*	形    参: 无
*	返 回 值: 返回对话框句柄
*********************************************************************************************************
*/
WM_HWIN CreateFramewin(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{
	
	/* 初始化 */
//	GUI_Init();
	
	/*
	 关于多缓冲和窗口内存设备的设置说明
	   1. 使能多缓冲是调用的如下函数，用户要在LCDConf_Lin_Template.c文件中配置了多缓冲，调用此函数才有效：
		  WM_MULTIBUF_Enable(1);
	   2. 窗口使能使用内存设备是调用函数：WM_SetCreateFlags(WM_CF_MEMDEV);
	   3. 如果emWin的配置多缓冲和窗口内存设备都支持，二选一即可，且务必优先选择使用多缓冲，实际使用
		  STM32F429BIT6 + 32位SDRAM + RGB565/RGB888平台测试，多缓冲可以有效的降低窗口移动或者滑动时的撕裂
		  感，并有效的提高流畅性，通过使能窗口使用内存设备是做不到的。
	   4. 所有emWin例子默认是开启三缓冲。
	*/
	WM_MULTIBUF_Enable(1);
	
	/*
       触摸校准函数默认是注释掉的，电阻屏需要校准，电容屏无需校准。如果用户需要校准电阻屏的话，执行
	   此函数即可，会将触摸校准参数保存到EEPROM里面，以后系统上电会自动从EEPROM里面加载。
	*/
    //TOUCH_Calibration();
	
	/* 创建对话框 */
	CreateFramewin();
		
	while(1) 
	{
		/* 每50ms给绘图控件添加一次新的数据 */
		GRAPH_DATA_YT_AddValue(ahData, rand()%100);
		GUI_Delay(50);
	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
