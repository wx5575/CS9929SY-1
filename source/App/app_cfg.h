/**
  ******************************************************************************
  * @file    app_cfg.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   用户应用程序配置信息
  ******************************************************************************
  */
#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/**
  * @brief  任务优先级
  */
typedef enum{
	START_TASK_PRIO = 3, //启动任务优先级
	TOUCH_TASK_PRIO,//TOUCH任务
	SCAN_KEY_TASK_PRIO,//SCAN_KEY_TASK_PRIO任务
	CH376_TASK_PRIO,//CH376任务
	MODULE_COMM_TASK_PRIO,//MODULE_COMM任务
	MAIN_TASK_PRIO,//主任务
}TASK_PRIO;

/**
  * @brief  任务堆栈大小
  */
typedef enum{
	START_STK_SIZE = 1024,///<启动任务优先级
	SCAN_KEY_STK_SIZE = 2048,///<键盘任务
	CH376_STK_SIZE = 2048,///<CH376任务
	EMWINDEMO_STK_SIZE = 1024*5,///<EMWINDEMO任务
	MODULE_COMM_STK_SIZE = 1024,///<MODULE_COMM任务
}TASK_STK_SIZE;




#endif //__APP_CFG_H__
