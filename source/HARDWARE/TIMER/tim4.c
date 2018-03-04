
#include "os.h"
#include "tim4.h"

//ʹ�ܶ�ʱ��4,ʹ���ж�.
void tim4_init(uint16_t arr,uint16_t psc)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��

	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ����ʱ��4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//�ⲿ�ж�4
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����NVIC
}

//��������������,��USMART����,�ŵ�����,����������ֲ. 
//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{
	OSIntEnter();
    
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)//����ж�
	{
        
	}
    
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
    
	OSIntExit();
}
