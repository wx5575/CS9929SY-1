
#include "os.h"
#include "tim3.h"

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
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
typedef void (*TIM_SERVER_FUN)(void);
#define TIM3_MAX_SERVER_FUN     20 //tim3最大的服务函数的个数
TIM_SERVER_FUN  tim_server_fun[TIM3_MAX_SERVER_FUN];

uint8_t register_tim3_server_fun(TIM_SERVER_FUN fun)
{
    int32_t i = 0;
    
    for(i = 0; i < TIM3_MAX_SERVER_FUN; i++)
    {
        if(tim_server_fun[i] == NULL)
        {
            tim_server_fun[i] = fun;
            break;
        }
    }
    
    if(i == TIM3_MAX_SERVER_FUN)
    {
        return 1;
    }
    
    return 0;
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    int32_t i = 0;
    
	OSIntEnter();
    
    for(i = 0; i < TIM3_MAX_SERVER_FUN; i++)
    {
        if(tim_server_fun[i] != NULL)
        {
            tim_server_fun[i]();
        }
    }
    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
        
	}
    
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
    
	OSIntExit();
}


    
