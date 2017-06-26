/**
  ******************************************************************************
  * @file    sys_manage_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   系统管理界面
  ******************************************************************************
  */
#ifndef __PASSWORD_WIN_H__
#define __PASSWORD_WIN_H__


/**
  * @brief  密码输入界面的编辑对象
  */
typedef enum{
    INPUT_PWD,///<输入的PWD
}PWD_EDIT_INDEX;

extern void init_back_up_will_enter_win_inf(void (*fun)(int), int data);
void create_input_password_ui(int hWin);

#endif //__PASSWORD_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
