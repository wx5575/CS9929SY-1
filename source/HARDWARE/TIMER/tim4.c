
#include "os.h"
#include "tim4.h"

//使能定时器4,使能中断.
void tim4_init(uint16_t arr,uint16_t psc)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟

	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化定时器4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//外部中断4
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置NVIC
}

//下面这两个函数,非USMART函数,放到这里,仅仅方便移植. 
//定时器4中断服务程序	 
void TIM4_IRQHandler(void)
{
	OSIntEnter();
    
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)//溢出中断
	{
        
	}
    
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
    
	OSIntExit();
}
