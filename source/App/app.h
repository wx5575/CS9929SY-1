/**
  ******************************************************************************
  * @file    app.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   用户程序
  ******************************************************************************
  */
#ifndef __APP_H__
#define __APP_H__

#include "app_cfg.h"
#include "os.h"
#include "ff.h"
#include "sys_level.h"



#ifndef NULL
#define NULL 0
#endif
/** 
  * @brief 应用程序使用的标记定义
  */
typedef struct {
	uint8_t stop_scan_key;///< 停止扫描键盘
    uint8_t calibration;///<校准标记
}APP_FLAG;

#ifdef   APP_GLOBALS
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif

APP_EXT		APP_FLAG	app_flag;


APP_EXT     FRESULT fs_result;
APP_EXT     FATFS fs_struct;

//任务控制块
APP_EXT	OS_TCB StartTaskTCB;
APP_EXT	OS_TCB ScanKeyTaskTCB;
APP_EXT	OS_TCB MainTaskTCB;
APP_EXT	OS_TCB TouchTaskTCB;
APP_EXT	OS_TCB Ch376TaskTCB;
APP_EXT	OS_TCB ModuleCommTaskTCB;
APP_EXT	OS_TCB ExceptionHandlingTaskTCB;

APP_EXT	OS_TMR 	timer_for_app;//服务应用程序的定时器

//任务堆栈
APP_EXT	CPU_STK		START_TASK_STK[START_STK_SIZE];
APP_EXT	CPU_STK		SCAN_KEY_TASK_STK[SCAN_KEY_STK_SIZE];
APP_EXT	CPU_STK		MAIN_TASK_STK[EMWINDEMO_STK_SIZE];
APP_EXT	CPU_STK		CH376_TASK_STK[CH376_STK_SIZE];
APP_EXT	CPU_STK		MODULE_COMM_TASK_STK[MODULE_COMM_STK_SIZE];
APP_EXT	CPU_STK		EXCEPTION_HANDLING_TASK_STK[MODULE_COMM_STK_SIZE];

//任务函数
extern void start_task(void *p_arg);
extern void main_task(void *p_arg);
extern void led0_task(void *p_arg);
extern void touch_task(void *p_arg);
extern uint32_t get_key_value(void);
extern void scan_key_task(void);
extern void read_par_from_memory(void);


#endif // __APP_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
