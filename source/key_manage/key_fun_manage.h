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

#include "sys_level.h"
#include "cs99xx_struct.h"
#include "keyboard.h"

/**
  * @brief  标准菜单键名称的枚举索引
  */
typedef enum{
	F_KEY_ENTER,///< 确认
	F_KEY_CANCEL,///< 取消
	F_KEY_NEXT,///< 下一个
	F_KEY_DEL,///< 删除
	F_KEY_BACKSPACE,///< 退格
	F_KEY_HOME,///< hom键
	F_KEY_END,///< end键
	F_KEY_SELECTED,///< 已选择
	F_KEY_SELECT,///< 选择
	
	F_KEY_FILE,///< 文件
	F_KEY_STEP,///< 步骤
	F_KEY_SYS,///< 系统
	F_KEY_RESULT,///< 结果
	F_KEY_MORE,///< 更多
    F_KEY_F0,///< 功能键F0
    F_KEY_F1,///< 功能键F1
    F_KEY_F2,///< 功能键F2
    F_KEY_F3,///< 功能键F3
    F_KEY_F4,///< 功能键F4
    F_KEY_F5,///< 功能键F5
    F_KEY_F6,///< 功能键F6
    
	F_KEY_NULL,///< 功能键空
	F_KEY_CUSTOM,///<自定义 功能键
	F_KEY_SAVE,///< 保存
	F_KEY_NEW,///< 新建
	F_KEY_READ,///< 读取
	F_KEY_EDIT,///< 编辑
	F_KEY_BACK,///< 返回
	F_KEY_DISPLAY,///< 显示
    F_KEY_TEST,///< 测试
    F_KEY_HELP,///< 帮助
    F_KEY_VOL,///< 电压
    F_KEY_CUR,///< 电流
    F_KEY_RANGE,///< 档位
    F_KEY_UPPER,///< 上限
    F_KEY_LOWER,///< 下限
    F_KEY_TIME,///< 时间
    F_KEY_MODE,///< 更多
    F_KEY_CLEAR,///< 清空
    F_KEY_DETAIL,///<详细
    F_KEY_INSTER,///<插入
    F_KEY_FORWARD,///<前移
    F_KEY_BACKWARD,///<后移
    F_KEY_ON,///<开启
    F_KEY_OFF,///<关闭
    F_KEY_GRADE,///<等级
    F_KEY_SETTING,///<设置
    F_KEY_AUTO,///<自动
    F_KEY_GND,///<接地
    F_KEY_FLOAT,///<浮地
    F_KEY_START,///<启动
    F_KEY_STOP,///<停止
    F_KEY_MODULE,///<模块
}MENU_KEY_INDEX_ENUM;

/**
  * @brief  菜单键使能状态
  */
typedef enum{
	SYS_KEY_EN,//菜单键使能
	SYS_KEY_DIS,//菜单键失能
}SYS_KEY_ST_ENUM;

/**
  * @brief  按键消息结构定义
  */
typedef struct{
	uint32_t key_value;///<键值
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

/** 
  * @brief 按键处理信息结构
  */
typedef struct
{
	KEY_CB_FUN fun;///<按键处理函数
	KEY_MESSAGE msg;///<按键消息
	SYS_KEY_ST_ENUM en;///<使能按键
}KEY_DISPOSE_FUN;
/** 
  * @brief 配置系统功能按键结构
  */
typedef struct{
	uint32_t key_value;///<键值
	KEY_CB_FUN key_up_dispose_fun;///<处理函数
    KEY_MESSAGE msg;///<按键消息
	SYS_KEY_ST_ENUM en;///<按键使能
}CONFIG_FUNCTION_KEY_INFO_T;

/** 
  * @brief 菜单键的配置信息
  */
typedef struct{
	uint8_t *name;///<按键名称
	CS_INDEX index;///<标准菜单键盘索引
    CONFIG_FUNCTION_KEY_INFO_T fun_key;///<功能键信息
}MENU_KEY_INFO_T;

/** 
  * @brief 功能按键处理信息结构定义
  */
typedef struct{
	KEY_DISPOSE_FUN	coded_right;///< 码盘右
	KEY_DISPOSE_FUN	coded_left;///< 码盘左
    
	KEY_DISPOSE_FUN	key_up;///< 向上键
	KEY_DISPOSE_FUN	key_down;///< 向下键
	KEY_DISPOSE_FUN	key_left;///< 向左键
	KEY_DISPOSE_FUN	key_right;///< 向右键
    
	KEY_DISPOSE_FUN	key_start;///< 启动键
	KEY_DISPOSE_FUN	key_stop;///< 复位键
	
	KEY_DISPOSE_FUN	key_enter;///< 确认键
	KEY_DISPOSE_FUN	key_exit;///< 退出键
	KEY_DISPOSE_FUN	key_offset;///< 偏移键
	KEY_DISPOSE_FUN	key_ulock;///< 界面上锁键
	KEY_DISPOSE_FUN	key_shift;///< 输入大小写SHIFT切换键
	
	KEY_DISPOSE_FUN	key_f0_up;///< 菜单功能键F0抬起
	KEY_DISPOSE_FUN	key_f1_up;///< 菜单功能键F1抬起
	KEY_DISPOSE_FUN	key_f2_up;///< 菜单功能键F2抬起
	KEY_DISPOSE_FUN	key_f3_up;///< 菜单功能键F3抬起
	KEY_DISPOSE_FUN	key_f4_up;///< 菜单功能键F4抬起
	KEY_DISPOSE_FUN	key_f5_up;///< 菜单功能键F5抬起
	KEY_DISPOSE_FUN	key_f6_up;///< 菜单功能键F6抬起
	
	KEY_DISPOSE_FUN	key_f0_down;///< 菜单功能键F0按下
	KEY_DISPOSE_FUN	key_f1_down;///< 菜单功能键F1按下
	KEY_DISPOSE_FUN	key_f2_down;///< 菜单功能键F2按下
	KEY_DISPOSE_FUN	key_f3_down;///< 菜单功能键F3按下
	KEY_DISPOSE_FUN	key_f4_down;///< 菜单功能键F4按下
	KEY_DISPOSE_FUN	key_f5_down;///< 菜单功能键F5按下
	KEY_DISPOSE_FUN	key_f6_down;///< 菜单功能键F6按下
    
	KEY_DISPOSE_FUN	key_clock_0;///< 组合键 KEY_CLOCK + KEY_0
	KEY_DISPOSE_FUN	key_clock_1;///< 组合键 KEY_CLOCK + KEY_1
    
	KEY_DISPOSE_FUN	key_clock_offset;///< 组合键 KEY_CLOCK + KEY_OFFSET
	KEY_DISPOSE_FUN	key_clock_enter;///< 组合键 KEY_CLOCK + KEY_ENTER
	KEY_DISPOSE_FUN	key_clock_exit;///< 组合键 KEY_CLOCK + KEY_EXIT
    
	KEY_DISPOSE_FUN	key_f1_0;///< 组合键 KEY_F1 + KEY_0
	KEY_DISPOSE_FUN	key_f1_1;///< 组合键 KEY_F1 + KEY_1
	KEY_DISPOSE_FUN	key_f1_4;///< 组合键 KEY_F1 + KEY_4
	
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
extern void enable_function_key(uint32_t key, SYS_KEY_ST_ENUM st);
extern uint8_t get_function_key_st(uint32_t key, CS_ERR *err);
extern KEY_CB_FUN get_function_key_dispose_fun(uint32_t key, CS_ERR *err);
extern void disable_system_fun_key_fun(void);
extern void recover_key_funcation_inf(void);
extern void backup_key_funcation_inf(void);

#endif //__KEY_FUN_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
