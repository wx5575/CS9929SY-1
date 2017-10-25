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

#include "IMAGE.H"
typedef struct{
    WM_HWIN handle;///<图片句柄
    void *buf;///<图片缓冲区
}CS_IMAGE_T;

void delete_image(CS_IMAGE_T* image);
IMAGE_Handle create_logo_imagex(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_logo_image(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_slogo_image(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_main_image(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_www_qr_code_image(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_wts_qr_code_image(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_test_image(WM_HWIN hWin, CS_IMAGE_T* image);
IMAGE_Handle create_miclogo_image(WM_HWIN hWin, CS_IMAGE_T* image);

#endif //__LOGO_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
