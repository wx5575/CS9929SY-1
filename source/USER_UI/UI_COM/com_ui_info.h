/**
  ******************************************************************************
  * @file    com_ui_info.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   提供公共界面服务接口
  ******************************************************************************
  */
#ifndef __COM_UI_INFO_H__
#define __COM_UI_INFO_H__

/* Includes ------------------------------------------------------------------*/
#include "GUI.H"
#include "WM.h"
#include "TEXT.h"
#include "sys_level.h"
#include "parameter_manage.h"
#include "key_fun_manage.h"
#include "fonts/fonts.h"
#include "cs99xx_struct.h"
#include "key_server.h"
#include "cs99xx_type.h"
#include "cs99xx_mem_api.h"
#include "cs99xx_usb_manager.h"

#pragma  diag_suppress 870 //消除870号编译警告

#define SYS_LANGUAGE		sys_par.language //CHINESE //ENGLISH //
#define SCREEM_SIZE         g_custom_sys_par.screem_size
/* 选择字体 如果当前系统字体为空就使用系统字体 不为空就正常使用 */
#define SEL_FONT(font)      font[SYS_LANGUAGE]==NULL?font[CHINESE]:font[SYS_LANGUAGE]
#define SELE_STR(S1, S2)	(SYS_LANGUAGE==CHINESE?S1:S2) ///< 根据系统语言选择不同的字符串
/* 选择要使用的字符串，如果对应语言的字符串不存在，就用中文字符串 */
#define SEL_USER_STR(str)      str[SYS_LANGUAGE]==NULL?str[CHINESE]:str[SYS_LANGUAGE]
#define WINDOWS_BAK_COLOR	GUI_BLUE	//GUI_GRAY ///< 窗口背景色

/** 
  * @brief 编辑控件结构
  */
typedef struct EDIT_ELE_T_   EDIT_ELE_T;

/** 
  * @brief 用户窗口结构
  */
typedef struct MYUSER_WINDOW    MYUSER_WINDOW_T;

/** 
  * @brief 窗口作用的字体的大小号字体索引
  */
typedef enum{
    FONT_SIZE_BIG,///<大字体
    FONT_SIZE_SMALL,///<小字体
    FONT_SIZE_NUM,///<字体个数
}FONT_SIZE;
/** 
  * @brief 控件位置尺寸结构
  */
typedef struct{
	uint16_t x;///<x坐标(像素)
	uint16_t y;///<y坐标(像素)
	uint16_t width;///<宽(像素)
	uint16_t height;///<高(像素)
}WIDGET_POS_SIZE_T;
/** 
  * @brief 控件显示信息结构
  */
typedef struct UI_ELE_DISPLAY_INFO{
	uint16_t base_x;///< x基坐标(像素)
	uint16_t base_y;///<y基坐标(像素)
    WIDGET_POS_SIZE_T pos_size;///<窗口的位置尺寸
	uint8_t max_len;///< 最大长度
    const GUI_FONT * font;//字体
	GUI_COLOR	font_color;///<字体颜色
	GUI_COLOR	back_color;///<背景颜色
	int align;///< 对齐方式
}UI_ELE_DISPLAY_INFO_T;
/** 
  * @brief 文本控件信息结构
  */
typedef struct TEST_UI_ELE{
	uint8_t *text[LANGUAGE_NUM];///< 内容
	CS_INDEX index;///<索引
	UI_ELE_DISPLAY_INFO_T dis_info;///<显示信息
	WM_HMEM	 handle;///< 句柄
	CS_LIST list;///< 文本链表
}TEXT_ELE_T;

/** 
  * @brief 用户回调函数指针类型
  */
typedef void (*USER_CALLBACK)(WM_MESSAGE *);

/** 
  * @brief 控件对象池结构
  */
typedef struct{
    void* pool;///<控件池类型
    uint32_t pool_size;///<控件池大小
    CS_INDEX *index_pool;///<控件索引池
    uint32_t index_size;///<索引池大小
    void (*init_create_fun)(MYUSER_WINDOW_T* win);///< 初始化并创建控件对象池中被索引的控件函数
    uint8_t pages;///<窗口中控件的总页数
	CS_LIST list_head;///< 控件链表头
}ELE_POOL_INF;
/** 
  * @brief 界面文本对象自动布局结构
  */
typedef struct{
    uint16_t base_x;///<x基坐标(像素)
    uint16_t base_y;///<y基坐标(像素)
    uint16_t width;///<文本控件的宽度(像素)
    uint16_t height;///<文本控件的高度(像素)
    uint8_t rows;///<最大行数
    uint8_t columns;///<最大列数
    uint8_t row_spacing;///<行距(像素)
    uint8_t column_spacing;///<列距(像素)
    const GUI_FONT * font[FONT_SIZE_NUM];//不同语言可以使用不同的字体
    GUI_COLOR font_color;///<字体颜色
    GUI_COLOR back_color;///<背景颜色
    int align;///对齐方式
    uint32_t max_len;///最大长度
}TEXT_ELE_AUTO_LAYOUT_T;
/** 
  * @brief 界面编辑对象自动布局结构
  */
typedef struct{
    uint16_t base_x;///<x基坐标
    uint16_t base_y;///<y基坐标
    uint16_t name_w;///<名称文本控件的宽度
    uint16_t edit_w;///<编辑控件的宽度
    uint16_t uint_w;///<单位文本控件的宽度
    uint16_t height;///<控件的高度
    uint8_t rows;///<最大行数
    uint8_t columns;///<最大列数
    uint16_t row_spacing;///<行距
    uint16_t column_spacing;///<列距
    const GUI_FONT *font;///<字体
    GUI_COLOR font_color;///<字体颜色
    GUI_COLOR back_color;///<背景颜色
    int name_align;///<对齐方式
    int edit_align;///<对齐方式
    int unit_align;///<对齐方式
    uint32_t max_len;///<最大长度
}EDIT_ELE_AUTO_LAYOUT_T;
/** 
  * @brief 调整项结构定义
  */
typedef struct{
    int32_t value;///<调整值
    CS_BOOL en;///<调整使能 CS_TRUE 表示调整使能 CS_FALSE 表示调整无效
}ADJUST_OPT;
/**
  * @brief  调整编辑对象布局结构定义 目前实际 使用中只需要调整控件的长度，
  * @brief  如编辑文件名时编辑框的长度要增加
  */
typedef struct{
    CS_INDEX index;///<编辑对象索引
    struct{
        ADJUST_OPT name;///<名称长度
        ADJUST_OPT edit;///<编辑控件长度
        ADJUST_OPT unit;///<单位显示的长度
    }width;///<宽度
}ADJUST_EDIT_ELE_LAYOUT;

/**
  * @brief  调整文本对象布局结构定义 目前实际 使用中只需要调整控件的长度
  */
typedef struct{
    CS_INDEX index;///<编辑对象索引
    ADJUST_OPT align;///<文本对齐方式
    struct{
        ADJUST_OPT base_x;///<x基坐标
        ADJUST_OPT offset_x;///<x坐标偏移值 >0 向右偏 <0向左偏
        ADJUST_OPT width;///<宽度
    }pos_size;
}ADJUST_TEXT_ELE_LAYOUT;

/** 
  * @brief 调整文本对象布局信息结构定义
  */
typedef struct{
    ADJUST_TEXT_ELE_LAYOUT *pool;///<窗口中文本对象调整布局信息池
    uint32_t size;
}ADJUST_TEXT_ELE_LAYOUT_INF;

/** 
  * @brief 调整编辑对象布局信息结构定义
  */
typedef struct{
    ADJUST_EDIT_ELE_LAYOUT *pool;///<窗口中编辑对象调整布局信息池
    uint32_t size;
}ADJUST_EDIT_ELE_LAYOUT_INF;

/** 
  * @brief 用户窗口自动布局信息结构
  */
typedef struct{
    TEXT_ELE_AUTO_LAYOUT_T **text_ele_auto_layout_inf;///<窗口中文本对象自动布局信息池
    EDIT_ELE_AUTO_LAYOUT_T **edit_ele_auto_layout_inf;///<窗口中编辑对象自动布局信息池
    
    ADJUST_TEXT_ELE_LAYOUT_INF **adjust_text_ele_layout_inf;///<文本对象调整布局信息池
    ADJUST_EDIT_ELE_LAYOUT_INF **adjust_edit_ele_layout_inf;///<编辑对象调整布局信息池
}AUTO_LAYOUT_POOL;
/** 
  * @brief 用户窗口结构
  */
struct MYUSER_WINDOW{
    uint8_t *win_name[LANGUAGE_NUM];///<窗口名称
	USER_CALLBACK call_back_fun;///<回调函数
	void (*init_key_fun)(WM_HMEM);///<窗口配套的按键更新函数
    ELE_POOL_INF text;///<文本控件索引池
    ELE_POOL_INF edit;///<编辑控件索引池
    ELE_POOL_INF com;///<公共文本控件索引池
    AUTO_LAYOUT_POOL auto_layout;///<自动布局信息
    WIDGET_POS_SIZE_T **pos_size_pool;///<窗口的位置尺寸池
    WIDGET_POS_SIZE_T pos_size;///<窗口的位置尺寸
	WM_HMEM	handle;///< 窗口句柄
	CS_LIST w_list;///< 窗口链表
};

/** 
  * @brief 对象类型枚举
  */
typedef enum{
	ELE_TEXT,///< 文本 
	ELE_EDIT_NUM,///< 数字编辑框 
	ELE_EDIT_STR,///< 字符串编辑框 
	ELE_DROPDOWN,///< 下拉选择框 
}ELE_TYPE;

/** 
  * @brief 数据所占的空间size 
  */
typedef enum{
	D_1_BYTES = 1,///< 1字节数据 
	D_2_BYTES = 2,///< 2字节数据 
	D_4_BYTES = 4,///< 4字节数据 
	D_8_BYTES = 8,///< 8字节数据 
	D_N_BYTES,///< 数据字节数不定,用于字符串数据和结构体数据 
}BYTE_NUM;

/** 
  * @brief 编辑控件可配置属性
  */
typedef struct{
    uint16_t width;///<宽(像素)
    uint16_t height;///<高(像素)
    const GUI_FONT *font;///<字体
    GUI_COLOR font_color;///<字体颜色
    GUI_COLOR back_color;///<背景颜色
    int align;///<对齐方式
    uint32_t max_len;///<最大长度
    WM_HMEM handle;///<句柄
}CONFIGURABLE_PROPERTY;

/** 
  * @brief 编辑控件相关显示内容信息
  */
typedef struct{
    CONFIGURABLE_PROPERTY name;///< 名称
    CONFIGURABLE_PROPERTY edit;///< 编辑
    CONFIGURABLE_PROPERTY unit;///< 单位
    uint16_t x;///< 所在窗口的 x 坐标(像素)
    uint16_t y;///< 所在窗口的 x 坐标(像素)
}EDIT_ELE_DISPLAY_INF;

/** 
  * @brief 编辑控件结构 包含3个子控件 名称+编辑控件+单位信息
  */
struct EDIT_ELE_T_{
	uint8_t *name[LANGUAGE_NUM];///< 名称
	CS_INDEX index;///< 通过枚举索引
	uint8_t *value[10];///< 默认值
	
    /* 数据 */
    struct{
        void *data;///< 数据指针
        uint8_t bytes;///< 字节数
    }data;
    
    /* 资源 */
    struct{
        void *table;///< 资源表
        uint8_t size;///< 资源表的条数
        void *user_data;///<用户数据
        uint8_t user_data_size;///<用户数据个数
        void *table_2[LANGUAGE_NUM];///< 可包含不同语言的资源
    }resource;
    
    /* 类型 */
    struct{
        ELE_TYPE type;///< 对象类型
    }type;
    
    /* 格式 */
    struct{
        uint8_t dec;///< 小数点位数
        uint8_t lon;///< 数据显示长度,如果是浮点数包含小数点，必须小于10
        UNIT_T unit;///< 单位
    }format;
    
    /* 范围 */
    struct{
        uint32_t high;///< 上限
        uint32_t low;///< 下限 
        uint8_t *notice[LANGUAGE_NUM];///< 提示信息包含中英文
        void (*check_value_validity)(EDIT_ELE_T*,uint32_t*);///<检查数据的正确性
        void (*provided_dis_range_fun)(EDIT_ELE_T*);///<提供的显示range的函数
    }range;
    
    /* 按键信息 包含系统键 菜单键 键盘服务函数 */
    struct{
        void (*fun_sys_key)(WM_HMEM);///< 方向键信息更新函数
        void (*fun_menu_key)(WM_HMEM);//菜单键更新函数
        void (*fun_key)(uint32_t);///< 设置该变量的键盘服务函数
    }key_inf;
    
    EDIT_ELE_DISPLAY_INF dis;///<显示相关的配置信息
    uint8_t num;///<显示页中的编号0-n
    uint8_t total;///<所在页总共的编辑控件个数
    uint8_t page;///<所在的页编号1-n
	CS_LIST e_list;///< 编辑控件链表
};
typedef struct{
    void (*dispose_cb)(WM_HWIN);///<处理使用的回调函数
    WM_HWIN handle;///<窗口句柄
}WIN_CB_INF;
/** 
  * @brief 警告提示信息结构
  */
typedef struct{
    TEXT_ELE_T title;///<标题
    TEXT_ELE_T content;///<内容
    WIDGET_POS_SIZE_T win_pos_size;///<窗口位置尺寸
    uint32_t dly_auto_close;///<延时自动关闭,计时为0表示不关闭，非0时间到自动关闭,单位ms
    WIN_CB_INF warning_enter_cb;///<当按下ENTER键时会使用的回调函数
    WIN_CB_INF warnig_cancle_cb;///<当按下CANCLE键时会使用的回调函数
}WARNING_INF;

/*********************************************************************/
/** 
  * @brief 范围信息是很多界面者会用到的公共文本控件
  */
typedef enum {
	COM_RANGE_NAME,///<主界面的通信状态
	COM_RANGE_NOTICE,///<主界面的系统时间
    COM_PAGE_NOTICE,///<页码提示信息 1/1 表示 第1页/共1页
    
    COM_UI_FILE_NAME    ,///< 记忆组文件名
    COM_UI_CUR_FILE_NAME,///< 记忆组文件名内容
    COM_UI_STEP         ,///< 记忆组步骤信息
    COM_UI_CUR_STEP     ,///< 记忆组步骤信息内容
    COM_UI_WORK_MODE    ,///< 记忆组工作模式
    COM_UI_CUR_WORK_MODE,///< 记忆组工作模式内容
    
    COM_ELE_NUM,///<公共控件的个数
}COM_TEXT_INDEX;

#define COM_RANGE_ELE_NUM   2   ///< 范围公共文本对象个数
#define COM_GRUOP_ELE_NUM   6   ///< 记忆组公共文本对象个数
#define COM_PAGE_ELE_NUM    1   ///< 页码提示信息公共文本对象个数

extern TEXT_ELE_T com_text_ele_pool[COM_ELE_NUM];
extern CS_INDEX range_com_ele_table[COM_RANGE_ELE_NUM];
extern CS_INDEX range_page_group_com_ele_table[COM_ELE_NUM];
extern CS_INDEX range_group_com_ele_table[COM_RANGE_ELE_NUM + COM_GRUOP_ELE_NUM];
extern CS_INDEX group_com_ele_table[COM_GRUOP_ELE_NUM];
extern void init_com_text_ele_dis_inf(MYUSER_WINDOW_T* win);
extern void init_page_num_com_text_ele_dis_inf(MYUSER_WINDOW_T* win);
extern void init_group_com_text_ele_dis_inf(MYUSER_WINDOW_T* win);
/*********************************************************************/
/** 
  * @brief 用户自定义消息
  */
typedef enum{
	CM_CHILD_W_MSG = 0XFFFFF,///<子窗口发来的消息 
    CM_UPDATE_USB_ST,///<更新usb的状态
	CM_FILE_UI_SAVE,///<文件存贮
	CM_FILE_UI_NEW,///<文件界面
	CM_FILE_UI_READ,///<文件读取
	CM_FILE_UI_EDIT,///<文件编辑
	CM_FILE_UI_DEL,///<文件删除
    CM_FILE_UI_CLEAR,///<清空文件
	CM_WARNING_UI,///<警告框
    CM_WARNING_INTO_SAVE_FILE,///<进入存贮文件的警告框
    CM_WARNING_INTO_DELETE_FILE,///<进入存贮文件的警告框
	CM_EDIT_WIDGET_CHANGE,///<当编辑控件改变时程序监控到后向控件的父窗口发送这个消息，通常是为了选择按键服务程序
	CM_DISTROY_DIALOG,///<销毁对话框时发给父窗口的消息
	
	CM_DIALOG_RETURN_OK,///<子窗口返回时按下了OK键 或 ENTER
	CM_DIALOG_RETURN_CANCLE,///<子窗口返回时按下了CANCLE键 或 EXIT
    CM_DIALOG_CHANGE_WORK_MODE,///<更改记忆组工作模式
}CUSTOM_MSG_ID;
/** 
  * @brief 用户自定义消息结构
  */
typedef struct{
	CUSTOM_MSG_ID id;///<所在子窗口的自定义ID
	int msg;///<命令
	int user_data;///<用户数据
}CUSTOM_MSG_T;

#ifdef   COM_UI_GLOBALS
#define  COM_UI_EXT
#else
#define  COM_UI_EXT  extern
#endif

COM_UI_EXT void (*recover_key_inf_fun)(void);///<恢复按键信息函数指针
COM_UI_EXT void (*backup_key_inf_fun)(void);///<备份按键信息函数指针
COM_UI_EXT CS_LIST 				windows_list;///<窗口链表
COM_UI_EXT MYUSER_WINDOW_T      *g_cur_win;///<当前窗口指针
COM_UI_EXT EDIT_ELE_T     *g_cur_edit_ele;///<当前编辑对象
COM_UI_EXT EDIT_ELE_T     *g_cur_edit_ele_bk;///<备份当前编辑对象
COM_UI_EXT TEXT_ELE_T           *g_cur_text_ele;///<当前文本对象
COM_UI_EXT CUSTOM_MSG_T 	    g_custom_msg;///<用户自定义消息实体变量
COM_UI_EXT uint32_t             id_base;///<全局控件ID变量

extern TEXT_ELE_T * get_text_ele_inf(TEXT_ELE_T *text_pool, uint32_t pool_size, CS_INDEX index, CS_ERR*err);
extern void init_window_text_ele(MYUSER_WINDOW_T* win);
extern void update_text_ele(CS_INDEX index, MYUSER_WINDOW_T* win, const uint8_t *str);
extern void set_text_ele_font_color(CS_INDEX index, MYUSER_WINDOW_T* win, GUI_COLOR color);
extern void create_user_window(MYUSER_WINDOW_T* win_info, CS_LIST *list_head, WM_HWIN h_parent);
extern void init_window_pos_size(MYUSER_WINDOW_T* win_inf, WIDGET_POS_SIZE_T *pos_size_inf);
extern void init_sys_function_key_inf(MYUSER_WINDOW_T* win);
extern void init_create_win_all_ele(MYUSER_WINDOW_T* win);
extern void create_user_dialog(MYUSER_WINDOW_T* win_info, CS_LIST *list_head, WM_HWIN hWin);
extern void set_user_window_handle(WM_HWIN hWin);
extern void set_cur_window(MYUSER_WINDOW_T* win_info);
extern void set_cur_edit_ele(EDIT_ELE_T *node);
extern void show_user_window(MYUSER_WINDOW_T* win_info);
extern void show_cur_window(void);
extern void back_win(WM_HWIN id);
extern void del_cur_window(void);
extern MYUSER_WINDOW_T * get_user_window_info(WM_HWIN hWin);
extern TEXT_ELE_T* get_text_ele_node(CS_INDEX index, CS_LIST* list, CS_ERR *err);
extern void set_com_text_ele_dis_inf(UI_ELE_DISPLAY_INFO_T *inf, CS_INDEX index);
extern void update_com_text_ele(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str);
extern void init_window_com_text_ele(MYUSER_WINDOW_T* win);
extern void init_window_text_ele_list(MYUSER_WINDOW_T *win);
extern void init_window_edit_ele_list(MYUSER_WINDOW_T *win);
extern void init_window_com_ele_list(MYUSER_WINDOW_T *win);
extern void init_window_text_ele_dis_inf(MYUSER_WINDOW_T *win, TEXT_ELE_AUTO_LAYOUT_T *inf);
extern void init_window_edit_ele_dis_inf(MYUSER_WINDOW_T *win, EDIT_ELE_AUTO_LAYOUT_T* inf);
extern void auto_init_win_edit_ele_dis_inf(MYUSER_WINDOW_T *win);
extern void dis_one_page_win_edit_eles(MYUSER_WINDOW_T *win, uint8_t page);
extern void auto_init_win_text_ele_dis_inf(MYUSER_WINDOW_T *win);
extern void adjust_win_edit_ele_dis_inf(MYUSER_WINDOW_T *win);
extern void adjust_win_text_ele_dis_inf(MYUSER_WINDOW_T *win);
extern void set_custom_msg_id(CUSTOM_MSG_ID id);
extern void update_win_menu_key(MYUSER_WINDOW_T* win);
extern void init_dialog(MYUSER_WINDOW_T * win);
extern void send_msg_to_parent(WM_HWIN hwin, int id, int val);
extern void send_no_par_msg_to_parent(WM_HWIN hwin, int id);
extern void update_all_windows_text(void);
extern void set_global_fun_key_dispose(void (*fun)(uint32_t));
extern void set_com_text_ele_inf(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str[]);
extern void set_group_text_ele_inf(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str);
extern void draw_group_inf_area(void);
extern void update_group_inf(MYUSER_WINDOW_T* win);
extern void delete_win_all_ele(MYUSER_WINDOW_T* win);
extern void delete_win_com_ele(MYUSER_WINDOW_T* win);
extern void delete_win_edit_ele(MYUSER_WINDOW_T* win);
extern void init_create_win_com_ele(MYUSER_WINDOW_T* win);
extern void update_range_name(uint8_t *str);
extern void update_default_range_name(void);
extern void update_page_num(MYUSER_WINDOW_T* win, EDIT_ELE_T *ele);
extern void myGUI_DrawRectEx(const GUI_RECT * pRect);
extern void recover_g_cur_edit_ele(void);
extern void backup_g_cur_edit_ele(void);
extern void register_backup_key_inf_fun(void(*fun)(void));
extern void register_recover_key_inf_fun(void(*fun)(void));

/* 各个窗口的创建接口.放在这里方便窗口间相互创建 */
extern void create_file_win(int hWin);
extern void create_sys_manager_win(int hWin);
extern void create_key_menu_window(void);

extern void show_key_menu_win(void);
extern void hide_key_menu_win(void);
extern void show_main_win(void);
extern void hide_main_win(void);

extern void create_test_win(int hWin);
extern void create_warning_dialog(int hWin);
extern void set_warning_ui_inf(WARNING_INF *warning);
extern void create_step_par_win(int id);
extern void create_password_ui(int hWin);
extern void init_back_up_will_enter_win_inf(void (*fun)(int), int data);
extern void create_input_password_ui(int hWin);
extern void create_result_win(int id);
extern void init_menu_key_info(MENU_KEY_INFO_T * info, uint32_t n, int data);
extern void unregister_system_key_fun(CONFIG_FUNCTION_KEY_INFO_T info[], uint32_t n);
extern void register_system_key_fun(CONFIG_FUNCTION_KEY_INFO_T info[], uint32_t n, int data);
extern void set_menu_function_status(uint32_t key_value, SYS_KEY_ST_ENUM st);
extern void set_menu_key_config_st(MENU_KEY_INFO_T * inf, uint32_t size,
                        CS_INDEX index, SYS_KEY_ST_ENUM st, CS_ERR *err);
extern void change_menu_key_font_color(uint32_t key_value, GUI_COLOR color);


extern void update_cur_step_crc(void);
extern void check_cur_step_changed_send_to_slave(void);
extern void get_mode_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void get_range_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void get_vol_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void get_test_time_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void get_lower_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void get_upper_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void get_auto_shift_edit_ele_inf(UN_STRUCT *step, EDIT_ELE_T* ret_ele, CS_ERR *err);
extern void create_env_par_dialog(int hWin);
extern void create_sys_time_dialog(int hWin);
extern void create_restor_factory_setting_dialog(int hWin);

extern void create_self_check_win(int hWin);
extern void create_start_win(int hWin);
extern void create_calibration_win(int hWin);
extern void create_module_win(int hWin);

#endif //__COM_UI_INFO_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
