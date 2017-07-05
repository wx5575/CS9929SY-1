/**
  ******************************************************************************
  * @file    key_fun_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键功能管理,按键功能的注册和查询
  ******************************************************************************
  */

#define KEY_DISPOSE_GLOBALS
#include "key_fun_manage.h"
#include "keyboard.h"
#include "string.h"
#include "tools.h"
#include "os.h"
#include "GUI.H"
#include "EDIT.h"

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
        
		case KEY_UP     & _KEY_LONG: temp_fun = &key_funcation.key_up;break;
		case KEY_DOWN   & _KEY_LONG: temp_fun = &key_funcation.key_down;break;
		case KEY_LEFT   & _KEY_LONG: temp_fun = &key_funcation.key_left;break;
		case KEY_RIGHT  & _KEY_LONG: temp_fun = &key_funcation.key_right;break;
        
		case KEY_F1 & KEY_0: temp_fun = &key_funcation.key_f1_0;break;
		case KEY_F1 & KEY_1: temp_fun = &key_funcation.key_f1_1;break;
		case KEY_F1 & KEY_4: temp_fun = &key_funcation.key_f1_4;break;
        
		case KEY_UNLOCK & KEY_0:      temp_fun = &key_funcation.key_clock_0;break;
		case KEY_UNLOCK & KEY_OFFSET: temp_fun = &key_funcation.key_clock_offset;break;
		case KEY_UNLOCK & KEY_ENTER:  temp_fun = &key_funcation.key_clock_enter;break;
		case KEY_UNLOCK & KEY_EXIT:   temp_fun = &key_funcation.key_clock_exit;break;
		
		case KEY_F0 & _KEY_UP: temp_fun = &key_funcation.key_f0_up;break;
		case KEY_F1 & _KEY_UP: temp_fun = &key_funcation.key_f1_up;break;
		case KEY_F2 & _KEY_UP: temp_fun = &key_funcation.key_f2_up;break;
		case KEY_F3 & _KEY_UP: temp_fun = &key_funcation.key_f3_up;break;
		case KEY_F4 & _KEY_UP: temp_fun = &key_funcation.key_f4_up;break;
		case KEY_F5 & _KEY_UP: temp_fun = &key_funcation.key_f5_up;break;
		case KEY_F6 & _KEY_UP: temp_fun = &key_funcation.key_f6_up;break;
        
		case KEY_F0 & _KEY_DOWN: temp_fun = &key_funcation.key_f0_down;break;
		case KEY_F1 & _KEY_DOWN: temp_fun = &key_funcation.key_f1_down;break;
		case KEY_F2 & _KEY_DOWN: temp_fun = &key_funcation.key_f2_down;break;
		case KEY_F3 & _KEY_DOWN: temp_fun = &key_funcation.key_f3_down;break;
		case KEY_F4 & _KEY_DOWN: temp_fun = &key_funcation.key_f4_down;break;
		case KEY_F5 & _KEY_DOWN: temp_fun = &key_funcation.key_f5_down;break;
		case KEY_F6 & _KEY_DOWN: temp_fun = &key_funcation.key_f6_down;break;
        default: break;
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
KEY_CB_FUN get_function_key_dispose_fun(uint32_t key, CS_ERR *err)
{
	KEY_DISPOSE_FUN *temp_fun;
	
    *err = CS_ERR_NONE;
    
	temp_fun = get_key_inf(key);
    
    if(temp_fun == NULL)
    {
        *err = CS_ERR_KEY_VALUE_INVALID;
        return 0;
    }
    
	return temp_fun->fun;
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
    
	temp = get_key_inf(key);
    
    exe_key_dispose_fun(temp);
}

void disable_system_fun_key_fun(void)
{
    uint32_t key_buf[]=
    {
        CODE_LEFT,
        CODE_RIGH,
//        KEY_EXIT,
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

void backup_key_funcation_inf(void)
{
    memcpy(&key_funcation_bk, &key_funcation, sizeof(KEY_FUNCATION));
}
void recover_key_funcation_inf(void)
{
    memcpy(&key_funcation, &key_funcation_bk, sizeof(KEY_FUNCATION));
}

/************************ (C) COPYRIGHT Nanjing Changsheng *****END OF FILE****/
