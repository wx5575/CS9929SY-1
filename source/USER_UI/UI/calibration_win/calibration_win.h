/**
  ******************************************************************************
  * @file    main_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   启动窗口
  ******************************************************************************
  */
#ifndef __CALIBRATION_WIN_H__
#define __CALIBRATION_WIN_H__

typedef enum{
    CAL_WIN_MODULE_ADDR,///<校准模块地址
    CAL_WIN_MODULE_ADDR_V,///<校准模块地址值
    CAL_WIN_MODULE_NUM,///<校准模块编号
    CAL_WIN_MODULE_NUM_V,///<校准模块编号值
    CAL_WIN_MODULE_PORT,///<校准模块串口号
    CAL_WIN_MODULE_PORT_V,///<校准模块串口号值
    CAL_WIN_MODULE_TOTAL,///<校准模块校准点总数
    CAL_WIN_MODULE_TOTAL_V,///<校准模块校准点总数值
}MODULE_TEXT_INDEX;

void update_cur_cal_point_inf(void);

#endif //__CALIBRATION_WIN_H__
