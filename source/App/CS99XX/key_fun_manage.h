/**
  ******************************************************************************
  * @file    key_fun_manage.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键功能管理,按键功能的注册和查询
  ******************************************************************************
  */
#ifndef __KEY_FUN_MANAGE_H__
#define __KEY_FUN_MANAGE_H__


/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "cs99xx_struct.h"

/**
  * @brief  标准菜单键名称的枚举索引
  */
typedef enum{
	F_KEY_ENTER,
	F_KEY_CANCLE,
	F_KEY_NEXT,
	F_KEY_DEL,
	F_KEY_BACKSPACE,
	F_KEY_HOME,
	F_KEY_END,
	F_KEY_SELECTED,
	F_KEY_SELECT,
	
	F_KEY_FILE,
	F_KEY_STEP,
	F_KEY_SYS,
	F_KEY_RESULT,
	F_KEY_MORE,
    F_KEY_F0,
    F_KEY_F1,
    F_KEY_F2,
    F_KEY_F3,
    F_KEY_F4,
    F_KEY_F5,
    F_KEY_F6,
    
	F_KEY_NULL,
	F_KEY_CUSTOM,///<自定义
	F_KEY_SAVE,
	F_KEY_NEW,
	F_KEY_READ,
	F_KEY_EDIT,
	F_KEY_BACK,
	F_KEY_DISPLAY,
    F_KEY_TEST,
    F_KEY_HELP,
    F_KEY_VOL,
    F_KEY_CUR,
    F_KEY_RANGE,
    F_KEY_UPPER,
    F_KEY_LOWER,
    F_KEY_TIME,
    F_KEY_MODE,
    F_KEY_CLEAR,
    F_KEY_DETAIL,///<详细
    F_KEY_INSTER,///<插入
    F_KEY_FORWARD,///<前移
    F_KEY_BACKWARD,///<后移
    F_KEY_ON,
    F_KEY_OFF,
    F_KEY_GRADE,
    F_KEY_SETTING,
    F_KEY_AUTO,
}MENU_KEY_INDEX_ENUM;

typedef enum{
	MENU_KEY_EN,//菜单键使能
	MENU_KEY_DIS,//菜单键失能
}MENU_KEY_ST_ENUM;

typedef struct{
	int user_data;///<传递给键盘处理函数的用户数据
    int custom_data;///<用户定制数据，用于在定制菜单信息时使用
}KEY_MESSAGE;

/** 按键回调函数指针 */
typedef void (*KEY_CB_FUN)(KEY_MESSAGE *);
/** 
  * @brief 定制菜单键信息结构
  */
typedef struct{
    uint8_t *name;///<模式的名称
    int data;///<自定义菜单键的数据
    KEY_CB_FUN fun;///<自定义菜单键的功能函数
}CUSTOM_MENU_KEY_INF;


typedef struct
{
	KEY_CB_FUN fun;///<按键处理函数
	KEY_MESSAGE msg;///<按键消息
	int en;///<使能按键
}KEY_DISPOSE_FUN;

/** 功能按键结构 */
typedef struct{
	uint32_t key_value;///<键值
	KEY_CB_FUN key_up_dispose_fun;///<处理函数
    KEY_MESSAGE msg;///<按键消息
	MENU_KEY_ST_ENUM en;///<按键使能
}FUNCTION_KEY_INFO_T;


/** 菜单键的配置信息 */
typedef struct{
	uint8_t *name;///<按键名称
	CS_INDEX index;///<标准菜单键盘索引
    FUNCTION_KEY_INFO_T fun_key;///<功能键信息
}MENU_KEY_INFO_T;
typedef struct{
	KEY_DISPOSE_FUN	coded_right;
	KEY_DISPOSE_FUN	coded_left;
    
	KEY_DISPOSE_FUN	key_up;
	KEY_DISPOSE_FUN	key_down;
	KEY_DISPOSE_FUN	key_left;
	KEY_DISPOSE_FUN	key_right;
	
	KEY_DISPOSE_FUN	key_enter;
	KEY_DISPOSE_FUN	key_exit;
	KEY_DISPOSE_FUN	key_offset;
	KEY_DISPOSE_FUN	key_ulock;
	KEY_DISPOSE_FUN	key_shift;
	
	KEY_DISPOSE_FUN	key_f0;
	KEY_DISPOSE_FUN	key_f1;
	KEY_DISPOSE_FUN	key_f2;
	KEY_DISPOSE_FUN	key_f3;
	KEY_DISPOSE_FUN	key_f4;
	KEY_DISPOSE_FUN	key_f5;
	KEY_DISPOSE_FUN	key_f6;
    
	KEY_DISPOSE_FUN	key_clock_0;
	KEY_DISPOSE_FUN	key_clock_1;
	KEY_DISPOSE_FUN	key_f1_0;
	KEY_DISPOSE_FUN	key_f1_1;
	KEY_DISPOSE_FUN	key_f1_4;
	
}KEY_FUNCATION;


#ifdef   KEY_DISPOSE_GLOBALS
#define  KEY_DISPOSE_EXT
#else
#define  KEY_DISPOSE_EXT  extern
#endif

/**
  * @brief  全局的功能键处理函数指针
  */
KEY_DISPOSE_EXT		void (*global_fun_key_dispose)(uint32_t key_value);
KEY_DISPOSE_EXT		void (*scan_key_custom_fun)();
KEY_DISPOSE_EXT		void (*scan_direct_key_fun)();
KEY_DISPOSE_EXT		 KEY_FUNCATION		key_funcation;
KEY_DISPOSE_EXT		 KEY_FUNCATION		key_funcation_bk;///<备份使用
KEY_DISPOSE_EXT		 KEY_DISPOSE_FUN 	golbal_key_info;
extern void dispose_funcation_key(uint32_t key);
extern void init_funcation_key_dispose_fun(KEY_FUNCATION *info);
extern void register_key_dispose_fun(uint32_t key, KEY_DISPOSE_FUN *fun);
extern void enable_function_key(uint32_t key, uint8_t st);
extern uint8_t get_function_key_st(uint32_t key, CS_ERR *err);
extern void disable_system_fun_key_fun(void);
extern void recover_key_funcation_inf(void);
extern void backup_key_funcation_inf(void);

#endif //__KEY_FUN_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
