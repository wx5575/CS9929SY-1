

#ifndef __SPI_FPGA_H
#define __SPI_FPGA_H


#include "sys_level.h"


extern void init_spi_fpga_gpio(void);
extern void read_fpga(u32 SendData, u8 bytes, u8 *buf);

#endif

/************************ (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
