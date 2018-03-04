
/**
  ******************************************************************************
  * @file    spi_flash.h
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   SPI����FLASH����
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "sys_level.h"

#define SPI_FLASH_PAGE_SIZE         528	//ÿҳ�ֽ���
#define PAGE_ADDR_OFFSET_BITS		10	//ҳ��ַƫ��λ��

#define SPI_FLASH_CHIP1             2   //����
#define SPI_FLASH_CHIP2             3   //���
#define SPI_FLASH_CHIP3             1   //У׼ϵ��

typedef struct
{
	uint32_t ChipID;		/* оƬID */
	char ChipName[16];		/* оƬ�ͺ��ַ�������Ҫ������ʾ */
	uint32_t TotalSize;		/* ������ */
	uint16_t PageSize;		/* ҳ���С */
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

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
