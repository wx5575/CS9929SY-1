
/**
  ******************************************************************************
  * @file    spi_flash.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   SPI串口FLASH驱动
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "sys_level.h"

#define SPI_FLASH_PAGE_SIZE         528	//每页字节数
#define PAGE_ADDR_OFFSET_BITS		10	//页地址偏移位数

#define SPI_FLASH_CHIP1             2   //参数
#define SPI_FLASH_CHIP2             3   //结果
#define SPI_FLASH_CHIP3             1   //校准系数

typedef struct
{
	uint32_t ChipID;		/* 芯片ID */
	char ChipName[16];		/* 芯片型号字符串，主要用于显示 */
	uint32_t TotalSize;		/* 总容量 */
	uint16_t PageSize;		/* 页面大小 */
}SFLASH_INFO;

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/

extern void init_flash(void);
extern void at45_buffer_write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
extern void at45_buffer_read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
extern void SPI_FLASH_PageErase(uint32_t PageAddr);
extern void erase_chip_spi_flash(void);
extern void flash_cs_set_at45db161(char n);

#endif /* __SPI_FLASH_H */

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
