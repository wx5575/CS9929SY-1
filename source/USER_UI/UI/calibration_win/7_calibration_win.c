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

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
