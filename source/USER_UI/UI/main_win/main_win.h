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
	MAIN_UI_COM_ST,///<主界面的通信状态
	MAIN_UI_SYS_TIME,///<主界面的系统时间
    MAIN_UI_CSALLWIN,///<主界面的公司名称
    MAIN_UI_VERSION,///<主界面的版本信息
    MAIN_UI_TYPE,///<主界面的型号
    MAIN_UI_WWW,///<主界面的公司网站
    MAIN_UI_WTS,///<主界面的微信技术服务
    MAIN_UI_ELE_NUM,///<主界面文本对象个数
}MAIN_UI_INDEX;

extern void update_usb_dis_status(void);
extern void set_main_win_progbar_value(int value);
extern void delete_main_win_progbar(void);
extern void set_main_win_progbar_show(void);
extern void main_ui_enter(void);

#endif //__MAIN_UI_COM_INFO_H__
