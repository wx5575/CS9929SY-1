/**
  ******************************************************************************
  * @file    scan_keyboard.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键扫描
  ******************************************************************************
  */

#define KEY_DISPOSE_GLOBALS
#include "scan_keyboard.h"
#include "keyboard.h"
#include "string.h"
#include "tools.h"
#include "os.h"
#include "GUI.H"
#include "EDIT.h"
/** 
  * @brief 用户界面使用的全局标记
  */
typedef struct{
    uint8_t shift_flag;///< SHIFT键按下标记
    uint8_t key_lock_flag;///< 键盘锁按下标记
}UI_FLAG;
static volatile UI_FLAG ui_flag;///<界面使用全局标记

void set_shift_status(uint8_t st)
{
    ui_flag.shift_flag = st;
}
uint8_t get_shift_status(void)
{
    return ui_flag.shift_flag;
}
void set_key_lock_flag(uint8_t flag)
{
    ui_flag.key_lock_flag = flag;
}
uint8_t get_key_lock_flag(void)
{
    return ui_flag.key_lock_flag;
}
void init_funcation_key_dispose_fun(KEY_FUNCATION *info)
{
	if(NULL == info)
	{
		memset(&key_funcation, 0, sizeof(key_funcation));
	}
	
	key_funcation = *info;
}

KEY_DISPOSE_FUN *get_key_inf(uint32_t key)
{
	KEY_DISPOSE_FUN *temp_fun = NULL;
    
	switch(key)
	{
        case CODE_LEFT:  temp_fun = &key_funcation.coded_left;break;
        case CODE_RIGH:  temp_fun = &key_funcation.coded_right;break;
        
		case KEY_EXIT:	 temp_fun = &key_funcation.key_exit;break;
		case KEY_ENTER:	 temp_fun = &key_funcation.key_enter;break;
		case KEY_OFFSET: temp_fun = &key_funcation.key_offset;break;
		case KEY_UNLOCK: temp_fun = &key_funcation.key_ulock;break;
		case KEY_SHIFT:	 temp_fun = &key_funcation.key_shift;break;
		
		case KEY_UP:	 temp_fun = &key_funcation.key_up;break;
		case KEY_DOWN:	 temp_fun = &key_funcation.key_down;break;
		case KEY_LEFT:	 temp_fun = &key_funcation.key_left;break;
		case KEY_RIGHT:	 temp_fun = &key_funcation.key_right;break;
        
		case KEY_UP     & _KEY_LONG:	 temp_fun = &key_funcation.key_up;break;
		case KEY_DOWN   & _KEY_LONG:	 temp_fun = &key_funcation.key_down;break;
		case KEY_LEFT   & _KEY_LONG:	 temp_fun = &key_funcation.key_left;break;
		case KEY_RIGHT  & _KEY_LONG:	 temp_fun = &key_funcation.key_right;break;
        
		case KEY_UNLOCK & KEY_0:temp_fun = &key_funcation.key_clock_0;break;
		case KEY_F1 & KEY_0:temp_fun = &key_funcation.key_f1_0;break;
		case KEY_F1 & KEY_1:temp_fun = &key_funcation.key_f1_1;break;
		case KEY_F1 & KEY_4:temp_fun = &key_funcation.key_f1_4;break;
		
		case KEY_F0 & _KEY_UP:   temp_fun = &key_funcation.key_f0;break;
		case KEY_F1 & _KEY_UP:	 temp_fun = &key_funcation.key_f1;break;
		case KEY_F2 & _KEY_UP:	 temp_fun = &key_funcation.key_f2;break;
		case KEY_F3 & _KEY_UP:	 temp_fun = &key_funcation.key_f3;break;
		case KEY_F4 & _KEY_UP:	 temp_fun = &key_funcation.key_f4;break;
		case KEY_F5 & _KEY_UP:	 temp_fun = &key_funcation.key_f5;break;
		case KEY_F6 & _KEY_UP:	 temp_fun = &key_funcation.key_f6;break;
        
		case KEY_F0 & _KEY_DOWN:    temp_fun = &key_funcation.key_f0;break;
		case KEY_F1 & _KEY_DOWN:    temp_fun = &key_funcation.key_f1;break;
		case KEY_F2 & _KEY_DOWN:    temp_fun = &key_funcation.key_f2;break;
		case KEY_F3 & _KEY_DOWN:    temp_fun = &key_funcation.key_f3;break;
		case KEY_F4 & _KEY_DOWN:    temp_fun = &key_funcation.key_f4;break;
		case KEY_F5 & _KEY_DOWN:    temp_fun = &key_funcation.key_f5;break;
		case KEY_F6 & _KEY_DOWN:    temp_fun = &key_funcation.key_f6;break;
        default: break;
	}
    
    return temp_fun;
}

KEY_DISPOSE_FUN *get_key_inf_(uint32_t key)
{
	KEY_DISPOSE_FUN *temp_fun = NULL;
    
	switch(key)
	{
        case CODE_LEFT:  temp_fun = &key_funcation.coded_left;break;
        case CODE_RIGH:  temp_fun = &key_funcation.coded_right;break;
        
		case KEY_EXIT:	 temp_fun = &key_funcation.key_exit;break;
		case KEY_ENTER:	 temp_fun = &key_funcation.key_enter;break;
		case KEY_OFFSET: temp_fun = &key_funcation.key_offset;break;
		case KEY_UNLOCK: temp_fun = &key_funcation.key_ulock;break;
		case KEY_SHIFT:	 temp_fun = &key_funcation.key_shift;break;
		
		case KEY_UP:	 temp_fun = &key_funcation.key_up;break;
		case KEY_DOWN:	 temp_fun = &key_funcation.key_down;break;
		case KEY_LEFT:	 temp_fun = &key_funcation.key_left;break;
		case KEY_RIGHT:	 temp_fun = &key_funcation.key_right;break;
        
		case KEY_UP     & _KEY_LONG:	 temp_fun = &key_funcation.key_up;break;
		case KEY_DOWN   & _KEY_LONG:	 temp_fun = &key_funcation.key_down;break;
		case KEY_LEFT   & _KEY_LONG:	 temp_fun = &key_funcation.key_left;break;
		case KEY_RIGHT  & _KEY_LONG:	 temp_fun = &key_funcation.key_right;break;
        
		case KEY_UNLOCK & KEY_0:temp_fun = &key_funcation.key_clock_0;break;
		case KEY_F1 & KEY_0:temp_fun = &key_funcation.key_f1_0;break;
		case KEY_F1 & KEY_1:temp_fun = &key_funcation.key_f1_1;break;
		case KEY_F1 & KEY_4:temp_fun = &key_funcation.key_f1_4;break;
		
		case KEY_F0 & _KEY_UP:   temp_fun = &key_funcation.key_f0;break;
		case KEY_F1 & _KEY_UP:	 temp_fun = &key_funcation.key_f1;break;
		case KEY_F2 & _KEY_UP:	 temp_fun = &key_funcation.key_f2;break;
		case KEY_F3 & _KEY_UP:	 temp_fun = &key_funcation.key_f3;break;
		case KEY_F4 & _KEY_UP:	 temp_fun = &key_funcation.key_f4;break;
		case KEY_F5 & _KEY_UP:	 temp_fun = &key_funcation.key_f5;break;
		case KEY_F6 & _KEY_UP:	 temp_fun = &key_funcation.key_f6;break;
        
        default:break;
	}
    
    return temp_fun;
}
uint8_t get_function_key_st(uint32_t key, CS_ERR *err)
{
	KEY_DISPOSE_FUN *temp_fun;
	
    *err = CS_ERR_NONE;
    
	temp_fun = get_key_inf(key);
    
    if(temp_fun == NULL)
    {
        *err = CS_ERR_KEY_VALUE_INVALID;
        return 0;
    }
    
	return temp_fun->en;
}
void enable_function_key(uint32_t key, uint8_t st)
{
	KEY_DISPOSE_FUN *temp_fun;
    
	temp_fun = get_key_inf(key);
    
    if(temp_fun == NULL)
    {
        return;
    }
	
	temp_fun->en = !!st;
}

void register_key_dispose_fun(uint32_t key, KEY_DISPOSE_FUN *fun)
{
	KEY_DISPOSE_FUN *temp;
	
	temp = get_key_inf(key);
    
	if(NULL != temp)
	{
		if(NULL != fun)
		{
            memcpy(temp, fun, sizeof(KEY_DISPOSE_FUN));
		}
		else
		{
            memset(temp, 0, sizeof(KEY_DISPOSE_FUN));
		}
	}
}

static void exe_key_dispose_fun(KEY_DISPOSE_FUN *fun)
{
	if(NULL != fun)
	{
        memcpy(&golbal_key_info, fun, sizeof(KEY_DISPOSE_FUN));
	}
}

void dispose_funcation_key(uint32_t key)
{
	KEY_DISPOSE_FUN *temp;
	
	temp = get_key_inf_(key);
    
    exe_key_dispose_fun(temp);
}

void disable_system_fun_key_fun(void)
{
    uint32_t key_buf[]=
    {
        CODE_LEFT,
        CODE_RIGH,
        KEY_EXIT,
        KEY_ENTER,
        KEY_OFFSET,
//         KEY_UNLOCK,
//         KEY_SHIFT,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_F0 & _KEY_UP,
        KEY_F1 & _KEY_UP,
        KEY_F2 & _KEY_UP,
        KEY_F3 & _KEY_UP,
        KEY_F4 & _KEY_UP,
        KEY_F5 & _KEY_UP,
        KEY_F6 & _KEY_UP,
    };
    uint32_t size = ARRAY_SIZE(key_buf);
    int32_t i = 0;
    
    for(i = 0; i < size; i++)
    {
        register_key_dispose_fun(key_buf[i], NULL);
    }
}

void keyboard_str(uint32_t key)
{
#define KEY_ALL_VALUE "0+/-1ABC2DEF3GHI4JKL5MNO6PQR7STU8VWX9YZ ."
#define KEY_ALL_VALUE_ "0+/-1abc2def3ghi4jkl5mno6pqr7stu8vwx9yz ."
	uint8_t bufx[]={
	4,4,4,4,4,//0 1 2 3 4
	4,4,4,4,4,//5 6 7 8 9
	1,		  //.
	};
	uint8_t num_buf[]={
		0 ,  4,  8, 12, 16,//0 1 2 3 4
		20, 24, 28, 32, 36,//5 6 7 8 9
		40, 			   //.
	};
	static uint8_t count_time = 0;
	static uint8_t key_index = 0;
	static uint8_t c = 0;
	static uint32_t key_back = KEY_NONE;
	static uint8_t index_back = 0xff;
	static uint8_t next_bit = 0;
	static uint8_t cour = 0;
	static uint8_t flag = 0;
	static uint8_t input_st = 0;
	static uint8_t temp_cur_num = 0;
    WM_HMEM handle = 0;
    uint8_t g_key_char;
	
    handle = get_cur_edit_handle();
	
	if(key != KEY_NONE)
	{
		count_time = 0;
		
		if(key == key_back)
		{
			c = (c + 1) % temp_cur_num;//buf[key_index];//4;
		}
		else
		{
			key_back = key;
			c = 0;
		}
	}
	
	if(input_st == 1)
	{
		if(++count_time > 80)
		{
			key_back = KEY_NONE;
			c = 0;
			count_time = 0;
			next_bit = 1;
			input_st = 2;
		}
	}
	
	key_index = 0xff;
	
	switch(key)
	{
		case KEY_0:
			key_index = 0;// * 4 + c;
			break;
		case KEY_1:
			key_index = 1;// * 4 + c;
			break;
		case KEY_2:
			key_index = 2;// * 4 + c;
			break;
		case KEY_3:
			key_index = 3;// * 4 + c;
			break;
		case KEY_4:
			key_index = 4;// * 4 + c;
			break;
		case KEY_5:
			key_index = 5;// * 4 + c;
			break;
		case KEY_6:
			key_index = 6;// * 4 + c;
			break;
		case KEY_7:
			key_index = 7;// * 4 + c;
			break;
		case KEY_8:
			key_index = 8;// * 4 + c;
			break;
		case KEY_9:
			key_index = 9;// * 4 + c;
			break;
		case KEY_POINT:
			key_index = 10;// * 4;
			break;
		default:
			break;
	}
	
	if(key_index != 0xff)
	{
		uint8_t buf[20] = {0};
		uint8_t temp = 0;
        
		if(handle == 0)
		{
            return;
		}
		
		EDIT_GetText(handle, (char*)buf, 20);
		cour = EDIT_GetCursorCharPos(handle);
		
		if(key_index != index_back)
		{
			index_back = key_index;
			EDIT_SetCursorAtChar(handle, cour + 1);
		}
		
		cour = EDIT_GetCursorCharPos(handle);
		
		temp_cur_num = bufx[key_index];
		temp = num_buf[key_index] + c;
		
		g_key_char = (ui_flag.shift_flag?KEY_ALL_VALUE:KEY_ALL_VALUE_)[temp];
		buf[cour] = g_key_char;
		EDIT_SetText(handle, (const char*)buf);
		EDIT_SetCursorAtChar(handle, ++cour);
		flag = 1;
		count_time = 0;
		input_st = 1;
	}
	
	if(input_st == 1)
	{
		if(flag)
		{
			flag = 0;
			EDIT_SetCursorAtChar(handle, cour - 1);
		}
		else if(next_bit)
		{
			next_bit = 0;
		}
	}
	else if(input_st == 2)
	{
		EDIT_SetCursorAtChar(handle, cour);
		input_st = 0;
	}
}

void keyboard_password(uint32_t key)
{
	#define ASCII_NUM 	"0123456789."
	uint8_t index = 0xff;
	static uint8_t cour = 0;
    WM_HMEM handle = 0;
    uint8_t buf[20] = {0};
    uint8_t max_len;
    uint8_t len;
	
    handle = get_cur_edit_handle();
	
    if(handle == 0)
    {
        return;
    }
    
	switch(key)
	{
		case KEY_0:index=0;break;
		case KEY_1:index=1;break;
		case KEY_2:index=2;break;
		case KEY_3:index=3;break;
		case KEY_4:index=4;break;
		case KEY_5:index=5;break;
		case KEY_6:index=6;break;
		case KEY_7:index=7;break;
		case KEY_8:index=8;break;
		case KEY_9:index=9;break;
        default:index = 0xff;break;
	}
	
	if(index != 0xff)
	{
        EDIT_GetText(handle, (char*)buf, 10);
        cour = EDIT_GetCursorCharPos(handle);
        max_len = pwd_max_len();
        add_a_char_into_edit_str(ASCII_NUM[index], cour, max_len);
        len = strlen((const char*)buf);
        
        if(len < max_len)
        {
            strcat((char*)buf, "*");
            EDIT_SetText(handle, (const char*)buf);
            EDIT_SetCursorAtChar(handle, cour + 1);
        }
	}
}

void keyboard_num(uint32_t key)
{
	#define ASCII_NUM 	"0123456789."
	
	switch(key)
	{
		case KEY_0:
			GUI_SendKeyMsg(ASCII_NUM[0], 1);
			break;
		case KEY_1:
			GUI_StoreKeyMsg(ASCII_NUM[1], 1);
			break;
		case KEY_2:
			GUI_SendKeyMsg(ASCII_NUM[2], 1);
			break;
		case KEY_3:
			GUI_SendKeyMsg(ASCII_NUM[3], 1);
			break;
		case KEY_4:
			GUI_SendKeyMsg(ASCII_NUM[4], 1);
			break;
		case KEY_5:
			GUI_SendKeyMsg(ASCII_NUM[5], 1);
			break;
		case KEY_6:
			GUI_SendKeyMsg(ASCII_NUM[6], 1);
			break;
		case KEY_7:
			GUI_SendKeyMsg(ASCII_NUM[7], 1);
			break;
		case KEY_8:
			GUI_SendKeyMsg(ASCII_NUM[8], 1);
			break;
		case KEY_9:
			GUI_SendKeyMsg(ASCII_NUM[9], 1);
			break;
		case KEY_POINT:
			GUI_SendKeyMsg(ASCII_NUM[10], 1);
			break;
		default:
			break;
	}
}

void set_scan_key_custom_fun(void (*fun)())
{
	scan_key_custom_fun = fun;
}

void keyboard_fun_str(uint32_t key_value)
{
	keyboard_str(key_value);
}

void keyboard_fun_num(uint32_t key_value)
{
	keyboard_num(key_value);
}

void keyboard_fun_pwd(uint32_t key_value)
{
    keyboard_password(key_value);
}

void scan_key_task(void)
{
	uint32_t key_value = KEY_NONE;
	OS_ERR	err;
	
//	(void)p_arg;		/* 避免编译器告警 */
	init_keyboard();
	set_scan_key_custom_fun(NULL);
// 	_create_menu_key_win();
	
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 10, 0, &err);
		report_key_value();
		key_value =  get_key_value();
		
// 		if(NULL != scan_direct_key_fun)
// 		{
// 			scan_direct_key_fun(key_value);
// 		}
		
// 		scan_direct_key_function(key_value);
		//系统功能按键处理
		dispose_funcation_key(key_value);
		
		/* 根据不同界面定制按键扫描函数 */
		if(NULL != scan_key_custom_fun)
		{
			scan_key_custom_fun(key_value);
		}
		
		/* 菜单按键扫描 */
		if(NULL != global_fun_key_dispose)
		{
			global_fun_key_dispose(key_value);
		}
	}					 
}

/************************ (C) COPYRIGHT Nanjing Changsheng *****END OF FILE****/
