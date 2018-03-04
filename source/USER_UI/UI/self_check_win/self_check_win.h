/**
  ******************************************************************************
  * @file    self_check_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   自检窗口
  ******************************************************************************
  */
#ifndef __SELF_CHECK_WIN_H__
#define __SELF_CHECK_WIN_H__

enum{
    SELF_CHECK_UI_POWER,///<电源自检
    SELF_CHECK_UI_KEY ,///<键盘自检
    SELF_CHECK_UI_MEM ,///<存储器自检
    SELF_CHECK_UI_VOL ,///<电压自检
    SELF_CHECK_UI_CUR ,///<电流自检
    SELF_CHECK_UI_SYS ,///<系统参数自检
};

#endif //__SELF_CHECK_WIN_H__
