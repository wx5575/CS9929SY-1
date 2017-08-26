/**
  ******************************************************************************
  * @file    start_stop_key.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ������λ������������
  ******************************************************************************
  */

#include "sys_level.h"
#include "keyboard.h"
#include "os.h"
#include "start_stop_key.h"



#define  GPIO_PORT_START		GPIOF
#define  GPIO_PIN_START			GPIO_Pin_11

#define  GPIO_PORT_STOP			GPIOB
#define  GPIO_PIN_STOP			GPIO_Pin_2

#define  GPIO_PORT_SYN_START		GPIOE
#define  GPIO_PIN_SYN_START			GPIO_Pin_5

#define  GPIO_PORT_SYN_STOP		GPIOE
#define  GPIO_PIN_SYN_STOP      GPIO_Pin_6
/**
  * @brief  ������λ���ⲿ�ж�����
  * @param  ��
  * @retval ��
  */
static void key_start_stop_exit_config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/*ʹ���ⲿ�жϸ���ʱ��*/
    
    /*ӳ��GPIOF��Pin6��EXTILine2*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource2);  
    EXTI_ClearITPendingBit(EXTI_Line2);        //���ⲿ��·0�ж�
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;      //��·0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //����ģʽΪ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;      //���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);
    
    /*ӳ��GPIOF��Pin11��EXTILine11*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource11);
    EXTI_ClearITPendingBit(EXTI_Line11);//���ⲿ��·0�ж�
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;//��·0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//����ģʽΪ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);
}
/**
  * @brief  ������λ��NVIC����
  * @param  ��
  * @retval ��
  */
static void key_start_stop_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ж�ͨ��Ϊͨ��0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�������ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//���ж�
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ж�ͨ��Ϊͨ��0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�������ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//���ж�
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ������λ�����ų�ʼ������
  * @param  ��
  * @retval ��
  */
void key_start_stop_gpio_init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    /* ������ */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_START;
    GPIO_Init(GPIO_PORT_START, &GPIO_InitStructure);//��ʼ��
    
    /* ��λ�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_STOP;
    GPIO_Init(GPIO_PORT_STOP, &GPIO_InitStructure);//��ʼ��
    
    /* ��һ·������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
    
    /* �ڶ�·������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
    
    /* ����·������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
    
    /* ����·������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
    
    key_start_stop_exit_config();
    key_start_stop_nvic_config();
    
    /* ͬ����������λ���� PE5 PE6 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    /* ͬ���������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SYN_START;
    GPIO_Init(GPIO_PORT_SYN_START, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_PORT_SYN_START, GPIO_PIN_SYN_START);
    
    /* ͬ����λ���� */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SYN_STOP;
    GPIO_Init(GPIO_PORT_SYN_STOP, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_PORT_SYN_STOP, GPIO_PIN_SYN_STOP);
}

/**
  * @brief  �ⲿ�ж���2���жϷ�����
  * @param  ��
  * @retval ��
  */
void EXTI2_IRQHandler(void)
{
    /* ��λ�� */
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2);
        
        if(GPIO_ReadInputDataBit(GPIO_PORT_STOP, GPIO_PIN_STOP) == 0)
        {
//            SYN_STOP_PIN = PIN_L;
        }
    }
}

/**
  * @brief  �ⲿ�ж���15-10���жϷ�����
  * @param  ��
  * @retval ��
  */
void EXTI15_10_IRQHandler(void)
{
    /* ������ */
    if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line11);
        
        if(GPIO_ReadInputDataBit(GPIO_PORT_START, GPIO_PIN_START) == 0)
        {
        }
    }
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    
    if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
