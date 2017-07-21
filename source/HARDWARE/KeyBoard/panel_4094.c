/**
  ******************************************************************************
  * @file    key_led_buzzer.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ������LED��������������
  ******************************************************************************
  */

#include "panel_4094.h"

#include "bit_banding.h"

#define NUM_OF_PANEL_4094       (2) //4094����������
#define PANEL_4094_DLY          (10)



/*********************4������*********************************/
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


/*********************4094����*********************************/
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
  * @brief  penel_���ݻ�����
  */
static  uint8_t panel_4094_data_pool[NUM_OF_PANEL_4094];   //������������

/**
  * @brief  C4094��ʱ����
  * @param  ��
  * @retval ��
  */
static void panel_4094_delay(uint32_t t)
{
	while(t--);
}

/**
  * @brief  penel_�������
  * @param  ��
  * @retval ��
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
  * @brief  C4094�������
  * @param  [in] Index��оƬ������
  * @param  [in] data�� Ҫ���������
  * @retval ��
  */
void panel_4094_write(uint8_t index,uint8_t data)
{
	panel_4094_data_pool[index] = data;
}

/**
  * @brief  ��4094ָ��оƬ�Ļ�������8λ
  * @param  [in] Index��оƬ������
  * @retval 8λ��������
  */
uint8_t panel_4094_read(uint8_t index)
{
	return panel_4094_data_pool[index];
}
/**
  * @brief  ����4094�����������ݵ�Ӳ��
  * @param  ��
  * @retval ��
  */
void update_panel_4094(void)
{
	panel_4094_output();
}
/**
  * @brief  4094��ת����ʼ��GPIO
  * @param  ��
  * @retval ��
  */
void panel_4094_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG, ENABLE);
	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
    
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN1;
	GPIO_Init(GPIO_PORT_IN1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN2;
	GPIO_Init(GPIO_PORT_IN2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN3;
	GPIO_Init(GPIO_PORT_IN3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN4;
	GPIO_Init(GPIO_PORT_IN4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Pin = PANEL_4094_ST_PIN;
	GPIO_Init(PANEL_4094_ST_PORT, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = PANEL_4094_CLK_PIN;
	GPIO_Init(PANEL_4094_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PANEL_4094_DATA_PIN;
	GPIO_Init(PANEL_4094_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TESTLED_PIN;
	GPIO_Init(TESTLED_PORT, &GPIO_InitStructure);
	
	/* ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* ���� EXTI Line8 �� PG8 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource8);
	
	/* ���� EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line    = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  ��������4�н�ֵ��һ��4�У������ֽڵĵ�4λ��
  * @param  ��
  * @retval 4��������ֵ
  */
uint8_t read_row_gpio(void)
{
    return (KEY_PIN_IN1 << 0) | (KEY_PIN_IN2 << 1) | (KEY_PIN_IN3 << 2) | (KEY_PIN_IN4 << 3);
}
/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
