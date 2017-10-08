/**
  ******************************************************************************
  * @file    sys_manage_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   系统管理界面
  ******************************************************************************
  */
#ifndef __LOGO_H__
#define __LOGO_H__

int create_logo_imagex(WM_HWIN hWin);
int create_logo_image(WM_HWIN hWin);
int create_slogo_image(WM_HWIN hWin);
int create_main_image(WM_HWIN hWin);
int create_www_qr_code_image(WM_HWIN hWin);
int create_wts_qr_code_image(WM_HWIN hWin);
int create_test_image(WM_HWIN hWin);
int create_miclogo_image(WM_HWIN hWin, void**mem);

#endif //__LOGO_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
