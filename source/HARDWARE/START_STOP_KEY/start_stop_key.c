/**
  ******************************************************************************
  * @file    start_stop_key.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   启动复位按键驱动程序
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
  * @brief  启动复位键外部中断配置
  * @param  无
  * @retval 无
  */
static void key_start_stop_exit_config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/*使能外部中断复用时钟*/
    
    /*映射GPIOF的Pin6至EXTILine2*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource2);  
    EXTI_ClearITPendingBit(EXTI_Line2);        //清外部线路0中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;      //线路0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //触发模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;      //开外部中断
    EXTI_Init(&EXTI_InitStructure);
    
    /*映射GPIOF的Pin11至EXTILine11*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource11);
    EXTI_ClearITPendingBit(EXTI_Line11);//清外部线路0中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;//线路0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//触发模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//开外部中断
    EXTI_Init(&EXTI_InitStructure);
}
/**
  * @brief  启动复位键NVIC配置
  * @param  无
  * @retval 无
  */
static void key_start_stop_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//中断通道为通道0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢断优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//开中断
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//中断通道为通道0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢断优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//开中断
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  启动复位键引脚初始化配置
  * @param  无
  * @retval 无
  */
void key_start_stop_gpio_init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    /* 启动键 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_START;
    GPIO_Init(GPIO_PORT_START, &GPIO_InitStructure);//初始化
    
    /* 复位键 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_STOP;
    GPIO_Init(GPIO_PORT_STOP, &GPIO_InitStructure);//初始化
    
    /* 第一路测试完成 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    
    /* 第二路测试完成 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    
    /* 第三路测试完成 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
    
    /* 第四路测试完成 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
    
    key_start_stop_exit_config();
    key_start_stop_nvic_config();
    
    /* 同步启动，复位引脚 PE5 PE6 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    /* 同步启动引脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SYN_START;
    GPIO_Init(GPIO_PORT_SYN_START, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_PORT_SYN_START, GPIO_PIN_SYN_START);
    
    /* 同步复位引脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SYN_STOP;
    GPIO_Init(GPIO_PORT_SYN_STOP, &GPIO_InitStructure);
    GPIO_SetBits(GPIO_PORT_SYN_STOP, GPIO_PIN_SYN_STOP);
}

/**
  * @brief  外部中断线2的中断服务函数
  * @param  无
  * @retval 无
  */
void EXTI2_IRQHandler(void)
{
    /* 复位键 */
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
  * @brief  外部中断线15-10的中断服务函数
  * @param  无
  * @retval 无
  */
void EXTI15_10_IRQHandler(void)
{
    /* 启动键 */
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

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
