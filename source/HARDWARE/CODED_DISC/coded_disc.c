/**
  ******************************************************************************
  * @file    coded_disc.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ������������
  ******************************************************************************
  */

#include "sys_level.h"
#include "coded_disc.h"
#include "keyboard.h"
#include "os.h"
#include "key_led_buzzer.h"

static const uint32_t SEND_MSG_RIGH = CODE_RIGH;///<������ת����Ϣ��������
static const uint32_t SEND_MSG_LEDT = CODE_LEFT;///<���̷�ת����Ϣ��������

#define C_DISC_RIGH_PORT    GPIOF
#define C_DISC_RIGH_PIN     GPIO_Pin_7
#define C_DISC_LEFT_PORT    GPIOF
#define C_DISC_LEFT_PIN     GPIO_Pin_6
static void(*send_coded_disc_msg_fun)(uint32_t *);

/**
  * @brief  �����ж�����
  * @param  ��
  * @retval ��
  */
static void coded_exit_config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/*ʹ���ⲿ�жϸ���ʱ��*/
    
    /*ӳ��GPIOF��Pin6��EXTILine6*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource6);  
    EXTI_ClearITPendingBit(EXTI_Line6);        //���ⲿ��·0�ж�
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;      //��·0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //����ģʽΪ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;      //���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);
    
    /*ӳ��GPIOF��Pin7��EXTILine7*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource7);
    EXTI_ClearITPendingBit(EXTI_Line7);//���ⲿ��·0�ж�
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;//��·0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//����ģʽΪ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  ����NVIC�ж�����
  * @param  ��
  * @retval ��
  */
static void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//Ƕ�׷���Ϊ��3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ж�ͨ��Ϊͨ��0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�������ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//���ж�
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ����Ӳ����ʼ��
  * @param  ��
  * @retval ��
  */
void coded_disc_init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOAʱ��
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    GPIO_InitStructure.GPIO_Pin = C_DISC_RIGH_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(C_DISC_RIGH_PORT, &GPIO_InitStructure);//��ʼ��
    
    GPIO_InitStructure.GPIO_Pin = C_DISC_LEFT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(C_DISC_LEFT_PORT, &GPIO_InitStructure);//��ʼ��
    
    coded_exit_config();
    NVIC_Config();
}

/**
  * @brief  ע�����̷�����Ϣ��������Ӧ�ò�ע��
  * @param  [in] fun ������Ϣ����
  * @retval ��
  */
void register_coded_disc_send_msg_fun(void(*fun)(uint32_t *))
{
    send_coded_disc_msg_fun = fun;
}

static void coded_disc_soft_delay_us(u32 dly_us)
{
	unsigned int dly_i;
	while(dly_us--)
	{
		for(dly_i=0;dly_i<802;dly_i++);
	}
}
static void coded_disc_soft_delay_10us(u32 dly_us)
{
    coded_disc_soft_delay_us(dly_us);
}
void scan_coded_disc(void)
{
    static int32_t count;
    static uint32_t left_count;
    static uint32_t right_count;
    const uint8_t COUNTX = 30;
    
//    if(++count < 2)
//    {
//        return;
//    }
    
    count = 0;
    
    if(1 == GPIO_ReadInputDataBit(C_DISC_LEFT_PORT, C_DISC_LEFT_PIN))
    {
        right_count = 0;
        if(++left_count > COUNTX)
        {
            left_count = 0;
            if(send_coded_disc_msg_fun != NULL)
            {
                BUZZER_ON_T(KEY_BUZZER_TIME);
                send_coded_disc_msg_fun((uint32_t *)&SEND_MSG_LEDT);
            }
        }
    }
    
    if(1 == GPIO_ReadInputDataBit(C_DISC_RIGH_PORT, C_DISC_RIGH_PIN))
    {
        left_count = 0;
        if(++right_count > COUNTX)
        {
            right_count = 0;
            if(send_coded_disc_msg_fun != NULL)
            {
                BUZZER_ON_T(KEY_BUZZER_TIME);
                send_coded_disc_msg_fun((uint32_t *)&SEND_MSG_RIGH);
            }
        }
    }
}
/**
  * @brief  �����жϷ�����
  * @param  [in] fun ������Ϣ����
  * @retval ��
  */
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();    
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        coded_disc_soft_delay_10us(100);
        if(1 == GPIO_ReadInputDataBit(C_DISC_LEFT_PORT, C_DISC_LEFT_PIN))
        {
            if(send_coded_disc_msg_fun != NULL)
            {
                BUZZER_ON_T(KEY_BUZZER_TIME);
                send_coded_disc_msg_fun((uint32_t *)&SEND_MSG_LEDT);
            }
        }
    }
    else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        coded_disc_soft_delay_10us(100);
        if(1 == GPIO_ReadInputDataBit(C_DISC_RIGH_PORT, C_DISC_RIGH_PIN))
        {
            if(send_coded_disc_msg_fun != NULL)
            {
                BUZZER_ON_T(KEY_BUZZER_TIME);
                send_coded_disc_msg_fun((uint32_t *)&SEND_MSG_RIGH);
            }
        }
    }
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    
    if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    
    EXTI_ClearITPendingBit(EXTI_Line7);
    EXTI_ClearITPendingBit(EXTI_Line6);
    
	OSIntExit();
}



/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
