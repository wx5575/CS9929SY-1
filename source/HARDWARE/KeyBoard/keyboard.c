/**
  ******************************************************************************
  * @file    keyboard.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   键盘管理模块
  ******************************************************************************
  */
#define KEY_GLOBALS

#include 	"keyboard.h"
#include    "key_led_buzzer.h"
#include    "start_stop_key.h"


typedef uint8_t (*IS_FUN)(uint32_t);

int16_t key_buzzer_time = KEY_DELAY_NORMAL;
static uint32_t cur_key_value;

static uint8_t IsKeyDown_key_start(uint32_t key_value)   {return START_PIN==RESET;}
static uint8_t IsKeyDown_key_stop(uint32_t key_value)    {return STOP_PIN==RESET; }

static uint8_t IsKeyDown_key_DF(uint32_t key_value)   {return (key_value == KEY_DIS_FORMAT);	}

static uint8_t IsKeyDown_key_F1(uint32_t key_value)   {return (key_value == KEY_F1);	}
static uint8_t IsKeyDown_key_F2(uint32_t key_value)   {return (key_value == KEY_F2);	}
static uint8_t IsKeyDown_key_F3(uint32_t key_value)   {return (key_value == KEY_F3);	}
static uint8_t IsKeyDown_key_F4(uint32_t key_value)   {return (key_value == KEY_F4);	}
static uint8_t IsKeyDown_key_F5(uint32_t key_value)   {return (key_value == KEY_F5);	}
static uint8_t IsKeyDown_key_F6(uint32_t key_value)   {return (key_value == KEY_F6);	}

static uint8_t IsKeyDown_key_up(uint32_t key_value) 	{return (key_value == KEY_UP);	}
static uint8_t IsKeyDown_key_down(uint32_t key_value)	{return (key_value == KEY_DOWN);	}
static uint8_t IsKeyDown_key_left(uint32_t key_value) {return (key_value == KEY_LEFT);	}
static uint8_t IsKeyDown_key_right(uint32_t key_value){return (key_value == KEY_RIGHT);	}

static uint8_t IsKeyDown_key_exit(uint32_t key_value) {return (key_value == KEY_EXIT);	}
static uint8_t IsKeyDown_key_enter(uint32_t key_value){return (key_value == KEY_ENTER);	}
static uint8_t IsKeyDown_key_offset(uint32_t key_value) {return (key_value == KEY_OFFSET);	}
static uint8_t IsKeyDown_key_unlock(uint32_t key_value){return (key_value == KEY_UNLOCK);	}

static uint8_t IsKeyDown_key_shift(uint32_t key_value){return (key_value == KEY_SHIFT);	}

static uint8_t IsKeyDown_key_0(uint32_t key_value) {return (key_value == KEY_0);	}
static uint8_t IsKeyDown_key_1(uint32_t key_value) {return (key_value == KEY_1);	}
static uint8_t IsKeyDown_key_2(uint32_t key_value) {return (key_value == KEY_2);	}
static uint8_t IsKeyDown_key_3(uint32_t key_value) {return (key_value == KEY_3);	}
static uint8_t IsKeyDown_key_4(uint32_t key_value) {return (key_value == KEY_4);	}
static uint8_t IsKeyDown_key_5(uint32_t key_value) {return (key_value == KEY_5);	}
static uint8_t IsKeyDown_key_6(uint32_t key_value) {return (key_value == KEY_6);	}
static uint8_t IsKeyDown_key_7(uint32_t key_value) {return (key_value == KEY_7);	}
static uint8_t IsKeyDown_key_8(uint32_t key_value) {return (key_value == KEY_8);	}
static uint8_t IsKeyDown_key_9(uint32_t key_value) {return (key_value == KEY_9);	}
static uint8_t IsKeyDown_key_point(uint32_t key_value) {return (key_value == KEY_POINT);	}

static uint8_t IsKeyDown_key_01(uint32_t key_value) {return (key_value == (KEY_0 & KEY_1));}
static uint8_t IsKeyDown_key_02(uint32_t key_value) {return (key_value == (KEY_0 & KEY_2));}//注意！(KEY_0 & KEY_2)必须加括号否则就会有只跟第一个键值比较的错误情况出现
static uint8_t IsKeyDown_key_03(uint32_t key_value) {return (key_value == (KEY_0 & KEY_3));}
static uint8_t IsKeyDown_key_04(uint32_t key_value) {return (key_value == (KEY_0 & KEY_4));}
static uint8_t IsKeyDown_key_05(uint32_t key_value) {return (key_value == (KEY_0 & KEY_5));}
static uint8_t IsKeyDown_key_06(uint32_t key_value) {return (key_value == (KEY_0 & KEY_6));}

static uint8_t IsKeyDown_key_lock_0(uint32_t key_value) {return (key_value == (KEY_UNLOCK & KEY_0));}
static uint8_t IsKeyDown_key_lock_1(uint32_t key_value) {return (key_value == (KEY_UNLOCK & KEY_1));}
static uint8_t IsKeyDown_key_lock_offset(uint32_t key_value) {return (key_value == (KEY_UNLOCK & KEY_OFFSET));}
static uint8_t IsKeyDown_key_lock_enter(uint32_t key_value) {return (key_value == (KEY_UNLOCK & KEY_ENTER));}
static uint8_t IsKeyDown_key_lock_exit(uint32_t key_value) {return (key_value == (KEY_UNLOCK & KEY_EXIT));}
static uint8_t IsKeyDown_key_F1_0(uint32_t key_value) {return (key_value == (KEY_F1 & KEY_0));}
static uint8_t IsKeyDown_key_F1_1(uint32_t key_value) {return (key_value == (KEY_F1 & KEY_1));}
static uint8_t IsKeyDown_key_F1_4(uint32_t key_value) {return (key_value == (KEY_F1 & KEY_4));}
static uint8_t IsKeyDown_key_F4_8(uint32_t key_value) {return (key_value == (KEY_F4 & KEY_8));}
static uint8_t IsKeyDown_key_F4_9(uint32_t key_value) {return (key_value == (KEY_F4 & KEY_9));}
static uint8_t IsKeyDown_key_F4_5(uint32_t key_value) {return (key_value == (KEY_F4 & KEY_5));}

static KEY_STRUCT s_Key_start;		/* KEY_START */
static KEY_STRUCT s_Key_stop;		/* KEY_STOP */

static KEY_STRUCT s_Key_df; /* KEY_DIS_FORMAT */

static KEY_STRUCT s_Key_f1; /* KEY_F1 */
static KEY_STRUCT s_Key_f2;	/* KEY_F2 */
static KEY_STRUCT s_Key_f3;	/* KEY_F3 */
static KEY_STRUCT s_Key_f4;	/* KEY_F4 */
static KEY_STRUCT s_Key_f5;	/* KEY_F5 */
static KEY_STRUCT s_Key_f6;	/* KEY_F6 */

static KEY_STRUCT s_Key_up;		/* KEY_UP */
static KEY_STRUCT s_Key_down;	/* KEY_DOWN */
static KEY_STRUCT s_Key_left;	/* KEY_LEFT */
static KEY_STRUCT s_Key_right;	/* KEY_RIGHT */

static KEY_STRUCT s_Key_exit;	/* KEY_EXIT */
static KEY_STRUCT s_Key_enter;	/* KEY_ENTER */
static KEY_STRUCT s_Key_offset;	/* KEY_OFFSET */
static KEY_STRUCT s_Key_unlock;	/* KEY_UNLOCK */

static KEY_STRUCT s_Key_shift;	/* KEY_SHIFT */

static KEY_STRUCT s_Key_0;		/* KEY_0 */
static KEY_STRUCT s_Key_1;		/* KEY_1 */
static KEY_STRUCT s_Key_2;		/* KEY_2 */
static KEY_STRUCT s_Key_3;		/* KEY_3 */
static KEY_STRUCT s_Key_4;		/* KEY_4 */
static KEY_STRUCT s_Key_5;		/* KEY_5 */
static KEY_STRUCT s_Key_6;		/* KEY_6 */
static KEY_STRUCT s_Key_7;		/* KEY_7 */
static KEY_STRUCT s_Key_8;		/* KEY_8 */
static KEY_STRUCT s_Key_9;		/* KEY_9 */
static KEY_STRUCT s_Key_point;	/* KEY_POINT */

static KEY_STRUCT s_Key_01;		/* KEY_1 & KEY_0 */
static KEY_STRUCT s_Key_02;		/* KEY_1 & KEY_2 */
static KEY_STRUCT s_Key_03;		/* KEY_1 & KEY_3 */
static KEY_STRUCT s_Key_04;		/* KEY_1 & KEY_4 */
static KEY_STRUCT s_Key_05;		/* KEY_1 & KEY_5 */
static KEY_STRUCT s_Key_06;		/* KEY_1 & KEY_6 */

static KEY_STRUCT s_Key_lock_0;		/* KEY_UNLOCK & KEY_0 */
static KEY_STRUCT s_Key_lock_1;		/* KEY_UNLOCK & KEY_1 */
static KEY_STRUCT s_Key_lock_offset;/* KEY_UNLOCK & KEY_OFFSET */
static KEY_STRUCT s_Key_lock_enter; /* KEY_UNLOCK & KEY_ENTER */
static KEY_STRUCT s_Key_lock_exit;  /* KEY_UNLOCK & KEY_EXIT */
static KEY_STRUCT s_Key_F4_8;		/* KEY_F4 & KEY_8 */
static KEY_STRUCT s_Key_F4_9;		/* KEY_F4 & KEY_9 */
static KEY_STRUCT s_Key_F4_5;		/* KEY_F4 & KEY_5 */

static KEY_STRUCT s_Key_F1_0;		/* KEY_F1 & KEY_0 */
static KEY_STRUCT s_Key_F1_1;		/* KEY_F1 & KEY_1 */
static KEY_STRUCT s_Key_F1_4;		/* KEY_F1 & KEY_4 */

static KEY_STRUCT *p_key_pool[100];
static uint16_t key_count;
static void(*send_key_msg_fun)(uint32_t *);

/*
 * 函数名：init_key_manage_inf
 * 描述  ：初始化按键
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_key_info(KEY_STRUCT * key_t, uint32_t k_down, uint32_t k_up, uint32_t k_long, uint8_t type, IS_FUN fun)
{
	key_t->IsKeyDownFunc = fun;	/* 判断按键按下的函数 */
	key_t->FilterTime = BUTTON_FILTER_TIME;	/* 按键滤波时间 */
	key_t->LongTime = BUTTON_LONG_TIME; /* 长按时间 */
	key_t->Count = key_t->FilterTime / 2; /* 计数器设置为滤波时间的一半 */
	key_t->State = 0; /* 按键缺省状态，0为未按下 */
	key_t->KeyCodeDown = k_down; /* 按键按下的键值代码 */
	key_t->KeyCodeUp = k_up; /* 按键弹起的键值代码 */
	key_t->KeyCodeLong = k_long; /* 按键被持续按下的键值代码 */
    key_t->type = type;/* 按键类型 */
    
    p_key_pool[key_count++] = key_t;/* 将按键信息记录到按键池中 */
}

void init_key_manage_inf(void)
{
	/************************启动按键******************************/
    init_key_info(&s_Key_start  , KEY_START , 0, 0, SINGLE_KEY, IsKeyDown_key_start);
	/************************复位按键******************************/
    init_key_info(&s_Key_stop   , KEY_STOP  , 0, 0, SINGLE_KEY, IsKeyDown_key_stop);
	
	/**********************组合键****************************/
    init_key_info(&s_Key_F1_0   , 0, KEY_F1 & KEY_0, 0, COMBINATION_KEY, IsKeyDown_key_F1_0);
    init_key_info(&s_Key_F1_1   , 0, KEY_F1 & KEY_1, 0, COMBINATION_KEY, IsKeyDown_key_F1_1);
    init_key_info(&s_Key_F1_4   , 0, KEY_F1 & KEY_4, 0, COMBINATION_KEY, IsKeyDown_key_F1_4);
    
    init_key_info(&s_Key_F4_5   , 0, KEY_F4 & KEY_5, 0, COMBINATION_KEY, IsKeyDown_key_F4_5);
    init_key_info(&s_Key_F4_9   , 0, KEY_F4 & KEY_9, 0, COMBINATION_KEY, IsKeyDown_key_F4_9);
    init_key_info(&s_Key_F4_8   , 0, KEY_F4 & KEY_8, 0, COMBINATION_KEY, IsKeyDown_key_F4_8);
    
    init_key_info(&s_Key_lock_1 , 0, KEY_1 & KEY_UNLOCK, 0, COMBINATION_KEY, IsKeyDown_key_lock_1);
    init_key_info(&s_Key_lock_0 , 0, KEY_0 & KEY_UNLOCK, 0, COMBINATION_KEY, IsKeyDown_key_lock_0);
    init_key_info(&s_Key_lock_offset , 0, KEY_UNLOCK & KEY_OFFSET, 0, COMBINATION_KEY, IsKeyDown_key_lock_offset);
    init_key_info(&s_Key_lock_enter , 0, KEY_UNLOCK & KEY_ENTER, 0, COMBINATION_KEY, IsKeyDown_key_lock_enter);
    init_key_info(&s_Key_lock_exit , 0, KEY_UNLOCK & KEY_EXIT, 0, COMBINATION_KEY, IsKeyDown_key_lock_exit);
	
    init_key_info(&s_Key_06   , 0, KEY_0 & KEY_6, 0, COMBINATION_KEY, IsKeyDown_key_06);
    init_key_info(&s_Key_05   , 0, KEY_0 & KEY_5, 0, COMBINATION_KEY, IsKeyDown_key_05);
    init_key_info(&s_Key_04   , 0, KEY_0 & KEY_4, 0, COMBINATION_KEY, IsKeyDown_key_04);
    init_key_info(&s_Key_03   , 0, KEY_0 & KEY_3, 0, COMBINATION_KEY, IsKeyDown_key_03);
    init_key_info(&s_Key_02   , 0, KEY_0 & KEY_2, 0, COMBINATION_KEY, IsKeyDown_key_02);
    init_key_info(&s_Key_01   , 0, KEY_0 & KEY_1, 0, COMBINATION_KEY, IsKeyDown_key_01);
	
	/***************************普通单按键********************************/
    init_key_info(&s_Key_df   , KEY_DIS_FORMAT & _KEY_DOWN, KEY_DIS_FORMAT & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_DF);
    init_key_info(&s_Key_f1   , KEY_F1 & _KEY_DOWN, KEY_F1 & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_F1);
    init_key_info(&s_Key_f2   , KEY_F2 & _KEY_DOWN, KEY_F2 & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_F2);
    init_key_info(&s_Key_f3   , KEY_F3 & _KEY_DOWN, KEY_F3 & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_F3);
    init_key_info(&s_Key_f4   , KEY_F4 & _KEY_DOWN, KEY_F4 & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_F4);
    init_key_info(&s_Key_f5   , KEY_F5 & _KEY_DOWN, KEY_F5 & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_F5);
    init_key_info(&s_Key_f6   , KEY_F6 & _KEY_DOWN, KEY_F6 & _KEY_UP, 0, SINGLE_KEY, IsKeyDown_key_F6);
	
    init_key_info(&s_Key_up		, KEY_UP    ,   0, KEY_UP    & _KEY_LONG, SINGLE_KEY, IsKeyDown_key_up);
    init_key_info(&s_Key_down	, KEY_DOWN  , 	0, KEY_DOWN  & _KEY_LONG, SINGLE_KEY, IsKeyDown_key_down);
    init_key_info(&s_Key_left	, KEY_LEFT  , 	0, KEY_LEFT  & _KEY_LONG, SINGLE_KEY, IsKeyDown_key_left);
    init_key_info(&s_Key_right	, KEY_RIGHT , 	0, KEY_RIGHT & _KEY_LONG, SINGLE_KEY, IsKeyDown_key_right);
	
    init_key_info(&s_Key_exit   , 0, KEY_EXIT   , 0, SINGLE_KEY, IsKeyDown_key_exit);
    init_key_info(&s_Key_enter  , 0, KEY_ENTER  , 0, SINGLE_KEY, IsKeyDown_key_enter);
    init_key_info(&s_Key_offset , 0, KEY_OFFSET , 0, SINGLE_KEY, IsKeyDown_key_offset);
    init_key_info(&s_Key_unlock , 0, KEY_UNLOCK , 0, SINGLE_KEY, IsKeyDown_key_unlock);
	
    init_key_info(&s_Key_shift  , 0, KEY_SHIFT , 0, SINGLE_KEY, IsKeyDown_key_shift);
    
    init_key_info(&s_Key_0  , 0, KEY_0, 0, SINGLE_KEY, IsKeyDown_key_0);
    init_key_info(&s_Key_1  , 0, KEY_1, 0, SINGLE_KEY, IsKeyDown_key_1);
    init_key_info(&s_Key_2  , 0, KEY_2, 0, SINGLE_KEY, IsKeyDown_key_2);
    init_key_info(&s_Key_3  , 0, KEY_3, 0, SINGLE_KEY, IsKeyDown_key_3);
    init_key_info(&s_Key_4  , 0, KEY_4, 0, SINGLE_KEY, IsKeyDown_key_4);
    init_key_info(&s_Key_5  , 0, KEY_5, 0, SINGLE_KEY, IsKeyDown_key_5);
    init_key_info(&s_Key_6  , 0, KEY_6, 0, SINGLE_KEY, IsKeyDown_key_6);
    init_key_info(&s_Key_7  , 0, KEY_7, 0, SINGLE_KEY, IsKeyDown_key_7);
    init_key_info(&s_Key_8  , 0, KEY_8, 0, SINGLE_KEY, IsKeyDown_key_8);
    init_key_info(&s_Key_9  , 0, KEY_9, 0, SINGLE_KEY, IsKeyDown_key_9);
	
    init_key_info(&s_Key_point  , 0, KEY_POINT, 0, SINGLE_KEY, IsKeyDown_key_point);
}

void register_key_send_msg_fun(void(*fun)(uint32_t *))
{
    send_key_msg_fun = fun;
}
/*
 * 函数名：DetectKey
 * 描述  ：监控按键是否按下 如果按下就对响应的标志位置位
 * 输入  ：p 指向当前监控按键的结构
 * 输出  ：无
 * 返回  ：无
 */
static void DetectKey(KEY_STRUCT *p, uint32_t key_value)
{
	/* 如果没有初始化按键函数，则报错*/
	if (p->IsKeyDownFunc == NULL)
	{
		return;
	}
    
	if (p->IsKeyDownFunc(key_value))
	{
		if (p->Count < p->FilterTime)
		{
			p->Count = p->FilterTime;
		}
		else if(p->Count < 2 * p->FilterTime)
		{
			p->Count++;
		}
		else
		{
			if (p->State == 0)
			{
				p->State = 1;
                
                if(p->KeyCodeDown != KEY_START &&  p->KeyCodeDown != KEY_STOP)
                {
                    BUZZER_ON_T(KEY_BUZZER_TIME);
                }
                
 				
// 				if(BUZZER_EN)
//                 {
//                     if(p != &s_Key_start && p != &s_Key_stop)/* 启动键不能响 */
//                     {
//                         if(sys_par.buzzer_sw)
//                         {
//                             BUZZER_SOUND(key_buzzer_time);
//                         }
//                     }
//                 }
				/* 发送按钮按下的消息 */
				if (p->KeyCodeDown > 0)
				{
					/* 键值放入按键FIFO */
                    if(send_key_msg_fun != NULL)
                    {
                        send_key_msg_fun(&p->KeyCodeDown);
                    }
				}
			}
			
			if (p->LongTime > 0)
			{
				if (p->LongCount < p->LongTime)
				{
					++p->LongCount;
				}
                /* 发送按钮持续按下的消息 */
                else
                {
					/* 键值放入按键FIFO */
					++p->LongCount;
                    
                    if(p->LongCount % 30 == 0)
                    {
                        if(send_key_msg_fun != NULL)
                        {
                            send_key_msg_fun(&p->KeyCodeLong);
                        }
                    }
                }
			}
		}	
	}
	else if(cur_key_value == KEY_NONE)
	{
		if(p->Count > p->FilterTime)
		{
			p->Count = p->FilterTime;
		}
		else if(p->Count != 0)
		{
			p->Count--;
		}
		else
		{
			if (p->State == 1)
			{
				p->State = 0;
                p->LongCount = 0;
                
				/* 发送按钮弹起的消息 */
				if (p->KeyCodeUp > 0)
				{
					/* 键值放入按键FIFO */
                    if(send_key_msg_fun != NULL)
                    {
                        send_key_msg_fun(&p->KeyCodeUp);
                    }
				}
			}
		}
		p->LongCount = 0;
	}
}

/*
 * 函数名：DetectKey
 * 描述  ：监控组合键是否按下 如果按下就对响应的标志位置位
 * 输入  ：p 指向当前监控按键的结构
 * 输出  ：无
 * 返回  ：无
 */
static void Det_combination(KEY_STRUCT *p, uint32_t key_value)
{
//    OS_ERR  err;
    int32_t i = 0;
    
	/* 如果没有初始化按键函数，则不处理 */
	if (p->IsKeyDownFunc == NULL)
	{
		return;//2014.10.28
	}
	
	/* 检测按键按下 */
	if (p->IsKeyDownFunc(key_value))
	{
		/* 滤波 */
		if (p->Count < p->FilterTime)
		{
			p->Count = p->FilterTime;
		}
		else if(p->Count < 2 * p->FilterTime)
		{
			p->Count++;
		}
		/* 标记按键已按下 */
		else
		{
			if (p->State == 0)
			{
				p->State = 1;
				
				BUZZER_ON_T(KEY_BUZZER_TIME);
				/* 按键蜂鸣 */
// 				if(BUZZER_EN)
//                 {
//                     if(p != &s_Key_start && p != &s_Key_stop)/* 启动键不能响 */
//                     {
//                         if(sys_par.buzzer_sw)
//                         {
//                             BUZZER_SOUND(key_buzzer_time);
//                         }
//                     }
//                 }
			}
		}
	}
	
	if(p->State)
	{
        for(i = 0; i < key_count; i++)
        {
            if(p_key_pool[i] != p)
            {
                p_key_pool[i]->State = 0;
            }
        }
        
        /* 等待按键释放 */
		while(scan_keyboard() != KEY_NONE)
		{
//            OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_DLY,&err);
		}
        
        p->State = 0;
        
        if(send_key_msg_fun != NULL)
        {
            send_key_msg_fun(&p->KeyCodeUp);
        }
	}
}


void init_keyboard(void)
{
    init_key_manage_inf();
}
/*
 * 函数名：scan_keyboard
 * 描述  ：键盘扫描 
 * 输入  ：无
 * 输出  ：无
 * 返回  ：返回扫描的键码值
 */
uint32_t scan_keyboard(void)
{
	uint32_t data;
    
    data = read_key_value();
    
	return data;
}

void check_key_st(KEY_STRUCT *p, uint32_t key_value)
{
    if(p->type == COMBINATION_KEY)
    {
        Det_combination(p, key_value);
    }
    else
    {
        DetectKey(p, key_value);
    }
}
/*
 * 函数名：report_key_value
 * 描述  ：用户程序通过此函数来监控是否有按键按下 如果有按下就记录到按键缓冲中
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void report_key_value(void)
{
    int32_t i = 0;
    
    for(i = 0; i < key_count; i++)
    {
		if(i % 3 == 0)
		{
			cur_key_value = scan_keyboard();
		}
        
        check_key_st(p_key_pool[i], cur_key_value);
    }
}

/**
  * @brief  模拟发送启动键
  * @param  无
  * @retval 无
  */
void simulate_send_start_key(void)
{
    if(send_key_msg_fun != NULL)
    {
        send_key_msg_fun(&s_Key_start.KeyCodeDown);
    }
}

/**
  * @brief  模拟发送复位键
  * @param  无
  * @retval 无
  */
void simulate_send_stop_key(void)
{
    if(send_key_msg_fun != NULL)
    {
        send_key_msg_fun(&s_Key_stop.KeyCodeDown);
    }
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
