/**
  ******************************************************************************
  * @file    tim3.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   定时器3驱动层定义
  ******************************************************************************
  */
#include "os.h"
#include "tim3.h"


#define TIM3_MAX_SERVER_FUN     20 ///< tim3最大的服务函数的个数

/**
  * @brief  定时器服务函数指针
  */
typedef void (*TIM_SERVER_FUN)(void);

/**
  * @brief  定时器3服务函数指针池
  */
static TIM_SERVER_FUN  tim_server_fun_pool[TIM3_MAX_SERVER_FUN];


/**
  * @brief  通用定时器3中断初始化 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
  *         Ft=定时器工作频率,单位:Mhz
  * @param  [in] arr 自动重装值。
  * @param  [in] psc 时钟预分频数
  * @retval 无
  */
void tim3_init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  注册定时器3的服务函数
  * @param  [in] fun 定时器服务函数
  * @retval 0 成功 1表示失败定时器服务函数池已满
  */
uint8_t register_tim3_server_fun(TIM_SERVER_FUN fun)
{
    int32_t i = 0;
    
    for(i = 0; i < TIM3_MAX_SERVER_FUN; i++)
    {
        if(tim_server_fun_pool[i] == NULL)
        {
            tim_server_fun_pool[i] = fun;
            break;
        }
    }
    
    if(i == TIM3_MAX_SERVER_FUN)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  定时器3中断服务函数
  * @param  无
  * @retval 无
  */
void TIM3_IRQHandler(void)
{
    int32_t i = 0;
    
	OSIntEnter();
    
    for(i = 0; i < TIM3_MAX_SERVER_FUN; i++)
    {
        /* 执行服务函数 */
        if(tim_server_fun_pool[i] != NULL)
        {
            tim_server_fun_pool[i]();
        }
        /* 遇到第一个NULL后面全为NULL所以退出 */
        else
        {
            break;
        }
    }
    
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
    
	OSIntExit();
}


    

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
