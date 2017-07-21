/**
  ******************************************************************************
  * @file    bit_banding.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   提供位绑定服务的宏定义
  ******************************************************************************
  */
#ifndef __BIT_BANDING_H__
#define __BIT_BANDING_H__


/**
  * @brief  M4内核的GPIO寄存器基地址等
  */
#if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx) || defined (STM32F401xx) || defined (STM32F40XX) || defined (STM32F427X)
//这些宏定义在使用最新库函数时，都会有定义

#include "stm32f4xx.h"

#define GPIO_REG_ADDR_BASE		0x40020000ul
#define GPIO_REG_ADDR_OFFSET	0x400ul

#define GPIO_ODR_OFFSET			0x14ul
#define GPIO_IDR_OFFSET			0x10ul

#endif

/**
  * @brief  M3内核的GPIO寄存器基地址
  */
#if defined (STM32F10X_LD) || defined (STM32F10X_LD_VL) || defined (STM32F10X_MD) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_XL) || defined (STM32F10X_CL) 

#include "stm32f10x.h"

#define GPIO_REG_ADDR_BASE		0x40010800ul
#define GPIO_REG_ADDR_OFFSET	0x400ul

#define GPIO_ODR_OFFSET			0x0Cul
#define GPIO_IDR_OFFSET			0x08ul

#endif


//别名区的基地址的末地址，在M3和M4中是一样的，位地址的计算公式也是一样的

//外设别名区的基地址
#define BIT_BAND_ALIAS_PERIPHERAL_BASE	0x42000000ul

#define BIT_BAND_ALIAS_PERIPHERAL_END	0x43FFFFFFul

//SRAM别名区基地址
#define BIT_BAND_ALIAS_SRAM_BASE		0x22000000ul
//
#define BIT_BAND_ALIAS_SRAM_END			0x23FFFFFFul


//在bitband区中 位地址的计算公式
#define BIT_WORD_ADDR(ByteAddr, BitNumber)	((ByteAddr&0xF0000000) + 0x2000000 + (ByteAddr&0x000FFFFF)*32 + (BitNumber&0x0000000F)*4)


//将地址转换成指针，这样才能读写地址中的值
#define CONVERT_TO_PTR(BitWordAddr)		( *( (volatile unsigned long *)BitWordAddr ) )

//BitNumber<16
//输出
#define PAout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOA_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PBout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOB_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PCout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOC_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PDout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOD_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PEout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOE_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PFout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOF_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PGout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOG_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PHout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOH_BASE + GPIO_ODR_OFFSET, BitNumber) )
//读输入
#define PAin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOA_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PBin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOB_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PCin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOC_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PDin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOD_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PEin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOE_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PFin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOF_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PGin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOG_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PHin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOH_BASE + GPIO_IDR_OFFSET, BitNumber) )

#endif


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
