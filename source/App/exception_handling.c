/**
  ******************************************************************************
  * @file    exception_handling.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.8.9
  * @brief   异常处理
  ******************************************************************************
  */

#include "exception_handling.h"

typedef enum{
    EX_CODE_WIN ,///<弹窗类异常 会创建弹窗用来显示异常信息
    EX_CODE_COM ,///<串口类异常 把异常信息通过串口发送出去 不会创建弹窗
    EX_CODE_WIN_AND_COM ,///<弹窗 + 串口类异常
    EX_CODE_STATUS_BAR,///<状态栏类异常 会在状态栏的位置显示异常信息
    EX_CODE_STATUS_BAR_AND_COM,///<状态栏 + 串口类异常
    EX_CODE_END,
}EX_CODE_ENUM;

void throw_exception(uint8_t *ex_inf, EX_CODE_ENUM ex_code)
{
    if(ex_code >= EX_CODE_END)
    {
        return;
    }
    
    switch(ex_code)
    {
        case EX_CODE_WIN:
            break;
        case EX_CODE_COM:
            break;
        case EX_CODE_WIN_AND_COM:
            break;
        case EX_CODE_STATUS_BAR:
            break;
        case EX_CODE_STATUS_BAR_AND_COM:
            break;
    }
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
