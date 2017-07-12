
#include "os.h"
#include "tim3.h"

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void tim3_init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
typedef void (*TIM_SERVER_FUN)(void);
#define TIM3_MAX_SERVER_FUN     20 //tim3���ķ������ĸ���
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

//��ʱ��3�жϷ�����
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
    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
        
	}
    
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
    
	OSIntExit();
}


    
