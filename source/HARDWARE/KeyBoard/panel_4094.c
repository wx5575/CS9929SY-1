/**
  ******************************************************************************
  * @file    key_led_buzzer.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键、LED、蜂鸣器驱动层
  ******************************************************************************
  */

#include "panel_4094.h"

#include "bit_banding.h"

#define NUM_OF_PANEL_4094       (2) //4094级联的数量
#define PANEL_4094_DLY          (10)



/*********************4列引脚*********************************/
#define KEY_PIN_IN1     PCin(3)
#define KEY_PIN_IN2     PCin(2)
#define KEY_PIN_IN3     PCin(1)
#define KEY_PIN_IN4     PCin(0)

#define  GPIO_PORT_IN1  GPIOC
#define  GPIO_PIN_IN1   GPIO_Pin_3

#define  GPIO_PORT_IN2  GPIOC
#define  GPIO_PIN_IN2   GPIO_Pin_2

#define  GPIO_PORT_IN3  GPIOC
#define  GPIO_PIN_IN3   GPIO_Pin_1

#define  GPIO_PORT_IN4  GPIOC
#define  GPIO_PIN_IN4   GPIO_Pin_0


/*********************4094引脚*********************************/
#define  PANEL_4094_ST_PORT     GPIOA
#define  PANEL_4094_ST_PIN      GPIO_Pin_5
#define  PANEL_4094_ST_SET()    ((GPIO_TypeDef *)PANEL_4094_ST_PORT)->BSRRL = PANEL_4094_ST_PIN
#define  PANEL_4094_ST_CLR()    ((GPIO_TypeDef *)PANEL_4094_ST_PORT)->BSRRH = PANEL_4094_ST_PIN

#define  PANEL_4094_CLK_PORT    GPIOA
#define  PANEL_4094_CLK_PIN     GPIO_Pin_6
#define  PANEL_4094_CLK_SET()   ((GPIO_TypeDef *)PANEL_4094_CLK_PORT)->BSRRL = PANEL_4094_CLK_PIN
#define  PANEL_4094_CLK_CLR()   ((GPIO_TypeDef *)PANEL_4094_CLK_PORT)->BSRRH = PANEL_4094_CLK_PIN

#define  PANEL_4094_DATA_PORT   GPIOA 
#define  PANEL_4094_DATA_PIN    GPIO_Pin_4
#define  PANEL_4094_DATA_SET()  ((GPIO_TypeDef *)PANEL_4094_DATA_PORT)->BSRRL = PANEL_4094_DATA_PIN  
#define  PANEL_4094_DATA_CLR()  ((GPIO_TypeDef *)PANEL_4094_DATA_PORT)->BSRRH = PANEL_4094_DATA_PIN  

#define  TESTLED_PORT            GPIOI
#define  TESTLED_PIN             GPIO_Pin_2

/**
  * @brief  penel_数据缓冲区
  */
static  uint8_t panel_4094_data_pool[NUM_OF_PANEL_4094];   //串行输出缓冲池

/**
  * @brief  C4094延时函数
  * @param  无
  * @retval 无
  */
static void panel_4094_delay(uint32_t t)
{
	while(t--);
}

/**
  * @brief  penel_输出函数
  * @param  无
  * @retval 无
  */
static void panel_4094_output(void)
{
	uint8_t i;
    
	PANEL_4094_ST_CLR();
    
	for(i = 0; i< (NUM_OF_PANEL_4094 * 8); i++)
	{	
		PANEL_4094_CLK_CLR();
        
		if(panel_4094_data_pool[NUM_OF_PANEL_4094 - 1 - (i/8)] & (0x80 >> (i % 8)))
		{
			PANEL_4094_DATA_SET();
		}
		else
		{
			PANEL_4094_DATA_CLR();
		}
        
		panel_4094_delay(PANEL_4094_DLY);
		PANEL_4094_CLK_SET();
		panel_4094_delay(PANEL_4094_DLY);
	}
    
	PANEL_4094_ST_SET();
}

/**
  * @brief  C4094输出函数
  * @param  [in] Index：芯片索引号
  * @param  [in] data： 要输出的数据
  * @retval 无
  */
void panel_4094_write(uint8_t index,uint8_t data)
{
	panel_4094_data_pool[index] = data;
}

/**
  * @brief  读4094指定芯片的缓冲数据8位
  * @param  [in] Index：芯片索引号
  * @retval 8位缓冲数据
  */
uint8_t panel_4094_read(uint8_t index)
{
	return panel_4094_data_pool[index];
}
/**
  * @brief  更新4094缓冲区的数据到硬件
  * @param  无
  * @retval 无
  */
void update_panel_4094(void)
{
	panel_4094_output();
}
/**
  * @brief  4094串转并初始化GPIO
  * @param  无
  * @retval 无
  */
void panel_4094_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG, ENABLE);
	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
    
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN1;
	GPIO_Init(GPIO_PORT_IN1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN2;
	GPIO_Init(GPIO_PORT_IN2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN3;
	GPIO_Init(GPIO_PORT_IN3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN4;
	GPIO_Init(GPIO_PORT_IN4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Pin = PANEL_4094_ST_PIN;
	GPIO_Init(PANEL_4094_ST_PORT, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = PANEL_4094_CLK_PIN;
	GPIO_Init(PANEL_4094_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PANEL_4094_DATA_PIN;
	GPIO_Init(PANEL_4094_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TESTLED_PIN;
	GPIO_Init(TESTLED_PORT, &GPIO_InitStructure);
	
	/* 使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* 连接 EXTI Line8 到 PG8 引脚 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource8);
	
	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line    = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  读出键盘4列脚值，一共4列，放在字节的低4位上
  * @param  无
  * @retval 4列下引脚值
  */
uint8_t read_row_gpio(void)
{
    return (KEY_PIN_IN1 << 0) | (KEY_PIN_IN2 << 1) | (KEY_PIN_IN3 << 2) | (KEY_PIN_IN4 << 3);
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
