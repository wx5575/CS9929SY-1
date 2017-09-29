/**
  ******************************************************************************
  * @file    coded_disc.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   码盘驱动程序
  ******************************************************************************
  */

#include "sys_level.h"
#include "coded_disc.h"
#include "keyboard.h"
#include "os.h"
#include "key_led_buzzer.h"

static const uint32_t SEND_MSG_RIGH = CODE_RIGH;///<码盘正转的消息常量定义
static const uint32_t SEND_MSG_LEDT = CODE_LEFT;///<码盘反转的消息常量定义

#define C_DISC_RIGH_PORT    GPIOF
#define C_DISC_RIGH_PIN     GPIO_Pin_7
#define C_DISC_LEFT_PORT    GPIOF
#define C_DISC_LEFT_PIN     GPIO_Pin_6
static void(*send_coded_disc_msg_fun)(uint32_t *);

/**
  * @brief  码盘中断配置
  * @param  无
  * @retval 无
  */
static void coded_exit_config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/*使能外部中断复用时钟*/
    
    /*映射GPIOF的Pin6至EXTILine6*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource6);  
    EXTI_ClearITPendingBit(EXTI_Line6);        //清外部线路0中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;      //线路0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //触发模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;      //开外部中断
    EXTI_Init(&EXTI_InitStructure);
    
    /*映射GPIOF的Pin7至EXTILine7*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource7);
    EXTI_ClearITPendingBit(EXTI_Line7);//清外部线路0中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;//线路0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//触发模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//开外部中断
    EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  码盘NVIC中断配置
  * @param  无
  * @retval 无
  */
static void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//嵌套分组为组3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//中断通道为通道0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢断优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//开中断
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  码盘硬件初始化
  * @param  无
  * @retval 无
  */
void coded_disc_init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_InitStructure.GPIO_Pin = C_DISC_RIGH_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(C_DISC_RIGH_PORT, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.GPIO_Pin = C_DISC_LEFT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(C_DISC_LEFT_PORT, &GPIO_InitStructure);//初始化
    
    coded_exit_config();
    NVIC_Config();
}

/**
  * @brief  注册码盘发送消息函数，由应用层注册
  * @param  [in] fun 发送消息函数
  * @retval 无
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
  * @brief  码盘中断服务函数
  * @param  [in] fun 发送消息函数
  * @retval 无
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



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
