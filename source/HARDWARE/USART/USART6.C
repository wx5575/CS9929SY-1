/************************************************************
Copyright (C), 2012-2022, yin.
FileName: main.c
Author: ycw Version :  1.0 Date: 2012.04.26
Description: USART6 SendData  
Version: V2.0                 
Function List:USART6 SendData 
History: V1.0                     
<author> <time> <version > <desc>
YCW 12/04/26 1.0 build this moudle
***********************************************************/
#include "sys_level.h"

/*定义USART6接受的数据存储变量*/
uint16_t usart6_get_data;

void GPIO_Config(void);
void USART_Config(void);
void NVIC_Config(void);
void Delay(uint32_t nCount);


/**
  * @brief  usart6 配置
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //开启USART6时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //开启GPIOC时钟
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);//这相当于M3的开启复用时钟？只配置复用的引脚，
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);//               
    
    /*配置GPIOC*/
    GPIO_StructInit(&GPIO_InitStructure);      //缺省值填入
    
    /*配置GPIOC_Pin6为TX输出*/
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;     //设置为复用，必须为AF，OUT不行
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    
    /*配置GPIOC_Pin7为RX输入*/
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;     //这也必须为复用，与M3不同！
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    
    /*IO引脚复用功能设置，与之前版本不同*/
    /*配置USART6*/
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate =115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART6, &USART_InitStructure);
    USART_ClockStructInit(&USART_ClockInitStruct);    //之前没有填入缺省值，是不行的
    USART_ClockInit(USART6, &USART_ClockInitStruct);
    
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);    //使能 USART6中断
    USART_Cmd(USART6, ENABLE);         //使能 USART6 
}
/**
  * @brief  usart6 NVIC配置
  * @param  无
  * @retval 无
  */
void USART6_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //嵌套优先级分组为 1
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn; //嵌套通道为USART6_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART6发送数据
  * @param  无
  * @retval 无
  */
void USART6_Puts(uint8_t *data, uint32_t len)
{
    while (len--)
    {
        USART_SendData(USART6, *data++);
        
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
    }
}

 /**
  * @brief  延时函数
  * @param  无
  * @retval 无
  */
static void Delay(uint32_t nCount)
{
    while (nCount--);
}

/**
  * @brief  USART6中断服务程序
  * @param  无
  * @retval 无
  */
void USART6_IRQHandler(void)
{
	OSIntEnter();
    
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) //判断为接收中断
    { 
        USART_SendData(USART6, USART_ReceiveData(USART6)); //发送收到的数据
    }
    
	OSIntExit();
}
