/**
  ******************************************************************************
  * @file    main_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   主窗口文件
  ******************************************************************************
  */
#ifndef __MAIN_UI_COM_INFO_H__
#define __MAIN_UI_COM_INFO_H__


typedef enum {
	STATUS_BAR_WIN_COM_ST,///<主界面的通信状态
	STATUS_BAR_WIN_SYS_TIME,///<主界面的系统时间
}MAIN_UI_INDEX;

extern void update_usb_dis_status(void);
extern void set_main_win_progbar_value(int value);
extern void delete_main_win_progbar(void);
extern void set_main_win_progbar_show(void);
extern void main_ui_enter(void);

#endif //__MAIN_UI_COM_INFO_H__
